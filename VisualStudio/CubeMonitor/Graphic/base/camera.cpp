
#include "stdafx.h"
#include "camera.h"


using namespace graphic;


cCamera::cCamera() :
	m_eyePos(0,100,-100)
,	m_lookAt(0,0,0)
,	m_up(0,1,0)
{
	UpdateViewMatrix();

}

cCamera::cCamera(const Vector3 &eyePos, const Vector3 &lookAt, const Vector3 &up) :
	m_eyePos(eyePos)
,	m_lookAt(lookAt)
,	m_up(up)
{
	UpdateViewMatrix();
}

cCamera::~cCamera()
{
}


// 카메라 파라메터를 초기화하고, 카메라 행렬을 만든다.
void cCamera::SetCamera(const Vector3 &eyePos, const Vector3 &lookAt, const Vector3 &up)
{
	m_eyePos = eyePos;
	m_lookAt = lookAt;
	m_up = up;
	UpdateViewMatrix();
}


void cCamera::SetProjection(const float fov, const float aspect, const float nearPlane, const float farPlane)
{
	m_proj.SetProjection(fov, aspect, nearPlane, farPlane);
	UpdateProjectionMatrix();
}


// Direction 단위 벡터를 리턴한다.
Vector3 cCamera::GetDirection() const 
{
	Vector3 v = m_lookAt - m_eyePos;
	return v.Normal();
}


// Right 단위 벡터를 리턴한다.
Vector3 cCamera::GetRight() const
{
	Vector3 v = m_up.CrossProduct( GetDirection() );
	return v.Normal();
}


void cCamera::Update()
{
	
}


void cCamera::UpdateViewMatrix()
{
	m_view.SetView2(m_eyePos, m_lookAt, m_up);
	if (GetDevice())
		GetDevice()->SetTransform(D3DTS_VIEW, (D3DMATRIX*)&m_view);
}

void cCamera::UpdateProjectionMatrix()
{
	if (GetDevice())
		GetDevice()->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&m_proj);
}


// Right 축으로 회전한다.
void cCamera::Pitch( const float radian )
{
	RET(radian == 0);

	const Vector3 axis = GetRight();
	const Quaternion q( axis, radian );
	const Matrix44 mat = q.GetMatrix();

	Vector3 v = m_lookAt - m_eyePos;
	v *= mat;
	m_lookAt = m_eyePos + v;

	UpdateViewMatrix();
}


// Up 축으로 회전한다.
void cCamera::Yaw( const float radian )
{
	RET(radian == 0);

	const Vector3 axis = GetUpVector();
	const Quaternion q( axis, radian );
	const Matrix44 mat = q.GetMatrix();

	Vector3 v = m_lookAt - m_eyePos;
	v *= mat;
	m_lookAt = m_eyePos + v;

	UpdateViewMatrix();	
}


// Direction 축으로 회전한다.
void cCamera::Roll( const float radian )
{
	RET(radian == 0);

	const Vector3 axis = GetDirection();
	const Quaternion q( axis, radian );
	const Matrix44 mat = q.GetMatrix();

	Vector3 v = m_lookAt - m_eyePos;
	v *= mat;
	m_lookAt = m_eyePos + v;

	UpdateViewMatrix();	
}



// Right 축으로 회전한다.
void cCamera::Pitch2( const float radian )
{
	RET(radian == 0);

	const Vector3 axis = GetRight();
	const Quaternion q( axis, radian );
	const Matrix44 mat = q.GetMatrix();

	Vector3 v = m_eyePos - m_lookAt;
	v *= mat;
	m_eyePos = m_lookAt + v;

	UpdateViewMatrix();
}


// Up 축으로 회전한다.
void cCamera::Yaw2( const float radian )
{
	RET(radian == 0);

	const Vector3 axis = GetUpVector();
	const Quaternion q( axis, radian );
	const Matrix44 mat = q.GetMatrix();

	Vector3 v = m_eyePos - m_lookAt;
	v *= mat;
	m_eyePos = m_lookAt + v;

	UpdateViewMatrix();
}


// Direction 축으로 회전한다.
void cCamera::Roll2( const float radian )
{
	RET(radian == 0);

	const Vector3 axis = GetDirection();
	const Quaternion q( axis, radian );
	const Matrix44 mat = q.GetMatrix();

	Vector3 v = m_lookAt - m_eyePos;
	v *= mat;
	m_lookAt = m_eyePos + v;

	UpdateViewMatrix();	
}


// 앞/뒤으로 이동
void cCamera::MoveFront( const float len )
{
	const Vector3 dir = GetDirection();
	m_lookAt += dir * len;
	m_eyePos += dir * len;
	UpdateViewMatrix();
}


// 위아래 이동
void cCamera::MoveUp( const float len )
{
	const Vector3 dir = GetUpVector();
	m_lookAt += dir * len;
	m_eyePos += dir * len;
	UpdateViewMatrix();
}


// 좌우 이동.
void cCamera::MoveRight( const float len )
{
	const Vector3 dir = GetRight();
	m_lookAt += dir * len;
	m_eyePos += dir * len;
	UpdateViewMatrix();
}


// dir 방향으로 이동한다.
void cCamera::MoveAxis( const Vector3 &dir, const float len )
{
	m_lookAt += dir * len;
	m_eyePos += dir * len;
	UpdateViewMatrix();
}


// lookAt 은 고정된채로 eyePos 를 이동한다.
void cCamera::Zoom( const float len )
{
	const Vector3 dir = GetDirection();
	m_eyePos += dir * len;
	UpdateViewMatrix();
}


// lookAt - eyePos 사이 거리
float cCamera::GetDistance() const 
{
	return (m_lookAt - m_eyePos).Length();
}
