//
// 큐브 비행체 정보를 관리한다.
//
//
#pragma once


class cCubeFlight
{
public:
	cCubeFlight();
	virtual ~cCubeFlight();

	bool Init();
	void Render(const Matrix44 &worldTm);
	void Update(const float deltaSeconds);

	void SetEulerAngle(const float roll, const float pitch, const float yaw);
	void ResetHeading();
	void ResetLocalSpace();
	void Thrust(const Vector3 &dir, const Vector3 &mov=Vector3(0,0,0));
	void CubeThrust(unsigned char buffer[24]);
	void SetRenderOption(const bool showThrust, const bool showIdealThrust, const bool showCubeThrust);


//protected:
	graphic::cModel m_model;
	Matrix44 m_tm;
	Matrix44 m_offset;
	Matrix44 m_localSpaceTM;

	// thrust vector arrow
	graphic::cModel m_arrow; // blue
	graphic::cModel m_arrow2; // red
	graphic::cModel m_arrow3; // green
	graphic::cLine m_line; 	// 모터의 추력 세기를 나타내는 선

	// 큐브 꼭지점
	struct sThrustVertex
	{
		Vector3 normal; // normal vector, local coordinate
		Vector3 axis[3]; // top,left,right 3축 direction local coordinate
		Vector3 thrust; // motor thrust vector, local coordinate
		Vector3 idealThrust; // ideal motor thrust vector, local coordinate
		Vector3 cubeThrust; // calculate cube thrust vector, local coordinate

		float power[3]; // 모터 추력 (0 ~ 255 사이값)
		float totalPower; // 모터 총 추력
		float totalCubePower; // 큐브에서 계산된 모터 총 추력
	};
	sThrustVertex m_thrust[8];

	// Render Option
	bool m_isShowThrust;
	bool m_isShowIdealThrust;
	bool m_isShowCubeThrust;
};


inline void cCubeFlight::SetRenderOption(const bool showThrust, 
	const bool showIdealThrust, const bool showCubeThrust) {
	m_isShowThrust = showThrust;
	m_isShowIdealThrust = showIdealThrust;
	m_isShowCubeThrust = showCubeThrust;
}

