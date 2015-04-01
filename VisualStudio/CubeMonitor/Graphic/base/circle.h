#pragma once


namespace graphic
{

	class cCircle
	{
	public:
		cCircle();
		virtual ~cCircle();

		bool Create(const Vector3 &center, const float radius, const int slice);
		void Render();


	private:
		float m_radius;
		Matrix44 m_tm;
		cVertexBuffer m_vtxBuff;
	};

}
