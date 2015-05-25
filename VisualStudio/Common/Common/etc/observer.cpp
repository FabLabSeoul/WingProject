
#include "stdafx.h"
#include "Observer.h"

using namespace common;


cObservable::cObservable()
{
	m_observers.reserve(16);
}

cObservable::~cObservable()
{
	m_observers.clear();
}


void cObservable::AddObserver(iObserver* observer)
{
	RemoveObserver(observer);
	m_observers.push_back(observer);
}


void cObservable::RemoveObserver(iObserver* observer)
{
	common::removevector(m_observers, observer);
}


void cObservable::NotifyObserver()
{
	BOOST_FOREACH (auto &observer, m_observers)
		observer->Update();
}
