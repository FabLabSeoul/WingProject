
#include "stdafx.h"
#include "Motor.h"


cMotor::cMotor()
{
	m_location = Vector3(0, .1f, 0.5f);
	m_direction = Vector3(0, 1, 0);
	m_power = 1;
	
	m_powerNoise = 0.1f;
	m_directionNoise = Vector3(0.01f, 0.01f, 0.01f);

	m_p = 0.5f;
	m_i = 0.03f;
}


// ���� �߷��� ����ؼ� �����Ѵ�.
Vector3 cMotor::GetForce(const float deltaSeconds, const bool isPowerNoize, const bool isDirectionNoize,
	const bool enablePID)
{
	// ���� ������ ����
	Vector3 dirNoise = (isDirectionNoize) ?
		Vector3( common::GetRandomMinMax(-m_directionNoise.x, m_directionNoise.x),
		common::GetRandomMinMax(-m_directionNoise.y, m_directionNoise.y),
		common::GetRandomMinMax(-m_directionNoise.z, m_directionNoise.z)) : Vector3(0, 0, 0);

	Vector3 dir = m_direction - dirNoise;
	dir.Normalize();

	// �߷� ������ ����.
	const float scale = 1.f + common::GetRandomMinMax(-m_powerNoise, m_powerNoise);

	// �߷� ���
	Vector3 force = dir * m_power * deltaSeconds * scale;	

	// PID ���� 
	if (enablePID)
	{
		m_deltaIntegral += (deltaSeconds * m_delta * m_i);

		//force *= (m_delta) * m_p;
		//if (m_i > 0)
		//	force *= (m_deltaIntegral) * m_i;
		const float maxPID = 3.14f + 3.14f;

		float a = (m_delta * m_p) + m_deltaIntegral;
		a /= maxPID;
		if (a > 1)
			a = 1;
		if (a < 0)
			a = 0;

		force *= a;
		
		// ���̰��� ���� ������ ũ�ٸ�, �߷��� ���ش�.
		//if (m_delta < 0)
		//	force = Vector3(0, 0, 0);
	}
	else
	{
		m_deltaIntegral = 0;
		m_delta = 0;
	}


	// ����������, ���� ������ �ݴ�������� �߷��� �߻��Ѵ�.
	return -force;
}
