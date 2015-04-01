
#include "stdafx.h"
#include "lightmanager.h"


using namespace graphic;


cLightManager::cLightManager()
{
	m_lights.reserve(4);

	// 기본 방향성 조명 추가. index = 0;
	cLight mainLight;
	mainLight.Init(cLight::LIGHT_DIRECTIONAL);

	const Vector3 lightPos(300,300,-300);
	mainLight.SetPosition(lightPos);
	mainLight.SetDirection(-lightPos.Normal()); 
	AddLight(mainLight);

}

cLightManager::~cLightManager()
{
}


int cLightManager::AddLight( const cLight &light )
{
	m_lights.push_back(light);
	return (int)m_lights.size()-1;
}


cLight& cLightManager::GetMainLight()
{
	return m_lights[ 0];
}


cLight* cLightManager::GetLight(const int index)
{
	RETV((int)m_lights.size() <= index, NULL);
	return &m_lights[ index];
}


bool cLightManager::Bind(const int index)
{
	RETV((int)m_lights.size() <= index, false);
	m_lights[ index].Bind(0);
	return true;
}
