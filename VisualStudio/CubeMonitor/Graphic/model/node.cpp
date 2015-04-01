
#include "stdafx.h"
#include "node.h"

using namespace graphic;


cNode::cNode( const int id, const string &name) : 
	m_id(id)
,	m_name(name)
,	m_parent(NULL)
,	m_shader(NULL)
,	m_isRender(true)
{

}

cNode::~cNode()
{
	Clear();
}


// ��带 �ڽ����� �߰��Ѵ�.
bool cNode::InsertChild(cNode *node)
{
	RETV(!node, false);
	RETV(FindNode(node->GetId()), false); // Already Exist
	
	node->m_parent = this;
	m_children.push_back(node);
	return true;
}


// �ڽ� ��� �߿� id �� ���� ��尡 �ִٸ� �����Ѵ�.
const cNode* cNode::FindNode(const int id) const
{
	if (m_id == id)
		return this;

	BOOST_FOREACH (auto node, m_children)
	{
		if (const cNode *ret = node->FindNode(id))
			return ret;
	}

	return NULL;
}


// �ڽ� ��� �߿� name�� ���� ��尡 �ִٸ� �����Ѵ�.
const cNode* cNode::FindNode(const string &name) const
{
	if (m_name == name)
		return this;

	BOOST_FOREACH (auto node, m_children)
	{
		if (const cNode *ret = node->FindNode(name))
			return ret;
	}

	return NULL;
}

// id ��带 �����Ѵ�. �޸𸮱��� ���ŵȴ�.
bool cNode::RemoveNode(const int id)
{
	BOOST_FOREACH (auto node, m_children)
	{
		if (node->GetId() == id)
		{
			delete node;
			common::removevector(m_children, node);
			return true;
		}
	}

	BOOST_FOREACH (auto node, m_children)
	{
		if (node->RemoveNode(id))
			return true;
	}

	return false;
}


// �ڽĳ���� Render �� ȣ���Ѵ�.
void cNode::Render(const Matrix44 &parentTm)
{
	RET(!m_isRender);
	
	BOOST_FOREACH (auto node, m_children)
	{
		node->Render(parentTm);
	}
}


// �ڽĳ���� RenderShadow�� ȣ���Ѵ�.
void cNode::RenderShadow(const Matrix44 &viewProj, 
	const Vector3 &lightPos, const Vector3 &lightDir, const Matrix44 &parentTm)
{
	BOOST_FOREACH (auto node, m_children)
	{
		node->RenderShadow(viewProj, lightPos, lightDir, parentTm);
	}
}



//// ���̴� ���, ���̴��� �������ϴ� ��ü���� �����Ѵ�.
//void cNode::RenderShader( const Matrix44 &parentTm )
//{
//	BOOST_FOREACH (auto node, m_children)
//	{
//		node->RenderShader(parentTm);
//	}
//}
//
//
//// ���̴��� �̿��ؼ� ����Ѵ�.
//void cNode::RenderShader( cShader &shader, const Matrix44 &parentTm )
//{
//	BOOST_FOREACH (auto node, m_children)
//	{
//		node->RenderShader(shader, parentTm);
//	}
//}
//
//
//// ���̴��� �̿��ؼ� ����Ѵ�.
//void cNode::RenderShadow( cShader &shader, const Matrix44 &parentTm )
//{
//	BOOST_FOREACH (auto node, m_children)
//	{
//		node->RenderShadow(shader, parentTm);
//	}
//}


// ��� ��带 �����Ѵ�.
void cNode::Clear()
{
	BOOST_FOREACH (auto node, m_children)
	{
		node->Clear();
		delete node;
	}

	m_children.clear();
}
