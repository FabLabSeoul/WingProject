
#include "stdafx.h"
#include "snow.h"


using namespace graphic;

cSnow::cSnow()
:	m_isRenderFrustum(false)
{

}

cSnow::~cSnow()
{

}


bool cSnow::Create( const Vector3 &_min, const Vector3 &_max, 
	const int particleCount )
{
	m_frustum.Create(_min, _max);

	cParticles::Create( "particle.bmp", particleCount );
	m_numToRelease = 8;
	m_releaseInterval = 0.8f;
	m_lifeCycle = 35.f;
	m_pointSize = .5f;
	m_gravity = Vector3(0,-.8f,0);
	m_velocityVar = 0.1f;

	m_emitPos = m_frustum.GetPos();
	m_emitPosMin = _min;
	m_emitPosMin.y = _max.y - 0.5f;
	m_emitPosMax = _max;

	for (u_int i=0; i < m_frustum.GetPlanes().size(); ++i)
	{
		Plane plane = m_frustum.GetPlanes()[ i];
		// 절두체는 평면이 밖을 향하고 있으므로 값을 반전시켜야 한다.
		plane.N = -plane.N;
		plane.D = -plane.D;

		AddCollisionPlane( plane, 1, PARTICLE_COLLISION_RESULT::RECYCLE );
	}

	return true;
}


void cSnow::Move( const float elapseTime)
{
	cParticles::Move(elapseTime);
}


void cSnow::Render()
{
	cParticles::Render();

	if (m_isRenderFrustum)
		m_frustum.Render();
}
