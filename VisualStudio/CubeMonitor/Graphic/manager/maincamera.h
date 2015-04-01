// 전역 카메라 클래스.
//

#pragma once


namespace graphic
{

	//DECLARE_TYPE_NAME_SCOPE(graphic, cMainCamera)
	class cMainCamera : public cCamera
									, public common::cSingleton<cMainCamera>
									//, public memmonitor::Monitor<cMainCamera, TYPE_NAME(cMainCamera)>
	{
	public:
		cMainCamera();
		virtual ~cMainCamera();

	};


	inline cMainCamera* GetMainCamera() { return cMainCamera::Get(); }
}
