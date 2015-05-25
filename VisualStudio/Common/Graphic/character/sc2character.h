// 스타크래프트2 캐릭터 클래스.
// 스타크래프트 전용 셰이더를 동작하게 한다.
#pragma once


namespace graphic
{

	class cSc2Character : public cCharacter
	{
	public:
		cSc2Character(const int id);
		virtual ~cSc2Character();

		virtual bool Create(const string &modelName, MODEL_TYPE::TYPE type = MODEL_TYPE::AUTO) override;
	};

}
