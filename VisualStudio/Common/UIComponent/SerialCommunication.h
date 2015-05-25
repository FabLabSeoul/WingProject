//
// 시리얼통신을 통해 정보를 주고 받는 역할을 한다.
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
	char m_buff[ MAX_BUFFER]; // 개행문자가 올 때까지의 정보를 저장한다.
	int m_currentBuffIndex = 0;
};
