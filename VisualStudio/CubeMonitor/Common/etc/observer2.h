
#pragma once


namespace common
{

	class iObserver2
	{
	public:
		virtual void Update(int type) = 0;
	};



	class cObservable2
	{
	public:
		cObservable2();
		virtual ~cObservable2();

		void AddObserver(iObserver2* observer);
		void RemoveObserver(iObserver2* observer);
		void NotifyObserver(int type=0);


	protected:
		vector<iObserver2*> m_observers; // reference
	};

}
