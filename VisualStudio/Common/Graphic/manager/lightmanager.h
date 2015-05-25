// 조명을 관리하는 클래스
// 기본 조명이 하나 생성되어 있다. (매인 조명)
#pragma once


namespace graphic
{


	class cLightManager : public common::cSingleton<cLightManager>
	{
	public:
		cLightManager();
		virtual ~cLightManager();

		int AddLight( const cLight &light );
		cLight& GetMainLight();
		cLight* GetLight(const int index);

		bool Bind(const int index=0);


	private:
		vector<cLight> m_lights;
	};


	inline cLight& GetMainLight() { return cLightManager::Get()->GetMainLight(); }
}
