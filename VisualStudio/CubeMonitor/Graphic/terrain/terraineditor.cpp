
#include "stdafx.h"
#include "terraineditor.h"
#include <objidl.h>
#include <gdiplus.h> 

using namespace Gdiplus;
using namespace graphic;



cTerrainEditor::cTerrainEditor()
{
	InitLayer();

	//m_shader.Create( "./media/shader/hlsl_terrain_splatting.fx", "TShader" );
	m_emptyTexture.Create( "./media/texture/emptyTexture.png" );
}

cTerrainEditor::~cTerrainEditor()
{

}


// ���� ������ �����Ѵ�.  ���� �ؽ��� ����.
// GRD �������� ����ȴ�.
bool cTerrainEditor::WriteGRDFile( const string &fileName )
{
	return m_grid.WriteFile(fileName);
}


// ���� ���� �ε�
bool cTerrainEditor::CreateFromGRDFile( const string &fileName )
{
	return m_grid.CreateFromFile(fileName);
}


// ���������� sRawTerrain�� �����Ѵ�.
void cTerrainEditor::GenerateRawTerrain( OUT sRawTerrain &out )
{
	out.rowCellCount = m_grid.GetRowCellCount();
	out.colCellCount = m_grid.GetColCellCount();
	out.cellSize = m_grid.GetCellSize();
	out.heightMapStyle = 1;//(m_heightMapFileName.empty()? 1 : 0);
	out.heightMap = cResourceManager::Get()->GetRelativePathToMedia(m_heightMapFileName);
	out.bgTexture = cResourceManager::Get()->GetRelativePathToMedia(m_grid.GetTexture().GetTextureName());
	out.alphaTextureWidth = ALPHA_TEXTURE_SIZE_W;
	out.alphaTextureHeight = ALPHA_TEXTURE_SIZE_H;
	out.textureFactor = m_grid.GetTextureUVFactor();
	out.heightFactor = m_heightFactor;
	out.renderWater = m_isRenderWater;

	for (u_int i=0; i < m_layer.size(); ++i)
	{
		if (m_layer[ i].texture)
			out.layer[ i].texture = cResourceManager::Get()->GetRelativePathToMedia(m_layer[ i].texture->GetTextureName());
	}

	out.models.reserve( m_rigids.size() );
	for (u_int i=0; i < m_rigids.size(); ++i)
	{
		out.models.push_back( sRawTerrrainModel() );
		out.models.back().fileName = cResourceManager::Get()->GetRelativePathToMedia(m_rigids[ i]->GetFileName());
		out.models.back().tm = m_rigids[ i]->GetTransform();
	}

}


// ���� ������ ���Ͽ� �����Ѵ�.
// TRN �������� ����ȴ�.
bool cTerrainEditor::WriteTRNFile(const string &fileName)
{
	sRawTerrain rawTerrain;
	GenerateRawTerrain(rawTerrain);

	// ���� ���� ����. (*.GRD)
	{
		string name = common::GetFileNameExceptExt(fileName);
		name += "_geo.grd";
		string path = common::GetFilePathExceptFileName(fileName);
		if (!path.empty())
			path += "\\";

		const string heigtmapFileName = path + name;
		rawTerrain.heightMap = graphic::cResourceManager::Get()->GetRelativePathToMedia(heigtmapFileName);
		WriteGRDFile( heigtmapFileName );
	}

	// ���� �ؽ��� ���� �̸� ����. (*.PNG)
	{
		string name = common::GetFileNameExceptExt(fileName);
		name += "_alpha.png";
		string path = common::GetFilePathExceptFileName(fileName);
		if (!path.empty())
			path += "\\";

		const string alphaTextureFileName = path + name;
		rawTerrain.alphaTexture = graphic::cResourceManager::Get()->GetRelativePathToMedia(alphaTextureFileName);
		m_alphaTexture.WritePNGFile(alphaTextureFileName);
	}

	return exporter::WriteRawTerrainFile(fileName, rawTerrain);
}


