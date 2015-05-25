// 눈 파티클 효과.
#pragma once


namespace graphic
{

	class cSnow : public cParticles
	{
	public:
		cSnow();
		virtual ~cSnow();

		bool Create( const Vector3 &_min, const Vector3 &_max, 
			const int particleCount );
		virtual void Move( const float elapseTime) override;
		virtual void Render();
		bool IsRenderFrustum() const;
		void SetRenderFrustum(bool isRender);


	private:
		cFrustum m_frustum; // 경계 박스.

		// 디버깅용.
		bool m_isRenderFrustum; // default : true
	};


	inline bool cSnow::IsRenderFrustum() const { return m_isRenderFrustum; }
	inline void cSnow::SetRenderFrustum(bool isRender) { m_isRenderFrustum = isRender; }
}
