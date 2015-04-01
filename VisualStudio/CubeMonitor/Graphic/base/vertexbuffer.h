// 버텍스 버퍼 클래스.
// LPDIRECT3DVERTEXBUFFER9 인터페이스를 쉽게 쓰기위해 만들어짐.
#pragma once


namespace graphic
{

	class cVertexBuffer
	{
	public:
		cVertexBuffer();
		virtual ~cVertexBuffer();

		bool Create(const int vertexCount, const int sizeofVertex, DWORD fvf);
		bool CreateVMem(const int vertexCount, const int sizeofVertex, DWORD fvf);
		bool Create(const int vertexCount, const int sizeofVertex, const cVertexDeclaration &decl);

		void* Lock();
		void* LockDiscard(const int idx=0, const int size=0);
		void* LockNooverwrite(const int idx=0, const int size=0);
		void Unlock();
		void Bind() const;
		void Clear();

		void RenderTriangleStrip();
		void RenderLineStrip();
		void RenderPointList(const int count=0);

		DWORD GetFVF() const;
		int GetSizeOfVertex() const;
		int GetVertexCount() const;
		int GetOffset( const BYTE usage, const BYTE usageIndex=0 );

		cVertexBuffer& operator=(cVertexBuffer &rhs);


	private:
		LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
		LPDIRECT3DVERTEXDECLARATION9 m_pVtxDecl;
		cVertexDeclaration m_vtxDecl;
		DWORD m_fvf; // m_pVtxDecl 를 사용할 때는 fvf 를 사용하지 않는다.
		int m_sizeOfVertex; // stride
		int m_vertexCount;
		bool m_isManagedPool;
	};

	
	inline DWORD cVertexBuffer::GetFVF() const { return m_fvf; }
	inline int cVertexBuffer::GetSizeOfVertex() const { return m_sizeOfVertex; }
	inline int cVertexBuffer::GetVertexCount() const { return m_vertexCount; }
	inline int cVertexBuffer::GetOffset( const BYTE usage, const BYTE usageIndex ) { return m_vtxDecl.GetOffset(usage, usageIndex); }
}
