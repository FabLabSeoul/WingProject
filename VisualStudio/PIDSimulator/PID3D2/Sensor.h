// 3 DOF �ڼ� ���� ������ ǥ���Ѵ�.
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

