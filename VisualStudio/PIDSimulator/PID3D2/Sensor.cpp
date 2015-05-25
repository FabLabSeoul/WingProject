
#include "stdafx.h"
#include "Sensor.h"


cSensor::cSensor()
{

}


// 현재 자세 정보에 노이즈를 추가해서, 결과 값을 m_tm에 저장한다.
Matrix44& cSensor::UpdateSensing(const Matrix44 &tm)
{

	m_tm = tm;

	return m_tm;
}