// ���÷��õ� ���� ���� �ؽ��ĸ� ���Ͽ� �����Ѵ�.
bool cTerrainEditor::WriteTerrainTextureToPNGFile( const string &fileName )
{
	cShader shader;
	shader.Create( "./media/shader/hlsl_terrain_splatting_texture_write.fx", "TShader" );

	cSurface surface;
	surface.CreateRenderTarget(512, 512);

	surface.Begin();

	GetDevice()->Clear(0L, NULL
		, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
		, 0x00000000, 1.0f, 0L);

	const float width = GetTerrainWidth();
	cCamera camera(Vector3(0,width/2,0), Vector3(0,0,0), Vector3(0,0,1));
	camera.SetProjection( D3DX_PI / 2.f, 1.f, 1, width/2 );

	Matrix44 matIdentity;
	shader.SetMatrix( "mVP", camera.GetViewProjectionMatrix());
	shader.SetVector( "vEyePos", camera.GetEyePos());
	shader.SetMatrix( "mWIT", matIdentity);
	shader.SetMatrix( "mWorld", matIdentity);
	
	if (m_layer.empty())
	{
		shader.SetRenderPass(0);
		m_grid.RenderShader(shader);
	}
	else
	{
		shader.SetTexture( "SplattingAlphaMap", m_alphaTexture );
		shader.SetFloat( "alphaUVFactor", GetTextureUVFactor() );

		const string texName[] = {"Tex1", "Tex2", "Tex3", "Tex4" };
		for (u_int i=0; i < m_layer.size(); ++i)
			shader.SetTexture( texName[ i], *m_layer[ i].texture );
		for (u_int i=m_layer.size(); i < MAX_LAYER; ++i)
			shader.SetTexture( texName[ i], m_emptyTexture );

		shader.SetRenderPass(1);
		m_grid.RenderShader(shader);
	}

	surface.End();

	const bool result = surface.WritePNGFile(fileName);
	return result;
}


// ������ �����̸� �����Ѵ�.
void cTerrainEditor::BrushTerrain( const cTerrainCursor &cursor, const float elapseT )
{
	if (!IsLoaded())
		return; // ���� ������ �ε����� �ʾƼ� �����Ѵ�.

	const int VERTEX_COL_COUNT = m_grid.GetColVertexCount();
	const int VERTEX_ROW_COUNT = m_grid.GetRowVertexCount();

	const Vector3 cursorPos = cursor.GetCursorPos();

	if (sVertexNormTex *pv = (sVertexNormTex*)m_grid.GetVertexBuffer().Lock())
	{
		for (int i=0; i < VERTEX_COL_COUNT; ++i)
		{
			for (int k=0; k < VERTEX_ROW_COUNT; ++k)
			{
				sVertexNormTex *vtx = pv + (k*VERTEX_COL_COUNT) + i;

				Vector3 v = vtx->p - cursorPos;
				const float length = v.Length();
				if (cursor.GetOuterBrushRadius() < length)
					continue;

				float offsetH = 0.f;
				if (cursor.GetTerrainEditMode() == TERRAIN_EDIT_MODE::UP)
				{
					offsetH = cursor.GetBrushSpeed() * elapseT;
				}
				else if (cursor.GetTerrainEditMode() == TERRAIN_EDIT_MODE::DOWN)
				{
					offsetH = -cursor.GetBrushSpeed() * elapseT;
				}

				vtx->p.y += offsetH * (cursor.GetOuterBrushRadius() - length) / cursor.GetOuterBrushRadius();
			}
		}

		m_grid.GetVertexBuffer().Unlock();
	}

	m_grid.CalculateNormals();
}


