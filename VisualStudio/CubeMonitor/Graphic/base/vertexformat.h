#pragma once


namespace graphic
{

	struct sVertexDiffuse
	{
		Vector3 p;
		DWORD c;
		enum {FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
	};


	struct sVertexPoint
	{
		Vector3 p;
		DWORD c;
		float size;
		enum {FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_PSIZE};
	};


	struct sVertexTex
	{
		Vector3 p;
		float u,v;

		sVertexTex() {}
		sVertexTex(float x0, float y0, float z0, float u0, float v0) : p(x0,y0,z0), u(u0), v(v0) {}
		enum {FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
	};


	struct sVertexNormDiffuse
	{
		Vector3 p;
		Vector3 n;
		DWORD c;
		enum {FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE };
	};


	struct sVertexNormTex
	{
		Vector3 p;
		Vector3 n;
		float u,v;

		enum {FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };
	};

	// 스키닝 + 범프 매핑
	struct sVertexNTBTex
	{
		Vector3 p;
		Vector3 n;
		float u,v;
		Vector3 t; // tangent;

		enum {FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2 |
			D3DFVF_TEXCOORDSIZE2(0)|		// texture
			D3DFVF_TEXCOORDSIZE3(1))};		// tangent
	};



	// vertex skinning
	// using texcoord
	struct sVertexNormTexSkin
	{
		Vector3 p;
		Vector3 n;
		float u,v;
		float weights[4];
		float matrixIndices[4];

		enum {FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX3 |
			D3DFVF_TEXCOORDSIZE2(0)|		// texture
			D3DFVF_TEXCOORDSIZE4(1)|		// blend weight
			D3DFVF_TEXCOORDSIZE4(2)) };	// blend indices
	};


	// vertex skinning
	// using D3DFVF_XYZB5 | D3DFVF_LASTBETA_D3DCOLOR
	struct sVertexNormTexSkin2
	{
		Vector3 p;
		float weights[4];
		DWORD matrixIndices;
		Vector3 n;
		float u,v;	

		enum {FVF = (D3DFVF_XYZB5 | D3DFVF_LASTBETA_D3DCOLOR | 
			D3DFVF_NORMAL | D3DFVF_TEX1)};
	};

}
