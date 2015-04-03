
#pragma once

namespace common
{
	struct Matrix44;
	struct Quaternion
	{
		float	x, y, z, w;

		Quaternion();
		Quaternion( const float fX, const float fY, const float fZ, const float fW );
		Quaternion( const Vector3& vAxis, const float fAngle );
		Quaternion( const Vector3& vDir1, const Vector3& vDir2 );

		Quaternion& operator *= ( const Quaternion& q )
		{
			*this = *this * q;
			return *this;
		} //operator *=

		Quaternion		operator * ( const Quaternion& q )
		{
			return Quaternion(
					x * q.w + y * q.z - z * q.y + w * q.x,
				-x * q.z + y * q.w + z * q.x + w * q.y,
					x * q.y - y * q.x + z * q.w + w * q.z,
				-x * q.x - y * q.y - z * q.z + w * q.w );
		} //operator *

		Quaternion		operator * ( const Matrix44& m )
		{
			return *this * m.GetQuaternion();
		} //operator *
	
		Quaternion Interpolate( const Quaternion& qNext, const float fTime ) const;
		Matrix44 GetMatrix() const;
		Vector3 GetDirection() const;

		void	SetRotationX( const float fRadian );
		void	SetRotationY( const float fRadian );
		void	SetRotationZ( const float fRadian );
		void	SetRotationArc( const Vector3& vDir, const Vector3& vDir2 );
		void Euler(const Vector3& euler);
		void Euler2(const Vector3& euler);
		Vector3 Euler() const;
		void	Normalize();
	};

}
