// ��ī�̹ڽ�
// Sphere �� �ϴ��� ǥ���ϰ�, ���̴��� ����Ѵ�.
// Frank Luna �� Sky Ŭ������ �������� �������.
// PondWater ���� Clipping Plane �� �������ؼ� ���̴��ε� ��ī�̹ڽ��� �ʿ��ߴ�.
#pragma once


namespace graphic
{

	class cSkyBox2
	{
	public:
		cSkyBox2();
		virtual ~cSkyBox2();

		bool Create(const string &skyboxFileName, const float radius);
		void Render(const Matrix44 &tm = Matrix44::Identity);

	
	private:
		ID3DXMesh* m_sphere;
		float m_radius;
		IDirect3DCubeTexture9* m_envMap;
		cShader m_shader;
		D3DXHANDLE m_hEnvMap;
		D3DXHANDLE m_hWVP;
	};

}

