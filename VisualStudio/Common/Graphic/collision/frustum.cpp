
#include "stdafx.h"
#include "frustum.h"

using namespace graphic;

/// �������ҿ� ��Ȯ�ϰ� ���Ե��� �ʴ���, �ణ�� ������ �־ �������ҿ� ���Խ�Ű�� ���� ��
#define PLANE_EPSILON	5.0f


cFrustum::cFrustum()
:	m_fullCheck(false)
,	m_plane(6) // ����ü ��� 6��
{
}

cFrustum::~cFrustum()
{
}


//-----------------------------------------------------------------------------//
// ī�޶�(view) * ��������(projection)����� �Է¹޾� 6���� ����� �����.
//-----------------------------------------------------------------------------//
bool cFrustum::Create( const Matrix44 &matViewProj )
{
	// ������ı��� ��ġ�� ��� 3���� ������ǥ�� ���� (-1,-1,0) ~ (1,1,1)������ ������ �ٲ��.
	SetCube(Vector3(-1,-1,0), Vector3(1,1,1) );

	// view * proj�� ������� ���Ѵ�.
	Matrix44 matInv = matViewProj.Inverse();

	// Vertex_���� = Vertex_local * Matrix_world * Matrix_view * Matrix_Proj �ε�,
	// Vertex_world = Vertex_local * Matrix_world�̹Ƿ�,
	// Vertex_���� = Vertex_world * Matrix_view * Matrix_Proj �̴�.
	// Vertex_���� = Vertex_world * ( Matrix_view * Matrix_Proj ) ����
	// �����( Matrix_view * Matrix_Proj )^-1�� �纯�� ���ϸ�
	// Vertex_���� * �����( Matrix_view * Matrix_Proj )^-1 = Vertex_World �� �ȴ�.
	// �׷��Ƿ�, m_Vtx * matInv = Vertex_world�� �Ǿ�, ������ǥ���� �������� ��ǥ�� ���� �� �ִ�.
	sVertexDiffuse *vertices = (sVertexDiffuse*)m_vtxBuff.Lock();
	RETV(!vertices, false);

	m_fullCheck = false;

	for (int i = 0; i < 8; i++)
		vertices[ i].p *= matInv;

	// 2���� 5���� ���������� near����� ������ܰ� �����ϴ��̹Ƿ�, ���� ��ǥ�� ���ؼ� 2�� ������
	// ī�޶��� ��ǥ�� ���� �� �ִ�.(��Ȯ�� ��ġ�ϴ� ���� �ƴϴ�.)
	m_pos = ( vertices[2].p + vertices[5].p ) / 2.0f;

	// ����� ������ǥ�� �������� ����� �����
	// ���Ͱ� �������� ���ʿ��� �ٱ������� ������ �����̴�.
	m_plane[3].Init( vertices[ 4].p, vertices[ 5].p, vertices[ 6].p );	// �� ���(far)
	m_plane[4].Init( vertices[ 0].p, vertices[ 2].p, vertices[ 6].p );	// �� ���(left)
	m_plane[5].Init( vertices[ 1].p, vertices[ 5].p, vertices[ 7].p );	// �� ���(right)

	m_vtxBuff.Unlock();

	return TRUE;
}


//-----------------------------------------------------------------------------//
// ������ü�� minimum pos �� maximum pos �� ����ü�� �����.
//-----------------------------------------------------------------------------//
bool cFrustum::Create( const Vector3 &_min, const Vector3 &_max )
{
	SetCube(_min, _max);

	sVertexDiffuse *vertices = (sVertexDiffuse*)m_vtxBuff.Lock();
	RETV(!vertices, false);

	m_fullCheck = true;

	m_pos = (_min + _max) / 2.0f;

	// ����� ������ǥ�� �������� ����� �����
	// ���Ͱ� �������� ���ʿ��� �ٱ������� ������ �����̴�.
	m_plane[0].Init( vertices[ 0].p, vertices[ 1].p, vertices[ 2].p );	// �� ���(near)
	m_plane[1].Init( vertices[ 0].p, vertices[ 4].p, vertices[ 1].p );	// �� ���(top)
	m_plane[2].Init( vertices[ 2].p, vertices[ 3].p, vertices[ 6].p );	// �Ʒ� ���(bottom)

	m_plane[3].Init( vertices[ 4].p, vertices[ 6].p, vertices[ 7].p );	// �� ���(far)
	m_plane[4].Init( vertices[ 0].p, vertices[ 2].p, vertices[ 6].p );	// �� ���(left)
	m_plane[5].Init( vertices[ 1].p, vertices[ 5].p, vertices[ 7].p );	// �� ���(right)

	m_vtxBuff.Unlock();
	return true;
}


