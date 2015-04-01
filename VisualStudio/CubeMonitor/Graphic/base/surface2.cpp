
#include "stdafx.h"
#include "surface2.h"

using namespace graphic;


cSurface2::cSurface2()
	: m_texture(NULL)
,	m_rts(NULL)
,	m_surface(NULL)
,	m_width(0)
,	m_height(0)
,	m_mipLevels(0)
,	m_autoGenMips(0)
{
}

cSurface2::~cSurface2()
{
	Clear();
}


bool cSurface2::Create(int width, int height, int mipLevels,
	D3DFORMAT texFormat, bool useDepthBuffer,
	D3DFORMAT depthFormat, D3DVIEWPORT9& viewport, bool autoGenMips)
{
	m_width = width;
	m_height = height;
	m_mipLevels = mipLevels;
	m_texFormat = texFormat;
	m_useDepthBuffer = useDepthBuffer;
	m_depthFormat = depthFormat;
	m_viewPort = viewport;
	m_autoGenMips = autoGenMips;

	UINT usage = D3DUSAGE_RENDERTARGET;
	if(m_autoGenMips)
		usage |= D3DUSAGE_AUTOGENMIPMAP;

	if (FAILED(D3DXCreateTexture(GetDevice(), width, height, mipLevels, 
		usage, texFormat, D3DPOOL_DEFAULT, &m_texture)))
	{
		return false;
	}

	if (FAILED(D3DXCreateRenderToSurface(GetDevice(), width, height, texFormat, 
		useDepthBuffer, depthFormat, &m_rts)))
	{
		return false;
	}

	if (FAILED(m_texture->GetSurfaceLevel(0, &m_surface)))
	{
		return false;
	}

	return true;
}


void cSurface2::Begin()
{
	RET(!m_rts);
	m_rts->BeginScene(m_surface, &m_viewPort);
}


void cSurface2::End()
{
	RET(!m_rts);
	m_rts->EndScene(D3DX_FILTER_NONE);
}


void cSurface2::Render(const int index) // index=1
{
	GetDevice()->SetTransform(D3DTS_WORLD, ToDxM(Matrix44::Identity));
	GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
	GetDevice()->SetTextureStageState(0,D3DTSS_COLOROP,	D3DTOP_SELECTARG1);
	GetDevice()->SetTextureStageState(0,D3DTSS_COLORARG1,	D3DTA_TEXTURE);
	GetDevice()->SetTextureStageState(1,D3DTSS_COLOROP, D3DTOP_DISABLE);

	GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	const float scale = 128.0f;
	typedef struct {FLOAT p[4]; FLOAT tu, tv;} TVERTEX;

	TVERTEX Vertex[4] = {
		// x  y  z rhw tu tv
		{0, scale*index, 0, 1, 0, 0,},
		{scale, scale*index,0, 1, 1, 0,},
		{scale, scale*(index+1),0, 1, 1, 1,},
		{0, scale*(index+1),0, 1, 0, 1,},
	};
	GetDevice()->SetTexture( 0, m_texture );
	GetDevice()->SetVertexShader(NULL);
	GetDevice()->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
	GetDevice()->SetPixelShader(NULL);
	GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex, sizeof(TVERTEX) );
}


void cSurface2::Clear()
{
	SAFE_RELEASE(m_texture);
	SAFE_RELEASE(m_rts);
	SAFE_RELEASE(m_surface);
}
