
#include "stdafx.h"
#include "dxinit.h"

using namespace graphic;


bool graphic::InitDirectX(HWND hWnd, const int width, const int height, OUT LPDIRECT3DDEVICE9 &pDevice )
{
	LPDIRECT3D9 d3d9;
	d3d9 = Direct3DCreate9( D3D_SDK_VERSION );

	// �ϵ���� ������ ������ �ͼ� �ڽ��� ���� ���μ��� Ÿ���� ������
	D3DCAPS9 caps;

	//Direct3D9 ��ü ���� ���� ī���� �ϵ���� ������ ������ �´�.
	d3d9->GetDeviceCaps(
		D3DADAPTER_DEFAULT,			//�� ���÷��� �׷��� ī�� �׳� D3DADAPTER_DEFAULT
		D3DDEVTYPE_HAL,				//����̽�Ÿ�� ���� �׳� D3DDEVTYPE_HAL
		&caps						//����̽� ������ �޾ƿ� D3DCAPS9 ������
		);

	//������� ó������� ������ �÷��� ��
	int vertexProcessing = 0;

	//���� ��ġ�� ���� ���� �ϵ���� ����� �����Ѱ�
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;


	//3. D3DPRESENT_PARAMETERS ����ü ������ ����
	//���� �̷��� Device �� ����ٴٶ�� ����

	D3DPRESENT_PARAMETERS d3dpp;		
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				//���� ���ۿ� ���׽� ���� ũ�� ���� 24bit ���ٽ� ���� 8 ��Ʈ
	d3dpp.BackBufferCount = 1;						//����� ���� �׳� 1��
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;			//����� ���� ���� ����8��Ʈ,����8��Ʈ,�׸�8��Ʈ,���8��Ʈ
	d3dpp.BackBufferHeight = height;				//����� �ȼ� ũ��
	d3dpp.BackBufferWidth = width;				//����� �ȼ� ũ��
	d3dpp.EnableAutoDepthStencil = true;						//�ڵ� ���̹��� ��� ���� ( �׳� true )
	d3dpp.Flags = 0;						//�߱� �÷��� ( �ϴ� 0 )
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;	//ȭ�� �ֻ��� ( �׳� D3DPRESENT_RATE_DEFAULT ����� �ֻ����� ���Ͻ� )
	d3dpp.hDeviceWindow = hWnd;					//Device �� ��µ� ������ �ڵ�
	d3dpp.MultiSampleQuality = 0;						//��Ƽ ���ø� ��
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;		//��Ƽ ���ø� Ÿ�� 
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	//ȭ�� ���� ���� ( �׳� D3DPRESENT_INTERVAL_ONE ����� �ֻ����� ���Ͻ� )
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//ȭ�� ���� ü�� ���
	d3dpp.Windowed = true;						//������ ����? ( �̰� false �� Ǯ��ũ�� �ȴ�! )

	//4. Device ����
	if (FAILED(d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,					//�� ���÷��� �׷��� ī�� �׳� D3DADAPTER_DEFAULT
		D3DDEVTYPE_HAL,						//����̽�Ÿ�� ���� �׳� D3DDEVTYPE_HAL
		hWnd,								//����̽��� ����� ������ �ڵ�
		vertexProcessing,					//���� ó�� ��Ŀ� ���� �÷���
		&d3dpp,								//�տ��� ������ D3DPRESENT_PARAMETERS ����ü ������
		&pDevice							//���� ����̽� ����������
		)))
	{
		//����̽� ���� ����
		d3d9->Release(); // Deivce �� ����� ���� ������ Direct3D9 ��ü�� ����
		d3d9 = NULL;

		MessageBoxA( hWnd, "CreateDevice() - FAILED", "FAILED", MB_OK );
		return false;
	}


	//����̽� ���� ����
	d3d9->Release(); // Deivce �� ��������� �� ���̻� �ʿ���� ( �������! )
	d3d9 = NULL;
	return true;
}
