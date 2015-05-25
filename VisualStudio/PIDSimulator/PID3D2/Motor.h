// 액추에이터 정보를 나타낸다.
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
	Vector3 m_directionNoise; // x,y,z : 0~1, noise direction (백색 잡음)
	float m_power;
	float m_powerNoise; // 0~1, scale of m_power (백색 잡음)
	float m_delta; // 목표값과의 차이값.
	float m_deltaIntegral; // 차이값을 계속 쌓아놓는 변수
	float m_p;
	float m_i;
	float m_d; // not work

	graphic::cCube2 m_cube;
	graphic::cLine m_line;
};

