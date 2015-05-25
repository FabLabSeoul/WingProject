// 평면 클래스.
#pragma once


namespace common
{

	struct Plane
	{
		Plane();
		Plane( float a, float b, float c, float d );
		Plane( Vector3& vN, float d );
		Plane( const Vector3& vN, const Vector3& vPos );
		Plane( const Vector3& vA, const Vector3& vB, const Vector3& vC );

		void Init( const Vector3& vA, const Vector3& vB, const Vector3& vC );
		float Distance( const Vector3& vP ) const;
		Vector3 Pick( const Vector3& vOrig, const Vector3& vDir ) const;
		float GetX( float fY, float fZ ) const;
		float GetY( float fX, float fZ ) const;
		float GetZ( float fX, float fY ) const;
		float Collision( Vector3& vP );
		BOOL CollisionSphere( const Vector3& vP, float fRadius );
		int LineCross( const Vector3& v0, const Vector3& v1, Vector3* pvOut ) const;
		Matrix44 GetReflectMatrix();

		const Plane operator * (const Matrix44 &rhs);

		Vector3 N; // Normal
		float	D; // D
	};

}
