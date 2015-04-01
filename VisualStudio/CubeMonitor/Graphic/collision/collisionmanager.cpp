
#include "stdafx.h"
#include "collisionmanager.h"
#include "collision.h"
#include <list>


using namespace graphic;


cCollisionManager::cCollisionManager()
{
	m_group1 = new sCollisionNode(-1);
	m_group2 = new sCollisionNode(-1);
}

cCollisionManager::~cCollisionManager()
{
	SAFE_DELETE(m_group1);
	SAFE_DELETE(m_group2);
}


//-----------------------------------------------------------------------------//
// nGroup: 0, 1 �� �����Ѵ�.
// �浹�׽�Ʈ�� �ٸ� �׷쳢���� �����ϴ�.
// pObj : �浹�׽�Ʈ�� ������Ʈ
// nTestNum : �浹�׽�Ʈ ��ȣ�̸�, �ٸ� �׷쿡�� ���� �浹�׽�Ʈ��ȣ ���� �浹�׽�Ʈ�Ѵ�.
//-----------------------------------------------------------------------------//
void cCollisionManager::InsertObject( int group, ICollisionable *obj, int nTestNum )
{
	sCollisionNode *node;
	if (0 == group) 
		node = m_group1;
	else if (1 == group) 
		node = m_group2;
	else 
		return;

	InsertObject( node, obj, nTestNum );
}


//-----------------------------------------------------------------------------//
// �浹�׽�Ʈ�� ������Ʈ �߰�
// pParent : �θ� �浹 ������Ʈ
// pParent ������Ʈ�� �浹���� �ڽ����� pObj ������Ʈ�� �浹�׽�Ʈ �˻�ȴ�.
//-----------------------------------------------------------------------------//
void cCollisionManager::InsertObject( ICollisionable *parent, ICollisionable *obj, int testNum )
{
	RET(!parent);

	// �α׷쿡�� �θ� ��带 ã�´�.
	sCollisionNode *node;
	node = (sCollisionNode*)m_group1->FindNode( parent->GetCollisionId() );
	if (!node) 
		node = (sCollisionNode*)m_group2->FindNode( parent->GetCollisionId() );

	InsertObject( node, obj, testNum );
}


//-----------------------------------------------------------------------------//
// �浹�׽�Ʈ�� ������Ʈ �߰�
// nTestNum �� 3�� ������Ʈ�� �̵��Ҷ� �浹�� üũ�ϴ� ��ȣ�̴�.
//-----------------------------------------------------------------------------//
void cCollisionManager::InsertObject( sCollisionNode *parent, ICollisionable *obj, int testNum )
{
	sCollisionNode *pnew = new sCollisionNode( obj->GetCollisionId() );
	pnew->testnum = testNum;
	pnew->pobj = obj;
	pnew->box = obj->GetCollisionBox();
	//pnew->psphere = pObj->GetSphere();

	parent->InsertChild(pnew);
}


//-----------------------------------------------------------------------------//
// ����Ʈ���� ������Ʈ �߰�
//-----------------------------------------------------------------------------//
void cCollisionManager::RemoveObject( ICollisionable *obj )
{
	m_group1->RemoveNode( obj->GetCollisionId() );
	m_group2->RemoveNode( obj->GetCollisionId() );
}


//-----------------------------------------------------------------------------//
// �浹�׽�Ʈ�ϱ� ���� CollisionBox ��ǥ�� ������Ʈ�Ѵ�.
// ���� �̵��Ǿ��ٸ� CollisionTest() �Լ� ȣ�� ���� �� �Լ��� ���� ȣ���ؾ� �Ѵ�.
//-----------------------------------------------------------------------------//
void cCollisionManager::UpdateCollisionBox()
{
	UpdateCollisionBoxRec(m_group1);
	UpdateCollisionBoxRec(m_group2);
}

void cCollisionManager::UpdateCollisionBoxRec( sCollisionNode *obj )
{
	RET(!obj);

	if (obj->pobj)
		obj->pobj->UpdateCollisionBox();
	BOOST_FOREACH (auto &node, obj->GetChildren())
		UpdateCollisionBoxRec((sCollisionNode*)node);
}


//-----------------------------------------------------------------------------//
// nTestNum ������ ������ CollisionBox�鸸 �浹�׽�Ʈ�Ѵ�.
// �����׷쳢���� �浹�׽�Ʈ ���� �ʴ´�.
// �浹�� ������Ʈ ������ �����Ѵ�.
//-----------------------------------------------------------------------------//
int cCollisionManager::CollisionTest( int testNum )
{
	m_collisionCount = 0;
	CollisionTestRec(m_group1, m_group2, testNum);

	// �浹�� ��ü�� Collision() �Լ��� ȣ���Ѵ�.
	for( int i=0; i < m_collisionCount; ++i )
	{
		m_checkTable[ i][ 0]->Collision( testNum, m_checkTable[ i][ 1] );
		m_checkTable[ i][ 1]->Collision( testNum, m_checkTable[ i][ 0] );
	}

	return m_collisionCount;
}


