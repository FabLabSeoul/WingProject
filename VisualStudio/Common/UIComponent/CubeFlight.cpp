
#include "stdafx.h"
#include "CubeFlight.h"


cCubeFlight::cCubeFlight() :
m_model(0)
, m_arrow(1)
{
}

cCubeFlight::~cCubeFlight()
{
}


// Ŭ���� �ʱ�ȭ.
bool cCubeFlight::Init()
{
// 	m_mesh.SetCube(Vector3(-1, -1, -1), Vector3(1, 1, 1));
// 	m_mesh.GetMaterial().InitBlue();

	m_model.Create("cube.dat");
	m_arrow.Create("arrow.dat");

	m_line.GetMaterial().InitRed();


	// 24���� ���� �߷º��͸� �����Ѵ�.
	//            1  ----------- 2
	//          / |                  / |
	//        /   |                /   |
	//       /    |              /    |
	//     0 ----------- 3      |
	//     |       5 ------|----6
	//     |      /           |     /
	//     |    /             |   /
	//     |  /               |  /
	//    4 -------------7
	//
	// �� �������� ������ ������ ����.

	//           /
	//         /
	//       /
	//     /                0 (Up)
	//   /     2     
	//   -----------------------
	//  |
	//  |
	//  |           1
	//  |
	//  |
	//
	// �������� 3�� ���� ������, ���� ���� �ʺ��� �ð�������� ��������.

	//     (Y)
	//     |        / (Z)
	//     |     /
	//     |  /
	//     --------------- (X)
	// ��ǥ ��
	
	// ť�� �������� Normal Vector�� ���Ѵ�.
	int idx = 0;
	// top plane
	m_thrust[idx++].normal = Vector3(-1, 1, -1);
	m_thrust[idx++].normal = Vector3(-1, 1, 1);
	m_thrust[idx++].normal = Vector3(1, 1, 1);
	m_thrust[idx++].normal = Vector3(1, 1, -1);

	// bottom plane
	m_thrust[idx++].normal = Vector3(-1, -1, -1);
	m_thrust[idx++].normal = Vector3(-1, -1, 1);
	m_thrust[idx++].normal = Vector3(1, -1, 1);
	m_thrust[idx++].normal = Vector3(1, -1, -1);

	for (int i = 0; i < 8; ++i)
	{
		m_thrust[i].normal.Normalize();
		m_thrust[i].thrust = m_thrust[i].normal;
	}

	// motor thrust vector
	m_thrust[0].axis[0] = Vector3(0, 1, 0);
	m_thrust[0].axis[1] = Vector3(0, 0, -1);
	m_thrust[0].axis[2] = Vector3(-1, 0, 0);

	m_thrust[1].axis[0] = Vector3(0, 1, 0);
	m_thrust[1].axis[1] = Vector3(-1, 0, 0);
	m_thrust[1].axis[2] = Vector3(0, 0, 1);

	m_thrust[2].axis[0] = Vector3(0, 1, 0);
	m_thrust[2].axis[1] = Vector3(0, 0, 1);
	m_thrust[2].axis[2] = Vector3(1, 0, 0);

	m_thrust[3].axis[0] = Vector3(0, 1, 0);
	m_thrust[3].axis[1] = Vector3(1, 0, 0);
	m_thrust[3].axis[2] = Vector3(0, 0, -1);

	m_thrust[4].axis[0] = Vector3(0, -1, 0);
	m_thrust[4].axis[1] = Vector3(-1, 0, 0);
	m_thrust[4].axis[2] = Vector3(0, 0, -1);

	m_thrust[5].axis[0] = Vector3(0, -1, 0);
	m_thrust[5].axis[1] = Vector3(0, 0, 1);
	m_thrust[5].axis[2] = Vector3(-1, 0, 0);

	m_thrust[6].axis[0] = Vector3(0, -1, 0);
	m_thrust[6].axis[1] = Vector3(1, 0, 0);
	m_thrust[6].axis[2] = Vector3(0, 0, 1);

	m_thrust[7].axis[0] = Vector3(0, -1, 0);
	m_thrust[7].axis[1] = Vector3(0, 0, -1);
	m_thrust[7].axis[2] = Vector3(1, 0, 0);

	return true;
}


