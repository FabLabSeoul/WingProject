
#include "board.h"
#include "mw.h"
#include "vector.h"
#include "quaternion.h"
#include <stm32f10x.h>
#include "matrix44.h"



void SetMatIdentity(Matrix44 *pmat)
{
	pmat->_11 = pmat->_22 = pmat->_33 = pmat->_44 = 1;
	pmat->_12 = pmat->_13 = pmat->_14 = pmat->_21 = pmat->_23 = pmat->_24 = pmat->_31 = pmat->_32 = pmat->_34 = pmat->_41 = pmat->_42 = pmat->_43 = 0;
}

void SetMatRotationX( Matrix44 *pmat, const float angle )
{
	float fCos = cosf( angle );
	float fSin = sinf( angle );
	SetMatIdentity(pmat);
	pmat->_22 = fCos;
	pmat->_23 = fSin;
	pmat->_32 = -fSin;
	pmat->_33 = fCos;	
}

void SetMatRotationY( Matrix44 *pmat, const float angle )
{
	float fCos = cosf( angle );
	float fSin = sinf( angle );
	SetMatIdentity(pmat);
	pmat->_11 = fCos;
	pmat->_13 = -fSin;
	pmat->_31 = fSin;
	pmat->_33 = fCos;	
}

void SetMatRotationZ( Matrix44 *pmat, const float angle )
{
	float fCos = cosf( angle );
	float fSin = sinf( angle );
	SetMatIdentity(pmat);
	pmat->_11 = fCos;
	pmat->_12 = fSin;
	pmat->_21 = -fSin;
	pmat->_22 = fCos;	
}

void SetMatTranslate( Matrix44 *pmat, const struct _Vector3 *pv )
{
	SetMatIdentity(pmat);
	pmat->_41 = pv->x;
	pmat->_42 = pv->y;
	pmat->_43 = pv->z;
}

void SetMatScale( Matrix44 *pmat, const struct _Vector3 *pscale)
{
	SetMatIdentity(pmat);
	pmat->_11 = pscale->x;
	pmat->_22 = pscale->y;
	pmat->_33 = pscale->z;
}
	

//Quaternion GetQuaternion() const;


