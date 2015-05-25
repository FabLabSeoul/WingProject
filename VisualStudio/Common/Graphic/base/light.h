// 조명 클래스
// D3DLIGHT9 데이타를 쉽게 다루기위해서 만들어짐.
#pragma once


namespace graphic
{

	class cLight
	{
	public:
		cLight();
		virtual ~cLight();

		enum TYPE 
		{
			LIGHT_POINT = 1,
			LIGHT_SPOT = 2,
			LIGHT_DIRECTIONAL = 3,
		};

		void Init(TYPE type, 
			const Vector4 &ambient = Vector4(1,1,1,1),
			const Vector4 &diffuse = Vector4(1,1,1,1),
			const Vector4 &specular = Vector4(1,1,1,1),
			const Vector3 &direction = Vector3(0,-1,0));

		void SetDirection( const Vector3 &direction );
		const Vector3& GetDirection() const;
		void SetPosition( const Vector3 &pos );
		const Vector3& GetPosition() const;

		void Bind(int lightIndex) const;
		void Bind(cShader &shader) const;

		void GetShadowMatrix( const Vector3 &modelPos, 
			OUT Vector3 &lightPos, OUT Matrix44 &view, OUT Matrix44 &proj, 
			OUT Matrix44 &tt );


		D3DLIGHT9 m_light;
	};


	inline const Vector3& cLight::GetDirection() const { return *(Vector3*)&m_light.Direction; }
	inline const Vector3& cLight::GetPosition() const { return *(Vector3*)&m_light.Position; }
}
