
#include "stdafx.h"
#include "CubeFlight.h"


cCubeFlight::cCubeFlight() :
m_model(0)
{
}

cCubeFlight::~cCubeFlight()
{
}


// Ŭ���� �ʱ�ȭ.
bool cCubeFlight::Init()
{
	m_mesh.SetCube(Vector3(-1, -1, -1), Vector3(1, 1, 1));
	m_mesh.GetMaterial().InitBlue();

	m_model.Create("cube.dat");

	return true;
}


// ȭ�鿡 ����Ѵ�.
void cCubeFlight::Render()
{
	//m_mesh.Render(m_tm);
	m_model.Render(m_tm);
}


// �� �����Ӹ��� ������ ���� �߰��Ѵ�.
void cCubeFlight::Update(const float deltaSeconds)
{
	m_model.Move(deltaSeconds);
}


// ť�� ������ ���Ϸ� ������ �����Ѵ�.
void cCubeFlight::SetEulerAngle(const float roll, const float pitch, const float yaw)
{
	//m_rotate.Euler2(Vector3(roll, pitch, yaw));
	m_rotate.Euler(Vector3(roll, pitch, yaw));
	m_tm = m_rotate.GetMatrix();
}
