
#include "stdafx.h"
#include "move.h"
#include "../object/aiinterface.h"

using namespace ai;


cMove::cMove() 
	: cAction(NULL, "move", "zealot_walk.ani", ACTION_TYPE::MOVE) 
,	m_rotateTime(0)
,	m_speed(3)
,	m_rotateInterval(0.3f)
{
}


void cMove::Init(IArtificialIntelligence *obj, const Vector3 &dest)
{
	SetOwner(obj);
	m_dest = dest;
	m_rotateTime = 0;
}


void cMove::Start(cAction *prevAction)
{
	cAction::Start(prevAction);

	IArtificialIntelligence *obj = GetOwner();
	RET(!obj);

	const Vector3 curPos = obj->aiGetTransform().GetPosition();
	m_dir = m_dest - curPos;
	m_dir.y = 0;
	m_dir.Normalize();

	Quaternion q;
	q.SetRotationArc(Vector3(0,0,-1), m_dir);

	m_fromDir = obj->aiGetTransform().GetQuaternion();
	m_toDir = q;
	m_rotateTime = 0;
}


bool cMove::ActionUpdate(const float elapseTime)
{
	IArtificialIntelligence *obj = GetOwner();
	RETV(!obj, false);

	const Vector3 curPos = obj->aiGetTransform().GetPosition();

	// ȸ�� ���� ���
	if (m_rotateTime < m_rotateInterval)
	{
		m_rotateTime += elapseTime;

		const float alpha = min(1, m_rotateTime / m_rotateInterval);
		const Quaternion q = m_fromDir.Interpolate(m_toDir, alpha);
		obj->aiGetTransform() = q.GetMatrix();
	}

	// ĳ���� �̵�.
	const Vector3 pos = curPos + m_dir * m_speed * elapseTime;
	obj->aiGetTransform().SetPosition(pos);

	// �������� �����ٸ� ����.
	// �������� ���� ��, ����Ǵ� ������ ���� �� �ִ�.
	if (pos.LengthRoughly(m_dest) < 0.01f)
		return false; // ������ ����. �׼�����.

	return true;
}

