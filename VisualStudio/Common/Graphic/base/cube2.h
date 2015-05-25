// 정육면체를 표현하는 클래스.
// 조명 효과를 넣을 수 있는 모델이다.
// Vertext + Normal + Texture UV 로 표현된다.
#pragma once


namespace graphic
{

	class cCube2
	{
	public:
		cCube2();
		cCube2(const Vector3 &vMin, const Vector3 &vMax );

		void SetCube(const Vector3 &vMin, const Vector3 &vMax );
		void SetCube(const cCube2 &cube);
		void SetTransform( const Matrix44 &tm );
		const Matrix44& GetTransform() const;
		const Vector3& GetMin() const;
		const Vector3& GetMax() const;
		const float Length() const; // length(min - max)
		cMaterial& GetMaterial();


		void Render(const Matrix44 &tm);


	protected:
		void InitCube();


	protected:
		cVertexBuffer m_vtxBuff;
		Vector3 m_min;
		Vector3 m_max;
		Matrix44 m_tm;
		cMaterial m_mtrl;
	};	


	inline void cCube2::SetTransform( const Matrix44 &tm ) { m_tm = tm; }
	inline const Matrix44& cCube2::GetTransform() const { return m_tm; }
	inline const Vector3& cCube2::GetMin() const { return m_min; }
	inline const Vector3& cCube2::GetMax() const { return m_max; }
	inline const float cCube2::Length() const { return (m_min - m_max).Length(); }
	inline cMaterial& cCube2::GetMaterial() { return m_mtrl; }
}
