#pragma once

// CollisionManager 클래스는 충돌테스트를 일괄적으로 처리한다.
// 충돌테스트될 오브젝트를 2개의 그룹으로 나누고, 각 그룹은
// 트리 자료구조 형태다. 즉 부모끼리 충돌테스트가 성공하면 자식으로 내려가 충돌테스트
// 하고, 재귀적으로 충돌테스트된 후 최종 노드까지 충돌테스트가 성공하면 충돌되었다고 알려준다.
// 각 오브젝트는 nTestNum을 설정해 다른 그룹끼리 충돌테스트시 같은 nTestNum끼리만 충돌테스트한다.
// 충돌테스트할 오브젝트는 ICollisionable 인터페이스를 상속받아야 한다.


namespace graphic
{
	class cCollisionManager
	{
	public:
		cCollisionManager();
		virtual ~cCollisionManager();
		enum { MAX_COLLISION_TABLE = 64 };
		ICollisionable* m_checkTable[ MAX_COLLISION_TABLE][ 2];		// [table size][ src/dst]


		// 충돌테스트할 오브젝트를 저장하는 트리
		struct sCollisionNode : cNode
		{
			int testnum; // 테스트번호 (0번을 제외한, 같은 테스트번호끼리 비교한다.)
			int boundingType;	// 0: box, 1: sphere
			ICollisionable *pobj;
			//Sphere *psphere;
			cBoundingBox *box;
			sCollisionNode( int id ) : cNode(id), testnum(0) , pobj(NULL)
			{}
		};


		void InsertObject( int group, ICollisionable *obj, int testNum=0 ); // 충돌테스트할 오브젝트 추가
		void InsertObject( ICollisionable *parent, ICollisionable *obj, int testNum=0 ); // 충돌테스트할 오브젝트 추가
		void RemoveObject( ICollisionable *obj ); // 리스트에서 오브젝트 추가
		void UpdateCollisionBox(); // 충돌박스의 좌표를 갱신한다.
		int CollisionTest( int testNum ); // 충돌테스트
		int GetCollisionCount() const;


	protected:
		void InsertObject( sCollisionNode *parent, ICollisionable *obj, int testNum );
		void UpdateCollisionBoxRec( sCollisionNode *obj );
		bool CheckNodeCollision(sCollisionNode *node1, sCollisionNode *node2, int testNum );
		void CollisionTestRec( sCollisionNode *node1, sCollisionNode *node2, int testNum ); // 충돌 테스트
		void CollisionTestNode1Loop( sCollisionNode *node1, sCollisionNode *node2, int testNum ); // 충돌 테스트
		void CollisionTestNode2Loop( sCollisionNode *node1, sCollisionNode *node2, int testNum ); // 충돌 테스트


	private:
		sCollisionNode *m_group1;
		sCollisionNode *m_group2;
		int m_collisionCount;
	};


	inline int cCollisionManager::GetCollisionCount() const { return m_collisionCount; }
}
