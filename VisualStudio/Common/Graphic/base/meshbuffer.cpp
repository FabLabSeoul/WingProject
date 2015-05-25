
#include "stdafx.h"
#include "meshbuffer.h"
#include "vertexdeclaration.h"


using namespace graphic;

cMeshBuffer::cMeshBuffer()
:	m_offset(0)
{
}

cMeshBuffer::cMeshBuffer(const sRawMesh &rawMesh)
:	m_offset(0)
{
	CreateMesh(rawMesh);
	CreateAttributes(rawMesh);
}

cMeshBuffer::cMeshBuffer(const sRawBone &rawBone)
:	m_offset(0)
{
	CreateMesh(rawBone.vertices, rawBone.normals, rawBone.tex, rawBone.indices);
}


void cMeshBuffer::Bind()
{
	m_idxBuff.Bind();
	m_vtxBuff.Bind();
}


void cMeshBuffer::CreateMesh( const vector<Vector3> &vertices, 
	const vector<Vector3> &normals, 
	const vector<Vector3> &tex,
	const vector<int> &indices)
{
	const bool isTexture = !tex.empty();

	// 버텍스 버퍼 생성
	if (m_vtxBuff.Create(vertices.size(), sizeof(sVertexNormTex), sVertexNormTex::FVF))
	{
		if (sVertexNormTex* pv = (sVertexNormTex*)m_vtxBuff.Lock())
		{
			sMinMax minMax;
			for (u_int i = 0; i < vertices.size(); i++)
			{
				pv[ i].p = vertices[ i];
				pv[ i].n = normals[ i];
				if (isTexture)
				{
					pv[ i].u = tex[ i].x;
					pv[ i].v = tex[ i].y;
				}

				minMax.Update(pv[ i].p);
			}
			m_vtxBuff.Unlock();

			// 경계박스 초기화.
			m_boundingBox.SetBoundingBox(minMax._min, minMax._max);
		}
	}	


	// 인덱스 버퍼 생성.
	if (m_idxBuff.Create(indices.size()))
	{
		WORD *pi = (WORD*)m_idxBuff.Lock();
		for (u_int i = 0; i < indices.size(); ++i)
			pi[ i] = indices[ i];
		m_idxBuff.Unlock();
	}
}


