
#include "stdafx.h"
#include "circle.h"

using namespace graphic;

cCircle::cCircle() :
	m_radius(1)
{
}

cCircle::~cCircle()
{	
}


// 원 생성
bool cCircle::Create(const Vector3 &center, const float radius, const int slice)
{
	m_radius = radius;
	m_vtxBuff.Create(slice+1, sizeof(sVertexDiffuse), sVertexDiffuse::FVF); 

	if (sVertexDiffuse *vertices = (sVertexDiffuse*)m_vtxBuff.Lock())
	{
		vertices[ 0].p = center;
		vertices[ 0].c = D3DCOLOR_XRGB(255,255,255);

		const float inc = MATH_PI * 2.f / (float)(slice-1);
		for (int i=0; i < slice; ++i)
		{
			vertices[ i+1].p = Vector3(cosf(inc*i) * radius, sinf(inc*i) * radius, 0);
			vertices[ i+1].c = D3DCOLOR_XRGB(255,255,255);
		}
	}
	m_vtxBuff.Unlock();

	return true;
}


// 출력.
void cCircle::Render()
{
	GetDevice()->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&m_tm );

	DWORD lighting;
	GetDevice()->GetRenderState( D3DRS_LIGHTING, &lighting );
	GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
	GetDevice()->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_vtxBuff.Bind();
	GetDevice()->DrawPrimitive( D3DPT_TRIANGLEFAN, 0, m_vtxBuff.GetVertexCount());
	GetDevice()->SetRenderState( D3DRS_LIGHTING, lighting );
	GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
}
