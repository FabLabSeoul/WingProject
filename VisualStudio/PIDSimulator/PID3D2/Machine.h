// 비행체 정보를 표현한다.
//
//
#pragma once

#include "Sensor.h"
#include "Motor.h"


class cMachine
{
public:
	cMachine();
	virtual ~cMachine();

	bool Init();
	void UpdatePhysX(const float deltaSeconds);
	void UpdateAfterPhysX(const float deltaSeconds);

	void EnablePIDControl(const bool enable);
	void UpdatePIDControl(const float deltaSeconds);


	enum { MAX_MOTOR = 2 };
	sPhysxBox m_box;
	cSensor m_sensor;
	cMotor *m_motor[MAX_MOTOR];
	bool m_enablePID;
};
