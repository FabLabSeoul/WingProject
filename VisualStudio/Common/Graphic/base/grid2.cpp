
#include "stdafx.h"
#include "grid2.h"
#include <fstream>


namespace graphic
{

	// cGrid 클래스를 파일에 저장하고 로드할 때 사용된다.
	struct sGridBinary
	{
		char header[3]; // GRD
		char version;
		int rowCellCount;
		int colCellCount;
		float cellSize;
		float textureUVFactor;
		float *heightMap; // (rowCellCount+1) * (colCellCount+1) 개수 만큰 저장된다.
									 // 높이 값을 저장한다.
	};

}


using namespace graphic;



cGrid2::cGrid2() :
	m_rowCellCount(0)
,	m_colCellCount(0)
,	m_cellSize(0)
,	m_textureUVFactor(8)
{
	m_mtrl.InitWhite();
}

cGrid2::~cGrid2()
{

}


void cGrid2::Create( const int rowCellCount, const int colCellCount, const float cellSize,
	const float textureUVFactor, const float offsetY)
{
	// init member
	m_rowCellCount = rowCellCount;
	m_colCellCount = colCellCount;
	m_cellSize = cellSize;
	m_textureUVFactor = textureUVFactor;

	// Init Grid
	const int rowVtxCnt  = rowCellCount+1;
	const int colVtxCnt  = colCellCount+1;
	const int cellCnt = rowCellCount * colCellCount;
	const int vtxCount= rowVtxCnt * colVtxCnt;

	m_vtxBuff.Create( vtxCount, sizeof(sVertexNormTex), sVertexNormTex::FVF);
	{
		sVertexNormTex *vertices = (sVertexNormTex*)m_vtxBuff.Lock();
		const float startx = -cellSize*(colCellCount/2);
		const float starty = cellSize*(rowCellCount/2);
		const float endx = startx + cellSize*colCellCount;
		const float endy = starty - cellSize*rowCellCount;

		const float uCoordIncrementSize = 1.0f / (float)colCellCount * textureUVFactor;
		const float vCoordIncrementSize = 1.0f / (float)rowCellCount * textureUVFactor;

		int i=0;
		for (float y=starty; y >= endy; y -= cellSize, ++i)
		{
			int k=0;
			for (float x=startx; x <= endx; x += cellSize, ++k )
			{
				int index = (i * colVtxCnt) + k;
				vertices[ index].p = Vector3(x, offsetY, y);
				vertices[ index].n = Vector3(0,1,0);
				vertices[ index].u = (float)k*uCoordIncrementSize;
				vertices[ index].v = (float)i*vCoordIncrementSize;
			}
		}
		m_vtxBuff.Unlock();
	}


	m_idxBuff.Create( cellCnt*2 );
	{
		WORD *indices = (WORD*)m_idxBuff.Lock();
		int baseIndex = 0;
		for( int i=0; i < rowCellCount; ++i )
		{
			for( int k=0; k < colCellCount; ++k )
			{
				indices[ baseIndex] = (i * colVtxCnt) + k;
				indices[ baseIndex + 1] = (i   * colVtxCnt) + k + 1;
				indices[ baseIndex + 2] = ((i+1) * colVtxCnt) + k;

				indices[ baseIndex + 3] = ((i+1) * colVtxCnt) + k;
				indices[ baseIndex + 4] = (i   * colVtxCnt) + k + 1;
				indices[ baseIndex + 5] = ((i+1) * colVtxCnt) + k + 1;

				// next quad
				baseIndex += 6;
			}
		}	
		m_idxBuff.Unlock();
	}

}


