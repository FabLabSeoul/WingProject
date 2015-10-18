
#include "board.h"
#include "mw.h"
#include "vector.h"
#include "matrix44.h"
#include "quaternion.h"


void InitQuaternion(Quaternion *q, const float x0, const float y0, const float z0, const float w0)
{
	q->x = x0;
	q->y = y0;
	q->z = z0;
	q->w = w0;	
}

void InitQuaternionRot(Quaternion *q, const Vector3 *pvAxis, const float angle)
{
	float s = sinf( angle / 2.f );
	float c = cosf( angle / 2.f);
	q->x = s * pvAxis->x;
	q->y = s * pvAxis->y;
	q->z = s * pvAxis->z;
	q->w = c;	
}

void	SetQuatRotationX( Quaternion *q, const float fRadian )
{
	float s = sinf( fRadian / 2 );
	float c = cosf( fRadian / 2 );
	q->x = s * 1.0F;
	q->y = s * 0.0F;
	q->z = s * 0.0F;
	q->w = c;
}

void	SetQuatRotationY( Quaternion *q, const float fRadian )
{
	float s = sinf( fRadian / 2 );
	float c = cosf( fRadian / 2 );
	q->x = s * 0.0F;
	q->y = s * 1.0F;
	q->z = s * 0.0F;
	q->w = c;	
}

void	SetQuatRotationZ( Quaternion *q, const float fRadian )
{
	float s = sinf( fRadian / 2 );
	float c = cosf( fRadian / 2 );
	q->x = s * 0.0F;
	q->y = s * 0.0F;
	q->z = s * 1.0F;
	q->w = c;	
}


void	SetSetRotationArcRotationArc(Quaternion *q, const Vector3 *pvDir1, const Vector3 *pvDir2, const Vector3 *pnorm)
{
	Vector3 vCross;
	CrossProduct(pvDir1, pvDir2, &vCross);
	const float len = VectorLength(&vCross);
	if (len <= 0.01f)
	{
		// v0 - v1 벡터가 정확히 반대 방향이거나, 정확히 같은 방향을 가르킬때,
		// 두 벡터에 직교하는 벡터 norm 에서 180도 회전하거나, 회전하지 않거나
		// 결정한다.
		//*this = Quaternion(norm, v0.DotProduct(v1) > 0? 0 : MATH_PI);
		return;
	}

	float fDot = DotProduct(pvDir1, pvDir2);
	float s = (float)sqrtf((1.0f + fDot) * 2.0f);

	q->x = vCross.x / s;
	q->y = vCross.y / s;
	q->z = vCross.z / s;
	q->w = s * 0.5f;		
}


void QuatMultiply(Quaternion *q, const Quaternion *q0, const Quaternion *q1)
{
	q->x = q0->x * q1->w + q0->y * q1->z - q0->z * q1->y + q0->w * q1->x;
	q->y = -q0->x * q1->z + q0->y * q1->w + q0->z * q1->x + q0->w * q1->y;
	q->z = q0->x * q1->y - q0->y * q1->x + q0->z * q1->w + q0->w * q1->z;
	q->w = -q0->x * q1->x - q0->y * q1->y - q0->z * q1->z + q0->w * q1->w;
}


void QuatMultiplyMat(Quaternion *q, const Quaternion *q0, const struct _Matrix44 *pm)
{
	Quaternion q1;
	GetMatQuaternion(pm, &q1);
	QuatMultiply(q, q0, &q1);
}


void QuatGetMatrix(const Quaternion *q, struct _Matrix44 *pmat)
{
	float X = q->x;
	float Y = q->y;
	float Z = q->z;
	float W = -q->w;

	float xx = X * X;
	float xy = X * Y;
	float xz = X * Z;
	float xw = X * W;
	float yy = Y * Y;
	float yz = Y * Z;
	float yw = Y * W;
	float zz = Z * Z;
	float zw = Z * W;

	pmat->_11 = 1.0f - 2.0f * (yy + zz);
	pmat->_12 = 2.0f * (xy - zw);
	pmat->_13 = 2.0f * (xz + yw);

	pmat->_21 = 2.0f * (xy + zw);
	pmat->_22 = 1 - 2.0f * (xx + zz);
	pmat->_23 = 2.0f * (yz - xw);

	pmat->_31 = 2.0f * (xz - yw);
	pmat->_32 = 2.0f * (yz + xw);
	pmat->_33 = 1.0f - 2.0f * (xx + yy);

	pmat->_14 = pmat->_24 = pmat->_34 = 0.0f;
	pmat->_41 = pmat->_42 = pmat->_43 = 0.0f;
	pmat->_44 = 1.0f;
}


void QuatEuler(Quaternion *q, const float roll, const float pitch, const float yaw)
{
	const float MATH_PI = 3.141592654f;	
	#define ANGLE2RAD(angle) ((MATH_PI*(angle)) * (0.00555555f))

	
	float euler[ 3];
	euler[ 0] = ANGLE2RAD(roll);
	euler[ 1] = ANGLE2RAD(pitch);
	euler[ 2] = ANGLE2RAD(yaw);

    float c1 = cos(euler[2] * 0.5f);
    float c2 = cos(euler[1] * 0.5f);
    float c3 = cos(euler[0] * 0.5f);
    float s1 = sin(euler[2] * 0.5f);
    float s2 = sin(euler[1] * 0.5f);
    float s3 = sin(euler[0] * 0.5f);

    q->x = c1*c2*s3 - s1*s2*c3;
    q->y = c1*s2*c3 + s1*c2*s3;
    q->z = s1*c2*c3 - c1*s2*s3;
    q->w = c1*c2*c3 + s1*s2*s3;
}


