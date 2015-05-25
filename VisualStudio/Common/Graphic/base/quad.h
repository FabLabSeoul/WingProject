// �簢���� ��Ÿ���� Ŭ������.
// �����ϰ� �ؽ��ĸ� ���� �� ����� �� �ִ� Ŭ����.
#pragma once


namespace graphic
{

	class cQuad
	{
	public:
		cQuad();
		virtual ~cQuad();

		bool Create( const float width, const float height, const Vector3 &pos, 
			const string &textureFileName="" );

		void Render();
		void RenderLine();

		void SetTransform( const Matrix44 &tm );
		const Matrix44& GetTransform() const;
		cMaterial& GetMaterial();
		cVertexBuffer& GetVertexBuffer();
		cTexture* GetTexture();


	private:
		cVertexBuffer m_vtxBuff;
		cMaterial m_material;
		cTexture *m_texture; // reference
		Matrix44 m_tm;
	};


	inline cMaterial& cQuad::GetMaterial() { return m_material; }
	inline cVertexBuffer& cQuad::GetVertexBuffer() { return m_vtxBuff; }
	inline void cQuad::SetTransform( const Matrix44 &tm ) { m_tm = tm; }
	inline const Matrix44& cQuad::GetTransform() const { return m_tm; }
	inline cTexture* cQuad::GetTexture() { return m_texture; }
}