// uv 값을 textureUVFactor 에 맞춰 재 조정한다.
void cGrid2::SetTextureUVFactor(const float textureUVFactor)
{
	// init member
	m_textureUVFactor = textureUVFactor;

	// 아직 grid가 초기화 되지 않았다면 uv 값을 조정하지 않는다.
	RET (m_rowCellCount <= 0);

	// Init Grid
	const int colVtxCnt  = GetColVertexCount();

	sVertexNormTex *vertices = (sVertexNormTex*)m_vtxBuff.Lock();
	const float startx = -m_cellSize*(m_colCellCount/2);
	const float starty = m_cellSize*(m_rowCellCount/2);
	const float endx = startx + m_cellSize*m_colCellCount;
	const float endy = starty - m_cellSize*m_rowCellCount;

	const float uCoordIncrementSize = 1.0f / (float)m_colCellCount * textureUVFactor;
	const float vCoordIncrementSize = 1.0f / (float)m_rowCellCount * textureUVFactor;

	int i=0;
	for (float y=starty; y >= endy; y -= m_cellSize, ++i)
	{
		int k=0;
		for (float x=startx; x <= endx; x += m_cellSize, ++k )
		{
			int index = (i * colVtxCnt) + k;
			vertices[ index].u = (float)k*uCoordIncrementSize;
			vertices[ index].v = (float)i*vCoordIncrementSize;
		}
	}
	m_vtxBuff.Unlock();
}


// GRD 포맷으로 저장된다.
// m_rowCellCount. m_colCellCount, m_cellSize, Height  값을 저장한다.
// Header : GRD
bool cGrid2::WriteFile(const string &fileName)
{
	const int vtxCount = GetRowVertexCount() * GetColVertexCount();

	// sGridBinary 구조체 생성.
	sGridBinary gbin;
	gbin.header[0] = 'G';
	gbin.header[1] = 'R';
	gbin.header[2] = 'D';
	gbin.version = '1';
	gbin.rowCellCount = m_rowCellCount;
	gbin.colCellCount = m_colCellCount;
	gbin.cellSize = m_cellSize;
	gbin.textureUVFactor = m_textureUVFactor;
	gbin.heightMap = new float[ vtxCount];
	
	if (sVertexNormTex *vertices = (sVertexNormTex*)m_vtxBuff.Lock())
	{
		for (int i=0; i < vtxCount; ++i)
			gbin.heightMap[ i] = vertices[ i].p.y;
		m_vtxBuff.Unlock();
	}

	
	// sGridBinary 구조체 파일에 저장.
	using namespace std;
	ofstream of(fileName, ios_base::binary | ios_base::out);
	if (!of.is_open())
		return false;

	of.write( (char*)&gbin, sizeof(gbin) );
	of.write( (char*)gbin.heightMap, sizeof(float) * vtxCount );

	
	// sGridBinary 제거.
	delete[] gbin.heightMap;

	return true;
}


// 파일로부터 정보를 가져와 Grid 클래스를 생성한다.
// GRD 포맷이어야 한다. 
// 현재는 version 1만 지원한다.
bool cGrid2::CreateFromFile(const string &fileName)
{
	using namespace std;
	ifstream ifs(fileName, ios_base::binary | ios_base::in);
	if (!ifs.is_open())
		return false;

	sGridBinary gbin;
	ifs.read( gbin.header, sizeof(gbin.header) );

	if ((gbin.header[ 0] != 'G') || (gbin.header[ 1] != 'R') || (gbin.header[ 2] != 'D'))
		return false;

	ifs.read( &gbin.version, sizeof(gbin.version) );
	if (gbin.version != '1')
		return false;

	ifs.seekg(0, ifs.beg);// 처음으로 돌아간다.

	ifs.read( (char*)&gbin, sizeof(gbin) );

	const int vtxCount = (gbin.rowCellCount+1) * (gbin.colCellCount+1);
	gbin.heightMap = new float[ vtxCount];
	ifs.read( (char*)gbin.heightMap, sizeof(float)*vtxCount );

	// cGrid 초기화.
	Clear();

	Create(gbin.rowCellCount, gbin.colCellCount, gbin.cellSize, gbin.textureUVFactor );
	
	// 높이맵 설정.
	if (sVertexNormTex *vertices = (sVertexNormTex*)m_vtxBuff.Lock())
	{
		for (int i=0; i < vtxCount; ++i)
			vertices[ i].p.y = gbin.heightMap[ i];
		m_vtxBuff.Unlock();
	}

	CalculateNormals();

	// sGridBinanry 제거.
	delete[] gbin.heightMap;

	return true;
}


