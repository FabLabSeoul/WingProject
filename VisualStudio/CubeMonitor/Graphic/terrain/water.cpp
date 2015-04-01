
#include "stdafx.h"
#include "water.h"

using namespace graphic;


cWater::cWater()
	: m_isRenderSurface(false)
{
	Matrix44 mWaterWorld;
	mWaterWorld.SetTranslate(Vector3(0,10,0));

	sInitInfo waterInitInfo;
	waterInitInfo.dirLight.Init(cLight::LIGHT_DIRECTIONAL);
	waterInitInfo.mtrl.InitWhite();
	waterInitInfo.vertRows = 64;
	waterInitInfo.vertCols = 64;
	waterInitInfo.dx = 1.0f;
	waterInitInfo.dz = 1.0f;
	waterInitInfo.waveMapFilename0 = "wave0.dds";
	waterInitInfo.waveMapFilename1 = "wave1.dds";
	waterInitInfo.waveMapVelocity0 = Vector2(0.09f, 0.06f);
	waterInitInfo.waveMapVelocity1 = Vector2(-0.05f, 0.08f);
	waterInitInfo.texScale = 10.0f; 
	waterInitInfo.refractBias = 0.1f;
	waterInitInfo.refractPower = 2.0f;
	//waterInitInfo.rippleScale  = Vector2(0.06f, 0.03f); 
	waterInitInfo.rippleScale  = Vector2(0.06f, 0.0f); 
	waterInitInfo.toWorld = mWaterWorld;

	m_initInfo = waterInitInfo;

	m_waveMapOffset0 = Vector2(0.0f, 0.0f);
	m_waveMapOffset1 = Vector2(0.0f, 0.0f);

}

cWater::~cWater()
{
}


// 물 클래스 생성.
bool cWater::Create()
{
	RETV(m_grid.GetVertexBuffer().GetVertexCount() > 0, true); // 이미 생성되었다면 리턴

	const int width = 512;
	const int  height = 512;
	D3DVIEWPORT9 vp = {0, 0, width, height, 0.0f, 1.0f};
	m_reflectMap.Create(width, height, 0, D3DFMT_X8R8G8B8, true, D3DFMT_D24X8, vp, true);
	m_refractMap.Create(width, height, 0, D3DFMT_X8R8G8B8, true, D3DFMT_D24X8, vp, true);

	m_shader.Create( cResourceManager::Get()->FindFile("water.fx"), "WaterTech" );
	m_grid.Create(m_initInfo.vertRows, m_initInfo.vertCols, 64, 8.f, 0.f);
	m_grid.GetTexture().Create( cResourceManager::Get()->FindFile("whitetex.dds") );

	m_waveMap0.Create( cResourceManager::Get()->FindFile(m_initInfo.waveMapFilename0) );
	m_waveMap1.Create( cResourceManager::Get()->FindFile(m_initInfo.waveMapFilename1) );

	m_hWVP = m_shader.GetValueHandle( "gWVP");
	m_hEyePosW = m_shader.GetValueHandle( "gEyePosW");
	m_hLight = m_shader.GetValueHandle( "gLight");
	m_hMtrl = m_shader.GetValueHandle( "gMtrl");
	m_hWaveMapOffset0 = m_shader.GetValueHandle( "gWaveMapOffset0");
	m_hWaveMapOffset1 = m_shader.GetValueHandle( "gWaveMapOffset1");
	m_hReflectMap = m_shader.GetValueHandle( "gReflectMap");
	m_hRefractMap = m_shader.GetValueHandle( "gRefractMap");


	// We don't need to set these every frame since they do not change
	m_shader.SetMatrix("gWorld", m_initInfo.toWorld);
	Matrix44 worldInv = m_initInfo.toWorld.Inverse();
	m_shader.SetMatrix("gWorldInv", worldInv);
	m_shader.SetTexture("gWaveMap0", m_waveMap0);
	m_shader.SetTexture("gWaveMap1", m_waveMap1);

	Vector3 dirW(0.0f, -1.0f, -3.0f);
	dirW.Normalize();
	m_initInfo.dirLight.m_light.Ambient = D3DXCOLOR(0.8f,0.8f,0.8f,1);
	m_initInfo.dirLight.Bind(m_shader);
	m_shader.SetVector("light.dirW", dirW);

	m_initInfo.mtrl.m_mtrl.Ambient = D3DXCOLOR(0.4f,0.4f,0.4f,1);
	m_initInfo.mtrl.m_mtrl.Specular = D3DXCOLOR(0.6f,0.6f,0.6f,0.6f);
	m_initInfo.mtrl.m_mtrl.Power = 200;
	m_initInfo.mtrl.Bind(m_shader);

	m_shader.SetFloat("gRefractBias", m_initInfo.refractBias);
	m_shader.SetFloat("gRefractPower", m_initInfo.refractPower);
	m_shader.SetVector("gRippleScale", m_initInfo.rippleScale);

	return true;
}


void cWater::Render()
{
	m_shader.SetMatrix("mVP", cMainCamera::Get()->GetViewProjectionMatrix());
	cLightManager::Get()->GetMainLight().Bind(m_shader);

	m_shader.SetMatrix(m_hWVP, m_initInfo.toWorld*cMainCamera::Get()->GetViewProjectionMatrix());
	m_shader.SetVector(m_hEyePosW, cMainCamera::Get()->GetEyePos());
	m_shader.SetVector(m_hWaveMapOffset0, m_waveMapOffset0);
	m_shader.SetVector(m_hWaveMapOffset1, m_waveMapOffset1);
	m_shader.SetTexture(m_hReflectMap, m_reflectMap.GetTexture());
	m_shader.SetTexture(m_hRefractMap, m_refractMap.GetTexture());

	m_grid.RenderShader(m_shader);

	// 디버깅용.
	if (m_isRenderSurface)
	{
		m_refractMap.Render(1);
		m_reflectMap.Render(2);
	}
}


void cWater::Move(const float elapseTime)
{
	// Update texture coordinate offsets.  These offsets are added to the
	// texture coordinates in the vertex shader to animate them.
	m_waveMapOffset0 += m_initInfo.waveMapVelocity0 * elapseTime;
	m_waveMapOffset1 += m_initInfo.waveMapVelocity1 * elapseTime;

	// Textures repeat every 1.0 unit, so reset back down to zero
	// so the coordinates do not grow too large.
	if(m_waveMapOffset0.x >= 1.0f || m_waveMapOffset0.x <= -1.0f)
		m_waveMapOffset0.x = 0.0f;
	if(m_waveMapOffset1.x >= 1.0f || m_waveMapOffset1.x <= -1.0f)
		m_waveMapOffset1.x = 0.0f;
	if(m_waveMapOffset0.y >= 1.0f || m_waveMapOffset0.y <= -1.0f)
		m_waveMapOffset0.y = 0.0f;
	if(m_waveMapOffset1.y >= 1.0f || m_waveMapOffset1.y <= -1.0f)
		m_waveMapOffset1.y = 0.0f;
}

void cWater::BeginRefractScene()
{
	m_refractMap.Begin();
}


void cWater::EndRefractScene()
{
	m_refractMap.End();
}


void cWater::BeginReflectScene()
{
	m_reflectMap.Begin();
}


void cWater::EndReflectScene()
{
	m_reflectMap.End();
}
