
#include "stdafx.h"
#include "math.h"

using namespace common;


Vector4 Vector4::operator * ( const float & rhs ) const
{
	return Vector4(x*rhs, y*rhs, z*rhs, w*rhs);
}

Vector4 Vector4::operator * ( const Matrix44& rhs ) const
{
	float	RHW = 1.0f / (x*rhs._14 + y*rhs._24 + z*rhs._34 + w*rhs._44);
	if (RHW >= FLT_MAX)
		return Vector4(0,0,0,1);

	Vector4 v;
	v.x = (x*rhs._11 + y*rhs._21 + z*rhs._31 + w*rhs._41) * RHW;
	v.y = (x*rhs._12 + y*rhs._22 + z*rhs._32 + w*rhs._42) * RHW;
	v.z = (x*rhs._13 + y*rhs._23 + z*rhs._33 + w*rhs._43) * RHW;
	v.w = (x*rhs._14 + y*rhs._24 + z*rhs._34 + w*rhs._44) * RHW;
	return v;
}


Vector4& Vector4::operator *= ( Matrix44& rhs )
{
	float	RHW = 1.0f / (x*rhs._14 + y*rhs._24 + z*rhs._34 + w*rhs._44);
	if (RHW >= FLT_MAX)
	{
		*this = Vector4(0,0,0,1);
		return *this;
	}

	Vector4 v;
	v.x = (x*rhs._11 + y*rhs._21 + z*rhs._31 + w*rhs._41) * RHW;
	v.y = (x*rhs._12 + y*rhs._22 + z*rhs._32 + w*rhs._42) * RHW;
	v.z = (x*rhs._13 + y*rhs._23 + z*rhs._33 + w*rhs._43) * RHW;
	v.w = (x*rhs._14 + y*rhs._24 + z*rhs._34 + w*rhs._44) * RHW;
	*this = v;
	return *this;
}
