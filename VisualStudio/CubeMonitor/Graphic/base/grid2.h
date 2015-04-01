// grid format
// vertex + normal + texture
#pragma once


namespace graphic
{

	class cGrid2
	{
	public:
		cGrid2();
		virtual ~cGrid2();

		void Create( const int rowCellCount, const int colCellCount, const float cellSize, 
			const float textureUVFactor=8.f, const float offsetY=0.f);
		bool CreateFromFile(const string &fileName);
		bool WriteFile(const string &fileName);

		void Render(const Matrix44 &tm = Matrix44::Identity, const int stage=0);
		void RenderLinelist();
		void RenderShader(cShader &shader, const Matrix44 &tm = Matrix44::Identity);

		void SetTextureUVFactor(const float textureUVFactor);
		float GetTextureUVFactor() const;
		int GetRowCellCount() const;
		int GetColCellCount() const;
		int GetRowVertexCount() const;
		int GetColVertexCount() const;
		float GetCellSize() const;
		float GetWidth() const;
		float GetHeight() const;

		void CalculateNormals();
		bool Pick( const Vector3 &orig, const Vector3 &dir, Vector3 &out );

		void Clear();

		cVertexBuffer& GetVertexBuffer();
		cIndexBuffer& GetIndexBuffer();
		cTexture& GetTexture();
		cMaterial& GetMaterial();		


	private:
		cVertexBuffer m_vtxBuff;
		cIndexBuffer m_idxBuff;
		cTexture m_tex;
		cMaterial m_mtrl;

		int m_rowCellCount;
		int m_colCellCount;
		float m_cellSize;
		float m_textureUVFactor;
	};


	inline cVertexBuffer& cGrid2::GetVertexBuffer() { return m_vtxBuff; }
	inline cIndexBuffer& cGrid2::GetIndexBuffer() { return m_idxBuff; }
	inline cTexture& cGrid2::GetTexture() { return m_tex; }
	inline cMaterial& cGrid2::GetMaterial() { return m_mtrl; }
	inline int cGrid2::GetRowCellCount() const { return m_rowCellCount; }
	inline int cGrid2::GetColCellCount() const { return m_colCellCount; }
	inline int cGrid2::GetRowVertexCount() const { return m_rowCellCount + 1; }
	inline int cGrid2::GetColVertexCount() const { return m_colCellCount + 1; }
	inline float cGrid2::GetCellSize() const { return m_cellSize; }
	inline float cGrid2::GetWidth() const { return m_colCellCount * m_cellSize; }
	inline float cGrid2::GetHeight() const { return m_rowCellCount * m_cellSize; }
	inline float cGrid2::GetTextureUVFactor() const { return m_textureUVFactor; }
}
