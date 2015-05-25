
#include "stdafx.h"
#include "cube2.h"

using namespace graphic;


cCube2::cCube2()
{
}

cCube2::cCube2(const Vector3 &vMin, const Vector3 &vMax )
{
	InitCube();
	SetCube(vMin, vMax);
}


void cCube2::InitCube()
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
	Vector3 normals[6] = {
		Vector3(0,0,-1), // front
		Vector3(0,0,1), // back
		Vector3(0,1,0), // top
		Vector3(0,-1,0), // bottom
		Vector3(-1,0,0), // left
		Vector3(1,0,0) // right
	};
	Vector2 uv[] = {
		Vector2(0, 0),
		Vector2(1, 0),
		Vector2(0, 1),
		Vector2(1, 1)
	};

	int normalIndices[36] = {
		// front
		0, 0, 0,
		0 ,0, 0,
		// back
		1, 1, 1,
		1, 1, 1,
		// top
		2, 2, 2,
		2, 2, 2,
		// bottom
		3, 3, 3,
		3, 3, 3,
		// left
		4, 4, 4,
		4, 4, 4,
		// right
		5, 5, 5,
		5, 5, 5,
	};

	int uvIndices[ 36] = {
		// front
		0, 3, 2,
		0 ,1, 3,
		// back
		0, 3, 2,
		0 ,1, 3,
		// top
		0, 3, 2,
		0 ,1, 3,
		// bottom
		0, 3, 2,
		0 ,1, 3,
		// left
		0, 3, 2,
		0 ,1, 3,
		// right
		0, 3, 2,
		0 ,1, 3,
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

	m_vtxBuff.Create(36, sizeof(sVertexNormTex), sVertexNormTex::FVF);
	if (sVertexNormTex *vbuff = (sVertexNormTex*)m_vtxBuff.Lock())
	{
		for (int i=0; i < 36; ++i)
		{
			vbuff[ i].p = vertices[ indices[ i]];
			vbuff[ i].n = normals[ normalIndices[ i]];
			vbuff[ i].u = uv[ uvIndices[ i]].x;
			vbuff[ i].v = uv[ uvIndices[ i]].y;
		}
		m_vtxBuff.Unlock();
	}

	m_min = Vector3(-1,-1,-1);
	m_max = Vector3(1,1,1);

	m_mtrl.InitWhite();
}


void cCube2::SetCube(const Vector3 &vMin, const Vector3 &vMax )
{
	if (m_vtxBuff.GetVertexCount() <= 0)
		InitCube();

	const Vector3 center = (vMin + vMax) / 2.f;
	const Vector3 v1 = vMin - vMax;
	const Vector3 v2 = m_max - m_min;
	Vector3 scale(sqrt(v1.x*v1.x) / sqrt(v2.x*v2.x),
		sqrt(v1.y*v1.y) / sqrt(v2.y*v2.y),
		sqrt(v1.z*v1.z) / sqrt(v2.z*v2.z));

	Matrix44 S;
	S.SetScale(scale);
	Matrix44 T;
	T.SetTranslate( center );
	Matrix44 tm = S * T;

	sVertexNormTex *vbuff = (sVertexNormTex*)m_vtxBuff.Lock();
	for (int i=0; i < m_vtxBuff.GetVertexCount(); ++i)
		vbuff[ i].p *= tm;
	m_vtxBuff.Unlock();

	m_min = vMin;
	m_max = vMax;
}


void cCube2::SetCube(const cCube2 &cube)
{
	SetCube(cube.GetMin(), cube.GetMax());
	m_tm = cube.GetTransform();
}


void cCube2::Render(const Matrix44 &tm)
{
	//DWORD cullMode;
	//DWORD fillMode;
	//DWORD lightMode;
	//GetDevice()->GetRenderState(D3DRS_CULLMODE, &cullMode);
	//GetDevice()->GetRenderState(D3DRS_FILLMODE, &fillMode);
	//GetDevice()->GetRenderState(D3DRS_LIGHTING, &lightMode);

	//GetDevice()->SetRenderState(D3DRS_CULLMODE, FALSE);
	//GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
	GetDevice()->SetTexture(0, NULL);

	Matrix44 mat = m_tm * tm;
	GetDevice()->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&mat );
	m_mtrl.Bind();
	m_vtxBuff.Bind();
	GetDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 12);
	//GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 
	//	m_vtxBuff.GetVertexCount(), 0, 12);

	//GetDevice()->SetRenderState(D3DRS_CULLMODE, cullMode);
	//GetDevice()->SetRenderState(D3DRS_FILLMODE, fillMode );
	//GetDevice()->SetRenderState( D3DRS_LIGHTING, lightMode );
}
