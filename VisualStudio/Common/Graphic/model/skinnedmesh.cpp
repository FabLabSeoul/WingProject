
#include "stdafx.h"
#include "skinnedmesh.h"

using namespace graphic;


cSkinnedMesh::cSkinnedMesh(const int id, vector<Matrix44> *palette, const sRawMesh &raw) : 
	cMesh(id, raw)
,	m_rawMesh(raw)
,	m_palette(palette)
,	m_skinnMeshBuffer(NULL)
{

	// 셰이더스키닝을 못하는 상황일 때는 소프트웨어 스키닝을 
	// 하기 위해서 버퍼를 동적으로 생성한다.
	if (m_palette && (64 < m_palette->size()))
	{
		m_skinnMeshBuffer = new cMeshBuffer(raw);
		SetMeshBuffer(m_skinnMeshBuffer);
	}

}

cSkinnedMesh::~cSkinnedMesh()
{
	SAFE_DELETE(m_skinnMeshBuffer);
}


void cSkinnedMesh::Render(const Matrix44 &parentTm)
{
	if (m_shader && m_palette && (m_palette->size() < 64))
	{
		ApplyPaletteShader(*m_shader);
		cMesh::RenderShader(*m_shader, parentTm);
	}
	else
	{
		ApplyPalette();
		cMesh::Render(parentTm);
	}
}

void cSkinnedMesh::RenderShader( cShader &shader, const Matrix44 &parentTm )
{
	ApplyPaletteShader(shader);
	cMesh::RenderShader(shader, parentTm);
}


void cSkinnedMesh::RenderShadow(cShader &shader, const Matrix44 &parentTm)
{
	ApplyPaletteShader(shader);
	cMesh::RenderShadow(shader, parentTm);
}


// 팔레트 적용.
void cSkinnedMesh::ApplyPalette()
{
	RET(!m_palette);

	// 소프트웨어 스키닝일 때 동적으로 할당된 버퍼가 없다면 새로 생성한다.
	if (!m_skinnMeshBuffer)
	{
		m_skinnMeshBuffer = new cMeshBuffer(m_rawMesh);
		SetMeshBuffer(m_skinnMeshBuffer);
	}


	cVertexBuffer &vtxBuffer = m_skinnMeshBuffer->GetVertexBuffer();
	const int pos_offset = vtxBuffer.GetOffset(D3DDECLUSAGE_POSITION);
	const int normal_offset = vtxBuffer.GetOffset(D3DDECLUSAGE_NORMAL);

	if (BYTE *vertices = (BYTE*)vtxBuffer.Lock())
	{
		BOOST_FOREACH (const sVertexWeight &weight, m_rawMesh.weights)
		{
			const int vtxIdx = weight.vtxIdx;
			Vector3 p(0,0,0);
			Vector3 n(0,0,0);

			switch (weight.size)
			{
			case 5:
				p += (m_rawMesh.vertices[ vtxIdx] * (*m_palette)[ weight.w[ 4].bone]) * weight.w[ 4].weight;
				n += m_rawMesh.normals[ vtxIdx].MultiplyNormal( (*m_palette)[ weight.w[ 4].bone]) * weight.w[ 4].weight;
			case 4:
				p += (m_rawMesh.vertices[ vtxIdx] * (*m_palette)[ weight.w[ 3].bone]) * weight.w[ 3].weight;
				n += m_rawMesh.normals[ vtxIdx].MultiplyNormal( (*m_palette)[ weight.w[ 3].bone]) * weight.w[ 3].weight;
			case 3:
				p += (m_rawMesh.vertices[ vtxIdx] * (*m_palette)[ weight.w[ 2].bone]) * weight.w[ 2].weight;
				n += m_rawMesh.normals[ vtxIdx].MultiplyNormal( (*m_palette)[ weight.w[ 2].bone]) * weight.w[ 2].weight;
			case 2: 
				p += (m_rawMesh.vertices[ vtxIdx] * (*m_palette)[ weight.w[ 1].bone]) * weight.w[ 1].weight;
				n += m_rawMesh.normals[ vtxIdx].MultiplyNormal( (*m_palette)[ weight.w[ 1].bone]) * weight.w[ 1].weight;
			case 1: 
				p += (m_rawMesh.vertices[ vtxIdx] * (*m_palette)[ weight.w[ 0].bone]) * weight.w[ 0].weight;
				n += m_rawMesh.normals[ vtxIdx].MultiplyNormal( (*m_palette)[ weight.w[ 0].bone]) * weight.w[ 0].weight;
			}

			Vector3 *pv = (Vector3*)(vertices + (vtxBuffer.GetSizeOfVertex() * vtxIdx) + pos_offset);
			Vector3 *pn = (Vector3*)(vertices + (vtxBuffer.GetSizeOfVertex() * vtxIdx) + normal_offset);
			*pv = p;
			*pn = n;
		}
	}

	vtxBuffer.Unlock();
}


// 팔레트 적용.
void cSkinnedMesh::ApplyPaletteShader(cShader &shader)
{
	RET(!m_palette);

	const int paletteSize = min(64, m_palette->size());
	shader.SetMatrixArray("mPalette", (Matrix44*)&(*m_palette)[0], paletteSize);
}