void cGrid2::Render(const Matrix44 &tm, const int stage)
{
	GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	GetDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	GetDevice()->SetTransform(D3DTS_WORLD, ToDxM(tm));

	m_mtrl.Bind();
	m_tex.Bind(stage);
	m_vtxBuff.Bind();
	m_idxBuff.Bind();
	GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_vtxBuff.GetVertexCount(), 
		0, m_idxBuff.GetFaceCount());

	GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
}


void cGrid2::RenderLinelist()
{
	m_vtxBuff.Bind();
	m_idxBuff.Bind();
	GetDevice()->DrawIndexedPrimitive( D3DPT_LINELIST, 0, 0, m_vtxBuff.GetVertexCount(), 
		0, m_idxBuff.GetFaceCount()*3/2);
}


void cGrid2::RenderShader(cShader &shader, const Matrix44 &tm )
{
	shader.SetMatrix( "mWorld", tm);
	Matrix44 tmInvs = tm.Inverse();
	tmInvs.Transpose();
	shader.SetMatrix( "mWIT", tmInvs);

	m_mtrl.Bind(shader);
	m_tex.Bind(shader, "colorMapTexture");

	shader.Begin();
	shader.BeginPass();

	m_vtxBuff.Bind();
	m_idxBuff.Bind();
	GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_vtxBuff.GetVertexCount(), 
		0, m_idxBuff.GetFaceCount());

	shader.EndPass();
	shader.End();
}


// 법선 벡터를 다시 계산한다.
void cGrid2::CalculateNormals()
{
	sVertexNormTex *vertices = (sVertexNormTex*)m_vtxBuff.Lock();
	WORD *indices = (WORD*)m_idxBuff.Lock();
	for (int i=0; i < m_idxBuff.GetFaceCount()*3; i+=3)
	{
		sVertexNormTex &p1 = vertices[ indices[ i]];
		sVertexNormTex &p2 = vertices[ indices[ i+1]];
		sVertexNormTex &p3 = vertices[ indices[ i+2]];

		Vector3 v1 = p2.p - p1.p;
		Vector3 v2 = p3.p - p1.p;
		v1.Normalize();
		v2.Normalize();
		Vector3 n = v1.CrossProduct(v2);
		n.Normalize();
		p1.n = n;
		p2.n = n;
		p3.n = n;
	}

	m_vtxBuff.Unlock();
	m_idxBuff.Unlock();
}


// 광선 orig, dir 에 충돌된 면이 있다면 true 를 리턴하고, 충돌 위치를 out에 
// 저장해서 리턴한다.
bool cGrid2::Pick( const Vector3 &orig, const Vector3 &dir, Vector3 &out )
{
	bool isFirst = true;
	sVertexNormTex *vertices = (sVertexNormTex*)m_vtxBuff.Lock();
	WORD *indices = (WORD*)m_idxBuff.Lock();
	RETV(!vertices || !indices, false);

	for (int i=0; i < m_idxBuff.GetFaceCount()*3; i+=3)
	{
		const Vector3 &p1 = vertices[ indices[ i]].p;
		const Vector3 &p2 = vertices[ indices[ i+1]].p;
		const Vector3 &p3 = vertices[ indices[ i+2]].p;

		const Triangle tri(p1, p2, p3);
		const Plane p(p1, p2, p3);

		const float dot = dir.DotProduct( p.N );
		if (dot >= 0)
			continue;

		float t;
		if (tri.Intersect(orig, dir, &t))
		{
			if (isFirst)
			{
				isFirst = false;
				out = orig + dir * t;
			}
			else
			{
				const Vector3 v = orig + dir * t;
				if (orig.LengthRoughly(v) < orig.LengthRoughly(out))
					out = v;
			}
		}
	}
	m_vtxBuff.Unlock();
	m_idxBuff.Unlock();

	return !isFirst;
}


// 초기화.
void cGrid2::Clear()
{
	m_vtxBuff.Clear();
	m_idxBuff.Clear();
	m_tex.Clear();

	m_rowCellCount = 0;
	m_colCellCount = 0;
	m_cellSize = 0;
	m_textureUVFactor = 8;
}
