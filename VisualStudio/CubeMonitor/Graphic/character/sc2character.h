// ��Ÿũ����Ʈ2 ĳ���� Ŭ����.
// ��Ÿũ����Ʈ ���� ���̴��� �����ϰ� �Ѵ�.
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
