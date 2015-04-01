// 메세지 관리하는 전역 싱글톤 클래스.
// 인공지능 객체들끼리 메세지를 주고 받을 때 사용한다.
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
