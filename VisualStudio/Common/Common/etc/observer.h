
#pragma once


namespace common
{

	class iObserver
	{
	public:
		virtual void Update() = 0;
	};



	class cObservable
	{
	public:
		cObservable();
		virtual ~cObservable();

		void AddObserver(iObserver* observer);
		void RemoveObserver(iObserver* observer);
		void NotifyObserver();


	protected:
		vector<iObserver*> m_observers; // reference
	};

}
