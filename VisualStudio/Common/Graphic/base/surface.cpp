
#include "stdafx.h"
#include "surface.h"

using namespace graphic;


cSurface::cSurface() : 
	m_width(0)
,	m_height(0)
,	m_texture(NULL)
,	m_surface(NULL)
,	m_zbuffer(NULL)
{

}

cSurface::~cSurface()
{
	Clear();
}


// ����Ÿ�� ���ǽ��� �����Ѵ�.
// �ؽ���, Ÿ�� ���ǽ�, Z���� ����.
bool cSurface::CreateRenderTarget(const int width, const int height)
{
	m_width = width;
	m_height = height;

	if (FAILED(GetDevice()->CreateTexture(width, height, 1, 
		D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT, &m_texture, NULL)))
		return false;

	if (FAILED(m_texture->GetSurfaceLevel(0, &m_surface)))
		return false;

	if (FAILED(GetDevice()->CreateDepthStencilSurface(
		width, height, D3DFMT_D24S8, 
		D3DMULTISAMPLE_NONE, 0, TRUE,
		&m_zbuffer, NULL)))
		return false;

	return true;
}


// ���Ϸ� ����.
bool cSurface::WritePNGFile(const string &fileName)
{
	if (FAILED(D3DXSaveTextureToFileA(fileName.c_str(), D3DXIFF_PNG, m_texture, NULL)))
	{
		return false;
	}

	return true;
}


// ���� Ÿ���� ��ü�Ѵ�.
void cSurface::Begin()
{
	GetDevice()->GetRenderTarget(0, &m_oldBackBuffer);
	GetDevice()->GetDepthStencilSurface(&m_oldZBuffer);
	GetDevice()->GetViewport(&m_oldViewport);

	GetDevice()->SetRenderTarget(0, m_surface);
	GetDevice()->SetDepthStencilSurface(m_zbuffer);
	
	// ����Ʈ����  x y  width    height   minz maxz
	D3DVIEWPORT9 viewport = {0, 0, m_width, m_height, 0.0f, 1.0f};
	GetDevice()->SetViewport(&viewport);
}


// ���� Ÿ���� �������� �ǵ�����.
void cSurface::End()
{
	GetDevice()->SetRenderTarget(0, m_oldBackBuffer);
	GetDevice()->SetDepthStencilSurface(m_oldZBuffer);
	GetDevice()->SetViewport(&m_oldViewport);

	SAFE_RELEASE(m_oldBackBuffer);
	SAFE_RELEASE(m_oldZBuffer);
}


// ���ǽ� ���. ����� �� �̴�.
void cSurface::Render()
{
	GetDevice()->SetTextureStageState(0,D3DTSS_COLOROP,	D3DTOP_SELECTARG1);
	GetDevice()->SetTextureStageState(0,D3DTSS_COLORARG1,	D3DTA_TEXTURE);
	GetDevice()->SetTextureStageState(1,D3DTSS_COLOROP,    D3DTOP_DISABLE);
	float scale = 128.0f;
	typedef struct {FLOAT p[4]; FLOAT tu, tv;} TVERTEX;

	TVERTEX Vertex[4] = {
		// x  y  z rhw tu tv
		{0, scale, 0, 1, 0, 0,},
		{scale, scale,0, 1, 1, 0,},
		{scale, scale+scale,0, 1, 1, 1,},
		{0, scale+scale,0, 1, 0, 1,},
	};
	GetDevice()->SetTexture( 0, m_texture );
	GetDevice()->SetVertexShader(NULL);
	GetDevice()->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
	GetDevice()->SetPixelShader(0);
	GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex, sizeof( TVERTEX ) );
}


// Ŭ���� �ʱ�ȭ.
void cSurface::Clear()
{
	SAFE_RELEASE(m_surface);
	SAFE_RELEASE(m_zbuffer);
	SAFE_RELEASE(m_texture);
}
