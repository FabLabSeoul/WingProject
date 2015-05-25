
#include "stdafx.h"
#include "frustum.h"

using namespace graphic;

/// 프러스텀에 정확하게 포함되지 않더라도, 약간의 여분을 주어서 프러스텀에 포함시키기 위한 값
#define PLANE_EPSILON	5.0f


cFrustum::cFrustum()
:	m_fullCheck(false)
,	m_plane(6) // 절두체 평면 6개
{
}

cFrustum::~cFrustum()
{
}


//-----------------------------------------------------------------------------//
// 카메라(view) * 프로젝션(projection)행렬을 입력받아 6개의 평면을 만든다.
//-----------------------------------------------------------------------------//
bool cFrustum::Create( const Matrix44 &matViewProj )
{
	// 투영행렬까지 거치면 모든 3차원 월드좌표의 점은 (-1,-1,0) ~ (1,1,1)사이의 값으로 바뀐다.
	SetCube(Vector3(-1,-1,0), Vector3(1,1,1) );

	// view * proj의 역행렬을 구한다.
	Matrix44 matInv = matViewProj.Inverse();

	// Vertex_최종 = Vertex_local * Matrix_world * Matrix_view * Matrix_Proj 인데,
	// Vertex_world = Vertex_local * Matrix_world이므로,
	// Vertex_최종 = Vertex_world * Matrix_view * Matrix_Proj 이다.
	// Vertex_최종 = Vertex_world * ( Matrix_view * Matrix_Proj ) 에서
	// 역행렬( Matrix_view * Matrix_Proj )^-1를 양변에 곱하면
	// Vertex_최종 * 역행렬( Matrix_view * Matrix_Proj )^-1 = Vertex_World 가 된다.
	// 그러므로, m_Vtx * matInv = Vertex_world가 되어, 월드좌표계의 프러스텀 좌표를 얻을 수 있다.
	sVertexDiffuse *vertices = (sVertexDiffuse*)m_vtxBuff.Lock();
	RETV(!vertices, false);

	m_fullCheck = false;

	for (int i = 0; i < 8; i++)
		vertices[ i].p *= matInv;

	// 2번과 5번은 프러스텀중 near평면의 좌측상단과 우측하단이므로, 둘의 좌표를 더해서 2로 나누면
	// 카메라의 좌표를 얻을 수 있다.(정확히 일치하는 것은 아니다.)
	m_pos = ( vertices[2].p + vertices[5].p ) / 2.0f;

	// 얻어진 월드좌표로 프러스텀 평면을 만든다
	// 벡터가 프러스텀 안쪽에서 바깥쪽으로 나가는 평면들이다.
	m_plane[3].Init( vertices[ 4].p, vertices[ 5].p, vertices[ 6].p );	// 원 평면(far)
	m_plane[4].Init( vertices[ 0].p, vertices[ 2].p, vertices[ 6].p );	// 좌 평면(left)
	m_plane[5].Init( vertices[ 1].p, vertices[ 5].p, vertices[ 7].p );	// 우 평면(right)

	m_vtxBuff.Unlock();

	return TRUE;
}


//-----------------------------------------------------------------------------//
// 정육면체의 minimum pos 와 maximum pos 로 절두체를 만든다.
//-----------------------------------------------------------------------------//
bool cFrustum::Create( const Vector3 &_min, const Vector3 &_max )
{
	SetCube(_min, _max);

	sVertexDiffuse *vertices = (sVertexDiffuse*)m_vtxBuff.Lock();
	RETV(!vertices, false);

	m_fullCheck = true;

	m_pos = (_min + _max) / 2.0f;

	// 얻어진 월드좌표로 프러스텀 평면을 만든다
	// 벡터가 프러스텀 안쪽에서 바깥쪽으로 나가는 평면들이다.
	m_plane[0].Init( vertices[ 0].p, vertices[ 1].p, vertices[ 2].p );	// 근 평면(near)
	m_plane[1].Init( vertices[ 0].p, vertices[ 4].p, vertices[ 1].p );	// 윗 평면(top)
	m_plane[2].Init( vertices[ 2].p, vertices[ 3].p, vertices[ 6].p );	// 아래 평면(bottom)

	m_plane[3].Init( vertices[ 4].p, vertices[ 6].p, vertices[ 7].p );	// 원 평면(far)
	m_plane[4].Init( vertices[ 0].p, vertices[ 2].p, vertices[ 6].p );	// 좌 평면(left)
	m_plane[5].Init( vertices[ 1].p, vertices[ 5].p, vertices[ 7].p );	// 우 평면(right)

	m_vtxBuff.Unlock();
	return true;
}


//-----------------------------------------------------------------------------//
// 한점 point가 프러스텀안에 있으면 TRUE를 반환, 아니면 FALSE를 반환한다.
//-----------------------------------------------------------------------------//
bool cFrustum::IsIn( const Vector3 &point )
{
	for (int i=0; i < 6; ++i)
	{
		// m_fullCheck 가 false 라면 near, top, bottom  평면 체크는 제외 된다.
		if (!m_fullCheck && (i < 3))
			continue;

		const float dist = m_plane[ i].Distance( point );
		if (dist > PLANE_EPSILON) 
			return false;
	}

	return true;
}


//-----------------------------------------------------------------------------//
// 중심(point)와 반지름(radius)를 갖는 경계구(bounding sphere)가 프러스텀안에 있으면
// TRUE를 반환, 아니면 FALSE를 반환한다.
//-----------------------------------------------------------------------------//
bool cFrustum::IsInSphere( const Vector3 &point, float radius )
{

	for (int i=0; i < 6; ++i)
	{
		// m_fullCheck 가 false 라면 near, top, bottom  평면 체크는 제외 된다.
		if (!m_fullCheck && (i < 3))
			continue;

		// 평면과 중심점의 거리가 반지름보다 크면 프러스텀에 없음
		const float dist = m_plane[ i].Distance( point );
		if (dist > (radius+PLANE_EPSILON)) 
			return false;
	}

	return true;
}


//-----------------------------------------------------------------------------//
// 출력
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

	//// 파란색으로 상,하 평면을 그린다.
	//GetDevice()->SetRenderState( D3DRS_LIGHTING, TRUE );
	//ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
	//mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	//GetDevice()->SetMaterial( &mtrl );
	//GetDevice()->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 8, 4, index, D3DFMT_INDEX16, vtx, sizeof( vtx[0] ) );

	//// 녹색으로 좌,우 평면을 그린다.
	//ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
	//mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	//GetDevice()->SetMaterial( &mtrl );
	//GetDevice()->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 8, 4, index+4*3, D3DFMT_INDEX16, vtx, sizeof( vtx[0] ) );

	//// 붉은색으로 원,근 평면을 그린다.
	//ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
	//mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	//GetDevice()->SetMaterial( &mtrl );
	//GetDevice()->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 8, 4, index+8*3, D3DFMT_INDEX16, vtx, sizeof( vtx[0] ) );

	//GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	//GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );

}
