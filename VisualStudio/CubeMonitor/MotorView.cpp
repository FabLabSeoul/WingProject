// MotorView.cpp : implementation file
//

#include "stdafx.h"
#include "CubeMonitor.h"
#include "MotorView.h"
#include "afxdialogex.h"


// CMotorView dialog

CMotorView::CMotorView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CMotorView::IDD, pParent)
	, m_IsSyncMotor(FALSE)
	, m_syncState(0)
	, m_updateIncTime(0)
	, m_IsMotorSyncSend(FALSE)
{
}

CMotorView::~CMotorView()
{
}

void CMotorView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	for (int i = 0; i < 24; ++i)
		DDX_Control(pDX, IDC_SLIDER1 + i, m_Slider[i]);
	for (int i = 0; i < 24; ++i)
		DDX_Control(pDX, IDC_EDIT1 + i, m_Edit[i]);
	DDX_Check(pDX, IDC_CHECK_SYNCMOTOR, m_IsSyncMotor);
	DDX_Check(pDX, IDC_CHECK_SYNCMOTOR_SEND, m_IsMotorSyncSend);
}


BEGIN_MESSAGE_MAP(CMotorView, CDockablePaneChildView)
	ON_BN_CLICKED(IDCANCEL, &CMotorView::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CMotorView::OnBnClickedOk)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_SYNCMOTOR, &CMotorView::OnBnClickedCheckSyncmotor)
	ON_NOTIFY_RANGE(NM_CUSTOMDRAW, IDC_SLIDER1, IDC_SLIDER24, &CMotorView::OnNMCustomdrawSlider)
	ON_BN_CLICKED(IDC_CHECK_SYNCMOTOR_SEND, &CMotorView::OnBnClickedCheckSyncmotorSend)
END_MESSAGE_MAP()


// CMotorView message handlers


void CMotorView::OnBnClickedCancel()
{
}
void CMotorView::OnBnClickedOk()
{
}


BOOL CMotorView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	for (int i = 0; i < 24; ++i)
	{
		m_Slider[i].SetRange(0, 255);	
	}

	return TRUE;
}


void CMotorView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePaneChildView::OnSize(nType, cx, cy);
}


void CMotorView::Update(const float deltaSeconds)
{
	RET(!m_IsSyncMotor);
	RET(!IsWindowVisible());

	m_updateIncTime += deltaSeconds;
	if (m_updateIncTime < 0.03f)
		return;
	m_updateIncTime = 0;

	SyncMotor();
}


void CMotorView::OnBnClickedCheckSyncmotor()
{
	UpdateData();

	if (m_IsSyncMotor)
	{
		// 시리얼포트가 열려 있지않으면, 모터싱크를 하지 않는다.
		if (!cController::Get()->GetSerial().IsOpened())
		{
			m_IsSyncMotor = FALSE;
			UpdateData(FALSE);
		}
	}

	if (m_IsSyncMotor)
		SetBackgroundColor(g_blueColor);
	else
		SetBackgroundColor(g_grayColor);
}


void CMotorView::OnBnClickedCheckSyncmotorSend()
{
	UpdateData();
}



void CMotorView::OnNMCustomdrawSlider(UINT Id, NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	const int index = Id - IDC_SLIDER1;
	const int pos = m_Slider[index].GetPos();

	CString str;
	str.Format(L"%d", pos);
	m_Edit[index].SetWindowTextW(str);

	*pResult = 0;
}


void CMotorView::SyncMotor()
{
	RET(!m_IsSyncMotor);

	if (m_syncState == 0)
	{
		SendCommand(cController::Get()->GetSerial(), MSP_CUBE_MOTOR);
		m_syncState = 1;
	}
	else
	{

		unsigned char buffer[24];
		if (RecvCommand(cController::Get()->GetSerial(), MSP_CUBE_MOTOR, buffer, sizeof(buffer)) > 0)
		{
			for (int i = 0; i < 24; ++i)
			{
				CString str;

				int power = buffer[i];
				str.Format(L"%d", power);
				m_Edit[i].SetWindowTextW(str);
				m_Slider[i].SetPos(power);
			}

			cController::Get()->GetCubeFlight().CubeThrust(buffer);
		}

		m_syncState = 0;
	}
}

