//
// �ø��� ����� ���� �Ѿ�� ������ �����ϴ� ����� �Ѵ�.
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
