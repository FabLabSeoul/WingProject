
#include "stdafx.h"
#include "Controller.h"
#include "CubeFlight.h"
#include "SerialCommunication.h"


cController::cController()
{
	m_cubeFlight = new cCubeFlight();
	m_serialComm = new cSerialCommunication();
}


cController::~cController()
{
	SAFE_DELETE(m_cubeFlight);
	SAFE_DELETE(m_serialComm);
}


// 변수들을 초기화 한다.
bool cController::Init()
{
	m_cubeFlight->Init();

	return true;
}


// 큐브와 시리얼통신으로 연결을 시도한다.
bool cController::ConnectSerial(const int portNum, const int baudRate)
{
	RETV(!m_serialComm, false);
	return m_serialComm->ConnectSerial(portNum, baudRate);
}


void cController::Update(const float deltaSeconds)
{
	RET(!m_serialComm);
	m_serialComm->ProcessSerialCommunicatoin(deltaSeconds);
	
	NotifyUpdateObserver(deltaSeconds);
}
