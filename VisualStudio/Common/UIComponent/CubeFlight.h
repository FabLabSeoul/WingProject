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
	void Render();
	void Update(const float deltaSeconds);

	void SetEulerAngle(const float roll, const float pitch, const float yaw);
	void ResetHeading();
	void Thrust(const Vector3 &dir, const Vector3 &mov=Vector3(0,0,0));
	void SetRenderOption(const bool showThrust, const bool showIdealThrust);


//protected:
	graphic::cModel m_model;
	Vector3 m_locate;
	Matrix44 m_tm;
	Matrix44 m_offset;

	// thrust vector arrow
	graphic::cModel m_arrow; // blue
	graphic::cModel m_arrow2; // red
	graphic::cLine m_line; 	// 모터의 추력 세기를 나타내는 선

	// 큐브 꼭지점
	struct sThrustVertext
	{
		Vector3 normal; // normal vector, local coordinate
		Vector3 axis[3]; // top,left,right 3축 direction local coordinate
		Vector3 thrust; // motor thrust vector, local coordinate
		Vector3 idealThrust; // ideal motor thrust vector, local coordinate

		float power[3]; // 모터 추력 (0 ~ 255 사이값)
		float totalPower; // 모터 총 추력
	};
	sThrustVertext m_thrust[8];

	// Render Option
	bool m_isShowThrust;
	bool m_isShowIdealThrust;
};


inline void cCubeFlight::SetRenderOption(const bool showThrust, const bool showIdealThrust) {
	m_isShowThrust = showThrust;
	m_isShowIdealThrust = showIdealThrust;
}

