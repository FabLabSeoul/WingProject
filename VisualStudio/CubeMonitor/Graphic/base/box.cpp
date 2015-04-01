
#include "stdafx.h"
#include "math.h"

using namespace graphic;


Box::Box() : m_box(8)
{

}


//-----------------------------------------------------------------------------//
// Box ����
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
// ��������� ������� min, max�� �����Ѵ�.
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
// bOwn = TRUE : ������� m_Min, m_Max �� �����Ѵ�.
// bOwn = FALSE: m_Box�� �ִ��ּҸ� ������ ���ϵȴ�.
// bUpdate = TRUE : ������ min, max���� ��������� ������Ʈ �Ѵ�.
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
		// world ��� ����
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
// �浹�׽�Ʈ �浹�Ǿ��ٸ� ���� TRUE
// AABB �浹�׽�Ʈ
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
// ���
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
// ����������½� ���ȴ�.
//-----------------------------------------------------------------------------//
void Box::Render_Hierarchy() const
{
	//g_pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	//g_pDevice->SetFVF( D3DFVF_XYZ );
	//g_pDevice->MultiplyTransform( D3DTS_WORLD, (D3DXMATRIX*)&m_matWorld );
	//g_pDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, 7, m_Box, sizeof(Vector3) );	
	//g_pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}

