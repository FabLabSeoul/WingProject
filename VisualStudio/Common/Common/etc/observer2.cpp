
#include "stdafx.h"
#include "Observer2.h"

using namespace common;


cObservable2::cObservable2()
{
	m_observers.reserve(16);
}

cObservable2::~cObservable2()
{
	m_observers.clear();
}


void cObservable2::AddObserver(iObserver2* observer)
{
	RemoveObserver(observer);
	m_observers.push_back(observer);
}


void cObservable2::RemoveObserver(iObserver2* observer)
{
	common::removevector(m_observers, observer);
}


void cObservable2::NotifyObserver(int type)// type=0
{
	BOOST_FOREACH (auto &observer, m_observers)
		observer->Update(type);
}
