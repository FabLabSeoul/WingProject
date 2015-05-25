#include "stdafx.h"
#include "sphere.h"


using namespace graphic;


cSphere::cSphere()
{

}

cSphere::cSphere(const float radius, const int stacks, const int slices)
{
	Create(radius, stacks, slices);
	m_mtrl.InitWhite();
}

cSphere::~cSphere()
{

}


void cSphere::Render(const Matrix44 &tm)
{
	GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );

	Matrix44 mat = m_tm * tm;
	GetDevice()->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&mat );
	m_vtxBuff.Bind();
	m_idxBuff.Bind();
	m_mtrl.Bind();
	GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 
		m_vtxBuff.GetVertexCount(), 0, m_idxBuff.GetFaceCount());

	GetDevice()->SetRenderState( D3DRS_LIGHTING, TRUE );
}


void cSphere::RenderShader(cShader &shader, const Matrix44 &tm)
{
	Matrix44 mat = m_tm * tm;
	shader.SetMatrix( "mWorld", mat);
	m_vtxBuff.Bind();
	m_idxBuff.Bind();

	shader.Begin();
	shader.BeginPass();
	GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 
		m_vtxBuff.GetVertexCount(), 0, m_idxBuff.GetFaceCount());
	shader.EndPass();
	shader.End();
}


// 구 생성
void cSphere::Create(const float radius, const int stacks, const int slices)
{
	if (m_vtxBuff.GetVertexCount() > 0)
		return;

	m_radius = radius;
	m_numSlices = slices;
	m_numStacks = stacks;

	const int dwVertices = (slices) * (((stacks-2) * 4) + 6);
	const int dwIndices = ((slices-1)*6) * stacks ;

	m_vtxBuff.Create(dwVertices, sizeof(sVertexNormTex), sVertexNormTex::FVF);
	m_idxBuff.Create(dwIndices);

	sVertexNormTex *pVertex = (sVertexNormTex*)m_vtxBuff.Lock();
	WORD *pIndices = (WORD*)m_idxBuff.Lock();

	int idxIdx = 0;
	int vtxIdx = 0;

	for( int t = 0 ; t < stacks ; t++ ) // stacks are ELEVATION so they count theta
	{
		const float theta1 = ( (float)t/(float)stacks ) *MATH_PI ;
		const float theta2 = ( (float)(t+1)/(float)stacks )*MATH_PI ;
		const float v1 = (float)t / (float)stacks;
		const float v2 = (float)(t+1)/ (float)stacks;

		for( int p = 0 ; p < slices ; p++ ) // slices are ORANGE SLICES so the count azimuth
		{
			const float phi1 = ( (float)p/(float)slices )*2*MATH_PI ; // azimuth goes around 0 .. 2*PI
			const float phi2 = ( (float)(p+1)/(float)slices )*2*MATH_PI ;
			const float u1 = (float)p / (float)slices;
			const float u2 = (float)(p+1) / (float)slices;

			//phi1   phi2
			// |      |
			// 1------2 -- theta1
			// |\ _   |
			// |    \ |
			// 4------3 -- theta2
			//

			const float r1 = sin(theta1) * radius;
			const float r2 = sin(theta2) * radius;
			const float y1 = cos(theta1) * radius;
			const float y2 = cos(theta2) * radius;

			const float x11 = cos(phi1) * r1;
			const float z11 = sin(phi1) * r1;
			const float x12 = cos(phi2) * r1;
			const float z12 = sin(phi2) * r1;

			const float x21 = cos(phi1) * r2;
			const float z21 = sin(phi1) * r2;
			const float x22 = cos(phi2) * r2;
			const float z22 = sin(phi2) * r2;

			const Vector3 vertex1(x11, y1, z11);
			const Vector3 vertex2(x12, y1, z12);
			const Vector3 vertex3(x22, y2, z22);
			const Vector3 vertex4(x21, y2, z21);

			const Vector3 n1 = vertex1.Normal();
			const Vector3 n2 = vertex2.Normal();
			const Vector3 n3 = vertex3.Normal();
			const Vector3 n4 = vertex4.Normal();

			// facing out
			if( t == 0 ) // top cap
			{
				pVertex[ vtxIdx].p = vertex1;
				pVertex[ vtxIdx].n = n1;
				pVertex[ vtxIdx].u = u1;
				pVertex[ vtxIdx].v = v1;
				++vtxIdx;

				pVertex[ vtxIdx].p = vertex3;
				pVertex[ vtxIdx].n = n3;
				pVertex[ vtxIdx].u = u2;
				pVertex[ vtxIdx].v = v2;
				++vtxIdx;

				pVertex[ vtxIdx].p = vertex4;
				pVertex[ vtxIdx].n = n4;
				pVertex[ vtxIdx].u = u1;
				pVertex[ vtxIdx].v = v2;
				++vtxIdx;

				*pIndices++ = idxIdx++;
				*pIndices++ = idxIdx++;
				*pIndices++ = idxIdx++;
			}
			else if( t + 1 == stacks ) //end cap
			{
				pVertex[ vtxIdx].p = vertex3;
				pVertex[ vtxIdx].n = n3;
				pVertex[ vtxIdx].u = u2;
				pVertex[ vtxIdx].v = v2;
				++vtxIdx;

				pVertex[ vtxIdx].p = vertex1;
				pVertex[ vtxIdx].n = n1;
				pVertex[ vtxIdx].u = u1;
				pVertex[ vtxIdx].v = v1;
				++vtxIdx;

				pVertex[ vtxIdx].p = vertex2;
				pVertex[ vtxIdx].n = n2;
				pVertex[ vtxIdx].u = u2;
				pVertex[ vtxIdx].v = v1;
				++vtxIdx;


				*pIndices++ = idxIdx++;
				*pIndices++ = idxIdx++;
				*pIndices++ = idxIdx++;
			}
			else
			{
				// body, facing OUT:
				pVertex[ vtxIdx].p = vertex1;
				pVertex[ vtxIdx].n = n1;
				pVertex[ vtxIdx].u = u1;
				pVertex[ vtxIdx].v = v1;
				++vtxIdx;

				pVertex[ vtxIdx].p = vertex2;
				pVertex[ vtxIdx].n = n2;
				pVertex[ vtxIdx].u = u2;
				pVertex[ vtxIdx].v = v1;
				++vtxIdx;

				pVertex[ vtxIdx].p = vertex3;
				pVertex[ vtxIdx].n = n3;
				pVertex[ vtxIdx].u = u2;
				pVertex[ vtxIdx].v = v2;
				++vtxIdx;

				pVertex[ vtxIdx].p = vertex4;
				pVertex[ vtxIdx].n = n4;
				pVertex[ vtxIdx].u = u1;
				pVertex[ vtxIdx].v = v2;
				++vtxIdx;

				const int base = idxIdx;

				*pIndices++ = base;
				*pIndices++ = base+1;
				*pIndices++ = base+3;

				*pIndices++ = base+1;
				*pIndices++ = base+2;
				*pIndices++ = base+3;

				idxIdx += 4;
			}
		}
	}

	m_vtxBuff.Unlock();
	m_idxBuff.Unlock();
}

