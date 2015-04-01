// 매인 인공지능 싱글톤 클래스
// 인공지능 전체를 관리하는 클래스다.
// 인공지능 라이브러리가 동작하기 위해서는 mainloop() 함수를
// 호출해줘야 한다.
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
