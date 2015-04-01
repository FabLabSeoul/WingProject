// 이동 행동.

#pragma once

#include "action.h"

namespace ai
{
	class cAction;
	class IArtificialIntelligence;

	class cMove : public cAction
						, public common::cMemoryPool<cMove>
	{
	public:
		cMove();

		void Init(IArtificialIntelligence *obj, const Vector3 &dest);
		virtual bool ActionUpdate(const float elapseTime) override;
		virtual void Start(cAction *prevAction) override;


	private:
		Vector3 m_dest;
		float m_speed; // 3.f
		float m_rotateInterval; // 회전 보간 시간, 0.3초

		Vector3 m_dir; // 이동 방향
		Quaternion m_fromDir; // 이동 전 방향
		Quaternion m_toDir; // 이동 후 방향
		float m_rotateTime; // 회전 중인 시간.
	};

}
