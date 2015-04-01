#pragma once


namespace common
{
	struct Triangle
	{
		Triangle();
		Triangle( const Vector3& v1, const Vector3& v2, const Vector3& v3 );

		void Create( const Vector3& vA, const Vector3& vB, const Vector3& vC );
		BOOL Intersect(	const Vector3& vOrig, const Vector3& vDir,
			float* pfT = NULL, float* pfU = NULL, float* pfV = NULL ) const;
		float Distance( const Vector3& vPos )  const;

		Vector3 a, b, c;
	};

}
