
#include "stdafx.h"
#include "math.h"

using namespace graphic;


Box::Box() : m_box(8)
{

}


//-----------------------------------------------------------------------------//
// Box 설정
//-----------------------------------------------------------------------------//
void Box::SetBox( const Vector3 &vMin, const Vector3 &vMax )
{
	//       2-----3
	//      /|       /|
	//     0-----1 |
	//	   | 6 ---| 7
	//	   |/       |/
	//	   4-----5
	m_box[ 0] = Vector3( vMax.x, vMax.y, vMin.z );
	m_box[ 1] = Vector3( vMin.x, vMax.y, vMin.z );
	m_box[ 2] = Vector3( vMax.x, vMax.y, vMax.z );
	m_box[ 3] = Vector3( vMin.x, vMax.y, vMax.z );
	m_box[ 4] = Vector3( vMax.x, vMin.y, vMin.z );
	m_box[ 5] = Vector3( vMin.x, vMin.y, vMin.z );
	m_box[ 6] = Vector3( vMax.x, vMin.y, vMax.z );
	m_box[ 7] = Vector3( vMin.x, vMin.y, vMax.z );
	m_min = vMin;
	m_max = vMax;
	m_matWorld.SetIdentity();
}


//-----------------------------------------------------------------------------//
// 월드행렬을 멤버변수 min, max에 적용한다.
//-----------------------------------------------------------------------------//
void Box::Update()
{
	Vector3 box[ 8];
	for( int i=0; i < 8; ++i )
		box[ i] = m_box[ i] * m_matWorld;

	Vector3 _min = box[ 0];
	Vector3 _max = box[ 1];
	for( int i=0; i < 8; i++ )
	{
		Vector3 *v = &box[ i];
		if( _min.x > v->x )
			_min.x = v->x;
		else if( _max.x < v->x )
			_max.x = v->x;
		if( _min.y > v->y )
			_min.y = v->y;
		else if( _max.y < v->y )
			_max.y = v->y;
		if( _min.z > v->z )
			_min.z = v->z;
		else if( _max.z < v->z )
			_max.z = v->z;
	}
	m_min = _min;
	m_max = _max;
}


//-----------------------------------------------------------------------------//
// bOwn = TRUE : 멤버변수 m_Min, m_Max 를 리턴한다.
// bOwn = FALSE: m_Box의 최대최소를 구한후 리턴된다.
// bUpdate = TRUE : 구해진 min, max값을 멤버변수에 업데이트 한다.
//-----------------------------------------------------------------------------//
void Box::GetMinMax( OUT Vector3 &vMin, OUT Vector3 &vMax, bool isOwn, bool isUpdate )  // isOwn = TRUE, isUpdate = FALSE
{
	if (isOwn)
	{
		vMin = m_min;
		vMax = m_max;
	}
	else
	{
		// world 행렬 적용
		Vector3 _min = m_min * m_matWorld;
		Vector3 _max = m_max * m_matWorld;

		vMin = _min;
		vMax = _max;

		if (isUpdate)
		{
			m_min = _min;
			m_max = _max;
		}
	}
}


//-----------------------------------------------------------------------------//
// 충돌테스트 충돌되었다면 리턴 TRUE
// AABB 충돌테스트
//-----------------------------------------------------------------------------//
bool Box::Collision( const Box &box ) const
{
	return ((m_min.x < box.m_max.x) && (m_min.y < box.m_max.y) && (m_min.z < box.m_max.z))
		&& ((box.m_min.x < m_max.x) && (box.m_min.y < m_max.y) && (box.m_min.z < m_max.z));
}

void Box::SetWorldTM( const Matrix44 &mat ) 
{ 
	m_matWorld = mat; 
}

void Box::MultiplyWorldTM( const Matrix44 &mat )
{
	m_matWorld *= mat;
}


// length( m_Max - m_Min )
float Box::GetSize() const
{
	return sqrt( m_max.LengthRoughly(m_min) );
}


//-----------------------------------------------------------------------------//
// 출력
//-----------------------------------------------------------------------------//
void Box::Render() const
{
	GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
	GetDevice()->SetFVF( D3DFVF_XYZ );
	GetDevice()->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&m_matWorld );
	GetDevice()->DrawPrimitiveUP( D3DPT_LINESTRIP, 7, &m_box[0], sizeof(Vector3) );	
	GetDevice()->SetRenderState( D3DRS_LIGHTING, TRUE );
}
//-----------------------------------------------------------------------------//
// 계층구조출력시 사용된다.
//-----------------------------------------------------------------------------//
void Box::Render_Hierarchy() const
{
	//g_pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	//g_pDevice->SetFVF( D3DFVF_XYZ );
	//g_pDevice->MultiplyTransform( D3DTS_WORLD, (D3DXMATRIX*)&m_matWorld );
	//g_pDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, 7, m_Box, sizeof(Vector3) );	
	//g_pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}

