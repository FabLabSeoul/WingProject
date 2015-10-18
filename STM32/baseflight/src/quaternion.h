
#pragma once



typedef struct _Quaternion
{
	float	x, y, z, w;

} Quaternion;



void InitQuaternion(Quaternion *q, const float x0, const float y0, const float z0, const float w0);
void InitQuaternionRot(Quaternion *q, const Vector3 *pvAxis, const float angle);
void	SetQuatRotationX( Quaternion *q, const float fRadian );
void	SetQuatRotationY( Quaternion *q, const float fRadian );
void	SetQuatRotationZ( Quaternion *q, const float fRadian );
void	SetSetRotationArcRotationArc(Quaternion *q, const Vector3 *pvDir1, const Vector3 *pvDir2, const Vector3 *pnorm);
void QuatEuler(Quaternion *q, const float roll, const float pitch, const float yaw);

	

struct _Matrix44;

void QuatMultiply(Quaternion *q, const Quaternion *q0, const Quaternion *q1);
void QuatMultiplyMat(Quaternion *q, const Quaternion *q0, const struct _Matrix44 *pm);
void QuatGetMatrix(const Quaternion *q, struct _Matrix44 *pmat);


