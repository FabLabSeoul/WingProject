// 카메라 클래스.
#pragma once


namespace graphic
{


	class cCamera
	{
	public:
		cCamera();
		cCamera(const Vector3 &eyePos, const Vector3 &lookAt, const Vector3 &up);
		virtual ~cCamera();

		void Update();

		void SetCamera(const Vector3 &eyePos, const Vector3 &lookAt, const Vector3 &up);
		void SetProjection(const float fov, const float aspect, const float nearPlane, const float farPlane);
		void SetEyePos(const Vector3 &eye);
		void SetLookAt(const Vector3 &lookAt);
		void SetUpVector(const Vector3 &up);
		void SetViewMatrix(const Matrix44 &view);
		void SetProjectionMatrix(const Matrix44 &proj);
		const Vector3& GetEyePos() const;
		const Vector3& GetLookAt() const;
		const Vector3& GetUpVector() const;
		const Matrix44& GetViewMatrix() const;
		const Matrix44& GetProjectionMatrix() const;
		const Matrix44& GetViewProjectionMatrix();
		Vector3 GetDirection() const;
		Vector3 GetRight() const;
		float GetDistance() const; // lookAt - eyePos 사이 거리.

		// LookAt 이 이동한다.
		void Pitch( const float radian );
		void Yaw( const float radian );
		void Roll( const float radian );

		// EyePos 가 이동한다.
		void Pitch2( const float radian );
		void Yaw2( const float radian );
		void Roll2( const float radian );

		void MoveFront( const float len );
		void MoveUp( const float len );
		void MoveRight( const float len );
		void MoveAxis( const Vector3 &dir, const float len );
		void Zoom( const float len );


	protected:
		void UpdateViewMatrix();
		void UpdateProjectionMatrix();


	private:
		Vector3 m_eyePos;
		Vector3 m_lookAt;
		Vector3 m_up;
		Matrix44 m_view; // 카메라 행렬.
		Matrix44 m_proj; // 투영 행렬.
		Matrix44 m_viewProj; // m_view X m_proj
	};


	inline void cCamera::SetEyePos(const Vector3 &eye) { m_eyePos = eye; UpdateViewMatrix(); }
	inline void cCamera::SetLookAt(const Vector3 &lookAt) { m_lookAt = lookAt; UpdateViewMatrix(); }
	inline void cCamera::SetUpVector(const Vector3 &up) { m_up = up; UpdateViewMatrix(); }
	inline void cCamera::SetViewMatrix(const Matrix44 &view) { m_view = view; }
	inline void cCamera::SetProjectionMatrix(const Matrix44 &proj) { m_proj = proj; UpdateProjectionMatrix(); }
	inline const Vector3& cCamera::GetEyePos() const { return m_eyePos; }
	inline const Vector3& cCamera::GetLookAt() const { return m_lookAt; }
	inline const Vector3& cCamera::GetUpVector() const { return m_up; }
	inline const Matrix44& cCamera::GetViewMatrix() const { return m_view; }
	inline const Matrix44& cCamera::GetProjectionMatrix() const { return m_proj; }
	inline const Matrix44& cCamera::GetViewProjectionMatrix() { m_viewProj = m_view * m_proj; return m_viewProj; }		 
}