// ȭ�鿡 ����Ѵ�.
void cCubeFlight::Render()
{
	m_model.Render(m_tm * m_offset);
	
	// ������ �߷º��� ���
	for (int i = 0; i < 8; ++i)
	{
		// ������ǥ�� ������ǥ�� ��ȯ�Ѵ�.
		Quaternion rot;
		rot.SetRotationArc(Vector3(0, 1, 0), m_thrust[i].thrust);
		Matrix44 r = rot.GetMatrix();
		Matrix44 s;
		s.SetScale(Vector3(1, 1, 1)*0.5f);
		Matrix44 t;
		t.SetTranslate(m_thrust[i].normal * 12);

		// �߷º��� ���
		m_arrow.Render(s * r * t * m_tm * m_offset);

		// 3�� ���� �߷� ���� ���
		for (int k = 0; k < 3; ++k)
		{
			const Vector3 p0 = m_thrust[i].normal * 8;
			const Vector3 p1 = m_thrust[i].axis[k] * m_thrust[i].power[k] * 5 + p0;
			m_line.SetLine(p0 * m_tm * m_offset, p1 * m_tm * m_offset, 0.2f);
			m_line.Render();
		}
	}

}


// �� �����Ӹ��� ������ ���� �߰��Ѵ�.
void cCubeFlight::Update(const float deltaSeconds)
{
	m_model.Move(deltaSeconds);
}


// ť�� ������ ���Ϸ� ������ �����Ѵ�.
void cCubeFlight::SetEulerAngle(const float roll, const float pitch, const float yaw)
{
	Quaternion rot;
	rot.Euler(Vector3(roll, pitch, yaw));
	m_tm = rot.GetMatrix();
}


// ���� �ڼ��� �⺻ �ڼ��� �Ѵ�.
void cCubeFlight::ResetHeading()
{
	m_offset = m_tm.Inverse();
}


// dir �������� ȸ��
// x,y ���� ��ȿ�ϴ�.
void cCubeFlight::Thrust(const Vector3 &dir)
{
	Vector3 centerV = Vector3(0, 0, 1).CrossProduct(dir);
	centerV.Normalize();

	const Matrix44 tm = m_tm * m_offset;
	const Matrix44 rtm = tm.Inverse();

	for (int i = 0; i < 8; ++i)
	{
		// ���� ��ǥ �󿡼� �߷º��� thrustV �� ����Ѵ�.
		Vector3 n = m_thrust[i].normal.MultiplyNormal(tm);
		n.Normalize();
		Vector3 v = centerV.CrossProduct(n);
		v.Normalize();

		// ���� ��ǥ�� ���� ��ǥü��� �ٲ۴�.
		m_thrust[i].thrust = v.MultiplyNormal(rtm);

		// �� ������ �߷��� ����Ѵ�.
		// �߷º��͸� x,y,z �������� �����Ѵ�. 
		// ȸ�� ������ �ݴ�������� �������� �������� �Ѵ�.
		const Vector3 thrust = -m_thrust[i].thrust;
		float x = thrust.DotProduct(m_thrust[i].axis[0]);
		float y = thrust.DotProduct(m_thrust[i].axis[1]);
		float z = thrust.DotProduct(m_thrust[i].axis[2]);
		
		x = common::clamp(x, 0, 1);
		y = common::clamp(y, 0, 1);
		z = common::clamp(z, 0, 1);

		m_thrust[i].power[0] = x;
		m_thrust[i].power[1] = y;
		m_thrust[i].power[2] = z;
	}
}

