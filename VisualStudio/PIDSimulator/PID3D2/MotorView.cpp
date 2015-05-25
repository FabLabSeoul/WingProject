// MotorView.cpp : implementation file
//

#include "stdafx.h"
#include "PID3D2.h"
#include "MotorView.h"
#include "afxdialogex.h"
#include "Motor.h"
#include "C3DPhysxPIDView.h"


// CMotorView dialog
CMotorView::CMotorView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CMotorView::IDD, pParent)
	, m_editPower(0)
	, m_editPowerNoise(0)
	, m_editDirX(0)
	, m_editDirY(0)
	, m_editDirZ(0)
	, m_editDirNoiseX(0)
	, m_editDirNoiseY(0)
	, m_editDirNoiseZ(0)
	, m_editP(0)
	, m_editI(0)
	, m_editD(0)
	, m_plotP(NULL)
	, m_plotI(NULL)
	, m_plotD(NULL)
	, m_incSeconds(0)
{
	m_motor = NULL;
}

CMotorView::~CMotorView()
{
}

void CMotorView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_POWER, m_sliderPower);
	DDX_Control(pDX, IDC_SLIDER_POWER_NOISE, m_sliderPowerNoise);
	DDX_Text(pDX, IDC_EDIT_POWER, m_editPower);
	DDX_Text(pDX, IDC_EDIT_POWER_NOISE, m_editPowerNoise);
	DDX_Control(pDX, IDC_SLIDER_DIR_X, m_sliderDirX);
	DDX_Control(pDX, IDC_SLIDER_DIR_Y, m_sliderDirY);
	DDX_Control(pDX, IDC_SLIDER_DIR_Z, m_sliderDirZ);
	DDX_Text(pDX, IDC_EDIT_DIR_X, m_editDirX);
	DDX_Text(pDX, IDC_EDIT_DIR_Y, m_editDirY);
	DDX_Text(pDX, IDC_EDIT_DIR_Z, m_editDirZ);
	DDX_Control(pDX, IDC_SLIDER_DIR_N_X, m_sliderDirNoiseX);
	DDX_Control(pDX, IDC_SLIDER_DIR_N_Y, m_sliderDirNoiseY);
	DDX_Control(pDX, IDC_SLIDER_DIR_N_Z, m_sliderDirNoiseZ);
	DDX_Text(pDX, IDC_EDIT_DIR_N_X, m_editDirNoiseX);
	DDX_Text(pDX, IDC_EDIT_DIR_N_Y, m_editDirNoiseY);
	DDX_Text(pDX, IDC_EDIT_DIR_N_Z, m_editDirNoiseZ);
	DDX_Text(pDX, IDC_EDIT_P, m_editP);
	DDX_Text(pDX, IDC_EDIT_I, m_editI);
	DDX_Text(pDX, IDC_EDIT_D, m_editD);
}


BEGIN_MESSAGE_MAP(CMotorView, CDockablePaneChildView)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_POWER, &CMotorView::OnNMCustomdrawSliderPower)
	ON_EN_CHANGE(IDC_EDIT_POWER, &CMotorView::OnEnChangeEditPower)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_POWER_NOISE, &CMotorView::OnNMCustomdrawSliderPowerNoise)
	ON_EN_CHANGE(IDC_EDIT_POWER_NOISE, &CMotorView::OnEnChangeEditPowerNoise)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_DIR_X, &CMotorView::OnNMCustomdrawSliderDirX)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_DIR_Y, &CMotorView::OnNMCustomdrawSliderDirY)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_DIR_Z, &CMotorView::OnNMCustomdrawSliderDirZ)
	ON_EN_CHANGE(IDC_EDIT_DIR_X, &CMotorView::OnEnChangeEditDirX)
	ON_EN_CHANGE(IDC_EDIT_DIR_Y, &CMotorView::OnEnChangeEditDirY)
	ON_EN_CHANGE(IDC_EDIT_DIR_Z, &CMotorView::OnEnChangeEditDirZ)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_DIR_N_X, &CMotorView::OnNMCustomdrawSliderDirNX)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_DIR_N_Y, &CMotorView::OnNMCustomdrawSliderDirNY)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_DIR_N_Z, &CMotorView::OnNMCustomdrawSliderDirNZ)
	ON_EN_CHANGE(IDC_EDIT_DIR_N_X, &CMotorView::OnEnChangeEditDirNX)
	ON_EN_CHANGE(IDC_EDIT_DIR_N_Y, &CMotorView::OnEnChangeEditDirNY)
	ON_EN_CHANGE(IDC_EDIT_DIR_N_Z, &CMotorView::OnEnChangeEditDirNZ)
	ON_BN_CLICKED(IDOK, &CMotorView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMotorView::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT_P, &CMotorView::OnEnChangeEditP)
	ON_EN_CHANGE(IDC_EDIT_I, &CMotorView::OnEnChangeEditI)
	ON_EN_CHANGE(IDC_EDIT_D, &CMotorView::OnEnChangeEditD)
END_MESSAGE_MAP()


