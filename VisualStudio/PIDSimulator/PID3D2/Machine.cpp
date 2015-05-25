
#include "stdafx.h"
#include "Machine.h"
#include "C3DPhysxPIDView.h"

using namespace physx;


cMachine::cMachine() :
	m_enablePID(false)
{
	common::initrand();

	m_motor[0] = new cMotor();
	m_motor[0]->m_location = Vector3(0, 0.1f, 1);
	m_motor[0]->m_direction = Vector3(0, 1, 0);
	m_motor[0]->m_power = 1;

	m_motor[1] = new cMotor();
	m_motor[1]->m_location = Vector3(0, 0.1f, -1);
	m_motor[1]->m_direction = Vector3(0, 1, 0);
	m_motor[1]->m_power = 1;
}

cMachine::~cMachine()
{
	for (int i = 0; i < MAX_MOTOR; ++i)
		SAFE_DELETE(m_motor[i]);
}


// ���� ��ü, 3D ��ü�� �����Ѵ�.
bool cMachine::Init()
{
	if (!g_physxView)
		return false;

	// ���� �⺻ ��ü ����
	{
		m_box.rigid = g_physxView->CreateBox(PxVec3(0, 0, 0), PxVec3(0.1f, 0.1f, 1), &PxVec3(0, 0, 0), 1);
		m_box.rigid->setAngularDamping(0);
		m_box.rigid->setLinearDamping(0);
		m_box.cube.SetCube(Vector3(-0.1f, -0.1f, -1.f), Vector3(0.1f, 0.1f, 1.f));
		Matrix44 mat;
		mat.SetTranslate(Vector3(0, 0, 0));
		m_box.cube.SetTransform(mat);
		m_box.cube.GetMaterial().InitBlue();
	}

	// ���� ����
	//PxTransform tm0;
	PxTransform tm1(0, 0.1f, 0,
		PxQuat(ANGLE2RAD(0), PxVec3(0, 1, 0)));

	tm1.createIdentity();
	if (PxRevoluteJoint*j = PxRevoluteJointCreate(*g_physxView->GetPhysx(),
		NULL, PxTransform(0, 0, 0),
		m_box.rigid, tm1)
		)
	{
		const PxReal tolerance = 0.2f;
		j->setProjectionLinearTolerance(tolerance);
	}
	//return true;

	// ����1 3D ������Ʈ ����
	{
		m_motor[0]->m_cube.SetCube(Vector3(-0.11f, -0.11f, -0.11f), Vector3(0.11f, 0.11f, 0.11f));
		Matrix44 mat;
		mat.SetTranslate(m_motor[0]->m_location );
		m_motor[0]->m_cube.SetTransform(mat);
		m_motor[0]->m_cube.GetMaterial().InitRed();

		m_motor[0]->m_line.SetLine(m_motor[0]->m_location, m_motor[0]->m_location + Vector3(0, 0.2f, 0), 0.1f);
		m_motor[0]->m_line.GetMaterial().InitRed();
	}

	// ����2 3D ������Ʈ ����
	{
		m_motor[1]->m_cube.SetCube(Vector3(-0.11f, -0.11f, -0.11f), Vector3(0.11f, 0.11f, 0.11f));
		Matrix44 mat;
		mat.SetTranslate(m_motor[1]->m_location);
		m_motor[1]->m_cube.SetTransform(mat);
		m_motor[1]->m_cube.GetMaterial().InitGreen();

		m_motor[1]->m_line.SetLine(m_motor[1]->m_location, m_motor[1]->m_location + Vector3(0, 0.2f, 0), 0.1f);
		m_motor[1]->m_line.GetMaterial().InitGreen();
	}

	return true;
}


// ���� ���� ������ ������, ���� ������ �����Ѵ�.
void	cMachine::UpdatePhysX(const float deltaSeconds)
{
	if (m_enablePID)
		UpdatePIDControl(deltaSeconds);

	{
		// 1�� ���� �߷� ����.
		const Vector3 pos = m_motor[0]->m_location * m_box.cube.GetTransform();
		const Vector3 force = m_motor[0]->GetForce(deltaSeconds, true, true, m_enablePID);

		PxRigidBodyExt::addLocalForceAtPos(*m_box.rigid,
			*(PxVec3*)&force,
			*(PxVec3*)&pos,
			PxForceMode::eIMPULSE);

		// �߷� ���� ���� ����
		m_motor[0]->m_line.SetLine(pos, (force.Normal().MultiplyNormal(m_box.cube.GetTransform()) * -0.4f) + pos, 0.01f);
	}

	{
		// 2�� ���� �߷� ����.
		const Vector3 pos = m_motor[1]->m_location * m_box.cube.GetTransform();
		const Vector3 force = m_motor[1]->GetForce(deltaSeconds, true, true, m_enablePID);

		PxRigidBodyExt::addLocalForceAtPos(*m_box.rigid,
			*(PxVec3*)&force,
			*(PxVec3*)&pos,
			PxForceMode::eIMPULSE);

		// �߷� ���� ���� ����
		m_motor[1]->m_line.SetLine(pos, (force.Normal().MultiplyNormal(m_box.cube.GetTransform()) * -0.4f) + pos, 0.01f);
	}

}


// PID ����.
void cMachine::UpdatePIDControl(const float deltaSeconds)
{
	RET(!m_enablePID);

	// Z��� ��� ���� ���� cos(theta) �� ��ȯ�� �����Ѵ�.
	const Vector3 machineZ = Vector3(0, 0, 1).MultiplyNormal(g_physxView->m_machine.m_sensor.m_tm);


	// Z�� ���� ������ PID �̵��� ����Ѵ�.
	// Motor1
	{
		float delta = acos(machineZ.DotProduct(Vector3(0, 0, 1)));
		Vector3 axis(0, 0, m_motor[0]->m_location.z);
		axis.Normalize();
		delta = (axis.CrossProduct(machineZ).x > 0) ? -delta : delta;

		m_motor[0]->m_delta = delta;						
	}

	// -Z�� ���� ������ PID �̵��� ����Ѵ�.
	// Motor2
	{
		float delta = acos(machineZ.DotProduct(Vector3(0, 0, -1)));
		Vector3 axis(0, 0, m_motor[1]->m_location.z);
		axis.Normalize();
		delta = (axis.CrossProduct(machineZ).x > 0) ? -delta : delta;

		m_motor[1]->m_delta = delta;
	}
}


// �������� ������ ��� ������ ������ �����Ѵ�.
void cMachine::UpdateAfterPhysX(const float deltaSeconds)
{
	// ť��, ������������ ���� ������ ������Ʈ �Ѵ�.
	PxTransform tm = m_box.rigid->getGlobalPose();
	Matrix44 tm1;
	tm1.SetTranslate(*(Vector3*)&tm.p);
	Quaternion q(tm.q.x, tm.q.y, tm.q.z, tm.q.w);
	Matrix44 tm2 = q.GetMatrix();
	m_box.cube.SetTransform(tm2 * tm1);
	
	// ���� ���� ������Ʈ
	m_sensor.UpdateSensing(tm2 * tm1);
}


// PID ���� �¿���.
void cMachine::EnablePIDControl(const bool enable)
{

	m_enablePID = enable;
}
