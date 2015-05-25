// 3 DOF 자세 센서 정보를 표현한다.
//
//
#pragma once


class cSensor
{
public:
	cSensor();

	Matrix44& UpdateSensing(const Matrix44 &tm);

	Matrix44 m_tm;
	Vector3 m_direction;
	Vector3 m_noise;
};

