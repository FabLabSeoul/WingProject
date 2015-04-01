
#include "stdafx.h"
#include "triangle.h"


using namespace common;

Triangle::Triangle()
{

}

Triangle::Triangle( const Vector3& v1, const Vector3& v2, const Vector3 &v3 ):	
a(v1), b(v2), c( v3 )
{
}

// Init
void Triangle::Create( const Vector3& v1, const Vector3& v2, const Vector3& v3 )
{
	a = v1;
	b = v2;
	c = v3;
}


// Triangle::Intersect
//pfT, pfU, pfV : out
BOOL Triangle::Intersect( const Vector3& vOrig, const Vector3& vDir, float *pfT, float *pfU, float *pfV	)  const
{
	static float u, v;

	// Find vectors for two edges sharing vert0

	static Vector3	vEdge1;
	static Vector3	vEdge2;

	vEdge1 = b - a;
	vEdge2 = c - a;

	// Begin calculating determinant - also used to calculate U parameter

	static Vector3	vP;
	vP = vDir.CrossProduct( vEdge2 );

	// If determinant is near zero, ray lies in plane of triangle

	float fDet = vEdge1.DotProduct( vP );
	if( fDet < 0.0001F )
	{
		return FALSE;
	} //if

	// Calculate distance from vert0 to ray origin

	static Vector3	vT;
	vT = vOrig - a;

	// Calculate U parameter and test bounds

	u = vT.DotProduct( vP );
	if( u < 0.0F || u > fDet )
	{
		return FALSE;
	} //if

	// Prepare to test V parameter

	static Vector3 vQ;
	vQ = vT.CrossProduct( vEdge1 );

	// Calculate V parameter and test bounds

	v = vDir.DotProduct( vQ );

	if( v < 0.0F || u + v > fDet )
	{
		return FALSE;
	} //if

	// Calculate t, scale parameters, ray intersects triangle

	float fInvDet = 1.0F / fDet;

	if (pfT)	*pfT = vEdge2.DotProduct( vQ ) * fInvDet;
	if (pfU) *pfU = u * fInvDet;
	if (pfV) *pfV = v * fInvDet;

	return TRUE;
}


// vPos와 Triangle의 거리를 리턴한다.
float Triangle::Distance( const Vector3& vPos )  const
{
	Vector3 center;
	center.x = (a.x + b.x + c.x) / 3.f;
	center.y = (a.y + b.y + c.y) / 3.f;
	center.z = (a.z + b.z + c.z) / 3.f;
	return (vPos - center).Length();
}

