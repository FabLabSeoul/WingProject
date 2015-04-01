
#include "stdafx.h"
#include "quad.h"


using namespace graphic;


cQuad::cQuad()
	: m_texture(NULL)
{
	m_material.InitWhite();
}

cQuad::~cQuad()
{
}


// 쿼드를 초기화 한다.
// width, height : 쿼드 크기
// pos : 쿼드 위치
bool cQuad::Create( const float width, const float height, 
	const Vector3 &pos, 
	const string &textureFileName // = " "
	)
{
	if (m_vtxBuff.GetVertexCount() <= 0)
	{
		m_vtxBuff.Create(4, sizeof(sVertexNormTex), sVertexNormTex::FVF);
	}

	sVertexNormTex *vertices = (sVertexNormTex*)m_vtxBuff.Lock();
	RETV(!vertices, false);

	vertices[ 0].p = Vector3(-width/2.f, height/2.f, 0);
	vertices[ 1].p = Vector3(width/2.f, height/2.f, 0);
	vertices[ 2].p = Vector3(-width/2.f,-height/2.f, 0);
	vertices[ 3].p = Vector3(width/2.f,- height/2.f, 0);

	vertices[ 0].n = Vector3(0, 0, -1);
	vertices[ 1].n = Vector3(0, 0, -1);
	vertices[ 2].n = Vector3(0, 0, -1);
	vertices[ 3].n = Vector3(0, 0, -1);

	vertices[ 0].u = 0;
	vertices[ 0].v = 0;
	vertices[ 1].u = 1;
	vertices[ 1].v = 0;
	vertices[ 2].u = 0;
	vertices[ 2].v = 1;
	vertices[ 3].u = 1;
	vertices[ 3].v = 1;

	m_vtxBuff.Unlock();

	if (!textureFileName.empty())
		m_texture = cResourceManager::Get()->LoadTexture(textureFileName);

	m_tm.SetTranslate(pos);

	return true;
}


void cQuad::Render()
{
	GetDevice()->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&m_tm );
	m_material.Bind();
	if (m_texture)
		m_texture->Bind(0);
	GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	GetDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_vtxBuff.RenderTriangleStrip();
	GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
}


void cQuad::RenderLine()
{
	DWORD flag;
	GetDevice()->GetRenderState(D3DRS_FILLMODE, &flag);
	GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	Render();
	GetDevice()->SetRenderState(D3DRS_FILLMODE, flag);
}
