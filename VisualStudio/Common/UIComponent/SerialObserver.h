//
// 시리얼 통신을 통해 넘어온 정보를 전달하는 기능을 한다.
//
//

#pragma once


class iSerialObserver
{
public:
	virtual void UpdateSerial(char *buffer) = 0;
};


class cSerialObservable
{
public:
	cSerialObservable();
	virtual ~cSerialObservable();

	void AddObserver(iSerialObserver* observer);
	void RemoveObserver(iSerialObserver* observer);
	void NotifyObserver(char *buffer);


protected:
	vector<iSerialObserver*> m_observers; // reference
};