//---------------------------
// Matrix44x4_Inverse
//---------------------------
void Matrix4x4_Inverse( const float b[][4], float a[][4] )
{
	long	indxc[4], indxr[4], ipiv[4];
	long	i = 0, icol = 0, irow = 0, j = 0, ir = 0, ic = 0;
	float	big, dum, pivinv, temp, bb;

	ipiv[0] = -1;
	ipiv[1] = -1;
	ipiv[2] = -1;
	ipiv[3] = -1;

	a[0][0] = b[0][0];
	a[1][0] = b[1][0];
	a[2][0] = b[2][0];
	a[3][0] = b[3][0];

	a[0][1] = b[0][1];
	a[1][1] = b[1][1];
	a[2][1] = b[2][1];
	a[3][1] = b[3][1];

	a[0][2] = b[0][2];
	a[1][2] = b[1][2];
	a[2][2] = b[2][2];
	a[3][2] = b[3][2];

	a[0][3] = b[0][3];
	a[1][3] = b[1][3];
	a[2][3] = b[2][3];
	a[3][3] = b[3][3];

	for (i = 0; i < 4; i++) {
		big = 0.0f;
		for (j = 0; j < 4; j++) {
			if (ipiv[j] != 0) {
				if (ipiv[0] == -1) {
					if ((bb = ( float) fabs(a[j][0])) > big) {
						big = bb;
						irow = j;
						icol = 0;
					} //if
				} else if (ipiv[0] > 0) {
					return;
				} //if..else if..
				if (ipiv[1] == -1) {
					if ((bb = ( float) fabs(( float) a[j][1])) > big) {
						big = bb;
						irow = j;
						icol = 1;
					} //if
				} else if (ipiv[1] > 0) {
					return;
				} //if..else if..
				if (ipiv[2] == -1) {
					if ((bb = ( float) fabs(( float) a[j][2])) > big) {
						big = bb;
						irow = j;
						icol = 2;
					} //if
				} else if (ipiv[2] > 0) {
					return;
				} //if..else if..
				if (ipiv[3] == -1) {
					if ((bb = ( float) fabs(( float) a[j][3])) > big) {
						big = bb;
						irow = j;
						icol = 3;
					} //if
				} else if (ipiv[3] > 0) {
					return;
				} //if..else if..
			} //if
		} //for

		++(ipiv[icol]);

		if (irow != icol) {

			temp = a[irow][0];
			a[irow][0] = a[icol][0];
			a[icol][0] = temp;

			temp = a[irow][1];
			a[irow][1] = a[icol][1];
			a[icol][1] = temp;

			temp = a[irow][2];
			a[irow][2] = a[icol][2];
			a[icol][2] = temp;

			temp = a[irow][3];
			a[irow][3] = a[icol][3];
			a[icol][3] = temp;
		} //if

		indxr[i] = irow;
		indxc[i] = icol;

		if (a[icol][icol] == 0.0) {
			return;
		} //if

		pivinv = 1.0f / a[icol][icol];
		a[icol][icol] = 1.0f;
		a[icol][0] *= pivinv;
		a[icol][1] *= pivinv;
		a[icol][2] *= pivinv;
		a[icol][3] *= pivinv;

		if (icol != 0) {
			dum = a[0][icol];
			a[0][icol] = 0.0f;
			a[0][0] -= a[icol][0] * dum;
			a[0][1] -= a[icol][1] * dum;
			a[0][2] -= a[icol][2] * dum;
			a[0][3] -= a[icol][3] * dum;
		} //if
		if (icol != 1) {
			dum = a[1][icol];
			a[1][icol] = 0.0f;
			a[1][0] -= a[icol][0] * dum;
			a[1][1] -= a[icol][1] * dum;
			a[1][2] -= a[icol][2] * dum;
			a[1][3] -= a[icol][3] * dum;
		} //if
		if (icol != 2) {
			dum = a[2][icol];
			a[2][icol] = 0.0f;
			a[2][0] -= a[icol][0] * dum;
			a[2][1] -= a[icol][1] * dum;
			a[2][2] -= a[icol][2] * dum;
			a[2][3] -= a[icol][3] * dum;
		} //if
		if (icol != 3) {
			dum = a[3][icol];
			a[3][icol] = 0.0f;
			a[3][0] -= a[icol][0] * dum;
			a[3][1] -= a[icol][1] * dum;
			a[3][2] -= a[icol][2] * dum;
			a[3][3] -= a[icol][3] * dum;
		} //if
	} //for
	if (indxr[3] != indxc[3]) {
		ir = indxr[3];
		ic = indxc[3];

		temp = a[0][ir];
		a[0][ir] = a[0][ic];
		a[0][ic] = temp;

		temp = a[1][ir];
		a[1][ir] = a[1][ic];
		a[1][ic] = temp;

		temp = a[2][ir];
		a[2][ir] = a[2][ic];
		a[2][ic] = temp;

		temp = a[3][ir];
		a[3][ir] = a[3][ic];
		a[3][ic] = temp;
	} //if
	if (indxr[2] != indxc[2]) {
		ir = indxr[2];
		ic = indxc[2];

		temp = a[0][ir];
		a[0][ir] = a[0][ic];
		a[0][ic] = temp;

		temp = a[1][ir];
		a[1][ir] = a[1][ic];
		a[1][ic] = temp;

		temp = a[2][ir];
		a[2][ir] = a[2][ic];
		a[2][ic] = temp;

		temp = a[3][ir];
		a[3][ir] = a[3][ic];
		a[3][ic] = temp;
	} //if
	if (indxr[1] != indxc[1]) {
		ir = indxr[1];
		ic = indxc[1];

		temp = a[0][ir];
		a[0][ir] = a[0][ic];
		a[0][ic] = temp;

		temp = a[1][ir];
		a[1][ir] = a[1][ic];
		a[1][ic] = temp;

		temp = a[2][ir];
		a[2][ir] = a[2][ic];
		a[2][ic] = temp;

		temp = a[3][ir];
		a[3][ir] = a[3][ic];
		a[3][ic] = temp;
	} //if
	if (indxr[0] != indxc[0]) {
		ir = indxr[0];
		ic = indxc[0];

		temp = a[0][ir];
		a[0][ir] = a[0][ic];
		a[0][ic] = temp;

		temp = a[1][ir];
		a[1][ir] = a[1][ic];
		a[1][ic] = temp;

		temp = a[2][ir];
		a[2][ir] = a[2][ic];
		a[2][ic] = temp;

		temp = a[3][ir];
		a[3][ir] = a[3][ic];
		a[3][ic] = temp;
	} //if
} //Matrix44x4_Invert


