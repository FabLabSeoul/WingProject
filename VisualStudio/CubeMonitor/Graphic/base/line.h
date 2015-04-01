// 정육면체 메쉬로해서 라인을 그린다.
#pragma once


namespace graphic
{

	class cLine
	{
	public:
		cLine();
		cLine(const Vector3 &p0, const Vector3 &p1, const float width);

		void Render();
		void SetLine(const Vector3 &p0, const Vector3 &p1, const float width);
		cMaterial& GetMaterial();		


	protected:
		void InitCube();


	private:
		cVertexBuffer m_vtxBuff;
		cIndexBuffer m_idxBuff;
		cMaterial m_material;
		Vector3 m_p0;
		Vector3 m_p1;
		float m_width;
		Matrix44 m_tm;
	};


	inline cMaterial& cLine::GetMaterial() { return m_material; }
}