// CMotorView message handlers
BOOL CMotorView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	m_editPower = m_motor->m_power;
	m_editPowerNoise = m_motor->m_powerNoise;
	m_editDirX = m_motor->m_direction.x;
	m_editDirY = m_motor->m_direction.y;
	m_editDirZ = m_motor->m_direction.z;
	m_editDirNoiseX = m_motor->m_directionNoise.x;
	m_editDirNoiseY = m_motor->m_directionNoise.y;
	m_editDirNoiseZ = m_motor->m_directionNoise.z;


	m_sliderPower.SetRange(0, 10);
	m_sliderPower.SetPos((int)m_motor->m_power);
	m_sliderPowerNoise.SetRange(0, 1000);
	m_sliderPowerNoise.SetPos((int)(m_motor->m_powerNoise * 1000.f));

	m_sliderDirX.SetRange(0, 2000);
	m_sliderDirY.SetRange(0, 2000);
	m_sliderDirZ.SetRange(0, 2000);
	m_sliderDirX.SetPos((int)(m_motor->m_direction.x * 1000.f) + 1000);
	m_sliderDirY.SetPos((int)(m_motor->m_direction.y * 1000.f) + 1000);
	m_sliderDirZ.SetPos((int)(m_motor->m_direction.z * 1000.f) + 1000);

	m_sliderDirNoiseX.SetRange(0, 2000);
	m_sliderDirNoiseY.SetRange(0, 2000);
	m_sliderDirNoiseZ.SetRange(0, 2000);
	m_sliderDirNoiseX.SetPos((int)(m_motor->m_directionNoise.x * 1000.f) + 1000);
	m_sliderDirNoiseY.SetPos((int)(m_motor->m_directionNoise.y * 1000.f) + 1000);
	m_sliderDirNoiseZ.SetPos((int)(m_motor->m_directionNoise.z * 1000.f) + 1000);

	m_editP = m_motor->m_p;
	m_editI = m_motor->m_i;
	m_editD = m_motor->m_d;


	{
		// Graph P
		CRect pr;
		GetDlgItem(IDC_STATIC_P)->GetWindowRect(pr);
		ScreenToClient(pr);

		m_plotP = new CPlotWindow();
		m_plotP->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW | WS_CHILD, pr,
			this, 10001);
		m_plotP->SetScrollSizes(MM_TEXT, CSize(pr.Width() - 30, 100));
		m_plotP->ShowWindow(SW_SHOW);
		m_plotP->SetPlot(0, 0, 0, 0, 0, 2);
	}

	{
		// Graph I
		CRect ir;
		GetDlgItem(IDC_STATIC_I)->GetWindowRect(ir);
		ScreenToClient(ir);

		m_plotI = new CPlotWindow();
		m_plotI->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW | WS_CHILD, ir,
			this, 10001);
		m_plotI->SetScrollSizes(MM_TEXT, CSize(ir.Width() - 30, 100));
		m_plotI->ShowWindow(SW_SHOW);
		m_plotI->SetPlot(0, 0, 0, 0, 0, 2);
	}

	{
		// Graph D
		CRect dr;
		GetDlgItem(IDC_STATIC_D)->GetWindowRect(dr);
		ScreenToClient(dr);

		m_plotD = new CPlotWindow();
		m_plotD->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW | WS_CHILD, dr,
			this, 10001);
		m_plotD->SetScrollSizes(MM_TEXT, CSize(dr.Width() - 30, 100));
		m_plotD->ShowWindow(SW_SHOW);
		m_plotD->SetPlot(0, 0, 0, 0, 0);
	}


	cController::Get()->AddUpdateObserver(this);

	UpdateData(FALSE);
	return TRUE; 
}


// PID 정보를 갱신한다.
void CMotorView::Update(const float deltaSeconds)
{
	// 30프레임 단위로 값을 저장하게 한다.
	m_incSeconds += deltaSeconds;
	if (m_incSeconds < 0.033f)
		return;

	// Z축과 벗어난 각도 값을 cos(theta) 로 변환해 저장한다.
	const Vector3 machineZ = Vector3(0, 0, 1).MultiplyNormal(g_physxView->m_machine.m_sensor.m_tm);
	float delta = acos(machineZ.DotProduct(Vector3(0, 0, 1)));
	Vector3 axis(0, 0, m_motor->m_location.z);
	axis.Normalize();
	delta = (axis.CrossProduct(machineZ).x > 0) ? -delta : delta;

	if (m_plotP)
	{
		m_plotP->SetPlotXY(0, delta);
		m_plotP->SetPlotXY(0, delta*m_motor->m_p, 1);
		m_plotP->DrawPlot(m_incSeconds);
	}

	if (m_plotI)
	{
		m_plotI->SetPlotXY(0, delta);
		m_plotI->SetPlotXY(0, m_motor->m_deltaIntegral, 1);
		m_plotI->DrawPlot(m_incSeconds);
	}

	if (m_plotD)
	{
		m_plotD->SetPlotXY(0, delta);
		m_plotD->DrawPlot(m_incSeconds);
	}

	m_incSeconds = 0;
}


