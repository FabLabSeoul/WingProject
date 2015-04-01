// cMeshBuffer
// 메쉬를 구성하는 버텍스 버퍼, 인덱스 버퍼, 속성 버퍼를 
// 관리하는 클래스다.
#pragma once


namespace graphic
{

	class cMeshBuffer
	{
	public:
		cMeshBuffer();
		cMeshBuffer(const sRawMesh &rawMesh);
		cMeshBuffer(const sRawBone &rawBone);

		void Bind();
		void Render(const int faceStart=0, const int faceCount=0);

		cMeshBuffer* Clone();
		vector<sAttribute>& GetAttributes();
		cVertexBuffer& GetVertexBuffer();
		cIndexBuffer& GetIndexBuffer();
		const cBoundingBox& GetBoundingBox() const;
		void SetOffset(const int offset);
		int GetOffset() const;


	protected:
		void CreateMesh( const vector<Vector3> &vertices, 
			const vector<Vector3> &normals, 
			const vector<Vector3> &tex,
			const vector<int> &indices);
		void CreateMesh( const sRawMesh &rawMesh );
		void CreateAttributes(const sRawMesh &rawMesh);


	private:
		vector<sAttribute> m_attributes;
		cVertexBuffer m_vtxBuff;
		cIndexBuffer m_idxBuff;
		int m_offset;
		cBoundingBox m_boundingBox;
	};


	inline vector<sAttribute>& cMeshBuffer::GetAttributes() { return m_attributes; }
	inline cVertexBuffer& cMeshBuffer::GetVertexBuffer() { return m_vtxBuff; }
	inline cIndexBuffer& cMeshBuffer::GetIndexBuffer() { return m_idxBuff; }
	inline const cBoundingBox& cMeshBuffer::GetBoundingBox() const { return m_boundingBox; }
	inline void cMeshBuffer::SetOffset(const int offset) { m_offset = offset; }
	inline int cMeshBuffer::GetOffset() const { return m_offset; }
}
