
#include "stdafx.h"
#include "material.h"

using namespace graphic;


cMaterial::cMaterial()
{
	InitRed();
}

cMaterial::~cMaterial()
{
}


// 메터리얼 초기화.
void cMaterial::Init(const Vector4 &ambient, 
	const Vector4 &diffuse,
	const Vector4 &specular,
	const Vector4 &emmisive,
	const float pow)
{
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));
	m_mtrl.Ambient = *(D3DXCOLOR*)&ambient;
	m_mtrl.Diffuse = *(D3DXCOLOR*)&diffuse;
	m_mtrl.Specular = *(D3DXCOLOR*)&specular;
	m_mtrl.Emissive = *(D3DXCOLOR*)&emmisive;
	m_mtrl.Power = 200.f;
}


void cMaterial::Init(const D3DMATERIAL9 &mtrl)
{
	m_mtrl = mtrl;
}


void cMaterial::Init(const sMaterial &mtrl)
{
	Init(mtrl.ambient, mtrl.diffuse, mtrl.specular, mtrl.emissive);
	m_mtrl.Power = mtrl.power;
}


void cMaterial::InitWhite()
{
	Init(Vector4(1,1,1,1), Vector4(1,1,1,1), Vector4(1,1,1,1));
}


void cMaterial::InitBlack()
{
	Init(Vector4(0,0,0,1), Vector4(0,0,0,1), Vector4(0,0,0,1));
}


void cMaterial::InitRed()
{
	Init(Vector4(1,0,0,1), Vector4(1,0,0,1), Vector4(1,0,0,1));
}


void cMaterial::InitBlue()
{
	Init(Vector4(0,0,1,1), Vector4(0,0,1,1), Vector4(0,0,1,1));
}


void cMaterial::InitGreen()
{
	Init(Vector4(0, 1, 0, 1), Vector4(0, 1, 0, 1), Vector4(0, 1, 0, 1));
}

void cMaterial::Bind()
{
	graphic::GetDevice()->SetMaterial(&m_mtrl);
}

void cMaterial::Bind(cShader &shader)
{
	static cShader *oldPtr = NULL;
	static D3DXHANDLE hAmbient = NULL;
	static D3DXHANDLE hDiffuse = NULL;
	static D3DXHANDLE hEmissive = NULL;
	static D3DXHANDLE hSpecular = NULL;
	static D3DXHANDLE hSineness = NULL;

	if (oldPtr != &shader)
	{
		hAmbient = shader.GetValueHandle("material.ambient");
		hDiffuse = shader.GetValueHandle("material.diffuse");
		hEmissive = shader.GetValueHandle("material.emissive");
		hSpecular = shader.GetValueHandle("material.specular");
		hSineness = shader.GetValueHandle("material.shininess");

		oldPtr = &shader;
	}

	shader.SetVector(hAmbient, *(Vector4*)&m_mtrl.Ambient);
	shader.SetVector( hDiffuse, *(Vector4*)&m_mtrl.Diffuse);
	shader.SetVector( hEmissive, *(Vector4*)&m_mtrl.Emissive);
	shader.SetVector( hSpecular, *(Vector4*)&m_mtrl.Specular);
	shader.SetFloat( hSineness, m_mtrl.Power);
}
