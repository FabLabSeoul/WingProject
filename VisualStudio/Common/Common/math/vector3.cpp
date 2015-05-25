
#include "stdafx.h"
#include "math.h"

using namespace common;


Vector3 Vector3::Min(-FLT_MAX, -FLT_MAX, -FLT_MAX);
Vector3 Vector3::Max(FLT_MAX, FLT_MAX, FLT_MAX);
Vector3 Vector3::Up(0, 1, 0);
Vector3 Vector3::Right(1,0,0);
Vector3 Vector3::Forward(0,0,1);


Vector3::Vector3(const Vector4 &rhs) :
	x(rhs.x), y(rhs.y), z(rhs.z)
{
}

bool Vector3::IsEmpty() const
{
	return (x==0) && (y==0) && (z==0);
}

float Vector3::Length() const
{
	return sqrt(x*x + y*y + z*z);
}

float Vector3::LengthRoughly(const Vector3 &rhs) const
{
	Vector3 v = *this - rhs;
	return v.x*v.x + v.y*v.y + v.z*v.z;
}

Vector3 Vector3::Normal() const
{
	const float len = Length();
	if (len < 0.001f)
		return Vector3(0,0,0);
	return *this / len;
}


void Vector3::Normalize()
{
	*this = Normal();
}


Vector3 Vector3::operator + () const
{
	return *this;
}


Vector3 Vector3::operator - () const
{
	return Vector3(-x, -y, -z);
}


Vector3 Vector3::operator + ( const Vector3& rhs ) const
{
	return Vector3(x+rhs.x, y+rhs.y, z+rhs.z);
}


Vector3 Vector3::operator - ( const Vector3& rhs ) const
{
	return Vector3(x-rhs.x, y-rhs.y, z-rhs.z);
}


Vector3& Vector3::operator += ( const Vector3& rhs )
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}


Vector3& Vector3::operator -= ( const Vector3& rhs )
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}


Vector3& Vector3::operator *= ( const Vector3& rhs )
{
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
	return *this;
}


Vector3& Vector3::operator /= ( const Vector3& rhs )
{
	x /= rhs.x;
	y /= rhs.y;
	z /= rhs.z;
	return *this;
}


Vector3 Vector3::operator * ( const Matrix44& rhs ) const
{
	float	RHW = 1.0f / (x*rhs._14 + y*rhs._24 + z*rhs._34 + rhs._44);
	if (RHW >= FLT_MAX)
		return Vector3(0,0,0);

	Vector3 v;
	v.x = (x*rhs._11 + y*rhs._21 + z*rhs._31 + rhs._41 ) * RHW;
	v.y = (x*rhs._12 + y*rhs._22 + z*rhs._32 + rhs._42 ) * RHW;
	v.z = (x*rhs._13 + y*rhs._23 + z*rhs._33 + rhs._43 ) * RHW;
	return v;
}


Vector3& Vector3::operator *= ( const Matrix44& rhs )
{
	float	RHW = 1.0f / (x * rhs._14 + y * rhs._24 + z * rhs._34 + rhs._44);
	if (RHW >= FLT_MAX)
	{
		*this = Vector3(0,0,0);
		return *this;
	}

	Vector3 v;
	v.x = (x * rhs._11 + y * rhs._21 + z * rhs._31 + rhs._41 ) * RHW;
	v.y = (x * rhs._12 + y * rhs._22 + z * rhs._32 + rhs._42 ) * RHW;
	v.z = (x * rhs._13 + y * rhs._23 + z * rhs._33 + rhs._43 ) * RHW;
	*this = v;
	return *this;
}


Vector3 Vector3::MultiplyNormal( const Matrix44& rhs ) const
{
	Vector3 v;
	v.x = x * rhs._11 + y * rhs._21 + z * rhs._31;
	v.y = x * rhs._12 + y * rhs._22 + z * rhs._32;
	v.z = x * rhs._13 + y * rhs._23 + z * rhs._33;
	v.Normalize();
	return v;
}


float Vector3::DotProduct( const Vector3& v ) const
{
	return x * v.x + y * v.y + z * v.z;
}


Vector3 Vector3::CrossProduct( const Vector3& v ) const
{
	return Vector3( 
		(y * v.z) - (z * v.y), 
		(z * v.x) - (x * v.z), 
		(x * v.y) - (y * v.x) );
}


Vector3 Vector3::Interpolate( const Vector3 &v, const float alpha) const
{
	return Vector3(x + (alpha * ( v.x - x ) ),
		y + (alpha * ( v.y - y ) ),
		z + (alpha * ( v.z - z ) ) );
}
