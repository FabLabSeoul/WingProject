// RenderTarget 전용 서피스.
// cSurface 클래스 기능과 비슷하지만, 좀더 심플한 기능을 가졌다.
// 그림자 전용으로 만들어진 클래스다.
#pragma once


namespace graphic
{

	class cSurface2
	{
	public:
		cSurface2();
		virtual ~cSurface2();

		bool Create(int width, int height, int mipLevels,
			D3DFORMAT texFormat, bool useDepthBuffer,
			D3DFORMAT depthFormat, D3DVIEWPORT9& viewport, bool autoGenMips);

		void Begin();
		void End();
		void Render(const int index=1);
		void Clear();

		bool IsLoaded() const;
		IDirect3DTexture9* GetTexture();
		const IDirect3DTexture9* GetTexture() const;


	private:
		IDirect3DTexture9 *m_texture;
		ID3DXRenderToSurface *m_rts;
		IDirect3DSurface9 *m_surface;

		int m_width;
		int m_height;
		int m_mipLevels;
		D3DFORMAT m_texFormat;
		bool m_useDepthBuffer;
		D3DFORMAT m_depthFormat;
		D3DVIEWPORT9 m_viewPort;
		bool m_autoGenMips;
	};


	inline bool cSurface2::IsLoaded() const { return m_texture? true : false; }
	inline IDirect3DTexture9* cSurface2::GetTexture() { return m_texture; }
	inline const IDirect3DTexture9* cSurface2::GetTexture() const { return m_texture; }
}
