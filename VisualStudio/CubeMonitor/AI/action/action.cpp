
#include "stdafx.h"
#include "../ai.h"

using namespace ai;


cAction::cAction(IArtificialIntelligence *obj, const string &name, 
	const string &animationName, const ACTION_TYPE::TYPE type)
:	m_owner(obj)
,	m_name(name)
,	m_animationName(animationName)
,	m_type(type)
,	m_state(ACTION_STATE::WAIT)
,	m_current(NULL)
{
}

cAction::~cAction()
{
	Clear();
}


void cAction::Start(cAction *prevAction)
{
	m_state = ACTION_STATE::RUN;
	m_current = m_children.empty()? NULL : m_children.back();
	if (m_current)
	{
		m_current->Start(prevAction);
	}
	else
	{
		// �ִϸ��̼� ����.
		if (m_owner && !m_animationName.empty())
			m_owner->aiSetAnimation(m_animationName);
	}
}


void cAction::Resume(cAction *prevAction)
{

}


void cAction::End()
{
	m_state = ACTION_STATE::STOP;
	ClearChildAction();
}


// Action�� ���� ���̶�� true�� �����ϰ�, 
// �ƹ� �۾��� ���� �ʴ´ٸ� false �� �����Ѵ�.
bool cAction::Move(const float elapseTime)
{
	if (!IsRun())
		return false;

	if (m_current)
	{
		if (!Update(elapseTime))
			return true;

		if (!m_current->Move(elapseTime))
			NextAction();
	}
	else
	{
		if (!ActionUpdate(elapseTime))
			return false;
	}

	return true;
}


// ���� �׼����� �Ѿ��.
void cAction::NextAction()
{
	cAction *prevAction = m_current;
	m_current = GetNextAction();
	if (m_current)
	{
		if (m_current->GetState() == ACTION_STATE::WAIT)
			m_current->Start(prevAction);
		else
			m_current->Resume(prevAction);
	}

	SAFE_DELETE(prevAction);
}


// false�� �����ϸ� child�� Move()�� ȣ������ �ʴ´�.
bool cAction::Update(const float elapseTime)
{
	// �Ļ��� Ŭ�������� �����Ѵ�.
	return true;
}


// Leaf��� �϶��� ȣ��ȴ�.
// �ൿ�� ó���� �� ���⿡ �ڵ��� �ϸ�ȴ�.
// false �� �����ϸ� �ൿ�� ����Ǿ��ٴ� ���̴�.
bool cAction::ActionUpdate(const float elapseTime)
{
	// �Ļ��� Ŭ�������� �����Ѵ�.
	return true;
}


// ���� ���� �ڽ� Action�� �޼����� ������.
bool cAction::MessageProccess(const sMsg &msg)
{
	if (!IsRun())
		return false;

	if (m_current)
		m_current->MessageProccess(msg);

	return true;
}


void cAction::Clear()
{
	m_state = ACTION_STATE::STOP;
	ClearChildAction();
}


// Action�� ���ÿ� �߰��Ѵ�.
void cAction::PushAction(cAction *action)
{
	action->SetParent(this);
	m_children.push_back(action);

	// currentAction �缳���Ѵ�. currentAction�� ���� �ֱ� ���̾�� �Ѵ�.
	m_current = action;
}


// Action�� �����ϰ�, �� Action �� current�� �Ҵ��Ѵ�.
void cAction::PopAction()
{
	if (m_current)
	{
		delete m_current;
	}

	m_current = GetNextAction();
}


// ���� �׼��� �����Ѵ�.
cAction* cAction::GetNextAction()
{
	if (m_children.empty())
		return NULL;
	m_children.pop_back();
	if (m_children.empty())
		return NULL;

	return m_children.back();
}


// ���ڷ� �Ѿ�� action�� �޸𸮱��� �����Ѵ�.
void cAction::RemoveAction(cAction *action)
{
	for (u_int i=m_children.size()-1; i >= 0; --i)
	{
		if (*action == *m_children[ i])
		{
			delete m_children[ i];
			rotatepopvector(m_children, i);
		}
	}

	// currentAction �缳���Ѵ�.
	m_current = m_children.empty()? NULL : m_children.back();
}


void cAction::RemoveAction(const string &actionName)
{
	for (u_int i=m_children.size()-1; i >= 0; --i)
	{
		if (actionName == m_children[ i]->GetName())
		{
			delete m_children[ i];
			rotatepopvector(m_children, i);
		}
	}

	// currentAction �缳���Ѵ�.
	m_current = m_children.empty()? NULL : m_children.back();
}


// �ڽ����� �߰��� Action�� �����Ѵ�.
void cAction::ClearChildAction()
{
	BOOST_FOREACH (auto action, m_children)
	{
		if (action->IsRun())
			action->End();
		delete action;
	}
	m_children.clear();
	m_current = NULL;
}


cAction* cAction::FindAction(const string &actionName, bool isFindChild)//isFindChild=true
{
	for (u_int i=0; i < m_children.size(); ++i)
	{
		if (actionName == m_children[ i]->GetName())
			return m_children[ i];
	}

	if (isFindChild)
	{
		for (u_int i=0; i < m_children.size(); ++i)
		{
			if (cAction *action = m_children[ i]->FindAction(actionName, isFindChild))
				return action;
		}
	}

	return NULL;
}


cAction* cAction::FindAction(ACTION_TYPE::TYPE type, bool isFindChild)//isFindChild=true
{
	for (u_int i=0; i < m_children.size(); ++i)
	{
		if (m_children[ i]->IsCurrentAction(type))
			return m_children[ i];
	}

	if (isFindChild)
	{
		for (u_int i=0; i < m_children.size(); ++i)
		{
			if (cAction *action = m_children[ i]->FindAction(type, isFindChild))
				return action;
		}
	}

	return NULL;
}


bool cAction::IsRun()
{
	switch (m_state)
	{
	case ACTION_STATE::RUN:
		return true;

	case ACTION_STATE::WAIT:
	case ACTION_STATE::STOP:
	default:
		break;
	}
	return false;
}


// Action Ÿ���� ���Ѵ�. this �� current �߿� 
// ���� �� Ÿ���� �ִٸ� true fmf flxjsgksek.
bool cAction::IsAction(ACTION_TYPE::TYPE type)
{
	if (IsCurrentAction(type))
		return true;
	if (m_current)
		m_current->IsAction(type);
	return false;
}


// Leaf ��带 �����Ѵ�.
cAction* cAction::GetLeafAction()
{
	cAction *action = GetCurrentAction();
	if (!action)
		return this;
	return action->GetLeafAction();
}

