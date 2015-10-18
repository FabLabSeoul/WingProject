
#include "board.h"
#include "mw.h"
#include "vector.h"
#include "matrix44.h"



void InitVectorZero(Vector3 *pv)
{
	pv->x = 0;
	pv->y = 0;
	pv->z = 0;
}

void InitVector(Vector3 *pv, const float x, const float y, const float z)
{
	pv->x = x;
	pv->y = y;
	pv->z = z;
}

Vector3 InitVector2(const float x, const float y, const float z)
{
	Vector3 v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;	
}

bool IsVectorEmpty(const Vector3 *pv) 
{
	return (pv->x==0) && (pv->y==0) && (pv->z==0);
}


float VectorLength(const Vector3 *pv) 
{
	return sqrt(pv->x*pv->x + pv->y*pv->y + pv->z*pv->z);
}


float VectorLengthRoughly(const Vector3 *pv, const Vector3 *rhs) 
{
	Vector3 v;
	v.x = pv->x - rhs->x;
	v.y = pv->y - rhs->y;
	v.z = pv->z - rhs->z;
	
	return v.x*v.x + v.y*v.y + v.z*v.z;
}


void VectorNormal(const Vector3 *pv, Vector3 *out)
{
	const float len = VectorLength(pv);
	
	out->x = 0;
	out->y = 0;
	out->z = 0;
	
	if (len < 0.001f)
		return;
	
	out->x = pv->x / len;
	out->y = pv->y / len;
	out->z = pv->z / len;	
}


void VectorNormalize(Vector3 *pv)
{
	Vector3 v;
	VectorNormal(pv, &v);
	*pv = v;
}


float	DotProduct( const Vector3 *pv1, const Vector3 *pv2 )
{
	return pv1->x * pv2->x + pv1->y * pv2->y + pv1->z * pv2->z;
}


void CrossProduct(const Vector3 *pv1, const Vector3 *pv2, Vector3 *out )
{
	out->x = (pv1->y * pv2->z) - (pv1->z * pv2->y); 
	out->y = (pv1->z * pv2->x) - (pv1->x * pv2->z); 
	out->z = (pv1->x * pv2->y) - (pv1->y * pv2->x);
}


//	Vector3 MultiplyNormal( const Matrix44& rhs ) const;
Vector3 VectorInterpolate( const Vector3 *pv1, const Vector3 *pv2, const float alpha)
{
	Vector3 v;
	v.x = pv1->x + (alpha * ( pv2->x - pv1->x ) );
	v.y = pv1->y + (alpha * ( pv2->y - pv1->y ) );
	v.z = pv1->z + (alpha * ( pv2->z - pv1->z ) );
	return v;
}


Vector3 VectorPlus(const Vector3 *pv1, const Vector3 *pv2)
{
	Vector3 v;
	v.x = pv1->x + pv2->x;
	v.y = pv1->y + pv2->y;
	v.z = pv1->z + pv2->z;
	return v;
}


Vector3 VectorMinus(const Vector3 *pv1, const Vector3 *pv2)
{
	Vector3 v;
	v.x = pv1->x - pv2->x;
	v.y = pv1->y - pv2->y;
	v.z = pv1->z - pv2->z;
	return v;	
}


Vector3 VectorNegative(const Vector3 *pv1)
{
	Vector3 v;
	v.x = -pv1->x;
	v.y = -pv1->y;
	v.z = -pv1->z;
	return v;
}


Vector3 VectorMultiplyFloat(const Vector3 *pv1, const float f)
{
	Vector3 v;
	v.x = pv1->x * f;
	v.y = pv1->y * f;
	v.z = pv1->z * f;
	return v;
}


Vector3 VectorMultiplyInt(const Vector3 *pv1, const int i)
{
	Vector3 v;
	v.x = pv1->x * i;
	v.y = pv1->y * i;
	v.z = pv1->z * i;
	return v;
}


Vector3 VectorDivideFloat(const Vector3 *pv1, const float f)
{
	Vector3 v;
	v.x = pv1->x / f;
	v.y = pv1->y / f;
	v.z = pv1->z / f;
	return v;
}


Vector3 VectorDivideInt(const Vector3 *pv1, const int i)
{
	Vector3 v;
	v.x = pv1->x / i;
	v.y = pv1->y / i;
	v.z = pv1->z / i;
	return v;
}

void VectorMultiplyNormal(const Vector3 *pv, const struct _Matrix44 *pmat, Vector3 *out )
{
	out->x = pv->x * pmat->_11 + pv->y * pmat->_21 + pv->z * pmat->_31;
	out->y = pv->x * pmat->_12 + pv->y * pmat->_22 + pv->z * pmat->_32;
	out->z = pv->x * pmat->_13 + pv->y * pmat->_23 + pv->z * pmat->_33;
	VectorNormalize(out);	
}

