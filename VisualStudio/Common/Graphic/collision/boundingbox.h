#pragma once


namespace graphic
{
	class cCube;


	// OBB 충돌박스.
	class cBoundingBox
	{
	public:
		cBoundingBox();
		cBoundingBox(const cCube &cube);
		void SetBoundingBox(const Vector3 &vMin, const Vector3 &vMax );
		void SetTransform( const Matrix44 &tm );
		bool Collision( cBoundingBox &box );
		bool Pick(const Vector3 &orig, const Vector3 &dir);
		float Length() const;
		Vector3 Center() const;

		cBoundingBox& operator=(const cCube &cube);


		Vector3 m_min;
		Vector3 m_max;
		Matrix44 m_tm;
	};


	// length(m_min, m_max)
	inline float cBoundingBox::Length() const { return (m_min - m_max).Length(); }
}
