// ���� �׸��� Ŭ����
// �� �ϳ��� �׸��� �ؽ��ĸ� �����.
// �ϳ� �̻��� ���� �׸��ڸ� ǥ�� �� ���� �� Ŭ������ ������� �ʴ°� ����.
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

		// ������ �Լ�.
		void RenderShadowMap();


	private:
		cSurface2 m_surface;
	};


	inline IDirect3DTexture9* cShadow1::GetTexture() { return m_surface.GetTexture(); }
	inline const IDirect3DTexture9* cShadow1::GetTexture() const { return m_surface.GetTexture(); }
	inline bool cShadow1::IsLoaded() const { return m_surface.IsLoaded(); }
}
