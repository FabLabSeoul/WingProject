// �ΰ����� ��ü �������̽�
// �ΰ������� �����Ϸ��� ��ü�� �� �������̽��� ��ӹ޾ƾ� �Ѵ�.
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
