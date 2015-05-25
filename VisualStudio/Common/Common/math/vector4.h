#pragma once


namespace common
{
	struct Vector3;
	struct Matrix44;
	struct Vector4
	{
		float x, y, z, w;

		Vector4() : x(0), y(0), z(0), w(0) {}
		Vector4(float x0, float y0, float z0, float w0) : x(x0), y(y0), z(z0), w(w0) {}
		Vector4(const Vector3 &v, float w0=1.f) : x(v.x), y(v.y), z(v.z), w(w0) {}

		Vector4 operator * ( const float & rhs ) const;
		Vector4 operator * ( const Matrix44& rhs ) const;
		Vector4& operator *= ( Matrix44& rhs );
	};

}
