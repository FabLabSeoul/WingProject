
#include "stdafx.h"
#include "billboard.h"


using namespace graphic;


cBillboard::cBillboard()
{
}

cBillboard::~cBillboard()
{
}


// 빌보드 초기화.
bool cBillboard::Create( const BILLBOARD_TYPE::TYPE type, 
	const float width, const float height, 
	const Vector3 &pos, const string &textureFileName )
{
	if (!__super::Create(width, height, pos, textureFileName))
		return false;

	m_type = type;
	return true;
}


// 화면에 출력.
void cBillboard::Render()
{
	Matrix44 mat;
	const Matrix44 view = cMainCamera::Get()->GetViewMatrix();

	switch (m_type)
	{
	case BILLBOARD_TYPE::Y_AXIS:
		// Y축 빌보드 행렬을 계산한다.
		mat._11 = view._11;
		mat._13 = view._13;
		mat._31 = view._31;
		mat._33 = view._33;
		break;

	case BILLBOARD_TYPE::ALL_AXIS:
		// 모든 축에서 빌보드 행렬을 계산한다.
		mat = view;
		mat._41 = mat._42 = mat._43 = 0;
		break;
	}

	Matrix44 T;
	T.SetTranslate(GetTransform().GetPosition());
	SetTransform(mat.Transpose() * T);
	__super::Render();
}
