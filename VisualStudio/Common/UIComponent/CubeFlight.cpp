
#include "stdafx.h"
#include "CubeFlight.h"


cCubeFlight::cCubeFlight() :
m_model(0)
, m_arrow(1)
, m_arrow2(2)
, m_arrow3(3)
, m_isShowThrust(true)
, m_isShowIdealThrust(false)
, m_isShowCubeThrust(false)
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
	m_arrow2.Create("arrow2.dat");
	m_arrow3.Create("arrow3.dat");

	m_line.GetMaterial().InitRed();


	// 24���� ���� �߷º��͸� �����Ѵ�.
	//            1  ----------- 2
	//          / |                  / |
	//        /   |  top         /   |
	//       /    |     front  /    |
	//     0 ----------- 3      |
	//     |       5 ------|----6
	//     |      / back   |     /
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
		rot.SetRotationArc(Vector3(0, 1, 0), m_thrust[i].thrust, Vector3(1,0,0));
		Quaternion idealRot;
		idealRot.SetRotationArc(Vector3(0, 1, 0), m_thrust[i].idealThrust, Vector3(1,0,0));
		Quaternion cubeRot;
		cubeRot.SetRotationArc(Vector3(0, 1, 0), m_thrust[i].cubeThrust, Vector3(1, 0, 0));
		Matrix44 r = rot.GetMatrix();
		Matrix44 ir = idealRot.GetMatrix();
		Matrix44 cr = cubeRot.GetMatrix();

		Matrix44 s; // Thrust Power ũ�⿡ ����, �� ũ�⸦ �����Ѵ�.
		s.SetScale(Vector3(1, 1, 1)*0.5f * m_thrust[i].totalPower/128);
		Matrix44 s2; // Thrust Power ũ�⿡ ����, �� ũ�⸦ �����Ѵ�.
		s2.SetScale(Vector3(1, 1, 1)*0.5f * m_thrust[i].totalCubePower / 128);
		Matrix44 t;
		t.SetTranslate(m_thrust[i].normal * 12);

		// �߷º��� ���
		if (m_isShowThrust)
			m_arrow.Render(s * r * t * m_tm * m_offset);
		if (m_isShowIdealThrust)
			m_arrow2.Render(s * ir * t * m_tm * m_offset);
		if (m_isShowCubeThrust)
			m_arrow3.Render(s2 * cr * t * m_tm * m_offset);

		// 3�� ���� �߷� ���� ���
		for (int k = 0; k < 3; ++k)
		{
			// ���� �߷¿� ���� �� ���̸� �����Ѵ�.
			const Vector3 p0 = m_thrust[i].normal * 8;
			const Vector3 p1 = m_thrust[i].axis[k] * m_thrust[i].power[k] / 255.f * 5 + p0;
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
// mov �������� �̵��Ѵ�.
// x,y,z ��� ��ȿ�ϴ�.
void cCubeFlight::Thrust(const Vector3 &dir, const Vector3 &mov) //mov=Vector3(0,0,0);
{
	const float rotationPowerRate = 0.5f;

	// thrustPower, movePower = (0 ~ 512 ���� ��)
	const float rotationPower = dir.Length();
	const float movePower = mov.Length();

	Vector3 centerV = Vector3(0, 0, 1).CrossProduct(dir.Normal());
	centerV.Normalize();

	const Matrix44 tm = m_tm * m_offset;
	const Matrix44 rtm = tm.Inverse();

	for (int i = 0; i < 8; ++i)
	{
		// �ʱ�ȭ
		m_thrust[i].power[0] = 0;
		m_thrust[i].power[1] = 0;
		m_thrust[i].power[2] = 0;
		m_thrust[i].idealThrust = Vector3(0, 0, 0);

		if (rotationPower > 10.f)
		{
			// ���� ��ǥ �󿡼� �߷º��� thrustV �� ����Ѵ�.
			Vector3 n = m_thrust[i].normal.MultiplyNormal(tm);
			n.Normalize();
			Vector3 v = centerV.CrossProduct(n);
			v.Normalize();

			// �� ������ �߷��� ����Ѵ�.
			// ���� ��ǥ�� ���� ��ǥü��� �ٲ۴�.
			// ȸ�� ������ �ݴ�������� �������� �������� �Ѵ�.
			const Vector3 dir = -v.MultiplyNormal(rtm);

			m_thrust[i].idealThrust += dir;

			// �߷º��͸� x,y,z �������� �����Ѵ�. 
			float x = dir.DotProduct(m_thrust[i].axis[0]);
			float y = dir.DotProduct(m_thrust[i].axis[1]);
			float z = dir.DotProduct(m_thrust[i].axis[2]);
		
			x = common::clamp(x, 0, 1);
			y = common::clamp(y, 0, 1);
			z = common::clamp(z, 0, 1);

			const float power = rotationPowerRate * rotationPower * 0.5f;
			m_thrust[i].power[0] += x * power;
			m_thrust[i].power[1] += y * power;
			m_thrust[i].power[2] += z * power;
		}

		
		if (movePower > 10.f)
		{
			// �̵� ������, ť���� ȸ���� �ݴ� �������� �ؾ�, ���� �ڼ��� �°� ������ ��������.
			const Vector3 dir = -mov.Normal().MultiplyNormal(rtm);
			
			m_thrust[i].idealThrust += dir;

			float x = dir.DotProduct(m_thrust[i].axis[0]);
			float y = dir.DotProduct(m_thrust[i].axis[1]);
			float z = dir.DotProduct(m_thrust[i].axis[2]);

			x = common::clamp(x, 0, 1);
			y = common::clamp(y, 0, 1);
			z = common::clamp(z, 0, 1);

			const float power = (1.f - rotationPowerRate) * movePower * 0.5f;

			m_thrust[i].power[0] += x * power;
			m_thrust[i].power[1] += y * power;
			m_thrust[i].power[2] += z * power;
		}


		// �߷º��� ���
		// ���� ���Ϳ� ���� �������� ���� ���� ���ȴ�.
		// �� ����, ������ ����� ����Ǳ� ������, �ϵ��� ������ �ް� �ȴ�.
		Vector3 thrust = m_thrust[i].axis[0] * m_thrust[i].power[0]
								+ m_thrust[i].axis[1] * m_thrust[i].power[1]
								+ m_thrust[i].axis[2] * m_thrust[i].power[2];
		const float totalPower = thrust.Length();
		m_thrust[i].totalPower = totalPower;

		if (totalPower > 1)
		{
			thrust.Normalize();
			m_thrust[i].thrust = thrust;
		}

		// �̻����� �߷¹����� ��Ÿ����.
		m_thrust[i].idealThrust.Normalize();
	}
}


// Naze32 Board���� ����� �߷°��� ���� ���͸� ����Ѵ�.
void cCubeFlight::CubeThrust(unsigned char buffer[24])
{

	for (int i = 0; i < 24; i+=3)
	{
		const int vertexIdx = i / 3;
		const int power1 = buffer[i];
		const int power2 = buffer[i + 1];
		const int power3 = buffer[i + 2];

		Vector3 thrust = m_thrust[vertexIdx].axis[0] * power1
								+ m_thrust[vertexIdx].axis[1] * power2
								+ m_thrust[vertexIdx].axis[2] * power3;
		const float totalPower = thrust.Length();
		m_thrust[vertexIdx].totalCubePower = totalPower;

		if (totalPower > 1)
		{
			thrust.Normalize();
			m_thrust[vertexIdx].cubeThrust = thrust;
		}
	}

}
