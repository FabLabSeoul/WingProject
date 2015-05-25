
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
		// 애니메이션 실행.
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


// Action이 실행 중이라면 true를 리턴하고, 
// 아무 작업도 하지 않는다면 false 를 리턴한다.
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


// 다음 액션으로 넘어간다.
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


// false를 리턴하면 child의 Move()는 호출하지 않는다.
bool cAction::Update(const float elapseTime)
{
	// 파생된 클래스에서 구현한다.
	return true;
}


// Leaf노드 일때만 호출된다.
// 행동을 처리할 때 여기에 코딩을 하면된다.
// false 를 리턴하면 행동이 종료되었다는 뜻이다.
bool cAction::ActionUpdate(const float elapseTime)
{
	// 파생된 클래스에서 구현한다.
	return true;
}


// 실행 중인 자식 Action에 메세지를 보낸다.
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


// Action을 스택에 추가한다.
void cAction::PushAction(cAction *action)
{
	action->SetParent(this);
	m_children.push_back(action);

	// currentAction 재설정한다. currentAction은 가장 최근 값이어야 한다.
	m_current = action;
}


// Action을 제거하고, 새 Action 을 current에 할당한다.
void cAction::PopAction()
{
	if (m_current)
	{
		delete m_current;
	}

	m_current = GetNextAction();
}


// 다음 액션을 리턴한다.
cAction* cAction::GetNextAction()
{
	if (m_children.empty())
		return NULL;
	m_children.pop_back();
	if (m_children.empty())
		return NULL;

	return m_children.back();
}


// 인자로 넘어온 action의 메모리까지 제거한다.
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

	// currentAction 재설정한다.
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

	// currentAction 재설정한다.
	m_current = m_children.empty()? NULL : m_children.back();
}


// 자식으로 추가된 Action을 제거한다.
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


// Action 타입을 비교한다. this 와 current 중에 
// 같은 의 타입이 있다면 true fmf flxjsgksek.
bool cAction::IsAction(ACTION_TYPE::TYPE type)
{
	if (IsCurrentAction(type))
		return true;
	if (m_current)
		m_current->IsAction(type);
	return false;
}


// Leaf 노드를 리턴한다.
cAction* cAction::GetLeafAction()
{
	cAction *action = GetCurrentAction();
	if (!action)
		return this;
	return action->GetLeafAction();
}

