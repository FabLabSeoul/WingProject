
#include "stdafx.h"
#include "SerialObserver.h"


cSerialObservable::cSerialObservable()
{
	m_observers.reserve(16);
}

cSerialObservable::~cSerialObservable()
{
	m_observers.clear();
}


void cSerialObservable::AddObserver(iSerialObserver* observer)
{
	RemoveObserver(observer);
	m_observers.push_back(observer);
}


void cSerialObservable::RemoveObserver(iSerialObserver* observer)
{
	common::removevector(m_observers, observer);
}


void cSerialObservable::NotifyObserver(char *buffer)
{
	BOOST_FOREACH(auto &observer, m_observers)
		observer->UpdateSerial(buffer);
}
