// ���߿����� ������ ��Ÿ����.
//
//

#pragma once


class cMotor
{
public:
	cMotor();

	Vector3 GetForce(const float deltaSeconds, const bool isPowerNoize=true, 
		const bool isDirectionNoize=true, const bool enablePID=false);


	Vector3 m_location;
	Vector3 m_direction;
	Vector3 m_directionNoise; // x,y,z : 0~1, noise direction (��� ����)
	float m_power;
	float m_powerNoise; // 0~1, scale of m_power (��� ����)
	float m_delta; // ��ǥ������ ���̰�.
	float m_deltaIntegral; // ���̰��� ��� �׾Ƴ��� ����
	float m_p;
	float m_i;
	float m_d; // not work

	graphic::cCube2 m_cube;
	graphic::cLine m_line;
};

