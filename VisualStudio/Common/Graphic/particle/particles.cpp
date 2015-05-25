
#include "stdafx.h"
#include "particles.h"


using namespace graphic;

inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }


cParticles::cParticles()
	: m_particleCount(0)
,	m_pointSize(1)
,	m_texture(NULL)
,	m_gravity(0, 0, 0)
,	m_wind(0,0,0)
,	m_velocity(0,0,0)
,	m_emitPos(0,0,0)
,	m_bAirResistence(true)
,	m_currentTime(0)
,	m_lifeCycle(1)
,	m_releaseInterval(1)
,	m_numToRelease(1)
,	m_velocityVar(1)
,	m_maxParticleCount(1)
{

}

cParticles::~cParticles()
{

}


// 파티클 초기화.
// 원본 코드는 D3DPOOL_DEFAULT 플래그를 이용해서 프래임을 높이려고 애쓰는
// 흔적이 있는데, 테스트 결과 거의 효과가 없어서, 그 부분은 제외 했다.
bool cParticles::Create( const string &textureFileName, const int maxParticle )
{
	m_vtxBuffer.Create(maxParticle, sizeof(sVertexDiffuse), sVertexDiffuse::FVF);
	m_particles.resize(maxParticle);
	m_emptyParticleIndices.resize(maxParticle);
	m_texture = cResourceManager::Get()->LoadTexture(textureFileName);
	m_maxParticleCount = maxParticle;

	return true;
}


void cParticles::Move( const float elapseTime )
{
	m_currentTime += elapseTime;
	if (m_currentTime - m_lastUpdate < 0.05f) // interval을 길게 줄수록 fps가 높아진다.
		return;

	int count = 0;
	int emtpyParticleCount = 0;

	// 파티클 움직임 계산
	for (u_int i=0; i < m_particles.size(); ++i)
	{
		sParticle &particle = m_particles[ i];
		if (!particle.enable)
		{
			m_emptyParticleIndices[ emtpyParticleCount++] = i;
			continue;
		}

		const float timePassed = m_currentTime - particle.initTime;
			
		if (timePassed > m_lifeCycle)
		{
			particle.enable = false;
			m_emptyParticleIndices[ emtpyParticleCount++] = i;
			continue;
		}

		particle.velocity += m_gravity * elapseTime;

		// Update velocity with respect to Wind (Acceleration based on 
		// difference of vectors)
        if (m_bAirResistence)
            particle.velocity += (m_wind - particle.velocity) * elapseTime;

		Vector3 oldPos = particle.pos;
		particle.pos += particle.velocity * elapseTime;

		// check boundingbox
		if (!m_planes.empty())
		{
			BOOST_FOREACH (sCollisionPlane &cplane, m_planes)
			{
				const float result = cplane.plane.Collision(particle.pos);
				if (FLOAT_EQ(result, 0.f) || (result < 0))
				{
					if (cplane.collisionResult == PARTICLE_COLLISION_RESULT::BOUNCE)
					{
						particle.pos  = oldPos;

						//-----------------------------------------------------------------
						//
						// The new velocity vector of a particle that is bouncing off
						// a plane is computed as follows:
						//
						// Vn = (N.V) * N
						// Vt = V - Vn
						// Vp = Vt - Kr * Vn
						//
						// Where:
						// 
						// .  = Dot product operation
						// N  = The normal of the plane from which we bounced
						// V  = Velocity vector prior to bounce
						// Vn = Normal force
						// Kr = The coefficient of restitution ( Ex. 1 = Full Bounce, 
						//      0 = Particle Sticks )
						// Vp = New velocity vector after bounce
						//
						//-----------------------------------------------------------------

						const float Kr = cplane.bounceFactor;
						Vector3 Vn = cplane.plane.N * cplane.plane.N.DotProduct(particle.velocity);
						Vector3 Vt = particle.velocity - Vn;
						Vector3 Vp = Vt - (Vn * Kr);
						particle.velocity = Vp;
					}
					else if (cplane.collisionResult == PARTICLE_COLLISION_RESULT::RECYCLE)
					{
						particle.initTime -= m_lifeCycle;
					}
					else if (cplane.collisionResult == PARTICLE_COLLISION_RESULT::STICK)
					{
						particle.pos = oldPos;
						particle.velocity = Vector3(0,0,0);
					}
				}
			}

		}

		++count;
	}


	// 새 파티클 추가.
	if (m_currentTime - m_lastUpdate > m_releaseInterval)
	{
		m_lastUpdate = m_currentTime;

		for (int i=0; i < m_numToRelease; ++i)
		{
			if (emtpyParticleCount > 0)
			{
				const int idx = m_emptyParticleIndices[ --emtpyParticleCount];

				sParticle &particle = m_particles[ idx];
				if (particle.enable) // 비어있는 파티클을 찾는다.
					continue;
				
				particle.enable = true;
				particle.velocity = m_velocity;

				if (m_velocityVar != 0)
				{
					Vector3 randomVec = GetRandomVector();
					particle.velocity += randomVec * m_velocityVar;
				}

				particle.initTime = m_currentTime;
				particle.pos = m_emitPos;

				// 파티클 시작 위치 랜덤.
				Vector3 randPos;
				randPos.x = GetRandomMinMax(m_emitPosMin.x, m_emitPosMax.x);
				randPos.y = GetRandomMinMax(m_emitPosMin.y, m_emitPosMax.y);
				randPos.z = GetRandomMinMax(m_emitPosMin.z, m_emitPosMax.z);
				particle.pos += randPos;				

				++count;
			}

			// 최대 파티클 수 를 넘었다면, 추가하는 것을 멈춘다.
			if (m_maxParticleCount < count)
				break;
		}
	}

}


