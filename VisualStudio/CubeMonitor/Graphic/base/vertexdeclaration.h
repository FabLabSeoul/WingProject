// ���ؽ� ����ü ������ ǥ���ϴ� Ŭ����
// D3DVERTEXELEMENT9 ����Ÿ�� ���� ����ϱ����� �������.
#pragma once


namespace graphic
{

	class cVertexDeclaration
	{
	public:
		cVertexDeclaration();
		virtual ~cVertexDeclaration();

		bool Create(const sRawMesh &rawMesh );
		int GetOffset( const BYTE usage, const BYTE usageIndex=0 );
		const vector<D3DVERTEXELEMENT9>& GetDecl() const;
		int GetElementSize() const;


	private:
		vector<D3DVERTEXELEMENT9> m_decl;
		int m_elementSize;
	};


	inline const vector<D3DVERTEXELEMENT9>& cVertexDeclaration::GetDecl() const { return m_decl; }
	inline int cVertexDeclaration::GetElementSize() const { return m_elementSize; }
}
