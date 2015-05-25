// 텍스쳐 클래스
// IDirect3DTexture9 인터페이스를 쉽게 사용하기위해 만들어짐.
#pragma once


namespace graphic
{

	class cTexture
	{
	public:
		cTexture();
		virtual ~cTexture();

		bool Create(const string &fileName, bool isSizePow2=true);
		bool Create(const int width, const int height, const D3DFORMAT format);
		bool WritePNGFile( const string &fileName );

		void Bind(const int stage);
		void Bind(cShader &shader, const string &key);
		void Unbind(const int stage);
		void Lock(D3DLOCKED_RECT &out);
		void Unlock();
		IDirect3DTexture9* GetTexture();
		const D3DXIMAGE_INFO& GetImageInfo() const;
		const string& GetTextureName() const;
		void Clear();
		

	protected:
		bool CreateEx(const string &fileName);


	private:
		IDirect3DTexture9 *m_texture;
		D3DXIMAGE_INFO m_imageInfo;
		string m_fileName;
	};


	inline IDirect3DTexture9* cTexture::GetTexture() { return m_texture; }
	inline const D3DXIMAGE_INFO& cTexture::GetImageInfo() const { return m_imageInfo; }
	inline const string& cTexture::GetTextureName() const { return m_fileName; }
}
