#pragma once

namespace common
{
	struct Vector3;
	struct Quaternion;

	struct Matrix44
	{
		union
		{
			struct
			{
				float	_11, _12, _13, _14;
				float	_21, _22, _23, _24;
				float	_31, _32, _33, _34;
				float	_41, _42, _43, _44;
			};
			float	m[4][4];
		};
		static const Matrix44 Identity; // 전역 단위행렬 상수.

		Matrix44();
		inline void SetIdentity();
		void	SetRotationX( const float angle );
		void	SetRotationY( const float angle );
		void	SetRotationZ( const float angle );
		void	SetTranslate( const Vector3& pos );
		void	SetScale( const Vector3& scale);
		Vector3 GetScale() const;
		Vector3 GetPosition() const;
		void SetPosition(const Vector3 &pos);
		void SetView( const Vector3& pos, const Vector3& dir0, const Vector3& up0);
		void SetView2( const Vector3& pos, const Vector3& lookAt, const Vector3& up0);
		void	SetProjection( const float fov, const float aspect, const float nearPlane, const float farPlane );
		Quaternion GetQuaternion() const;
		Matrix44 Inverse() const;
		Matrix44& Transpose();

		Matrix44 operator * ( const Matrix44& rhs ) const;
		Matrix44& operator *= ( const Matrix44& rhs );
	};


	inline Vector3 Matrix44::GetScale() const { return Vector3(_11, _22, _33); }
	inline Vector3 Matrix44::GetPosition() const { return Vector3(_41, _42, _43); }
	inline void Matrix44::SetPosition(const Vector3 &pos) { _41=pos.x; _42 = pos.y; _43 = pos.z; }
}
