
#include "board.h"
#include "mw.h"
#include "vector.h"
#include "matrix44.h"
#include "quaternion.h"
#include "cube.h"


Cube g_cube;


float clamp(const float val, const float min0, const float max0)
{
	if (val < min0)
		return min0;
	if (val > max0)
		return max0;
	return val;
}



bool CubeInit(void)
{
	// 24���� ���� �߷º��͸� �����Ѵ�.
	//            1  ----------- 2
	//          / |                  / |
	//        /   |                /   |
	//       /    |              /     |
	//     0 ----------- 3     |
	//     |       5 7 ------|----6
	//     |      /            |      /
	//     |    /              |    /
	//     |  /                |   /
	//    4 -------------7 5
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
	g_cube.thrust[idx++].normal = InitVector2(-1, 1, -1);
	g_cube.thrust[idx++].normal = InitVector2(-1, 1, 1);
	g_cube.thrust[idx++].normal = InitVector2(1, 1, 1);
	g_cube.thrust[idx++].normal = InitVector2(1, 1, -1);

	// bottom plane
	g_cube.thrust[idx++].normal = InitVector2(-1, -1, -1);
	g_cube.thrust[idx++].normal = InitVector2(1, -1, -1);
	g_cube.thrust[idx++].normal = InitVector2(1, -1, 1);
	g_cube.thrust[idx++].normal = InitVector2(-1, -1, 1);
	
	for (int i = 0; i < 8; ++i)
	{
		VectorNormalize(&g_cube.thrust[i].normal);
		g_cube.thrust[i].thrust = g_cube.thrust[i].normal;
	}

	// motor thrust vector
	g_cube.thrust[0].axis[0] = InitVector2(0, 1, 0);
	g_cube.thrust[0].axis[1] = InitVector2(0, 0, -1);
	g_cube.thrust[0].axis[2] = InitVector2(-1, 0, 0);

	g_cube.thrust[1].axis[0] = InitVector2(0, 1, 0);
	g_cube.thrust[1].axis[1] = InitVector2(-1, 0, 0);
	g_cube.thrust[1].axis[2] = InitVector2(0, 0, 1);

	g_cube.thrust[2].axis[0] = InitVector2(0, 1, 0);
	g_cube.thrust[2].axis[1] = InitVector2(0, 0, 1);
	g_cube.thrust[2].axis[2] = InitVector2(1, 0, 0);

	g_cube.thrust[3].axis[0] = InitVector2(0, 1, 0);
	g_cube.thrust[3].axis[1] = InitVector2(1, 0, 0);
	g_cube.thrust[3].axis[2] = InitVector2(0, 0, -1);

	g_cube.thrust[4].axis[0] = InitVector2(0, -1, 0);
	g_cube.thrust[4].axis[1] = InitVector2(-1, 0, 0);
	g_cube.thrust[4].axis[2] = InitVector2(0, 0, -1);

	g_cube.thrust[5].axis[0] = InitVector2(0, -1, 0);
	g_cube.thrust[5].axis[1] = InitVector2(0, 0, -1);
	g_cube.thrust[5].axis[2] = InitVector2(1, 0, 0);	

	g_cube.thrust[6].axis[0] = InitVector2(0, -1, 0);
	g_cube.thrust[6].axis[1] = InitVector2(1, 0, 0);
	g_cube.thrust[6].axis[2] = InitVector2(0, 0, 1);
	
	g_cube.thrust[7].axis[0] = InitVector2(0, -1, 0);
	g_cube.thrust[7].axis[1] = InitVector2(0, 0, 1);
	g_cube.thrust[7].axis[2] = InitVector2(-1, 0, 0);


	SetMatIdentity(&g_cube.m_tm);
	SetMatIdentity(&g_cube.m_offset);
	
	return true;	
}


void CubeResetHeading(void)
{
	MatInverse(&g_cube.m_tm, &g_cube.m_offset );
}


