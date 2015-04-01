// 파티클을 처리하는 기본 클래스다.
// 다양한 파티클 효과는 이 클래스를 상속받아 구현된다.
// 
// 참조 코드.
// http://www.codesampler.com/dx9src/dx9src_7.htm
#pragma once


namespace graphic
{


	// Collision Results
	namespace PARTICLE_COLLISION_RESULT
	{
		enum TYPE {
			BOUNCE,
			STICK,
			RECYCLE,
		};
	}


	class cParticles
	{
	protected:
		struct sParticle
		{
			bool enable;
			Vector3 pos;
			Vector3 velocity;
			float initTime;
		};

		struct sCollisionPlane
		{
			Plane plane;
			// 경계면에 닿은 파티클의 반사상수.
			float bounceFactor; 
			// 파티클이 경계면에 닿았을 때 어떻게 처리할지 여부를 저장
			PARTICLE_COLLISION_RESULT::TYPE collisionResult; 
		};


	public:
		cParticles();
		virtual ~cParticles();

		bool Create( const string &textureFileName, const int maxParticle );
		
		virtual void Move( const float elapseTime);
		virtual void Render();

		void AddCollisionPlane(const Plane &plane, const float bounceFactor = 1.0f, 
			const PARTICLE_COLLISION_RESULT::TYPE cr = PARTICLE_COLLISION_RESULT::BOUNCE);


		// 설정해야할 변수들이 많아서 public 으로 처리함
	public:
		int m_maxParticleCount;
		int m_particleCount;
		float m_pointSize;

		float m_currentTime;
		float m_lastUpdate;

		float m_lifeCycle; // default: 1
		float m_releaseInterval; // default: 1
		Vector3 m_emitPos; // 0,0,0
		Vector3 m_emitPosMin;
		Vector3 m_emitPosMax;
		int m_numToRelease; // default: 1

		Vector3 m_gravity;
		Vector3 m_wind;
		Vector3 m_velocity;
		bool m_bAirResistence; // default: true
		float m_velocityVar; // default: 1

		vector<sParticle> m_particles;
		vector<int> m_emptyParticleIndices;
		vector<sCollisionPlane> m_planes;
		cVertexBuffer m_vtxBuffer;
		cTexture *m_texture; // reference
	};


}
