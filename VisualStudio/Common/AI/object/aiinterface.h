// 인공지능 객체 인터페이스
// 인공지능을 적용하려는 객체는 이 인터페이스를 상속받아야 한다.
#pragma once


namespace ai
{

	class IArtificialIntelligence
	{
	public:
		IArtificialIntelligence() {}
		virtual ~IArtificialIntelligence() {}

		virtual Matrix44& aiGetTransform()=0;
		virtual void aiSetAnimation(const string &animationName) = 0;
	};

}
