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

	bool Init();
	bool ConnectSerial(const int portNum, const int baudRate);
	void Update(const float deltaSeconds);

	cCubeFlight& GetCubeFlight();
	cSerialCommunication& GetSerialComm();


protected:
	cCubeFlight *m_cubeFlight;
	cSerialCommunication *m_serialComm;
};


inline cCubeFlight& cController::GetCubeFlight() { return *m_cubeFlight;  }
inline cSerialCommunication& cController::GetSerialComm() { return *m_serialComm;  }