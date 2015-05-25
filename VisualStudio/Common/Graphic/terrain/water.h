// 물 효과
// Frank Luna, Pond Water 예제를 참조해서 만들었다.
// http://blog.naver.com/swoosungi/90086817771
#pragma once


namespace graphic
{

	class cWater
	{
		struct sInitInfo
		{
			cLight dirLight;
			cMaterial mtrl;
			int vertRows;
			int vertCols;
			float dx;
			float dz;
			string waveMapFilename0;
			string waveMapFilename1;
			Vector2 waveMapVelocity0;
			Vector2 waveMapVelocity1;
			float texScale;
			float refractBias;
			float refractPower;
			Vector2 rippleScale;
			Matrix44 toWorld;
		};

	public:
		cWater();
		virtual ~cWater();

		bool Create();
		virtual void Render();
		virtual void Move(const float elapseTime);

		void BeginRefractScene();
		void EndRefractScene();
		void BeginReflectScene();
		void EndReflectScene();
		void SetRenderReflectMap(const bool enable);
		bool IstRenderReflectMap() const;


	private:
		sInitInfo m_initInfo;
		cSurface2 m_reflectMap;
		cSurface2 m_refractMap;
		cGrid2 m_grid;
		cShader m_shader;
		cTexture m_waveMap0;
		cTexture m_waveMap1;
		Vector2 m_waveMapOffset0;
		Vector2 m_waveMapOffset1;
		bool m_isRenderSurface;

		D3DXHANDLE m_hWVP;
		D3DXHANDLE m_hLight;
		D3DXHANDLE m_hMtrl;
		D3DXHANDLE m_hEyePosW;
		D3DXHANDLE m_hWaveMapOffset0;
		D3DXHANDLE m_hWaveMapOffset1;
		D3DXHANDLE m_hReflectMap;
		D3DXHANDLE m_hRefractMap;
	};

	
	inline void cWater::SetRenderReflectMap(const bool enable) { m_isRenderSurface = enable; }
	inline bool cWater::IstRenderReflectMap() const { return m_isRenderSurface; }
}
