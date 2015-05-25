#pragma once

namespace graphic
{

	class cRenderer : public common::cSingleton<cRenderer>
	{
	public:
		cRenderer();
		virtual ~cRenderer();

		bool CreateDirectX(HWND hWnd, const int width, const int height);
		void Update(const float elpaseT);
		LPDIRECT3DDEVICE9 GetDevice();
		HWND GetHwnd() const;

		void RenderAxis();
		void RenderFPS();
		void RenderGrid();


	protected:
		void MakeGrid( const float width, const int count, DWORD color, vector<sVertexDiffuse> &out );
		void MakeAxis( const float length, DWORD xcolor, DWORD ycolor, DWORD zcolor, vector<sVertexDiffuse> &out );


	private:
		LPDIRECT3DDEVICE9 m_pDevice;		
		HWND m_hWnd;

		// Grid
		vector<sVertexDiffuse> m_grid;

		// Axis
		vector<sVertexDiffuse> m_axis;

		// Display FPS 
		cText m_textFps;
		//ID3DXFont *m_font;
		//string m_fpsText;
		float m_elapseTime;
		int m_fps;
	};


	// 렌더러 초기화.
	bool InitRenderer(HWND hWnd, const int width, const int height);
	void ReleaseRenderer();
	inline cRenderer* GetRenderer() { return cRenderer::Get(); }
	inline LPDIRECT3DDEVICE9 GetDevice() { return cRenderer::Get()->GetDevice(); }
	inline LPDIRECT3DDEVICE9 cRenderer::GetDevice() { return m_pDevice; }
	inline HWND cRenderer::GetHwnd() const { return m_hWnd; }

}
