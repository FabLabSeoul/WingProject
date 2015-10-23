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
	, m_imuModel(NULL)
{
	g_3DView = this;
}

C3DView::~C3DView()
{
	SAFE_DELETE(m_imuModel);
	SAFE_DELETE(m_cubeFlight);
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

	const int WINSIZE_X = 800;		//초기 윈도우 가로 크기
	const int WINSIZE_Y = 600;	//초기 윈도우 세로 크기
	
	// Create DirectX
	if (!graphic::InitRenderer(GetSafeHwnd(), WINSIZE_X, WINSIZE_Y))
	{
		::AfxMessageBox(L"DirectX Initialize Error!!");
		return 0;
	}

	graphic::cResourceManager::Get()->SetMediaDirectory("./");
	GetMainCamera()->SetCamera(Vector3(12, 20, -10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GetMainCamera()->SetProjection(D3DX_PI / 4.f, (float)WINSIZE_X / (float)WINSIZE_Y, 1.f, 1000.0f);
	GetMainCamera()->SetEyePos(Vector3(20, 30, -35));
	GetMainCamera()->SetLookAt(Vector3(10, 0, 0));

	GetDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	GetDevice()->LightEnable(0,true);

	// 주 광원 초기화.
	cLightManager::Get()->GetMainLight().Bind(0);
	cLightManager::Get()->GetMainLight().Init(cLight::LIGHT_DIRECTIONAL,
		Vector4(0.7f, 0.7f, 0.7f, 0), Vector4(0.2f, 0.2f, 0.2f, 0));

	m_isInitDx = true;

	m_imuModel = new cCubeFlight();
	m_imuModel->Init();

	m_cubeFlight = new cCubeFlight();
	m_cubeFlight->Init();

	cController::Get()->AddUpdateObserver(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void C3DView::Update(const float deltaSeconds)
{
	RET(!m_isInitDx);

	m_IncSeconds += deltaSeconds;

	RET(m_isFixedFrame && (m_IncSeconds < 0.01f)); // 시간 간격이 짧으면 실행하지 않는다.
	if (m_IncSeconds > 0.02f) // 너무 값이 크면, 최소 값으로 설정한다.
		m_IncSeconds = 0.02f;

// 	GetMainCamera()->SetEyePos(Vector3(20, 30, -35));
// 	GetMainCamera()->SetLookAt(Vector3(10, 0, 0));

	// 상속받는 클래스에서 구현한다.
	UpdateBefore(m_IncSeconds);


	GetRenderer()->Update(m_IncSeconds);
	//cController::Get()->GetCubeFlight().Update(m_IncSeconds);
	if (m_imuModel)
		m_imuModel->Update(m_IncSeconds);
	if (m_cubeFlight)
		m_cubeFlight->Update(m_IncSeconds);

	// 출력.
	Render();


	UpdateAfter(m_IncSeconds);
	m_IncSeconds = 0;
}


void C3DView::Render()
{
	RET(!m_isInitDx);

	//화면 청소
	if (SUCCEEDED(GetDevice()->Clear(
		0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_XRGB(150, 150, 150), 1.0f, 0)))
	{
		GetDevice()->BeginScene();

		cLightManager::Get()->GetMainLight().Bind(0);

		// 백그라운드 그리드, 축 출력.
		GetRenderer()->RenderGrid();
		GetRenderer()->RenderAxis();

		if (m_imuModel)
			m_imuModel->Render(Matrix44::Identity);
		if (m_cubeFlight)
		{
			Matrix44 t;
			t.SetPosition(Vector3(20, 0, 0));
			Matrix44 tm = m_imuModel->m_localSpaceTM * m_imuModel->m_tm * m_imuModel->m_offset;
			m_cubeFlight->Render(tm * t);
		}

		// 파생받는 클래스에서 구현한다.
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
