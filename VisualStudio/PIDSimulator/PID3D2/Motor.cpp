
#include "stdafx.h"
#include "Motor.h"


cMotor::cMotor()
{
	m_location = Vector3(0, .1f, 0.5f);
	m_direction = Vector3(0, 1, 0);
	m_power = 1;
	
	m_powerNoise = 0.1f;
	m_directionNoise = Vector3(0.01f, 0.01f, 0.01f);

	m_p = 1;
}


// �Ŀ� ������ ����, 
Vector3 cMotor::GetForce(const float deltaSeconds, const bool isPowerNoize, const bool isDirectionNoize,
	const bool enablePID)
{	
	Vector3 dirNoise = (isDirectionNoize) ?
		Vector3( common::GetRandomMinMax(-m_directionNoise.x, m_directionNoise.x),
		common::GetRandomMinMax(-m_directionNoise.y, m_directionNoise.y),
		common::GetRandomMinMax(-m_directionNoise.z, m_directionNoise.z)) : Vector3(0, 0, 0);

	Vector3 dir = m_direction - dirNoise;
	dir.Normalize();

	const float scale = 1.f + common::GetRandomMinMax(-m_powerNoise, m_powerNoise);
	Vector3 force = dir * m_power * deltaSeconds * scale;	

	if (enablePID)
	{
		force *= (m_delta) * m_p;

		// ���̰��� ���� ������ ũ�ٸ�, �߷��� ���ش�.
		if (m_delta < 0)
			force = Vector3(0, 0, 0);
	}


	// ����������, ���� ������ �ݴ�������� �߷��� �߻��Ѵ�.
	return -force;
}