// �浹�˻� ��� �Լ� ����.
void cCollisionManager::CollisionTestRec( 
	sCollisionNode *node1, sCollisionNode *node2, int testNum )
{
	RET(!node1 || !node2);

	if (node1->GetChildren().empty() && node2->GetChildren().empty())
	{
		// ��Ʈ �׷��� testnum ���� 0�̴�. 
		// ��Ʈ �׷��� �浹 �˻翡 �������� �ʴ´�.
		if ((node1->testnum != 0) && (node2->testnum != 0))
		{
			m_checkTable[ m_collisionCount][ 0] = node1->pobj;
			m_checkTable[ m_collisionCount][ 1] = node2->pobj;
			m_collisionCount++;
		}

		return;
	}
	

	BOOST_FOREACH (cNode *p1, node1->GetChildren())
	{
		sCollisionNode *collisionNode1 = (sCollisionNode*)p1;
		if (!collisionNode1->pobj->IsTest(testNum))
			continue;

		BOOST_FOREACH (cNode *p2, node2->GetChildren())
		{
			sCollisionNode *collisionNode2 = (sCollisionNode*)p2;
			if (!collisionNode2->pobj->IsTest(testNum))
				continue;

			// �浹 �׽�Ʈ
			if (CheckNodeCollision(collisionNode1, collisionNode2, testNum))
			{
				if (collisionNode1->GetChildren().empty())
				{
					CollisionTestNode1Loop(collisionNode1, collisionNode2, testNum);
				}
				else if (collisionNode2->GetChildren().empty())
				{
					CollisionTestNode2Loop(collisionNode1, collisionNode2, testNum);
				}
				else
				{
					CollisionTestRec(collisionNode1, collisionNode2, testNum);
				}
			}
		}
	}

}


// node1 ����, node2 children loop
void cCollisionManager::CollisionTestNode1Loop( 
	sCollisionNode *node1, sCollisionNode *node2, int testNum )
{
	RET(!node1 || !node2);

	if (node1->GetChildren().empty() && node2->GetChildren().empty())
	{
		m_checkTable[ m_collisionCount][ 0] = node1->pobj;
		m_checkTable[ m_collisionCount][ 1] = node2->pobj;
		m_collisionCount++;
		return;
	}

	BOOST_FOREACH (cNode *p2, node2->GetChildren())
	{
		sCollisionNode *collisionNode = (sCollisionNode*)p2;
		if (!collisionNode->pobj->IsTest(testNum))
			continue;

		// �浹 �׽�Ʈ
		if (CheckNodeCollision(node1, collisionNode, testNum))
		{
			CollisionTestNode1Loop(node1, collisionNode, testNum);
		}
	}
}


// node2 ����, node1 children loop
void cCollisionManager::CollisionTestNode2Loop( 
	sCollisionNode *node1, sCollisionNode *node2, int testNum )
{
	RET(!node1 || !node2);

	if (node1->GetChildren().empty() && node2->GetChildren().empty())
	{
		m_checkTable[ m_collisionCount][ 0] = node1->pobj;
		m_checkTable[ m_collisionCount][ 1] = node2->pobj;
		m_collisionCount++;
		return;
	}

	BOOST_FOREACH (cNode *p1, node1->GetChildren())
	{
		sCollisionNode *collisionNode = (sCollisionNode*)p1;
		if (!collisionNode->pobj->IsTest(testNum))
			continue;

		// �浹 �׽�Ʈ
		if (CheckNodeCollision(collisionNode, node2, testNum))
		{
			CollisionTestNode2Loop(collisionNode, node2, testNum);
		}
	}

}


// �浹 üũ.
bool cCollisionManager::CheckNodeCollision(sCollisionNode *node1, sCollisionNode *node2, int testNum )
{
	RETV(!node1 || !node2, false);

	// Src TestNum���� 0�϶��� Target�� TestNum������� �浹�׽�Ʈ�� �Ѵ�.
	// Src TestNum���� �����Ǿ� �������� ���ڷ� �Ѿ�� nTestNum���� Target�� TestNum�� �������� �浹�׽�Ʈ�Ѵ�.
	if ( ((0 != node1->testnum) && (testNum != node1->testnum)) 
		|| (((0 != node1->testnum) && (0 != node2->testnum)) && (node1->testnum != node2->testnum)) )
		return false;

	// Sphere vs Sphere �浹�׽�Ʈ

	// Box vs Box �浹�׽�Ʈ
	if (node1->box && node2->box)
	{
		if (node1->box->Collision(*node2->box))
		{
			return true;
		}
	}

	return false;
}
