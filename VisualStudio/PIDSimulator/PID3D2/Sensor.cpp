
#include "stdafx.h"
#include "Sensor.h"


cSensor::cSensor()
{

}


// ���� �ڼ� ������ ����� �߰��ؼ�, ��� ���� m_tm�� �����Ѵ�.
Matrix44& cSensor::UpdateSensing(const Matrix44 &tm)
{

	m_tm = tm;

	return m_tm;
}