void cParticles::Render()
{
	GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE);

	GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	GetDevice()->SetRenderState( D3DRS_POINTSPRITEENABLE, TRUE );       // Turn on point sprites
	GetDevice()->SetRenderState( D3DRS_POINTSCALEENABLE,  TRUE );       // Allow sprites to be scaled with distance
	GetDevice()->SetRenderState( D3DRS_POINTSIZE,     FtoDW(m_pointSize) ); // Float value that specifies the size to use for point size computation in cases where point size is not specified for each vertex.
	GetDevice()->SetRenderState( D3DRS_POINTSIZE_MIN, FtoDW(1.0f) );    // Float value that specifies the minimum size of point primitives. Point primitives are clamped to this size during rendering. 
	GetDevice()->SetRenderState( D3DRS_POINTSCALE_A,  FtoDW(0.0f) );    // Default 1.0
	GetDevice()->SetRenderState( D3DRS_POINTSCALE_B,  FtoDW(0.0f) );    // Default 0.0
	GetDevice()->SetRenderState( D3DRS_POINTSCALE_C,  FtoDW(1.0f) );    // Default 0.0


	m_particleCount  = 0;
	if (sVertexDiffuse *vertices = (sVertexDiffuse*)m_vtxBuffer.Lock())
	{
		for (u_int i=0; i < m_particles.size(); ++i)
		{
			sParticle &particle = m_particles[ i];
			if (particle.enable)
			{
				vertices->p = particle.pos;
				vertices->c = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
				++vertices;
				++m_particleCount;
			}
		}
	}

	if (m_texture)
		m_texture->Bind(0);
	m_vtxBuffer.RenderPointList(m_particleCount);

	m_texture->Unbind(0);
	GetDevice()->SetRenderState( D3DRS_POINTSPRITEENABLE, FALSE );
	GetDevice()->SetRenderState( D3DRS_POINTSCALEENABLE,  FALSE );

	GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );

	GetDevice()->SetRenderState( D3DRS_LIGHTING, TRUE);
}


void cParticles::AddCollisionPlane(const Plane &plane, const float bounceFactor, 
	const PARTICLE_COLLISION_RESULT::TYPE cr)
{
	sCollisionPlane cp;
	cp.plane = plane;
	cp.bounceFactor = bounceFactor;
	cp.collisionResult = cr;
	m_planes.push_back(cp);
}