// ������ �ؽ��ĸ� ������.
void cTerrainEditor::BrushTexture( const cTerrainCursor &cursor )
{
	RET(!cursor.GetBrushTexture());

	if (GetTopLayer().texture != cursor.GetBrushTexture())
	{
		// �� ���̾� �� ���� �� ���̾ �߰��Ѵ�.
		if (GetTopLayer().texture && !AddLayer())
			return; // �� �̻� �߰��� ���̾ ���ٸ� �����Ѵ�.
	}

	sSplatLayer &curLayer = GetTopLayer();
	const int layerIdx = m_layer.size()-1;
	curLayer.texture = (cTexture*)cursor.GetBrushTexture();
	//const int MASK = ~(0xFF << (24 - (layerIdx * 8)));
	const int MASK = ~GetAlphaMask(layerIdx);

	float u, v;
	GetTextureUV(cursor.GetCursorPos(), u, v);

	const float WIDTH = GetTerrainWidth();
	const float HEIGHT = GetTerrainHeight();
	
	D3DLOCKED_RECT lockrect;
	m_alphaTexture.Lock(lockrect);

	BYTE *pbits = (BYTE*)lockrect.pBits;
	for (int ay=0; ay < ALPHA_TEXTURE_SIZE_H; ++ay)
	{
		for (int ax=0; ax < ALPHA_TEXTURE_SIZE_W; ++ax)
		{
			const float au = (float)ax / (float)(ALPHA_TEXTURE_SIZE_W - 1);
			const float av = (float)ay / (float)(ALPHA_TEXTURE_SIZE_H - 1);

			const float ru = fabs(au-u) * WIDTH;
			const float rv = fabs(av-v) * HEIGHT;
			const float len = sqrt( ru*ru + rv*rv );

			// A8R8G8B8 Format
			DWORD *ppixel = (DWORD*)(pbits + (ax*4) + (lockrect.Pitch * ay));

			if (len <= cursor.GetInnerBrushRadius())
			{
				int color = (int)(255.f * cursor.GetInnerBrushAlpha());
				if (cursor.IsEraseMode()) // ���찳 ����� ���� ���� ���� �ְ��Ѵ�.
					color = 255-color;
				color = color << (24 - (layerIdx * 8));

				*ppixel = color | (*ppixel & MASK);
			}
			else if (len <= cursor.GetOuterBrushRadius())
			{
				// ����
				const float w = (float)cursor.GetOuterBrushRadius() - (float)cursor.GetInnerBrushRadius();
				const float delta = 1.f - ((len - cursor.GetInnerBrushRadius()) / w);
				int color = (int)(cursor.GetInnerBrushAlpha() * delta * 255.f);
				if (cursor.IsEraseMode()) // ���찳 ����� ���� ���� ���� �ְ��Ѵ�.
					color = 255-color;

				const int dest = (*ppixel >> (24 - (layerIdx * 8))) & 0xFF;

				if ( (cursor.IsEraseMode() && (color < dest)) ||
					(!cursor.IsEraseMode() && (color > dest)))
				{
					color = color << (24 - (layerIdx * 8));
					*ppixel = color | (*ppixel & MASK);
				}
			}
		}
	}

	m_alphaTexture.Unlock();
}


void cTerrainEditor::GetTextureUV(const Vector3 &pos, OUT float &tu, OUT float &tv)
{
	const float width = GetColCellCount() * GetCellSize();
	const float height = GetRowCellCount() * GetCellSize();

	float x = pos.x + width/2.f;
	float z = pos.z + height/2.f;
	z = height - z;

	tu = x / (float)width;
	tv = z / (float)height;
}


// heightFactor ���� �����Ѵ�.
// �̹� ������ �ε��� ���¶��, ���� ���� heightFactor ���� ����
// �����Ѵ�.
void cTerrainEditor::SetHeightFactor(const float heightFactor)
{
	m_heightFactor = heightFactor;
	if (!IsLoaded())
		return; // ���� ������ �ε����� �ʾƼ� �����Ѵ�.
	
	const wstring wfileName = common::str2wstr(m_heightMapFileName);
	Bitmap bmp(wfileName.c_str());
	if (Ok != bmp.GetLastStatus())
		return;

	const int VERTEX_COL_COUNT = m_grid.GetColVertexCount();
	const int VERTEX_ROW_COUNT = m_grid.GetRowVertexCount();
	const float WIDTH = m_grid.GetWidth();
	const float HEIGHT = m_grid.GetHeight();

	const float incX = (float)(bmp.GetWidth()-1) / (float)m_grid.GetColCellCount();
	const float incY = (float)(bmp.GetHeight()-1) /(float)m_grid.GetRowCellCount();

	sVertexNormTex *pv = (sVertexNormTex*)m_grid.GetVertexBuffer().Lock();

	for (int i=0; i < VERTEX_COL_COUNT; ++i)
	{
		for (int k=0; k < VERTEX_ROW_COUNT; ++k)
		{
			sVertexNormTex *vtx = pv + (k*VERTEX_COL_COUNT) + i;

			Color color;
			bmp.GetPixel((int)(i*incX), (int)(k*incY), &color);
			const float h = ((color.GetR() + color.GetG() + color.GetB()) / 3.f) 
				* heightFactor;
			vtx->p.y = h;
		}
	}

	m_grid.GetVertexBuffer().Unlock();

	m_grid.CalculateNormals();
}


// textureUVFactor ���� �����Ѵ�.
// �̹� ������ �ε��� ���¶��, UV ���� textureUVFactor ���� ����
// �����Ѵ�.
void cTerrainEditor::SetTextureUVFactor(const float textureUVFactor)
{
	m_grid.SetTextureUVFactor(textureUVFactor);
}
