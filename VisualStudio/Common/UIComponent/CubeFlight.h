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
	void Thrust(const Vector3 &dir);


//protected:
	graphic::cModel m_model;
	Vector3 m_locate;
	Matrix44 m_tm;
	Matrix44 m_offset;

	// thrust vector arrow
	graphic::cModel m_arrow;
	graphic::cLine m_line; 	// 모터의 추력 세기를 나타내는 선

	// 큐브 꼭지점
	struct sThrustVertext
	{
		Vector3 normal; // local 
		Vector3 axis[3]; // top,left,right 3축 local
		Vector3 thrust;; // local
		float power[3]; // 모터 추력
	};
	sThrustVertext m_thrust[8];
};