void MatInverse(const Matrix44 *pmat, Matrix44 *out)
{
	#define ABS( x )	( (x) >= 0 ? (x) : -(x) )
	
	if( fabs( pmat->_44 - 1.0F ) > 0.001F )
	{
		Matrix4x4_Inverse( pmat->m, out->m );
		return;
	} //if

	if( fabs( pmat->_14 ) > 0.001F || fabs( pmat->_24 ) > 0.001F || fabs( pmat->_34 ) > 0.001F )
	{
		Matrix4x4_Inverse( pmat->m, out->m );
		return;
	} //if

	float det =   pmat->_11 * ( pmat->_22 * pmat->_33 - pmat->_23 * pmat->_32 )
		- pmat->_12 * ( pmat->_21 * pmat->_33 - pmat->_23 * pmat->_31 )
		+ pmat->_13 * ( pmat->_21 * pmat->_32 - pmat->_22 * pmat->_31 );

	if( ABS( det ) < 0.0005f )
	{
		Matrix4x4_Inverse( pmat->m, out->m );
		return;
	} //if

	det = 1.0F / det;

	out->_11 =  det * ( pmat->_22 * pmat->_33 - pmat->_23 * pmat->_32 );
	out->_12 = -det * ( pmat->_12 * pmat->_33 - pmat->_13 * pmat->_32 );
	out->_13 =  det * ( pmat->_12 * pmat->_23 - pmat->_13 * pmat->_22 );
	out->_14 = 0.0F;

	out->_21 = -det * ( pmat->_21 * pmat->_33 - pmat->_23 * pmat->_31 );
	out->_22 =  det * ( pmat->_11 * pmat->_33 - pmat->_13 * pmat->_31 );
	out->_23 = -det * ( pmat->_11 * pmat->_23 - pmat->_13 * pmat->_21 );
	out->_24 = 0.0F;

	out->_31 =  det * ( pmat->_21 * pmat->_32 - pmat->_22 * pmat->_31 );
	out->_32 = -det * ( pmat->_11 * pmat->_32 - pmat->_12 * pmat->_31 );
	out->_33 =  det * ( pmat->_11 * pmat->_22 - pmat->_12 * pmat->_21 );
	out->_34 = 0.0F;

	out->_41 = -( pmat->_41 * out->_11 + pmat->_42 * out->_21 + pmat->_43 * out->_31 );
	out->_42 = -( pmat->_41 * out->_12 + pmat->_42 * out->_22 + pmat->_43 * out->_32 );
	out->_43 = -( pmat->_41 * out->_13 + pmat->_42 * out->_23 + pmat->_43 * out->_33 );
	out->_44 = 1.0F;

}


void MatTranspose(Matrix44 *pmat)
{
	Matrix44 m;
	m._11 = pmat->_11;
	m._12 = pmat->_21;
	m._13 = pmat->_31;
	m._14 = pmat->_41;
	m._21 = pmat->_12;
	m._22 = pmat->_22;
	m._23 = pmat->_32;
	m._24 = pmat->_42;
	m._31 = pmat->_13;
	m._32 = pmat->_23;
	m._33 = pmat->_33;
	m._34 = pmat->_43;
	m._41 = pmat->_14;
	m._42 = pmat->_24;
	m._43 = pmat->_34;
	m._44 = pmat->_44;
	*pmat = m;
}


void GetMatQuaternion(const Matrix44 *pmat, struct _Quaternion *q)
{
	float fTr = pmat->_11 + pmat->_22 + pmat->_33 + pmat->_44;

	if( fTr >= 1.0F )	// w >= 0.5
	{
		float s = sqrtf( fTr );
		q->x = ( pmat->_32 - pmat->_23 ) / ( 2.0F * s );
		q->y = ( pmat->_13 - pmat->_31 ) / ( 2.0F * s );
		q->z = ( pmat->_21 - pmat->_12 ) / ( 2.0F * s );
		q->w = 0.5F * s;
	}
	else
	{
		float v[3];
		int i, j, k;

		if( pmat->_11 > pmat->_22 )		i = 0;
		else				i = 1;
		if( pmat->_33 > pmat->m[i][i] )	i = 2;

		j = ( i + 1 ) % 3;
		k = ( j + 1 ) % 3;

		float s = sqrtf( pmat->m[i][i] - pmat->m[j][j] - pmat->m[k][k] + 1.0F );
		v[i] = 0.5F * s;
		v[j] = ( pmat->m[j][i] + pmat->m[i][j] ) / ( 2.0F * s );
		v[k] = ( pmat->m[k][i] + pmat->m[i][k] ) / ( 2.0F * s );

		q->x = v[0];
		q->y = v[1];
		q->z = v[2];
		q->w = ( pmat->m[k][j] - pmat->m[j][k] ) / ( 2.0F * s );
	}	
	
}


void MatMultiply(const Matrix44 *pmat1, const Matrix44 *pmat2, Matrix44 *out)
{
	memset(out, 0, sizeof(Matrix44));

	for( int i = 0 ; i < 4 ; ++i )
	{
		for( int j = 0 ; j < 4 ; ++j )
		{			
			for( int k = 0 ; k < 4 ; ++k )
			{
				out->m[i][j] += pmat1->m[i][k] * pmat2->m[k][j];
			}
		}
	}	
}

