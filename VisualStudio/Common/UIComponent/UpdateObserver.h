//
// �������� ���� ȣ��Ǵ� Update() �Լ� �̺�Ʈ�� �ޱ� ���� ������ ��ü��.
//
//
#pragma once


class iUpdateObserver
{
public:
	virtual void Update(const float deltaSeconds) = 0;
};


class cUpdateObservable
{
public:
	cUpdateObservable();
	virtual ~cUpdateObservable();

	void AddUpdateObserver(iUpdateObserver* observer);
	void RemoveUpdateObserver(iUpdateObserver* observer);
	void NotifyUpdateObserver(const float deltaSeconds);


protected:
	vector<iUpdateObserver*> m_updatObservers; // reference
};




inline cUpdateObservable::cUpdateObservable()
{
	m_updatObservers.reserve(16);
}

inline cUpdateObservable::~cUpdateObservable()
{
	m_updatObservers.clear();
}

inline void cUpdateObservable::AddUpdateObserver(iUpdateObserver* observer)
{
	RemoveUpdateObserver(observer);
	m_updatObservers.push_back(observer);
}

inline void cUpdateObservable::RemoveUpdateObserver(iUpdateObserver* observer)
{
	common::removevector(m_updatObservers, observer);
}

inline void cUpdateObservable::NotifyUpdateObserver(const float deltaSeconds)
{
	BOOST_FOREACH(auto &observer, m_updatObservers)
		observer->Update(deltaSeconds);
}
