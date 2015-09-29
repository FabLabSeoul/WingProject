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


//protected:
	graphic::cModel m_model;
	graphic::cCube2 m_mesh;
	Vector3 m_locate;
	Quaternion m_rotate;
	Matrix44 m_tm;
};

