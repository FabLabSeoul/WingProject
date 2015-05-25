// 렌더 타겟 전용 서피스 클래스.
// 지형 스플래팅 텍스쳐를 저장할 때,
// 그림자 맵을 만들 때 사용한다.
#pragma once


namespace graphic
{

	class cSurface
	{
	public:
		cSurface();
		virtual ~cSurface();

		bool CreateRenderTarget(const int width, const int height);
		bool WritePNGFile(const string &fileName);

		void Begin();
		void End();
		void Render();
		void Clear();

		bool IsLoaded() const;
		IDirect3DTexture9* GetTexture();
		IDirect3DSurface9* GetRenderTarget();
		IDirect3DSurface9* GetZBuffer();


	private:
		int m_width;
		int m_height;
		IDirect3DTexture9 *m_texture;
		IDirect3DSurface9 *m_surface;
		IDirect3DSurface9 *m_zbuffer;

		// 렌더타겟 스위칭시 저장되는 임시 변수들.
		IDirect3DSurface9 *m_oldBackBuffer;
		IDirect3DSurface9 *m_oldZBuffer;
		D3DVIEWPORT9 m_oldViewport;
	};


	inline bool cSurface::IsLoaded() const { return m_texture? true : false; }
	inline IDirect3DTexture9* cSurface::GetTexture() { return m_texture; }
	inline IDirect3DSurface9* cSurface::GetRenderTarget() { return m_surface; }
	inline IDirect3DSurface9* cSurface::GetZBuffer() { return m_zbuffer; }
}
