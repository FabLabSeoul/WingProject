#pragma once


namespace graphic
{

	class cSkinnedMesh : public cMesh
	{
	public:
		cSkinnedMesh(const int id, vector<Matrix44> *palette, const sRawMesh &raw);
		virtual ~cSkinnedMesh();

		virtual void Render(const Matrix44 &parentTm) override;

		void SetPalette(vector<Matrix44> *palette);


	protected:
		virtual void RenderShader( cShader &shader, const Matrix44 &parentTm );
		virtual void RenderShadow(cShader &shader, const Matrix44 &parentTm);

		void ApplyPalette();
		void ApplyPaletteShader(cShader &shader);


	private:
		const sRawMesh &m_rawMesh;
		vector<Matrix44> *m_palette; // reference
		cMeshBuffer *m_skinnMeshBuffer;
	};


	inline void cSkinnedMesh::SetPalette(vector<Matrix44> *palette) { m_palette = palette; }
}
