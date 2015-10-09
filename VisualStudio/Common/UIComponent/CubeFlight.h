//
// ť�� ����ü ������ �����Ѵ�.
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
	graphic::cLine m_line; 	// ������ �߷� ���⸦ ��Ÿ���� ��

	// ť�� ������
	struct sThrustVertext
	{
		Vector3 normal; // local 
		Vector3 axis[3]; // top,left,right 3�� local
		Vector3 thrust;; // local
		float power[3]; // ���� �߷�
	};
	sThrustVertext m_thrust[8];
};
