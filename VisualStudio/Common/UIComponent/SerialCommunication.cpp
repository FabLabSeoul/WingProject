
#include "stdafx.h"
#include "SerialCommunication.h"
#include "Controller.h"
#include "CubeFlight.h"


cSerialCommunication::cSerialCommunication()
{

}

cSerialCommunication::~cSerialCommunication()
{

}


// 시리얼과 통신 연결을 시도한다. 실패하면 false를 리턴, 성공하면 true를 리턴한다.
bool cSerialCommunication::ConnectSerial(const int portNum, const int baudRate)
{
	return m_serial.Open(portNum, baudRate) ? true : false;
}


// 시리얼 통신을 정보를 주고 받는다.
// 통신이 끊겼다면 false를 리턴한다.
bool cSerialCommunication::ProcessSerialCommunicatoin(const float deltaSeconds)
{
	RETV(!m_serial.IsOpened(), false);

	char data = 0;
	const int readLength = m_serial.ReadData(&data, 1);
	if (readLength == 0)
		return true;

	RETV(readLength < 0, false);

	m_buff[m_currentBuffIndex++] = data;

	const bool isEndOfBuffer = (m_currentBuffIndex >= (MAX_BUFFER - 1));

	// 한 라인이 모두 완성되었다면, 정보를 분석한다.
	if (isEndOfBuffer || (data == '\n'))
	{
		m_buff[m_currentBuffIndex] = NULL;

		float t, roll, pitch, yaw;
		sscanf_s(m_buff, "Radian, %f, %f, %f, %f", &t, &roll, &pitch, &yaw);

		cController::Get()->GetCubeFlight().SetEulerAngle(-roll, yaw, -pitch);

		// 시리얼통신으로 넘어온 정보를 옵져버에게 알린다.
		NotifyObserver(m_buff);

		m_currentBuffIndex = 0;
	}

	return true;
}
