
#include "stdafx.h"
#include "../ai.h"

using namespace ai;


cObject::cObject(cObject *parent, const int id, const string &name)
	: m_parent(parent)
,	m_id(id)
,	m_name(name)
,	m_rootAction(NULL)
{
}

cObject::~cObject()
{
	Clear();
}


void cObject::AddChild(cObject *obj)
{
	m_children.push_back(obj);
}


bool cObject::RemoveChild(cObject *obj)
{
	RETV(!obj, false);

	for (u_int i = m_children.size()-1; i >= 0; --i)
	{
		if (*m_children[ i] == *obj)
		{
			delete m_children[ i];
			common::rotatepopvector(m_children, i);
		}
	}

	return true;
}


cObject* FindRec(const vector<cObject*> objs, int objId)
{
	for (u_int i=0; i < objs.size(); ++i)
	{
		cObject *obj = objs[ i]->Find(objId, true);
		if (obj)
			return obj;
	}
	return NULL;
}

// objId 찾기. 해당된 아이와 같은 cObject를 리턴한다. (this 포함)
cObject* cObject::Find(const int objId, bool isfindRecursive) // isfindRecursive=true
{
	if (GetId() == objId)
		return this;

	if (isfindRecursive)
	{
		return FindRec(m_children, objId);
	}
	else
	{
		for (u_int i=0; i < m_children.size(); ++i)
		{
			if (m_children[ i]->GetId() == objId)
				return m_children[ i];
		}
	}
	return NULL;
}


void cObject::PostMsg(const sMsg &msg)
{

}


void cObject::SendMsg(const sMsg &msg)
{

}


void cObject::Clear()
{
	ClearAction();

	BOOST_FOREACH (auto obj, m_children)
		delete obj;
	m_children.clear();
}


bool cObject::Move(const float elapseTime)
{

	return true;
}


bool cObject::DispatchMessage(const sMsg &msg)
{

	return true;
}


void cObject::SetAction(cAction *action)
{
	if (m_rootAction)
		ClearAction();

	m_rootAction = new cRootAction(this);
	StartAction();
	m_rootAction->PushAction(action);
}


void cObject::PushAction(cAction *action)
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


void cObject::StartAction()
{
	if (m_rootAction)
		m_rootAction->Start(NULL);
}


void cObject::EndAction()
{
	if (m_rootAction)
		m_rootAction->End();
	ClearAction();
}


bool cObject::IsAction(ACTION_TYPE::TYPE type)
{
	RETV(!m_rootAction, false);
	return m_rootAction->IsAction(type);
}


void cObject::ClearAction()
{
	SAFE_DELETE(m_rootAction);
}
