// 쉐이더 클래스
// LPD3DXEFFECT 인터페이스를 쉽게사용하기 위해 만들어짐.
#pragma once


namespace graphic
{

	class cShader
	{
	public:
		cShader();
		virtual ~cShader();

		bool Create(const string &fileName, const string &technique, const bool showMsgBox=true);
		void Begin();
		void BeginPass(int pass=-1);
		void EndPass();
		void End();

		void SetInt(const string &key, const int val );
		void SetMatrix(const string &key, const Matrix44 &mat);
		void SetTexture(const string &key, cTexture &texture);
		void SetTexture(const string &key, IDirect3DTexture9 *texture);
		void SetTexture(const string &key, IDirect3DCubeTexture9 *texture);
		void SetFloat(const string &key, float val);
		void SetVector(const string &key, const Vector2 &vec );
		void SetVector(const string &key, const Vector3 &vec );
		void SetVector(const string &key, const Vector4 &vec );
		void SetMatrixArray(const string &key, const Matrix44 *mat, const int count );

		void SetInt(D3DXHANDLE handle, const int val );
		void SetMatrix(D3DXHANDLE handle, const Matrix44 &mat);
		void SetTexture(D3DXHANDLE handle, cTexture &texture);
		void SetTexture(D3DXHANDLE handle, IDirect3DTexture9 *texture);
		void SetFloat(D3DXHANDLE handle, float val);
		void SetVector(D3DXHANDLE handle, const Vector2 &vec );
		void SetVector(D3DXHANDLE handle, const Vector3 &vec );
		void SetVector(D3DXHANDLE handle, const Vector4 &vec );
		void SetMatrixArray(D3DXHANDLE handle, const Matrix44 *mat, const int count );

		void CommitChanges();
		LPD3DXEFFECT GetEffect() { return m_effect; }
		void SetRenderPass(int pass);
		int GetRenderPass() const;
		D3DXHANDLE GetValueHandle(const string &key);
		const string& GetFileName() const;


	private:
		LPD3DXEFFECT m_effect;
		D3DXHANDLE m_hTechnique;
		int m_renderPass; // default = 0;
		string m_fileName;
	};


	inline void cShader::SetRenderPass(int pass) { m_renderPass = pass; }
	inline int cShader::GetRenderPass() const { return m_renderPass; }
	inline const string& cShader::GetFileName() const { return m_fileName; }
}
