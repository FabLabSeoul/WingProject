
#include "stdafx.h"
#include "dxinit.h"

using namespace graphic;


bool graphic::InitDirectX(HWND hWnd, const int width, const int height, OUT LPDIRECT3DDEVICE9 &pDevice )
{
	LPDIRECT3D9 d3d9;
	d3d9 = Direct3DCreate9( D3D_SDK_VERSION );

	// 하드웨어 정보를 가지고 와서 자신의 정점 프로세스 타입을 정하자
	D3DCAPS9 caps;

	//Direct3D9 객체 통해 비디오 카드의 하드웨어 정보를 가지고 온다.
	d3d9->GetDeviceCaps(
		D3DADAPTER_DEFAULT,			//주 디스플레이 그래픽 카드 그냥 D3DADAPTER_DEFAULT
		D3DDEVTYPE_HAL,				//디바이스타입 설정 그냥 D3DDEVTYPE_HAL
		&caps						//디바이스 정보를 받아올 D3DCAPS9 포인터
		);

	//정점계산 처리방식을 지정할 플레그 값
	int vertexProcessing = 0;

	//정점 위치와 광원 계산시 하드웨어 사용이 가능한가
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;


	//3. D3DPRESENT_PARAMETERS 구조체 정보를 생성
	//내가 이러한 Device 를 만들겟다라는 정보

	D3DPRESENT_PARAMETERS d3dpp;		
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				//뎁스 버퍼와 스테실 버퍼 크기 뎁스 24bit 스텐실 버퍼 8 비트
	d3dpp.BackBufferCount = 1;						//백버퍼 갯수 그냥 1개
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;			//백버퍼 색상 포맷 알파8비트,레드8비트,그린8비트,블루8비트
	d3dpp.BackBufferHeight = height;				//백버퍼 픽셀 크기
	d3dpp.BackBufferWidth = width;				//백버퍼 픽셀 크기
	d3dpp.EnableAutoDepthStencil = true;						//자동 깊이버퍼 사용 여부 ( 그냥 true )
	d3dpp.Flags = 0;						//추기 플래그 ( 일단 0 )
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;	//화면 주사율 ( 그냥 D3DPRESENT_RATE_DEFAULT 모니터 주사율과 동일시 )
	d3dpp.hDeviceWindow = hWnd;					//Device 가 출력될 윈도우 핸들
	d3dpp.MultiSampleQuality = 0;						//멀티 샘플링 질
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;		//멀티 샘플링 타입 
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	//화면 전송 간격 ( 그냥 D3DPRESENT_INTERVAL_ONE 모니터 주사율과 동일시 )
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//화면 스왑 체인 방식
	d3dpp.Windowed = true;						//윈도우 모드냐? ( 이게 false 면 풀스크린 된다! )

	//4. Device 생성
	if (FAILED(d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,					//주 디스플레이 그래픽 카드 그냥 D3DADAPTER_DEFAULT
		D3DDEVTYPE_HAL,						//디바이스타입 설정 그냥 D3DDEVTYPE_HAL
		hWnd,								//디바이스를 사용할 윈도우 핸들
		vertexProcessing,					//정점 처리 방식에 대한 플레그
		&d3dpp,								//앞에서 정의한 D3DPRESENT_PARAMETERS 구조체 포인터
		&pDevice							//얻어올 디바이스 더블포인터
		)))
	{
		//디바이스 생성 실패
		d3d9->Release(); // Deivce 를 만들기 위해 생성된 Direct3D9 객체를 해제
		d3d9 = NULL;

		MessageBoxA( hWnd, "CreateDevice() - FAILED", "FAILED", MB_OK );
		return false;
	}


	//디바이스 생성 성공
	d3d9->Release(); // Deivce 를 만들었으니 넌 더이상 필요없다 ( 사라져라! )
	d3d9 = NULL;
	return true;
}
