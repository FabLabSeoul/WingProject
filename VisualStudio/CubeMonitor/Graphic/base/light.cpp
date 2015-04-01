
#include "stdafx.h"
#include "light.h"

using namespace graphic;

cLight::cLight()
{

}


cLight::~cLight()
{

}


void cLight::Init(TYPE type, 
	const Vector4 &ambient, // Vector4(1, 1, 1, 1),
	const Vector4 &diffuse, // Vector4(0.2, 0.2, 0.2, 1)
	const Vector4 &specular, // Vector4(1,1,1,1)
	const Vector3 &direction) // Vector3(0,-1,0)
{
	ZeroMemory(&m_light, sizeof(m_light));
	m_light.Type = (D3DLIGHTTYPE)type;
	m_light.Ambient = *(D3DCOLORVALUE*)&ambient;
	m_light.Diffuse = *(D3DCOLORVALUE*)&diffuse;
	m_light.Specular = *(D3DCOLORVALUE*)&specular;
	m_light.Direction = *(D3DXVECTOR3*)&direction;
}


void cLight::SetDirection( const Vector3 &direction )
{
	m_light.Direction = *(D3DXVECTOR3*)&direction;
}


void cLight::SetPosition( const Vector3 &pos )
{
	m_light.Position = *(D3DXVECTOR3*)&pos;
}


// 그림자를 출력하기 위한 정보를 리턴한다.
// modelPos : 그림자를 출력할 모델의 위치 (월드상의)
// lightPos : 광원의 위치가 저장되어 리턴.
// view : 광원에서 모델을 바라보는 뷰 행렬
// proj : 광원에서 모델을 바라보는 투영 행렬
// tt : 투영 좌표에서 텍스쳐 좌표로 변환하는 행렬.
void cLight::GetShadowMatrix( const Vector3 &modelPos, 
	OUT Vector3 &lightPos, OUT Matrix44 &view, OUT Matrix44 &proj, 
	OUT Matrix44 &tt )
{
	if (D3DLIGHT_DIRECTIONAL == m_light.Type)
	{
		// 방향성 조명이면 Direction 벡터를 통해 위치를 계산하게 한다.
		Vector3 pos = *(Vector3*)&m_light.Position;
		Vector3 dir = *(Vector3*)&m_light.Direction;
		lightPos = -dir * pos.Length();
	}
	else
	{
		lightPos = *(Vector3*)&m_light.Position;
	}

	view.SetView2( lightPos, modelPos, Vector3(0,1,0));

	proj.SetProjection( D3DX_PI/8.f, 1, 0.1f, 10000);

	D3DXMATRIX mTT= D3DXMATRIX(0.5f, 0.0f, 0.0f, 0.0f
		, 0.0f,-0.5f, 0.0f, 0.0f
		, 0.0f, 0.0f, 1.0f, 0.0f
		, 0.5f, 0.5f, 0.0f, 1.0f);
	tt = *(Matrix44*)&mTT;
}


void cLight::Bind(int lightIndex)  const
{
	GetDevice()->SetLight(lightIndex, &m_light); // 광원 설정.
}


// 셰이더 변수에 라이팅에 관련된 변수를 초기화 한다.
void cLight::Bind(cShader &shader)  const
{
	static cShader *oldPtr = NULL;
	static D3DXHANDLE hDir = NULL;
	static D3DXHANDLE hPos = NULL;
	static D3DXHANDLE hAmbient = NULL;
	static D3DXHANDLE hDiffuse = NULL;
	static D3DXHANDLE hSpecular = NULL;
	static D3DXHANDLE hTheta = NULL;
	static D3DXHANDLE hPhi = NULL;

	if (oldPtr != &shader)
	{
		hDir = shader.GetValueHandle("light.dir");
		hPos = shader.GetValueHandle("light.pos");
		hAmbient = shader.GetValueHandle("light.ambient");
		hDiffuse = shader.GetValueHandle("light.diffuse");
		hSpecular = shader.GetValueHandle("light.specular");
		hTheta = shader.GetValueHandle("light.spotInnerCone");
		hPhi = shader.GetValueHandle("light.spotOuterCone");

		oldPtr = &shader;
	}

	shader.SetVector( hDir, *(Vector3*)&m_light.Direction);
	shader.SetVector( hPos, *(Vector3*)&m_light.Position);
	shader.SetVector( hAmbient, *(Vector4*)&m_light.Ambient);
	shader.SetVector( hDiffuse, *(Vector4*)&m_light.Diffuse);
	shader.SetVector( hSpecular, *(Vector4*)&m_light.Specular);
	shader.SetFloat( hTheta, m_light.Theta);
	shader.SetFloat( hPhi, m_light.Phi);
	//shader.SetFloat( "light.radius", m_light.r);
}
