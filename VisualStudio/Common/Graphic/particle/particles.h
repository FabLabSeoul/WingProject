// ��ƼŬ�� ó���ϴ� �⺻ Ŭ������.
// �پ��� ��ƼŬ ȿ���� �� Ŭ������ ��ӹ޾� �����ȴ�.
// 
// ���� �ڵ�.
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
			// ���鿡 ���� ��ƼŬ�� �ݻ���.
			float bounceFactor; 
			// ��ƼŬ�� ���鿡 ����� �� ��� ó������ ���θ� ����
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


		// �����ؾ��� �������� ���Ƽ� public ���� ó����
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
