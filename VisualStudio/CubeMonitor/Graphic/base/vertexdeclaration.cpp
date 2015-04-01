
#include "stdafx.h"
#include "vertexdeclaration.h"


using namespace graphic;


cVertexDeclaration::cVertexDeclaration()
{
}

cVertexDeclaration::~cVertexDeclaration()
{
}


bool cVertexDeclaration::Create(const sRawMesh &rawMesh )
{
	int offset = 0;

	if (!rawMesh.vertices.empty())
	{
		D3DVERTEXELEMENT9 element = {0,  offset, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0};
		m_decl.push_back( element );
		offset += 12;
	}

	if (!rawMesh.normals.empty())
	{
		D3DVERTEXELEMENT9 element =  {0, offset, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0};
		m_decl.push_back( element );
		offset += 12;
	}

	if (!rawMesh.tex.empty())
	{
		D3DVERTEXELEMENT9 element =  {0, offset, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0};
		m_decl.push_back( element );
		offset += 8;
	}

	if (!rawMesh.tangent.empty())
	{
		D3DVERTEXELEMENT9 element =  {0, offset, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0};
		m_decl.push_back( element );
		offset += 12;
	}

	if (!rawMesh.binormal.empty())
	{
		D3DVERTEXELEMENT9 element =  {0, offset, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL,  0};
		m_decl.push_back( element );
		offset += 12;
	}

	if (!rawMesh.weights.empty())
	{
		// 텍스쳐 좌표에 블랜딩 정보를 저장한다.
		// blend Weight 4
		D3DVERTEXELEMENT9 element1 =  {0, offset, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  1};
		m_decl.push_back( element1 );
		offset += 16;

		// blend Indices 4
		D3DVERTEXELEMENT9 element2 =  {0, offset, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  2};
		m_decl.push_back( element2 );
		offset += 16;
	}

	D3DVERTEXELEMENT9 endElement = D3DDECL_END();
	m_decl.push_back( endElement );

	m_elementSize = offset;

	return true;
}


int cVertexDeclaration::GetOffset( const BYTE usage, const BYTE usageIndex ) //usageIndex=0
{
	for (u_int i=0; i < m_decl.size(); ++i)
	{
		if ((usage == m_decl[ i].Usage) && (usageIndex == m_decl[ i].UsageIndex))
			return m_decl[ i].Offset;
	}
	return -1;
}
