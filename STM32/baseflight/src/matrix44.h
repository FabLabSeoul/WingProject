
#pragma once



typedef struct _Matrix44
{
	union
	{
		struct
		{
			float	_11, _12, _13, _14;
			float	_21, _22, _23, _24;
			float	_31, _32, _33, _34;
			float	_41, _42, _43, _44;
		};
		float	m[4][4];
	};

} Matrix44;



void SetMatIdentity(Matrix44 *pmat);
void SetMatRotationX( Matrix44 *pmat, const float angle );
void SetMatRotationY( Matrix44 *pmat, const float angle );
void SetMatRotationZ( Matrix44 *pmat, const float angle );

struct _Vector3;
void SetMatTranslate( Matrix44 *pmat, const struct _Vector3 *pv );
void SetMatScale( Matrix44 *pmat, const struct _Vector3 *pscale);

struct _Quaternion;
void GetMatQuaternion(const Matrix44 *pmat, struct _Quaternion *q);
void MatInverse(const Matrix44 *pmat, Matrix44 *out);
void MatTranspose(Matrix44 *pmat);

void MatMultiply(const Matrix44 *pmat1, const Matrix44 *pmat2, Matrix44 *out);


