
#include "stdafx.h"
#include "ray.h"


using namespace common;

Ray::Ray()
{
}

Ray::Ray(const int x, const int y, const int screenWidth, const int screenHeight,  
	const Matrix44 &matProj, const Matrix44 &matView)
{
	Create(x, y, screenWidth, screenHeight, matProj, matView);
}


//-----------------------------------------------------------------------------//
// 스크린좌표 x,y로 proj, view행렬을 통해 view원점 좌표와, pick된 좌표를 얻는다.
//-----------------------------------------------------------------------------//
void Ray::Create( const int nX, const int nY, const int width, const int height,  
	const Matrix44 &matProj, const Matrix44 &matView )
{
	float x =  ( (nX * 2.0f / width  ) - 1.0f );
	float y = -( (nY * 2.0f / height) - 1.0f );

	Vector3 v;
	v.x = ( x - matProj._31 ) / matProj._11;
	v.y = ( y - matProj._32 ) / matProj._22;
	v.z =  1.0f;

	Matrix44 &m = matView.Inverse();

	dir.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
	dir.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
	dir.z = v.x * m._13 + v.y * m._23 + v.z * m._33;
	dir.Normalize();

	orig.x = m._41;
	orig.y = m._42;
	orig.z = m._43;
}
