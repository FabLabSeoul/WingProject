
#include "stdafx.h"
#include "character.h"

using namespace graphic;


cCharacter::cCharacter(const int id) :
	cModel((id==-1)? common::GenerateId() : id)
,	m_action(CHARACTER_ACTION::NONE)
,	m_rootAction(NULL)
{
}

cCharacter::~cCharacter()
{
	Clear();
}


// ĳ���Ϳ��� ����� ������.
void cCharacter::Action(const CHARACTER_ACTION::TYPE type, const int param1, const int param2)
	// param1=0, param2 = 0
{
	RET(m_action == type);

	BOOST_FOREACH (auto &action, m_actions)
	{
		if (action.type == type)
		{
			m_action = type;
			SetAnimation(action.animationFile, 0, true);
			break;
		}
	}

}


// 
void cCharacter::SetActionData(const vector<sActionData> &actions)
{
	m_actions = actions;
}


bool cCharacter::Move(const float elapseTime) 
{
	cModel::Move(elapseTime);

	if (m_rootAction)
		m_rootAction->Move(elapseTime);

	return true;
}


void cCharacter::SetAction(ai::cAction *action)
{
	if (m_rootAction)
		ClearAction();

	m_rootAction = new ai::cRootAction(this);
	StartAction();
	m_rootAction->PushAction(action);
}


void cCharacter::PushAction(ai::cAction *action)
{
	RET(!action);

	if (m_rootAction)
	{
		m_rootAction->PushAction(action);
	}
	else
	{
		SetAction(action);
	}
}


void cCharacter::StartAction()
{
	if (m_rootAction)
		m_rootAction->Start(NULL);
}


void cCharacter::EndAction()
{
	if (m_rootAction)
		m_rootAction->End();
	ClearAction();
}


bool cCharacter::IsAction(ai::ACTION_TYPE::TYPE type)
{
	RETV(!m_rootAction, false);
	return m_rootAction->IsAction(type);
}


void cCharacter::ClearAction()
{
	SAFE_DELETE(m_rootAction);
}


Matrix44& cCharacter::aiGetTransform() 
{
	return m_TM; 
}


void cCharacter::aiSetAnimation(const string &animationName)
{
	// ���� �ִϸ��̼��̶�� �ٽ� �÷������� �ʴ´�.
	if (m_curAniName != animationName)
	{
		SetAnimation(animationName, 0, true);
		m_curAniName = animationName;
	}
}


void cCharacter::Clear()
{
	cModel::Clear();
	ClearAction();
}