void CubeThrust(const Vector3 *pdir, const Vector3 *pmov)
{
	//const float rotationPowerRate = 0.5f;

	// thrustPower, movePower = (0 ~ 512 ���� ��)
	const float rotationPower =  min(255, VectorLength(pdir) / 2); // 0~255
	const float movePower = min(255, VectorLength(pmov) / 2); // 0~255
	
	Vector3 zAxis;
	zAxis.x = 0;
	zAxis.y = 0;
	zAxis.z = 1;
	
	Vector3 dirNorm;
	VectorNormal(pdir, &dirNorm);
	
	Vector3 centerV;
	CrossProduct(&zAxis, &dirNorm, &centerV);
	VectorNormalize(&centerV);

	Matrix44 tm;
	MatMultiply(&g_cube.m_tm, &g_cube.m_offset, &tm);
	
	Matrix44 rtm;
	MatInverse(&tm, &rtm);

	for (int i = 0; i < 8; ++i)
	{
		uint16_t power1 = 0;
		uint16_t power2 = 0;
		uint16_t power3 = 0;

		//g_cube.thrust[i].idealThrust = Vector3(0, 0, 0);

		if (rotationPower > 10.f)
		{
			// ���� ��ǥ �󿡼� �߷º��� thrustV �� ����Ѵ�.
			Vector3 n ;
			VectorMultiplyNormal(&g_cube.thrust[i].normal, &tm, &n);
			Vector3 v;
			CrossProduct(&centerV, &n, &v);
			VectorNormalize(&v);
			

			// �� ������ �߷��� ����Ѵ�.
			// ���� ��ǥ�� ���� ��ǥü��� �ٲ۴�.
			// ȸ�� ������ �ݴ�������� �������� �������� �Ѵ�.
			Vector3 dir;
			VectorMultiplyNormal(&v, &rtm, &dir);
			//dir  = VectorNegative(&dir);

			//m_thrust[i].idealThrust += dir;

			// �߷º��͸� x,y,z �������� �����Ѵ�. 
			float x = DotProduct(&dir, &g_cube.thrust[i].axis[0]);
			float y = DotProduct(&dir, &g_cube.thrust[i].axis[1]);
			float z = DotProduct(&dir, &g_cube.thrust[i].axis[2]);

			x = clamp(x, 0, 1);
			y = clamp(y, 0, 1);
			z = clamp(z, 0, 1);

			power1 += min(255, (uint8_t)(x * rotationPower));
			power2 += min(255, (uint8_t)(y * rotationPower));
			power3 += min(255, (uint8_t)(z * rotationPower));
		}

		
		if (movePower > 10.f)
		{
			// �̵� ������, ť���� ȸ���� �ݴ� �������� �ؾ�, ���� �ڼ��� �°� ������ ��������.
			Vector3 nmov;
			VectorNormal(pmov, &nmov);
			Vector3 dir;
			VectorMultiplyNormal(&nmov, &rtm, &dir);
			dir = VectorNegative(&dir);

			//m_thrust[i].idealThrust += dir;

			float x = DotProduct(&dir, &g_cube.thrust[i].axis[0]);
			float y = DotProduct(&dir, &g_cube.thrust[i].axis[1]);
			float z = DotProduct(&dir, &g_cube.thrust[i].axis[2]);

			x = clamp(x, 0, 1);
			y = clamp(y, 0, 1);
			z = clamp(z, 0, 1);

			power1 += min(255, (uint8_t)(x * movePower));
			power2 += min(255, (uint8_t)(y * movePower));
			power3 += min(255, (uint8_t)(z * movePower));			
		}
		
		// ���� �߷� ����
		g_cube.thrust[i].power[0] = min(255, power1);
		g_cube.thrust[i].power[1] = min(255, power2);
		g_cube.thrust[i].power[2] = min(255, power3);
	}

}


void CubeAttitude(void)
{
	// �ڼ����� ������Ʈ
	int roll = angle[0]; // +- 1800
	int pitch = angle[1]; // +- 860
	int yaw = heading; // 0 ~ 360
	
	Quaternion qr, qp, qy;
	QuatEuler(&qr, 0, 0, -(float)roll*0.1f);
	QuatEuler(&qp, (float)pitch*0.1f, 0, 0);
	QuatEuler(&qy, 0, (float)yaw, 0);


	// q = qy * qp * qr;
	Quaternion q0;
	QuatMultiply(&q0, &qy, &qp);
	Quaternion q;
	QuatMultiply(&q, &q0, &qr);
	
	g_cube.quat = q;
	g_cube.qr = qr;
	g_cube.qp = qp;
	g_cube.qy = qy;


	QuatGetMatrix(&q, &g_cube.m_tm);
	
	
	Vector3 zAxis;
	zAxis.x = 0;
	zAxis.y = 0;
	zAxis.z = 1;	
	VectorMultiplyNormal(&zAxis, &g_cube.m_tm, &g_cube.dir);
	VectorNormalize(&g_cube.dir);
	
}


void CubeControl(void)
{
	// Rotation
	// roll, pitch ������ ť�긦 ȸ����Ű�� ���͸� �����Ѵ�.
	const int roll = rcData[0]; // 988 ~ 1500 ~ 2010
	const int pitch = rcData[1]; // 988 ~ 1500 ~ 2010
	const float rot_x = (float)(roll - 1500);// / (float)(2010 - 988);
	const float rot_y = (float)(pitch - 1500);// / (float)(2010 - 988);
	Vector3 rotation;
	InitVector(&rotation, rot_x, rot_y, 0);
	
	// Move
	const int up = rcData[3]; // 988 ~ 1500 ~ 2010
	const int right = rcData[2]; // 988 ~ 1500 ~ 2010
	const int forward = rcData[5]; // 988 ~ 1500 ~ 2010
	const float mov_x = (float)(right - 1500);// / (float)(2010 - 988);
	const float mov_y = (float)(up - 1500);// / (float)(2010 - 988);
	const float mov_z = (float)(forward - 1500);// / (float)(2010 - 988);
	Vector3 move;
	InitVector(&move, mov_x, mov_y, mov_z);

	CubeThrust(&rotation, &move);
}


