
#include "stdafx.h"
#include "CubeFlight.h"


cCubeFlight::cCubeFlight()
{
}

cCubeFlight::~cCubeFlight()
{

}


// 클래스 초기화.
bool cCubeFlight::Init()
{
	m_mesh.SetCube(Vector3(-1, -1, -1), Vector3(1, 1, 1));

	return true;
}


// 화면에 출력한다.
void cCubeFlight::Render()
{
	m_mesh.Render(m_tm);
}


// 매 프레임마다 연산할 것을 추가한다.
void cCubeFlight::Update(const float deltaSeconds)
{
}


// 큐브 각도를 오일러 각으로 설정한다.
void cCubeFlight::SetEulerAngle(const float roll, const float pitch, const float yaw)
{
	m_rotate.Euler2(Vector3(roll, pitch, yaw));
	m_tm = m_rotate.GetMatrix();
}
