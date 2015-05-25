
#include "stdafx.h"
#include "Renderer.h"
#include "../base/DxInit.h"
#include "resourcemanager.h"

using namespace graphic;


// 렌더러 초기화.
bool graphic::InitRenderer(HWND hWnd, const int width, const int height)
{
	if (!cRenderer::Get()->CreateDirectX(hWnd, width, height))
		return false;

	return true;
}

void graphic::ReleaseRenderer()
{
	cRenderer::Release();
	cResourceManager::Release();
	cMainCamera::Release();
	cLightManager::Release();
}



////////////////////////////////////////////////////////////////////////////////////////////////
// Renderer

cRenderer::cRenderer() :
	m_pDevice(NULL)
,	m_elapseTime(0)
,	m_fps(0)
{

}

cRenderer::~cRenderer()
{
	SAFE_RELEASE(m_pDevice);
}


// DirectX Device 객체 생성.
bool cRenderer::CreateDirectX(HWND hWnd, const int width, const int height)
{
	if (!InitDirectX(hWnd, width, height, m_pDevice))
		return false;

	m_textFps.Create();
	m_textFps.SetPos(10, 10);
	m_textFps.SetColor(D3DXCOLOR(1,1,1,1));

	m_hWnd = hWnd;
	return true;
}


// x, y, z 축을 출력한다.
void cRenderer::RenderAxis()
{
	RET(!m_pDevice);

	if (m_axis.empty())
		MakeAxis(500.f,  D3DXCOLOR(1,0,0,0),  D3DXCOLOR(0,1,0,0),  D3DXCOLOR(0,0,1,0), m_axis);


	// 가장 위에 출력되기 위해서 zbuffer 를 끈다.
	//m_pDevice->SetRenderState(D3DRS_ZENABLE, 0);

	DWORD lighting;
	m_pDevice->GetRenderState( D3DRS_LIGHTING, &lighting );
	m_pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	m_pDevice->SetTexture(0, NULL);
	Matrix44 identity;
	m_pDevice->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&identity );
	m_pDevice->SetFVF( sVertexDiffuse::FVF );
	m_pDevice->DrawPrimitiveUP( D3DPT_LINELIST, 3, &m_axis[0], sizeof(sVertexDiffuse) );
	m_pDevice->SetRenderState( D3DRS_LIGHTING, lighting );

	//m_pDevice->SetRenderState(D3DRS_ZENABLE, 1);
}


// FPS 출력.
void cRenderer::RenderFPS()
{
	//RET(!m_font);
	m_textFps.Render();
	//RECT rc = {10,10,200,200};
	//m_font->DrawTextA( NULL, m_fpsText.c_str(), -1, &rc,
	//	DT_NOCLIP, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
}


// 그리드 출력.
void cRenderer::RenderGrid()
{
	static int gridSize = 0;
	if (m_grid.empty())
	{
		MakeGrid(50, 64, D3DXCOLOR(0.8f,0.8f,0.8f,1), m_grid);
		gridSize = m_grid.size() / 2;
	}

	if (gridSize > 0)
	{
		// 가장 위에 출력되기 위해서 zbuffer 를 끈다.
		m_pDevice->SetRenderState(D3DRS_ZENABLE, 0);

		DWORD lighting;
		m_pDevice->GetRenderState( D3DRS_LIGHTING, &lighting );
		m_pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
		m_pDevice->SetTexture(0, NULL);
		Matrix44 identity;
		m_pDevice->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&identity );
		m_pDevice->SetFVF( sVertexDiffuse::FVF );
		m_pDevice->DrawPrimitiveUP( D3DPT_LINELIST, gridSize, &m_grid[0], sizeof(sVertexDiffuse) );
		m_pDevice->SetRenderState( D3DRS_LIGHTING, lighting);
		m_pDevice->SetRenderState(D3DRS_ZENABLE, 1);
	}
}



void cRenderer::Update(const float elapseT)
{
	// fps 계산 ---------------------------------------
	++m_fps;
	m_elapseTime += elapseT;
	if( 1.f <= m_elapseTime )
	{
		m_textFps.SetText(format("fps: %d", m_fps));
		m_fps = 0;
		m_elapseTime = 0;
	}
	//--------------------------------------------------
}


// 격자무늬 버텍스를 만든다. 
// width = 격자 하나의 폭 크기 (정 사각형이므로 인자값은 하나만 받는다)
// count = 격자 가로 세로 개수
void cRenderer::MakeGrid( const float width, const int count, DWORD color, vector<sVertexDiffuse> &out )
{
	if (out.empty())
	{
		out.reserve(count * 4);
		const Vector3 start(count/2 * -width, 0, count/2 * width);

		for (int i=0; i < count+1; ++i)
		{
			sVertexDiffuse vtx;
			vtx.p = start;
			vtx.p.x += (i * width);
			vtx.c = color;
			out.push_back(vtx);

			vtx.p += Vector3(0, 0.001f,-width*count);
			out.push_back(vtx);
		}

		for (int i=0; i < count+1; ++i)
		{
			sVertexDiffuse vtx;
			vtx.p = start;
			vtx.p.z -= (i * width);
			vtx.c = color;
			out.push_back(vtx);

			vtx.p += Vector3(width*count, 0.001f, 0);
			out.push_back(vtx);
		}
	}
}


// x, y, z 축을 만든다.
void cRenderer::MakeAxis( const float length, DWORD xcolor, DWORD ycolor, DWORD zcolor, 
	vector<sVertexDiffuse> &out )
{
	RET(!out.empty());

	out.reserve(6);

	sVertexDiffuse v;

	// x axis
	v.p = Vector3( 0.f, 0.001f, 0.f );
	v.c = xcolor;
	out.push_back(v);

	v.p = Vector3( length, 0.001f, 0.f );
	v.c = xcolor;
	out.push_back(v);

	// y axis
	v.p = Vector3( 0.f, 0.f, 0.f );
	v.c = ycolor;
	out.push_back(v);

	v.p = Vector3( 0.f, length, 0.f );
	v.c = ycolor;
	out.push_back(v);

	// z axis
	v.p = Vector3( 0.f, 0.001f, 0.f );
	v.c = zcolor;
	out.push_back(v);

	v.p = Vector3( 0.f, 0.001f, length );
	v.c = zcolor;
	out.push_back(v);
}