// ���͸� �ϳ��� �׽�Ʈ�ϱ����� ���
void CubeMotorTest(void)
{
	uint32_t crc = 0;	
	uint16_t thro = 0;
	uint16_t delayTime = 2;
	static int16_t oldRcData4 = 1000;
	static int motorIdx = 0;
	
	// 990 ~ 2010
	thro = rcData[THROTTLE] - 990;
	thro = thro / 4;
	crc = 0;
	const int rcD4 = (int)rcData[AUX1];
	const int offsetRc = rcD4 - oldRcData4;
	const bool isRcD4Neutral = (rcD4 > 1200) && (rcD4 < 1800);

	if (offsetRc < -300)
	{
		if (!isRcD4Neutral)
		{
			--motorIdx;
			if (motorIdx < 0)
				motorIdx = 23;
		}

		oldRcData4 = rcD4;
	}
	else if (offsetRc > 300)
	{
		if (!isRcD4Neutral)
		{
			motorIdx += 1;
			if (motorIdx >= 24)
				motorIdx = 0;
		}
		
		oldRcData4 = rcD4;
	}

	
	// Serial Send Delay
	if (isSerialTransmitBufferEmpty(core.mainport))
		delay(delayTime);
	
	serialWrite(core.mainport, 'S');
	for (int i=0; i < 24; ++i)
	{
		// Serial Send Delay
		if (isSerialTransmitBufferEmpty(core.mainport))
			delay(delayTime);

		if (motorIdx == i)
		{
			crc += thro;
			serialWrite(core.mainport, thro);
		}
		else
			serialWrite(core.mainport, 0);
	}

	// Serial Send Delay
	crc = crc % 255;
	if (isSerialTransmitBufferEmpty(core.mainport))
		delay(delayTime);		
	serialWrite(core.mainport, crc);

	// Serial Send Delay
	if (isSerialTransmitBufferEmpty(core.mainport))
		delay(delayTime);
	serialWrite(core.mainport, 'E');
}


void CubeWriteMotorsSerial(void)
{
	uint32_t crc = 0;	
	uint16_t delayTime = 2;
	
	// Serial Send Delay
	if (isSerialTransmitBufferEmpty(core.mainport))
		delay(delayTime);
	
	serialWrite(core.mainport, 'S');
	for (int i=0; i < 24; ++i)
	{
		uint16_t vertexIdx = i / 3;
		uint16_t motorIdx = i % 3;
		
		// Serial Send Delay
		if (isSerialTransmitBufferEmpty(core.mainport))
			delay(delayTime);

		uint8_t thro = g_cube.thrust[vertexIdx].power[motorIdx];
		crc += thro;
		serialWrite(core.mainport, thro);
	}

	// Serial Send Delay
	crc = crc % 255;
	if (isSerialTransmitBufferEmpty(core.mainport))
		delay(delayTime);		
	serialWrite(core.mainport, crc);

	// Serial Send Delay
	if (isSerialTransmitBufferEmpty(core.mainport))
		delay(delayTime);
	serialWrite(core.mainport, 'E');	
}


// Bluetooth ��� �ʱ�ȭ ������ ��ģ��.
void CubePairing(void)
{
	uint16_t delayTime = 2;
	
	// Serial Send Delay
	if (isSerialTransmitBufferEmpty(core.mainport))
		delay(delayTime);
	
	printf("at+btscan,3,0\n");
	
	blinkLED(10, 15, 10);

/*	
	serialWrite(core.mainport, 'A');
	serialWrite(core.mainport, 'T');
	serialWrite(core.mainport, '+');
	serialWrite(core.mainport, 'B');
	serialWrite(core.mainport, 'T');
	serialWrite(core.mainport, 'S');
	serialWrite(core.mainport, 'C');
	serialWrite(core.mainport, 'A');
	serialWrite(core.mainport, 'N');
	serialWrite(core.mainport, ',');
	serialWrite(core.mainport, '3');
	serialWrite(core.mainport, ',');
	serialWrite(core.mainport, '0');
	serialWrite(core.mainport, '\n');	
	*/
}


// Mode Select
void CubeWriteMotors(void)
{
	static bool initHead = false;
	static bool initBluetooth = false;
	
	if (rcData[AUX4] > 1800)
		initBluetooth = true; // ������� ���� ���
	
	if (initBluetooth && (rcData[AUX4] < 1500))
	{
		initBluetooth = false;
		CubePairing();
		return;
	}
	
	
	// 990 ~ 2010
	if ((rcData[AUX3] >= 1200) && (rcData[AUX3] < 1800))
	{
		// Nothging
		initHead = true;
	}
	else if (rcData[AUX3] < 1200)
	{
		initHead = true;
		CubeMotorTest();
	}
	else
	{
		if (initHead)
		{
			initHead = false;
			CubeResetHeading();
		}
		
		CubeWriteMotorsSerial();
	}
	
	
}