void CMotorView::OnNMCustomdrawSliderPower(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	const int pos = m_sliderPower.GetPos();
	m_editPower = (float)pos;
	m_motor->m_power = (float)pos;
	UpdateData(FALSE);

	*pResult = 0;
}


void CMotorView::OnEnChangeEditPower()
{
	UpdateData();
	m_sliderPower.SetPos((int)m_editPower);
}


void CMotorView::OnNMCustomdrawSliderPowerNoise(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	const int pos = m_sliderPowerNoise.GetPos();
	m_editPowerNoise = (float)pos / 1000.f;
	m_motor->m_powerNoise = (float)pos / 1000.f;
	UpdateData(FALSE);

	*pResult = 0;
}


void CMotorView::OnEnChangeEditPowerNoise()
{
	UpdateData();
	m_sliderPowerNoise.SetPos( (int)(max(0.f, min(m_editPowerNoise, 1.f)) * 1000.f) );
}


void CMotorView::OnNMCustomdrawSliderDirX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	const int pos = m_sliderDirX.GetPos();
	m_editDirX = ((float)pos / 1000.f) - 1.f;
	m_motor->m_direction.x = ((float)pos / 1000.f) - 1.f;
	m_motor->m_direction.Normalize();
	UpdateData(FALSE);

	*pResult = 0;
}


void CMotorView::OnNMCustomdrawSliderDirY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	const int pos = m_sliderDirY.GetPos();
	m_editDirY = ((float)pos / 1000.f) - 1.f;
	m_motor->m_direction.y = ((float)pos / 1000.f) - 1.f;
	m_motor->m_direction.Normalize();
	UpdateData(FALSE);

	*pResult = 0;
}


void CMotorView::OnNMCustomdrawSliderDirZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	const int pos = m_sliderDirZ.GetPos();
	m_editDirZ = ((float)pos / 1000.f) - 1.f;
	m_motor->m_direction.z = ((float)pos / 1000.f) - 1.f;
	m_motor->m_direction.Normalize();
	UpdateData(FALSE);

	*pResult = 0;
}


void CMotorView::OnEnChangeEditDirX()
{
	UpdateData();
	m_sliderDirX.SetPos((int)(max(-1.f, min(m_editDirX, 1.f)) * 1000.f) + 1000);
}


void CMotorView::OnEnChangeEditDirY()
{
	UpdateData();
	m_sliderDirY.SetPos((int)(max(-1.f, min(m_editDirY, 1.f)) * 1000.f) + 1000);
}


void CMotorView::OnEnChangeEditDirZ()
{
	UpdateData();
	m_sliderDirZ.SetPos((int)(max(-1.f, min(m_editDirZ, 1.f)) * 1000.f) + 1000);
}


void CMotorView::OnNMCustomdrawSliderDirNX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	const int pos = m_sliderDirNoiseX.GetPos();
	m_editDirNoiseX = ((float)pos / 1000.f) - 1.f;
	m_motor->m_directionNoise.x = ((float)pos / 1000.f) - 1.f;
	UpdateData(FALSE);

	*pResult = 0;
}


void CMotorView::OnNMCustomdrawSliderDirNY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	const int pos = m_sliderDirNoiseY.GetPos();
	m_editDirNoiseY = ((float)pos / 1000.f) - 1.f;
	m_motor->m_directionNoise.y = ((float)pos / 1000.f) - 1.f;
	UpdateData(FALSE);

	*pResult = 0;
}


void CMotorView::OnNMCustomdrawSliderDirNZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	const int pos = m_sliderDirNoiseZ.GetPos();
	m_editDirNoiseZ = ((float)pos / 1000.f) - 1.f;
	m_motor->m_directionNoise.z = ((float)pos / 1000.f) - 1.f;
	UpdateData(FALSE);

	*pResult = 0;
}


void CMotorView::OnEnChangeEditDirNX()
{
	UpdateData();
	m_sliderDirNoiseX.SetPos((int)(max(-1.f, min(m_editDirNoiseX, 1.f)) * 1000.f) + 1000);
}


void CMotorView::OnEnChangeEditDirNY()
{
	UpdateData();
	m_sliderDirNoiseY.SetPos((int)(max(-1.f, min(m_editDirNoiseY, 1.f)) * 1000.f) + 1000);
}


void CMotorView::OnEnChangeEditDirNZ()
{
	UpdateData();
	m_sliderDirNoiseZ.SetPos((int)(max(-1.f, min(m_editDirNoiseZ, 1.f)) * 1000.f) + 1000);
}


void CMotorView::OnBnClickedOk()
{
	// Enter키 Close 막음.
}


void CMotorView::OnBnClickedCancel()
{
	// Esc키 Close 막음.
}


void CMotorView::OnEnChangeEditP()
{
	UpdateData();
	m_motor->m_p = m_editP;
}


void CMotorView::OnEnChangeEditI()
{
	UpdateData();
	m_motor->m_i = m_editI;
}


void CMotorView::OnEnChangeEditD()
{
	UpdateData();
	m_motor->m_d = m_editD;
}
