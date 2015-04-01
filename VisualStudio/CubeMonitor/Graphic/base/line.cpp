
#include "stdafx.h"
#include "line.h"

using namespace graphic;

cLine::cLine()
{
	m_material.InitWhite();
}

cLine::cLine(const Vector3 &p0, const Vector3 &p1, const float width)
{
	SetLine(p0, p1, width);
	m_material.InitWhite();
}


void cLine::Render()
{
	GetDevice()->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&m_tm );
	m_material.Bind();
	m_vtxBuff.Bind();
	m_idxBuff.Bind();
	GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 
		m_vtxBuff.GetVertexCount(), 0, 12);
}


void cLine::SetLine(const Vector3 &p0, const Vector3 &p1, const float width)
{
	m_p0 = p0;
	m_p1 = p1;
	m_width = width;

	InitCube();

	Vector3 v = p1 - p0;
	const float len = v.Length();
	v.Normalize();

	Matrix44 matS;
	matS.SetScale(Vector3(width, width, len/2.f));

	Matrix44 matT;
	matT.SetTranslate( (p0 + p1) / 2.f );

	Quaternion q(Vector3(0,0,1), v);
	m_tm = matS * q.GetMatrix() * matT;
}


void cLine::InitCube()
{
	if (m_vtxBuff.GetVertexCount() > 0)
		return;

	//        4 --- 5
	//      / |  |  /|
	//   0 --- 1   |
	//   |   6-|- -7
	//   | /     | /
	//   2 --- 3
	//
	Vector3 vertices[8] = {
		Vector3(-1,1,-1), Vector3(1,1,-1), Vector3(-1,-1,-1), Vector3(1,-1,-1),
		Vector3(-1,1, 1), Vector3(1,1, 1), Vector3(-1,-1,1), Vector3(1,-1,1),
	};
	WORD indices[36] = {
		// front
		0, 3, 2,
		0 ,1, 3,
		// back
		5, 6, 7,
		5, 4, 6,
		// top
		4, 1, 0,
		4, 5, 1,
		// bottom
		2, 7, 6,
		2, 3, 7,
		// left
		4, 2, 6,
		4, 0, 2,
		// right
		1, 7, 3,
		1, 5, 7,
	};

	m_vtxBuff.Create(8, sizeof(sVertexDiffuse), sVertexDiffuse::FVF);
	m_idxBuff.Create(12);

	sVertexDiffuse *vbuff = (sVertexDiffuse*)m_vtxBuff.Lock();
	WORD *ibuff = (WORD*)m_idxBuff.Lock();

	for (int i=0; i < 8; ++i)
		vbuff[ i].p = vertices[ i];

	for (int i=0; i < 36; ++i)
		ibuff[ i] = indices[ i];

	m_vtxBuff.Unlock();
	m_idxBuff.Unlock();
}