//-----------------------------------------------------------------------------//
// ���� point�� �������Ҿȿ� ������ TRUE�� ��ȯ, �ƴϸ� FALSE�� ��ȯ�Ѵ�.
//-----------------------------------------------------------------------------//
bool cFrustum::IsIn( const Vector3 &point )
{
	for (int i=0; i < 6; ++i)
	{
		// m_fullCheck �� false ��� near, top, bottom  ��� üũ�� ���� �ȴ�.
		if (!m_fullCheck && (i < 3))
			continue;

		const float dist = m_plane[ i].Distance( point );
		if (dist > PLANE_EPSILON) 
			return false;
	}

	return true;
}


//-----------------------------------------------------------------------------//
// �߽�(point)�� ������(radius)�� ���� ��豸(bounding sphere)�� �������Ҿȿ� ������
// TRUE�� ��ȯ, �ƴϸ� FALSE�� ��ȯ�Ѵ�.
//-----------------------------------------------------------------------------//
bool cFrustum::IsInSphere( const Vector3 &point, float radius )
{

	for (int i=0; i < 6; ++i)
	{
		// m_fullCheck �� false ��� near, top, bottom  ��� üũ�� ���� �ȴ�.
		if (!m_fullCheck && (i < 3))
			continue;

		// ���� �߽����� �Ÿ��� ���������� ũ�� �������ҿ� ����
		const float dist = m_plane[ i].Distance( point );
		if (dist > (radius+PLANE_EPSILON)) 
			return false;
	}

	return true;
}


//-----------------------------------------------------------------------------//
// ���
//-----------------------------------------------------------------------------//
void cFrustum::Render()
{
	cCube::Render(Matrix44::Identity);

	//WORD	index[] = { 0, 1, 2,
	//	0, 2, 3,
	//	4, 7, 6,
	//	4, 6, 5,
	//	1, 5, 6,
	//	1, 6, 2,
	//	0, 3, 7,
	//	0, 7, 4,
	//	0, 4, 5,
	//	0, 5, 1,
	//	3, 7, 6,
	//	3, 6, 2 };

	//D3DMATERIAL9 mtrl;
	//ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );

	//typedef struct tagVTX
	//{
	//	Vector3 p;
	//} VTX;

	//VTX vtx[8];

	//for( int i = 0 ; i < 8 ; i++ )
	//	vtx[i].p = m_vtx[i];

	//GetDevice()->SetFVF( D3DFVF_XYZ );
	//GetDevice()->SetStreamSource( 0, NULL, 0, sizeof(VTX) );
	//GetDevice()->SetTexture( 0, NULL );
	//GetDevice()->SetIndices( 0 );
	//GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	//GetDevice()->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	//GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	//GetDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
	//GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	//// �Ķ������� ��,�� ����� �׸���.
	//GetDevice()->SetRenderState( D3DRS_LIGHTING, TRUE );
	//ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
	//mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	//GetDevice()->SetMaterial( &mtrl );
	//GetDevice()->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 8, 4, index, D3DFMT_INDEX16, vtx, sizeof( vtx[0] ) );

	//// ������� ��,�� ����� �׸���.
	//ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
	//mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	//GetDevice()->SetMaterial( &mtrl );
	//GetDevice()->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 8, 4, index+4*3, D3DFMT_INDEX16, vtx, sizeof( vtx[0] ) );

	//// ���������� ��,�� ����� �׸���.
	//ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
	//mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	//GetDevice()->SetMaterial( &mtrl );
	//GetDevice()->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 8, 4, index+8*3, D3DFMT_INDEX16, vtx, sizeof( vtx[0] ) );

	//GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	//GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );

}
