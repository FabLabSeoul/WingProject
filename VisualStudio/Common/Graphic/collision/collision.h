#pragma once


namespace graphic
{
	// interface ICollisionable
	// 충돌테스트할 오브젝트는 ICollisionable 인터페이스를 상속받아 구현해야 한다.
	struct ICollisionable
	{
		// Collision Test할지 여부
		virtual bool IsTest( int testNum )=0;
		virtual int GetCollisionId()=0;

		// 충돌테스트 하기전에 충돌박스, 충돌구의 좌표를 업데이트 한다.
		// UpdateCollisionBox() 함수는 충돌테스트하기 전에만 호출하면 된다.
		virtual void UpdateCollisionBox()=0;

		// 모델이 가진 충돌박스를 리스트에 저장한후 리턴한다. 
		virtual cBoundingBox* GetCollisionBox()=0;

		// 충돌된후 충돌을 일으킨 객체에게 호출됨
		// TestNum: 테스트 번호
		// model : 충돌된 객체
		virtual void Collision( int testNum, ICollisionable *obj )=0;
	};

}
