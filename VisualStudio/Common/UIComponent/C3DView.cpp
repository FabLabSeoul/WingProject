// C3DDialog.cpp : implementation file
//

#include "stdafx.h"
#include "C3DView.h"
#include "afxdialogex.h"


using namespace graphic;

C3DView *g_3DView = NULL;


// C3DDialog dialog
C3DView::C3DView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(C3DView::IDD, pParent)
{
	g_3DView = this;
}

C3DView::~C3DView()
{
	graphic::ReleaseRenderer();
}

void C3DView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(C3DView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &C3DView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &C3DView::OnBnClickedCancel)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_CONTEXTMENU()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// C3DDialog message handlers


void C3DView::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDockablePaneChildView::OnOK();
}


void C3DView::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//CDockablePaneChildView::OnCancel();
}


BOOL C3DView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	const int WINSIZE_X = 800;		//�ʱ� ������ ���� ũ��
	const int WINSIZE_Y = 600;	//�ʱ� ������ ���� ũ��
	
	// Create DirectX
	if (!graphic::InitRenderer(GetSafeHwnd(), WINSIZE_X, WINSIZE_Y))
	{
		::AfxMessageBox(L"DirectX Initialize Error!!");
		return 0;
	}

	graphic::cResourceManager::Get()->SetMediaDirectory("./");
	GetMainCamera()->SetCamera(Vector3(12, 20, -10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GetMainCamera()->SetProjection(D3DX_PI / 4.f, (float)WINSIZE_X / (float)WINSIZE_Y, 1.f, 1000.0f);
	GetMainCamera()->SetEyePos(Vector3(17, 20, -25));

	GetDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	GetDevice()->LightEnable(0,true);

	// �� ���� �ʱ�ȭ.
	cLightManager::Get()->GetMainLight().Bind(0);
	cLightManager::Get()->GetMainLight().Init(cLight::LIGHT_DIRECTIONAL,
		Vector4(0.7f, 0.7f, 0.7f, 0), Vector4(0.2f, 0.2f, 0.2f, 0));

	m_isInitDx = true;

	//m_box.SetBox(Vector3(0, 0, 0), Vector3(2, 2, 2));
	m_sphere.Create(10, 10, 10);
	m_sphere.GetMaterial().InitBlue();
	m_sphere.GetMaterial().GetMtrl().Emissive = *(D3DCOLORVALUE*)&Vector4(1, 1, 0, 1);

	//m_cube.SetCube(Vector3(0, 0, 0), Vector3(1, 1, 1));
	
	cController::Get()->Init();
	cController::Get()->AddUpdateObserver(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void C3DView::Update(const float deltaSeconds)
{
	RET(!m_isInitDx);

	m_IncSeconds += deltaSeconds;

	RET(m_isFixedFrame && (m_IncSeconds < 0.01f)); // �ð� ������ ª���� �������� �ʴ´�.
	if (m_IncSeconds > 0.02f) // �ʹ� ���� ũ��, �ּ� ������ �����Ѵ�.
		m_IncSeconds = 0.02f;


	// ��ӹ޴� Ŭ�������� �����Ѵ�.
	UpdateBefore(m_IncSeconds);


	GetRenderer()->Update(m_IncSeconds);
	cController::Get()->GetCubeFlight().Update(m_IncSeconds);

	static float angle = 0;
	angle += m_IncSeconds;
	Matrix44 mat;
	mat.SetRotationY(angle);
	m_cubeTm = mat;

	// ���.
	Render();


	UpdateAfter(m_IncSeconds);
	m_IncSeconds = 0;
}


void C3DView::Render()
{
	RET(!m_isInitDx);

	//ȭ�� û��
	if (SUCCEEDED(GetDevice()->Clear(
		0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_XRGB(150, 150, 150), 1.0f, 0)))
	{
		GetDevice()->BeginScene();

		cLightManager::Get()->GetMainLight().Bind(0);

		// ��׶��� �׸���, �� ���.
		GetRenderer()->RenderGrid();
		GetRenderer()->RenderAxis();

		//m_box.Render();
		//m_sphere.GetMaterial().Bind();
		//m_sphere.Render(Matrix44::Identity);
		cController::Get()->GetCubeFlight().Render();


		// �Ļ��޴� Ŭ�������� �����Ѵ�.
		RenderChild();


		GetRenderer()->RenderFPS();

		GetDevice()->EndScene();
		GetDevice()->Present(NULL, NULL, NULL, NULL);
	}
}


void C3DView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	SetFocus();
	m_LButtonDown = true;
	m_curPos = point;
	CDockablePaneChildView::OnLButtonDown(nFlags, point);
}


void C3DView::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	m_LButtonDown = false;
	CDockablePaneChildView::OnLButtonUp(nFlags, point);
}


void C3DView::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	SetCapture();
	m_RButtonDown = true;
	m_curPos = point;
	CDockablePaneChildView::OnRButtonDown(nFlags, point);
}


void C3DView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	m_RButtonDown = false;
	CDockablePaneChildView::OnRButtonUp(nFlags, point);
}


void C3DView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_LButtonDown)
	{
		const CPoint pos = point - m_curPos;
		m_curPos = point;
		Quaternion q1(GetMainCamera()->GetRight(), -pos.y * 0.01f);
		Quaternion q2(GetMainCamera()->GetUpVector(), -pos.x * 0.01f);
		m_rotateTm *= (q2.GetMatrix() * q1.GetMatrix());
	}
	else if (m_RButtonDown)
	{
		const CPoint pos = point - m_curPos;
		m_curPos = point;

		GetMainCamera()->Yaw2(pos.x * 0.005f);
		GetMainCamera()->Pitch2(pos.y * 0.005f);
	}

	CDockablePaneChildView::OnMouseMove(nFlags, point);
}


BOOL C3DView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	const float len = GetMainCamera()->GetDistance();
	float zoomLen = (len > 100) ? 50 : (len / 4.f);
	if (nFlags & 0x4)
		zoomLen = zoomLen / 10.f;

	GetMainCamera()->Zoom((zDelta < 0) ? -zoomLen : zoomLen);

	return CDockablePaneChildView::OnMouseWheel(nFlags, zDelta, pt);
}


void C3DView::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	
}


int C3DView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePaneChildView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}
