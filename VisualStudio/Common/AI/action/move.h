// �̵� �ൿ.

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
		float m_rotateInterval; // ȸ�� ���� �ð�, 0.3��

		Vector3 m_dir; // �̵� ����
		Quaternion m_fromDir; // �̵� �� ����
		Quaternion m_toDir; // �̵� �� ����
		float m_rotateTime; // ȸ�� ���� �ð�.
	};

}
