// C3DDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CubeMonitor.h"
#include "C3DDialog.h"
#include "afxdialogex.h"
#include "Controller.h"
#include "CubeFlight.h"

using namespace graphic;

C3DDialog *g_3DView = NULL;


// C3DDialog dialog
C3DDialog::C3DDialog(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(C3DDialog::IDD, pParent)
{
	g_3DView = this;
}

C3DDialog::~C3DDialog()
{
	graphic::ReleaseRenderer();
}

void C3DDialog::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(C3DDialog, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &C3DDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &C3DDialog::OnBnClickedCancel)
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


void C3DDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDockablePaneChildView::OnOK();
}


void C3DDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//CDockablePaneChildView::OnCancel();
}


BOOL C3DDialog::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	const int WINSIZE_X = 800;		//초기 윈도우 가로 크기
	const int WINSIZE_Y = 600;	//초기 윈도우 세로 크기
	
	// Create DirectX
	if (!graphic::InitRenderer(GetSafeHwnd(), WINSIZE_X, WINSIZE_Y))
	{
		::AfxMessageBox(L"DirectX Initialize Error!!");
		return 0;
	}

	GetMainCamera()->SetCamera(Vector3(100, 300, -500), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GetMainCamera()->SetProjection(D3DX_PI / 4.f, (float)WINSIZE_X / (float)WINSIZE_Y, 1.f, 10000.0f);

	GetDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	GetDevice()->LightEnable(0,true);

	// 주 광원 초기화.
	cLightManager::Get()->GetMainLight().Bind(0);

	m_isInitDx = true;

	m_box.SetBox(Vector3(-10, -10, -10), Vector3(10, 10, 10));
	m_sphere.Create(10, 10, 10);
	m_sphere.GetMaterial().InitBlue();
	m_sphere.GetMaterial().GetMtrl().Emissive = *(D3DCOLORVALUE*)&Vector4(1, 1, 0, 1);

	m_cube.SetCube(Vector3(-10, -10, -10), Vector3(10, 10, 10));

	cController::Get()->Init();
	cController::Get()->AddUpdateObserver(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void C3DDialog::Update(const float deltaSeconds)
{
	RET(!m_isInitDx);

	m_IncSeconds += deltaSeconds;

	RET(m_IncSeconds < 0.01f); // 시간 간격이 짧으면 실행하지 않는다.


	GetRenderer()->Update(m_IncSeconds);
	cController::Get()->GetCubeFlight().Update(m_IncSeconds);

	static float angle = 0;
	angle += m_IncSeconds;
	Matrix44 mat;
	mat.SetRotationY(angle);
	m_cubeTm = mat;

	// 출력.
	Render();

	m_IncSeconds = 0;
}


void C3DDialog::Render()
{
	RET(!m_isInitDx);

	//화면 청소
	if (SUCCEEDED(GetDevice()->Clear(
		0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_XRGB(150, 150, 150), 1.0f, 0)))
	{
		GetDevice()->BeginScene();

		//cLightManager::Get()->GetMainLight().Bind(0);

		//m_box.Render();
		//m_sphere.GetMaterial().Bind();
		//m_sphere.Render(Matrix44::Identity);
		cController::Get()->GetCubeFlight().Render();
		//m_cube.Render(m_cubeTm);

		// 백그라운드 그리드, 축 출력.
		GetRenderer()->RenderGrid();
		GetRenderer()->RenderAxis();
		GetRenderer()->RenderFPS();

		GetDevice()->EndScene();
		GetDevice()->Present(NULL, NULL, NULL, NULL);
	}
}


void C3DDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	SetFocus();
	m_LButtonDown = true;
	m_curPos = point;
	CDockablePaneChildView::OnLButtonDown(nFlags, point);
}


void C3DDialog::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	m_LButtonDown = false;
	CDockablePaneChildView::OnLButtonUp(nFlags, point);
}


void C3DDialog::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	SetCapture();
	m_RButtonDown = true;
	m_curPos = point;
	CDockablePaneChildView::OnRButtonDown(nFlags, point);
}


void C3DDialog::OnRButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	m_RButtonDown = false;
	CDockablePaneChildView::OnRButtonUp(nFlags, point);
}


void C3DDialog::OnMouseMove(UINT nFlags, CPoint point)
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


BOOL C3DDialog::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	const float len = GetMainCamera()->GetDistance();
	float zoomLen = (len > 100) ? 50 : (len / 4.f);
	if (nFlags & 0x4)
		zoomLen = zoomLen / 10.f;

	GetMainCamera()->Zoom((zDelta < 0) ? -zoomLen : zoomLen);

	return CDockablePaneChildView::OnMouseWheel(nFlags, zDelta, pt);
}


void C3DDialog::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	
}


int C3DDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePaneChildView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}
