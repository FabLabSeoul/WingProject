//
// �ø�������� ���� ������ �ְ� �޴� ������ �Ѵ�.
//
#pragma once

#include "SerialObserver.h"


class cSerialCommunication : public cSerialObservable
{
public:
	cSerialCommunication();
	virtual ~cSerialCommunication();

	bool ConnectSerial(const int portNum, const int baudRate);
	bool ProcessSerialCommunicatoin(const float deltaSeconds);


protected: 
	enum { MAX_BUFFER = 256 };

	CSerial m_serial;
	char m_buff[ MAX_BUFFER]; // ���๮�ڰ� �� �������� ������ �����Ѵ�.
	int m_currentBuffIndex = 0;
};
