
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
// nGroup: 0, 1 만 지원한다.
// 충돌테스트는 다른 그룹끼리만 가능하다.
// pObj : 충돌테스트할 오브젝트
// nTestNum : 충돌테스트 번호이며, 다른 그룹에서 같은 충돌테스트번호 끼리 충돌테스트한다.
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
// 충돌테스트할 오브젝트 추가
// pParent : 부모 충돌 오브젝트
// pParent 오브젝트가 충돌된후 자식으로 pObj 오브젝트가 충돌테스트 검사된다.
//-----------------------------------------------------------------------------//
void cCollisionManager::InsertObject( ICollisionable *parent, ICollisionable *obj, int testNum )
{
	RET(!parent);

	// 두그룹에서 부모 노드를 찾는다.
	sCollisionNode *node;
	node = (sCollisionNode*)m_group1->FindNode( parent->GetCollisionId() );
	if (!node) 
		node = (sCollisionNode*)m_group2->FindNode( parent->GetCollisionId() );

	InsertObject( node, obj, testNum );
}


//-----------------------------------------------------------------------------//
// 충돌테스트할 오브젝트 추가
// nTestNum 이 3는 오브젝트가 이동할때 충돌을 체크하는 번호이다.
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
// 리스트에서 오브젝트 추가
//-----------------------------------------------------------------------------//
void cCollisionManager::RemoveObject( ICollisionable *obj )
{
	m_group1->RemoveNode( obj->GetCollisionId() );
	m_group2->RemoveNode( obj->GetCollisionId() );
}


//-----------------------------------------------------------------------------//
// 충돌테스트하기 전에 CollisionBox 좌표를 업데이트한다.
// 모델이 이동되었다면 CollisionTest() 함수 호출 전에 이 함수를 먼저 호출해야 한다.
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
// nTestNum 값으로 설정된 CollisionBox들만 충돌테스트한다.
// 같은그룹끼리는 충돌테스트 하지 않는다.
// 충돌된 오브젝트 갯수를 리턴한다.
//-----------------------------------------------------------------------------//
int cCollisionManager::CollisionTest( int testNum )
{
	m_collisionCount = 0;
	CollisionTestRec(m_group1, m_group2, testNum);

	// 충돌한 객체의 Collision() 함수를 호출한다.
	for( int i=0; i < m_collisionCount; ++i )
	{
		m_checkTable[ i][ 0]->Collision( testNum, m_checkTable[ i][ 1] );
		m_checkTable[ i][ 1]->Collision( testNum, m_checkTable[ i][ 0] );
	}

	return m_collisionCount;
}


// 충돌검사 재귀 함수 시작.
void cCollisionManager::CollisionTestRec( 
	sCollisionNode *node1, sCollisionNode *node2, int testNum )
{
	RET(!node1 || !node2);

	if (node1->GetChildren().empty() && node2->GetChildren().empty())
	{
		// 루트 그룹은 testnum 값이 0이다. 
		// 루트 그룹은 충돌 검사에 포함하지 않는다.
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

			// 충돌 테스트
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


// node1 고정, node2 children loop
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

		// 충돌 테스트
		if (CheckNodeCollision(node1, collisionNode, testNum))
		{
			CollisionTestNode1Loop(node1, collisionNode, testNum);
		}
	}
}


// node2 고정, node1 children loop
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

		// 충돌 테스트
		if (CheckNodeCollision(collisionNode, node2, testNum))
		{
			CollisionTestNode2Loop(collisionNode, node2, testNum);
		}
	}

}


// 충돌 체크.
bool cCollisionManager::CheckNodeCollision(sCollisionNode *node1, sCollisionNode *node2, int testNum )
{
	RETV(!node1 || !node2, false);

	// Src TestNum값이 0일때는 Target의 TestNum상관없이 충돌테스트를 한다.
	// Src TestNum값이 설정되어 있을때는 인자로 넘어온 nTestNum값과 Target의 TestNum이 같을때만 충돌테스트한다.
	if ( ((0 != node1->testnum) && (testNum != node1->testnum)) 
		|| (((0 != node1->testnum) && (0 != node2->testnum)) && (node1->testnum != node2->testnum)) )
		return false;

	// Sphere vs Sphere 충돌테스트

	// Box vs Box 충돌테스트
	if (node1->box && node2->box)
	{
		if (node1->box->Collision(*node2->box))
		{
			return true;
		}
	}

	return false;
}
