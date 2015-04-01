#pragma once


namespace graphic
{


	class cSkyBox
	{
	public:
		cSkyBox();
		virtual ~cSkyBox();

		bool Create(const string &textureFilePath);
		void Render(const Matrix44 &tm = Matrix44::Identity);


	protected:
		bool CreateVertexBuffer();	


	private:
		enum {FRONT, BACK, LEFT, RIGHT, TOP, BOTTOM, MAX_FACE};
		cVertexBuffer m_vtxBuff;
		cTexture m_textures[ MAX_FACE];
	};


}
