
#pragma once



typedef struct _ThrustVertex
{
	Vector3 normal; // normal vector, local coordinate
	Vector3 axis[3]; // top,left,right 3축 direction local coordinate
	Vector3 thrust; // motor thrust vector, local coordinate
	Vector3 idealThrust; // ideal motor thrust vector, local coordinate
	
	uint8_t power[3]; // 모터 추력 (0 ~ 255 사이값)
	float totalPower; // 모터 총 추력
	
} ThrustVertex;



typedef struct _Cube
{
	ThrustVertex thrust[8];
	
	Matrix44 m_tm;
	Matrix44 m_offset;
	
	Vector3 dir;
	Quaternion qr;
	Quaternion qp;
	Quaternion qy;
	Quaternion quat;
	
} Cube;

extern Cube g_cube;


bool CubeInit(void);
void CubeResetHeading(void);
void CubeThrust(const Vector3 *pdir, const Vector3 *pmov);

void CubeAttitude(void);
void CubeControl(void);
void CubeWriteMotors(void);
void CubePairing(void);
