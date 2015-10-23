//
// 전역 제어 객체
//
// 큐브 모니터 프로그램을 제어한다.
//
#pragma once

#include "UpdateObserver.h"


class cCubeFlight;
class cSerialCommunication;
class cController : public common::cSingleton<cController>
							, public cUpdateObservable
{
public:
	cController();
	virtual ~cController();

	bool ConnectSerial(const int portNum, const int baudRate);
	void Update(const float deltaSeconds);

	cSerialCommunication& GetSerialComm();
	CSerial& GetSerial();


protected:
	cSerialCommunication *m_serialComm;
	CSerial m_serial;
};

inline cSerialCommunication& cController::GetSerialComm() { return *m_serialComm;  }
inline CSerial& cController::GetSerial() { return m_serial; }
