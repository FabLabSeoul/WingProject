
#include "stdafx.h"
#include "Math.h"
#include <D3dx9math.h>

using namespace common;


//--------------------------------
//	Constructor
//--------------------------------
Quaternion::Quaternion()
{

} //Quaternion::Quaternion
//--------------------------------
//
//--------------------------------
Quaternion::Quaternion( const float fX, const float fY, const float fZ, const float fW )
:	x	( fX ),
	y	( fY ),
	z	( fZ ),
	w	( fW )
{	
} //Quaternion

//--------------------------------
//
//--------------------------------
Quaternion::Quaternion( const Vector3& vAxis, const float fAngle )
{
	float s = sinf( fAngle / 2 );
	float c = cosf( fAngle / 2 );
	x = s * vAxis.x;
	y = s * vAxis.y;
	z = s * vAxis.z;
	w = c;
} //Quaternion::Quaternion
//--------------------------------
//
//--------------------------------
Quaternion::Quaternion( const Vector3& vDir1, const Vector3& vDir2 )
{
	SetRotationArc( vDir1, vDir2 );
} //Quaternion::Quaternion

//--------------------------------
//
//--------------------------------
Quaternion Quaternion::Interpolate( const Quaternion& qNext, const float fTime ) const
{
	Quaternion qC;
	const Quaternion&	qA = *this;
	Quaternion			qB = qNext;

	float	fCos =
		qA.x * qB.x +
		qA.y * qB.y +
		qA.z * qB.z +
		qA.w * qB.w;

	if( fCos < 0.0F )
	{	// 쿼터니언 뒤집기.
		qB.x = -qB.x;
		qB.y = -qB.y;
		qB.z = -qB.z;
		qB.w = -qB.w;
		fCos = -fCos;
	} //if

	float	fAlpha	= fTime;
	float	fBeta	= 1.0F - fTime;

	if( ( 1.0F - fCos ) > 0.001F )
	{	// 구 보간 적용.
		float	fTheta = acosf( fCos );

		fBeta	= sinf( fTheta * ( 1.0F - fTime ) ) / sinf( fTheta );
		fAlpha	= sinf( fTheta * fTime            ) / sinf( fTheta );
	} //if

	// 선형 보간.
	qC.x = fBeta * qA.x + fAlpha * qB.x;
	qC.y = fBeta * qA.y + fAlpha * qB.y;
	qC.z = fBeta * qA.z + fAlpha * qB.z;
	qC.w = fBeta * qA.w + fAlpha * qB.w;

	return qC;
} //Quaternion::Interpolate

//--------------------------------
//
//--------------------------------
Matrix44 Quaternion::GetMatrix() const
{
	Matrix44 m;
/*
	float	xx = x * x;
	float	yy = y * y;
	float	zz = z * z;
	float	ww = w * w;
	float	xy = x * y;
	float	yz = y * z;
	float	zx = z * x;
	float	wx = w * x;
	float	wy = w * y;
	float	wz = w * z;

	m._11 = 1.0F - 2.0F * ( yy + zz );
	m._12 =        2.0F * ( xy - wz );
	m._13 =        2.0F * ( zx + wy );

	m._21 =        2.0F * ( xy + wz );
	m._22 = 1.0F - 2.0F * ( xx + zz );
	m._23 =        2.0F * ( yz - wx );

	m._31 =        2.0F * ( zx - wy );
	m._32 =        2.0F * ( yz + wx );
	m._33 = 1.0F - 2.0F * ( xx + yy );

	m._14 = m._24 = m._34 = 0.0F;
	m._41 = m._42 = m._43 = 0.0F;
	m._44 = 1.0F;
/**/

	D3DXMatrixRotationQuaternion( (D3DXMATRIX*)&m, (D3DXQUATERNION*)this );
	return m;
} //Quaternion::GetMatrix4

//--------------------------------
//
//--------------------------------
Vector3 Quaternion::GetDirection() const
{
//	Matrix44		matRot = GetMatrix4();
//	return	-Vector3( matRot._31, matRot._32, matRot._33 );

	float xx = x * x;
	float yy = y * y;
//	float zz = z * z;
//	float xy = x * y;
	float yz = y * z;
	float zx = z * x;
	float wx = w * x;
	float wy = w * y;
//	float wz = w * z;

	return -Vector3( 2.0F * ( zx - wy ), 2.0F * ( yz + wx ), 1.0F - 2.0F * ( xx + yy ) ).Normal();
} //Quaternion::GetDirection

//--------------------------------
//
//--------------------------------
void Quaternion::SetRotationX( const float fRadian )
{
	float s = sinf( fRadian / 2 );
	float c = cosf( fRadian / 2 );
	x = s * 1.0F;
	y = s * 0.0F;
	z = s * 0.0F;
	w = c;
} //Quaternion::SetRotationX

