// ����ü Ŭ����.
#pragma once


namespace graphic
{


	class cFrustum : public cCube
	{
	public:
		cFrustum();
		virtual ~cFrustum();

		bool Create( const Matrix44 &matViewProj );
		bool Create( const Vector3 &_min, const Vector3 &_max );
		bool IsIn( const Vector3 &point );
		bool IsInSphere( const Vector3 &point, float radius );
		void Render();
		const Vector3& GetPos() const;
		vector<Plane>& GetPlanes();
		

	private:
		bool m_fullCheck;	// IsIn, IsInSphere �Լ� ȣ��� üũ���� default : false
		vector<Plane> m_plane;	// frustum�� 6�� ���
		Vector3 m_pos; // ����ü ��ġ
	};


	inline const Vector3& cFrustum::GetPos() const { return m_pos; }
	inline vector<Plane>& cFrustum::GetPlanes() { return m_plane; }
}
