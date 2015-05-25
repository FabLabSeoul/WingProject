// 투영 그림자 클래스
// 모델 하나의 그림자 텍스쳐를 만든다.
// 하나 이상의 모델의 그림자를 표현 할 때는 이 클래스를 사용하지 않는게 좋다.
#pragma once


namespace graphic
{

	class cShadow1
	{
	public:
		cShadow1();
		virtual ~cShadow1();

		bool Create(const int textureWidth, const int textureHeight);
		void UpdateShadow( cNode &node );
		IDirect3DTexture9* GetTexture();
		const IDirect3DTexture9* GetTexture() const;
		bool IsLoaded() const;

		// 디버깅용 함수.
		void RenderShadowMap();


	private:
		cSurface2 m_surface;
	};


	inline IDirect3DTexture9* cShadow1::GetTexture() { return m_surface.GetTexture(); }
	inline const IDirect3DTexture9* cShadow1::GetTexture() const { return m_surface.GetTexture(); }
	inline bool cShadow1::IsLoaded() const { return m_surface.IsLoaded(); }
}
