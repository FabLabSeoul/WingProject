// ���� �ΰ����� �̱��� Ŭ����
// �ΰ����� ��ü�� �����ϴ� Ŭ������.
// �ΰ����� ���̺귯���� �����ϱ� ���ؼ��� mainloop() �Լ���
// ȣ������� �Ѵ�.
#pragma once


namespace ai
{

	class cAIMain : public common::cSingleton<cAIMain>
	{
	public:
		cAIMain();
		virtual ~cAIMain();

		void MainLoop(const float elapseTime);
		void Clear();

		
	private:
		//cObject *m_rootObject;
	};

}
