
#pragma once



typedef struct _Vector3
{
	float x,y,z;	
	
} Vector3;


void InitVectorZero(Vector3 *pv);
void InitVector(Vector3 *pv, const float x, const float y, const float z);
Vector3 InitVector2(const float x, const float y, const float z);
bool IsVectorEmpty(const Vector3 *pv);
float VectorLength(const Vector3 *pv);
float VectorLengthRoughly(const Vector3 *pv, const Vector3 *rhs);

void VectorNormal(const Vector3 *pv, Vector3 *out);
void VectorNormalize(Vector3 *pv);
float	DotProduct( const Vector3 *pv1, const Vector3 *pv2 );
void CrossProduct( const Vector3 *pv1, const Vector3 *pv2, Vector3 *out);
Vector3 VectorInterpolate( const Vector3 *pv1, const Vector3 *pv2, const float alpha);

Vector3 VectorPlus(const Vector3 *pv1, const Vector3 *pv2);
Vector3 VectorMinus(const Vector3 *pv1, const Vector3 *pv2);
Vector3 VectorNegative(const Vector3 *pv1);
Vector3 VectorMultiplyFloat(const Vector3 *pv1, const float f);
Vector3 VectorMultiplyInt(const Vector3 *pv1, const int i);
Vector3 VectorDivideFloat(const Vector3 *pv1, const float f);
Vector3 VectorDivideInt(const Vector3 *pv1, const int i);

struct _Matrix44;
void VectorMultiplyNormal(const Vector3 *pv, const struct _Matrix44 *pmat, Vector3 *out );



//	Vector3 operator * ( const Matrix44& rhs ) const;
//	Vector3& operator *= ( const Matrix44& rhs );
