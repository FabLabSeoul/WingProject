
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


// �ø���� ��� ������ �õ��Ѵ�. �����ϸ� false�� ����, �����ϸ� true�� �����Ѵ�.
bool cSerialCommunication::ConnectSerial(const int portNum, const int baudRate)
{
	return m_serial.Open(portNum, baudRate) ? true : false;
}


// �ø��� ����� ������ �ְ� �޴´�.
// ����� ����ٸ� false�� �����Ѵ�.
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

	// �� ������ ��� �ϼ��Ǿ��ٸ�, ������ �м��Ѵ�.
	if (isEndOfBuffer || (data == '\n'))
	{
		m_buff[m_currentBuffIndex] = NULL;

		float t, roll, pitch, yaw;
		sscanf_s(m_buff, "Radian, %f, %f, %f, %f", &t, &roll, &pitch, &yaw);

		cController::Get()->GetCubeFlight().SetEulerAngle(-roll, yaw, -pitch);

		// �ø���������� �Ѿ�� ������ ���������� �˸���.
		NotifyObserver(m_buff);

		m_currentBuffIndex = 0;
	}

	return true;
}