void cMeshBuffer::CreateMesh( const sRawMesh &rawMesh )
{
	const bool isTexture = !rawMesh.tex.empty();

	cVertexDeclaration decl;
	decl.Create( rawMesh );


	// 버텍스 버퍼 생성
	if (m_vtxBuff.Create(rawMesh.vertices.size(), decl.GetElementSize(), decl))
	{
		if (BYTE* pv = (BYTE*)m_vtxBuff.Lock())
		{
			sMinMax minMax;
			const int pos_offset = decl.GetOffset(D3DDECLUSAGE_POSITION);
			for (u_int i = 0; i < rawMesh.vertices.size(); i++)
			{
				BYTE *vertices = pv + (decl.GetElementSize() * i);
				Vector3 *position = (Vector3*)(vertices + pos_offset);
				*position = rawMesh.vertices[ i];
				minMax.Update(rawMesh.vertices[ i]);
			}
			// 경계박스 초기화.
			m_boundingBox.SetBoundingBox(minMax._min, minMax._max);

			// normal
			const int norm_offset = decl.GetOffset(D3DDECLUSAGE_NORMAL);
			for (u_int i = 0; i < rawMesh.normals.size(); i++)
			{
				BYTE *vertices = pv + (decl.GetElementSize() * i);
				Vector3 *normal = (Vector3*)(vertices + norm_offset);
				*normal = rawMesh.normals[ i];
			}

			// texture
			const int tex_offset = decl.GetOffset(D3DDECLUSAGE_TEXCOORD);
			for (u_int i = 0; i < rawMesh.tex.size(); i++)
			{
				BYTE *vertices = pv + (decl.GetElementSize() * i);
				Vector2 *tex = (Vector2*)(vertices + tex_offset);
				tex->x = rawMesh.tex[ i].x;
				tex->y = rawMesh.tex[ i].y;
			}

			// tagent
			const int tangent_offset = decl.GetOffset(D3DDECLUSAGE_TANGENT);
			for (u_int i = 0; i < rawMesh.tangent.size(); i++)
			{
				BYTE *vertices = pv + (decl.GetElementSize() * i);
				Vector3 *tangent = (Vector3*)(vertices + tangent_offset);
				*tangent = rawMesh.tangent[ i];
			}

			// binormal
			const int binormal_offset = decl.GetOffset(D3DDECLUSAGE_BINORMAL);
			for (u_int i = 0; i < rawMesh.binormal.size(); i++)
			{
				BYTE *vertices = pv + (decl.GetElementSize() * i);
				Vector3 *binormal = (Vector3*)(vertices + binormal_offset);
				*binormal = rawMesh.binormal[ i];
			}

			// bone weight
			const int blendWeight_offset = decl.GetOffset(D3DDECLUSAGE_TEXCOORD, 1);
			const int blendIndices_offset = decl.GetOffset(D3DDECLUSAGE_TEXCOORD, 2);
			for (u_int i = 0; i < rawMesh.weights.size(); i++)
			{
				BYTE *vertices = pv + (decl.GetElementSize() * i);
				float *vtxWeight = (float*)(vertices + blendWeight_offset); // float4
				float *vtxIndices = (float*)(vertices + blendIndices_offset); // float4
				
				const sVertexWeight &weight = rawMesh.weights[ i];
				const int vtxIdx = weight.vtxIdx;

				ZeroMemory(vtxWeight, sizeof(float)*4);
				ZeroMemory(vtxIndices,  sizeof(float)*4);
				//pv[ vtxIdx].matrixIndices = 0;

				for (int k=0; (k < weight.size) && (k < 4); ++k)
				{
					const sWeight *w = &weight.w[ k];
					if (k < 3)
					{
						vtxWeight[ k] = w->weight;
					}
					else // k == 3 (마지막 가중치)
					{
						vtxWeight[ k] = 
							1.f - (vtxWeight[ 0] +vtxWeight[ 1] + vtxWeight[ 2]);
					}

					vtxIndices[ k] = (float)w->bone;
					//const int boneIdx = (w->bone << (8*(3-k)));
					//pv[ vtxIdx].matrixIndices |= boneIdx;
				}
			}

			m_vtxBuff.Unlock();
		}
	}	

	// 인덱스 버퍼 생성.
	if (m_idxBuff.Create(rawMesh.indices.size()))
	{
		WORD *pi = (WORD*)m_idxBuff.Lock();
		for (u_int i = 0; i < rawMesh.indices.size(); ++i)
			pi[ i] = rawMesh.indices[ i];
		m_idxBuff.Unlock();
	}
}


// 속성버퍼 초기화.
void cMeshBuffer::CreateAttributes(const sRawMesh &rawMesh)
{
	m_attributes = rawMesh.attributes;
}


// 인스턴스를 깊이복사해서 리턴한다.
cMeshBuffer* cMeshBuffer::Clone()
{
	cMeshBuffer *mesh = new cMeshBuffer();
	mesh->m_attributes = m_attributes;
	mesh->m_vtxBuff = m_vtxBuff;
	mesh->m_idxBuff = m_idxBuff;
	return mesh;
}


// 출력. 
void cMeshBuffer::Render(const int faceStart, const int faceCount)
	//faceStart=0, faceCount=0
{
	GetDevice()->DrawIndexedPrimitive( 
		D3DPT_TRIANGLELIST, 
		m_offset, 
		0, 
		m_vtxBuff.GetVertexCount(), 
		faceStart, 
		(faceCount==0)? m_idxBuff.GetFaceCount() : faceCount );
}