//--------------------------------
//
//--------------------------------
void Quaternion::SetRotationY( const float fRadian )
{
	float s = sinf( fRadian / 2 );
	float c = cosf( fRadian / 2 );
	x = s * 0.0F;
	y = s * 1.0F;
	z = s * 0.0F;
	w = c;
} //Quaternion::SetRotationY

//--------------------------------
//
//--------------------------------
void Quaternion::SetRotationZ( const float fRadian )
{
	float s = sinf( fRadian / 2 );
	float c = cosf( fRadian / 2 );
	x = s * 0.0F;
	y = s * 0.0F;
	z = s * 1.0F;
	w = c;
} //Quaternion::SetRotationZ


//--------------------------------
//
//--------------------------------
void Quaternion::SetRotationArc( const Vector3& v0, const Vector3& v1 )
{
	Vector3 vCross = v0.CrossProduct(v1);
	const float len = vCross.Length();
	if (len <= 0.01f)
	{
		x = 0; y = 0; z = 0; w = 1;
		return;
	}

	float fDot = v0.DotProduct( v1 );
	float s = (float)sqrtf((1.0f + fDot) * 2.0f);
	//if (0.1f >  s)
	//{
	//	x = 0; y = 1; z = 0; w = 0;
	//	return;
	//}

	x = vCross.x / s;
	y = vCross.y / s;
	z = vCross.z / s;
	w = s * 0.5f;
} //Quaternion::SetRotationArc

//--------------------------------
//
//--------------------------------
void Quaternion::Normalize()
{
	float norm = sqrtf( SQR(x) + SQR(y) + SQR(z) + SQR(w) );

	if( FLOAT_EQ( 0.0F, norm ) )
	{
		return ;
	} //if

	norm = 1.0F / norm;

	x = x * norm;
	y = y * norm;
	z = z * norm;
	w = w * norm;

	norm = sqrtf( SQR(x) + SQR(y) + SQR(z) + SQR(w) );

	if( !FLOAT_EQ( 1.0F, norm ) )
	{
		return ;
	} //if

	LIMIT_RANGE(-1.0f, w, 1.0f);

	LIMIT_RANGE(-1.0f, x, 1.0f);
	LIMIT_RANGE(-1.0f, y, 1.0f);
	LIMIT_RANGE(-1.0f, z, 1.0f);
} //Quaternion::Normalize



/**
    * @brief Computes the quaternion that is equivalent to a given
    * euler angle rotation.
    * @param euler A 3-vector in order:  roll-pitch-yaw.
	* http://ai.stanford.edu/~acoates/quaternion.h
    */
void Quaternion::Euler(const Vector3& v) 
{
	float euler[ 3];
	euler[ 0] = ANGLE2RAD(v.x);
	euler[ 1] = ANGLE2RAD(v.y);
	euler[ 2] = ANGLE2RAD(v.z);

    float c1 = cos(euler[2] * 0.5f);
    float c2 = cos(euler[1] * 0.5f);
    float c3 = cos(euler[0] * 0.5f);
    float s1 = sin(euler[2] * 0.5f);
    float s2 = sin(euler[1] * 0.5f);
    float s3 = sin(euler[0] * 0.5f);

    x = c1*c2*s3 - s1*s2*c3;
    y = c1*s2*c3 + s1*c2*s3;
    z = s1*c2*c3 - c1*s2*s3;
    w = c1*c2*c3 + s1*s2*s3;
}


/** @brief Returns an equivalent euler angle representation of
	* this quaternion.
	* @return Euler angles in roll-pitch-yaw order.
	* http://ai.stanford.edu/~acoates/quaternion.h
	*/
Vector3 Quaternion::Euler(void) const 
{
	float euler[ 3];
	const static float PI_OVER_2 = MATH_PI * 0.5f;
	const static float EPSILON = MATH_EPSILON;
	float sqw, sqx, sqy, sqz;

	// quick conversion to Euler angles to give tilt to user
	sqw = w*w;
	sqx = x*x;
	sqy = y*y;
	sqz = z*z;

	euler[1] = asin(2.0f * (w*y - x*z));
	if (PI_OVER_2 - fabs(euler[1]) > EPSILON) 
	{
		euler[2] = atan2(2.0f * (x*y + w*z),
			sqx - sqy - sqz + sqw);
		euler[0] = atan2(2.0f * (w*x + y*z),
			sqw - sqx - sqy + sqz);
	} 
	else 
	{
		// compute heading from local 'down' vector
		euler[2] = atan2(2*y*z - 2*x*w,
			2*x*z + 2*y*w);
		euler[0] = 0.0f;

		// If facing down, reverse yaw
		if (euler[1] < 0)
			euler[2] = MATH_PI - euler[2];
	}

	return Vector3(euler[0], euler[1], euler[2]);
}