// �޼��� �����ϴ� ���� �̱��� Ŭ����.
// �ΰ����� ��ü�鳢�� �޼����� �ְ� ���� �� ����Ѵ�.
#pragma once

#include <queue>

namespace ai
{

	class cMessageManager : public common::cSingleton<cMessageManager>
	{
	public:
		cMessageManager();
		virtual ~cMessageManager();

		void PostMsg(const sMsg &msg);
		void SendMsg(const sMsg &msg);
		void DispatchMsg();
		//void SetDefaultReceiver(cObject *obj);
		void Clear();


	protected:
		void ProcessMsg(const sMsg &msg);


	private:
		std::queue<sMsg> m_msgs;
		//cObject *m_defaultObject; // reference
	};


	//inline void cMessageManager::SetDefaultReceiver(cObject *obj) { m_defaultObject = obj; }
}
