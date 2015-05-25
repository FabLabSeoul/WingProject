// 구 를 출력하는 클래스.
#pragma once


namespace graphic
{

	class cSphere
	{
	public:
		cSphere();
		cSphere(const float radius, const int stacks, const int slices);
		virtual ~cSphere();

		void Create(const float radius, const int stacks, const int slices);
		void SetTransform( const Matrix44 &tm );
		const Matrix44& GetTransform() const;
		cMaterial& GetMaterial();

		void Render(const Matrix44 &tm);
		void RenderShader(cShader &shader, const Matrix44 &tm);


	private:
		cVertexBuffer m_vtxBuff;
		cIndexBuffer m_idxBuff;
		Matrix44 m_tm;
		float m_radius;
		int m_numSlices;
		int m_numStacks;
		cMaterial m_mtrl;
	};	


	inline void cSphere::SetTransform( const Matrix44 &tm ) { m_tm = tm; }
	inline const Matrix44& cSphere::GetTransform() const { return m_tm; }
	inline cMaterial& cSphere::GetMaterial() { return m_mtrl; }
}
