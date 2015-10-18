// CubeSerialView.cpp : implementation file
//

#include "stdafx.h"
#include "CubeMonitor.h"
#include "CubeSerialView.h"
#include "afxdialogex.h"


// CCubeSerialView dialog
CCubeSerialView::CCubeSerialView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CCubeSerialView::IDD, pParent)
	, m_isStart(false)
	, m_resetHead(false)
	, m_SendText(_T(""))
	, m_serialRcvCount(0)
	, m_updateIncTime(0)
	, m_IsSyncIMU(TRUE)
	, m_syncIMUState(0)
	, m_errorCount(0)
	, m_IsShowThrust(TRUE)
	, m_IsShowIdealThrust(FALSE)
	, m_IsShowCubeThrust(FALSE)
{
}

CCubeSerialView::~CCubeSerialView()
{
}

void CCubeSerialView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COM, m_ComPort);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_Baudrate);
	DDX_Control(pDX, IDC_BUTTON_START, m_StartButton);
	DDX_Control(pDX, IDC_RICHEDIT2_RCVEDIT, m_RecvEdit);
	DDX_Control(pDX, IDC_STATIC_RCV_COUNT, m_RcvCount);
	DDX_Text(pDX, IDC_EDIT_SEND, m_SendText);
	DDX_Check(pDX, IDC_CHECK_IMU, m_IsSyncIMU);
	DDX_Control(pDX, IDC_EDIT_CUBE_ATTITUDE, m_AttitudeEdit);
	DDX_Check(pDX, IDC_CHECK_SHOWTHRUST, m_IsShowThrust);
	DDX_Check(pDX, IDC_CHECK_SHOWIDEALTHRUST, m_IsShowIdealThrust);
	DDX_Check(pDX, IDC_CHECK_SHOWCUBETHRUST, m_IsShowCubeThrust);
}


BEGIN_MESSAGE_MAP(CCubeSerialView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CCubeSerialView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCubeSerialView::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_START, &CCubeSerialView::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CCubeSerialView::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_CHECK_IMU, &CCubeSerialView::OnBnClickedCheckImu)
	ON_BN_CLICKED(IDC_BUTTON_RESET_HEADING, &CCubeSerialView::OnBnClickedButtonResetHeading)
	ON_BN_CLICKED(IDC_CHECK_SHOWTHRUST, &CCubeSerialView::OnBnClickedCheckShowthrust)
	ON_BN_CLICKED(IDC_CHECK_SHOWIDEALTHRUST, &CCubeSerialView::OnBnClickedCheckShowidealthrust)
	ON_BN_CLICKED(IDC_CHECK_SHOWCUBETHRUST, &CCubeSerialView::OnBnClickedCheckShowcubethrust)
END_MESSAGE_MAP()


// CCubeSerialView message handlers


void CCubeSerialView::OnBnClickedOk()
{
}
void CCubeSerialView::OnBnClickedCancel()
{
}


BOOL CCubeSerialView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	m_ComPort.InitList();

	const int baudRate[] = { 9600, 14400, 19200, 38400, 56000, 57600, 115200, 921600 };
	for (int i = 0; i < ARRAYSIZE(baudRate); ++i)
	{
		TCHAR brateName[32];
		wsprintf(brateName, L"%d", baudRate[i]);
		m_Baudrate.InsertString(m_Baudrate.GetCount(), brateName);
	}
	m_Baudrate.SetCurSel(m_Baudrate.GetCount()-1);

	m_RecvEdit.SetBackgroundColor(FALSE, RGB(0, 0, 0));

	m_RcvCount.SetWindowTextW(L"0");

	return TRUE;
}


// 매프레임마다 호출됨.
void CCubeSerialView::Update(const float deltaSeconds)
{
	RET(!m_isStart);
	RET(!IsWindowVisible());

	m_updateIncTime += deltaSeconds;
	RET(m_updateIncTime < 0.03f);
	m_updateIncTime = 0;

	if (m_IsSyncIMU)
	{
		if (SyncIMU())
			UpdateRecvCount();
		return;
	}

	char buffer[128];
	ZeroMemory(buffer, sizeof(buffer));
	if (cController::Get()->GetSerial().ReadData(buffer, sizeof(buffer)) > 0)
	{
		AppendToLogAndScroll(&m_RecvEdit, common::str2wstr(string(buffer) + "\n").c_str(), RGB(200, 200, 200));
		UpdateRecvCount();
	}
}


// Start 버튼 클릭
void CCubeSerialView::OnBnClickedButtonStart()
{
	if (m_isStart)
	{
		Stop();
	}
	else
	{
		Start();
	}
}


void CCubeSerialView::Start()
{
	const int portNumber = m_ComPort.GetPortNum();
	CString baudRate;
	m_Baudrate.GetWindowTextW(baudRate);

	if (cController::Get()->GetSerial().Open(portNumber, _wtoi(baudRate)))
	{
		m_isStart = true;
		m_StartButton.SetWindowTextW(L"Stop");
		SetBackgroundColor(g_blueColor);
	}
	else
	{
		// error
		AfxMessageBox(L"COM Connect Error!!");
	}
}


void CCubeSerialView::Stop()
{
	m_isStart = false;
	cController::Get()->GetSerial().Close();
	m_StartButton.SetWindowTextW(L"Start");
	SetBackgroundColor(g_grayColor);
}


// 스트링 전송 버튼 클릭
void CCubeSerialView::OnBnClickedButtonSend()
{
	RET(!m_isStart);

	UpdateData();

	string sendText = wstr2str((LPCTSTR)m_SendText);
	cController::Get()->GetSerial().SendData(sendText.c_str(), sendText.size());

	AppendToLogAndScroll(&m_RecvEdit, m_SendText, RGB(240, 240, 240));

	m_SendText = "";
	UpdateData(FALSE);
}


// IMU 체크박스 버튼 클릭
void CCubeSerialView::OnBnClickedCheckImu()
{
	UpdateData();
}


void CCubeSerialView::UpdateRecvCount()
{
	++m_serialRcvCount;
	CString serialCnt;
	serialCnt.Format(L"%d", m_serialRcvCount);
	m_RcvCount.SetWindowTextW(serialCnt);
}


// Cube보드에 IMU정보를 요청하고, 응답한 정보를 토대로 IMU정보를 갱신한다.
bool CCubeSerialView::SyncIMU()
{
	RETV(!m_isStart, false);

	if (m_syncIMUState == 0)
	{
		SendCommand(cController::Get()->GetSerial(), MSP_ATTITUDE);
		m_syncIMUState = 1;
	}
	else if (m_syncIMUState == 1)
	{
		m_syncIMUState = 0;
		BYTE buffer[64];
		const int len = RecvCommand(cController::Get()->GetSerial(), MSP_ATTITUDE, buffer, sizeof(buffer));
		if (len > 0)
		{
			// 자세정보 업데이트
			int roll = *(short*)&buffer[0]; // +- 1800
			int pitch = *(short*)&buffer[2]; // +- 860
			int yaw = *(short*)&buffer[4]; // 0 ~ 360
			//common::dbg::Print("%d %d %d", roll, pitch, yaw);

			Quaternion qr, qp, qy;
			qr.Euler(Vector3(0,0,-roll*0.1f));
			qp.Euler(Vector3(pitch*0.1f, 0,0));
			qy.Euler(Vector3(0, (float)yaw, 0));

			Quaternion q = qy * qp * qr;
			cController::Get()->GetCubeFlight().m_tm = q.GetMatrix();

			CString attitudeStr;
			attitudeStr.Format(L"%f %f %f", -roll*0.1f, pitch*0.1f, (float)yaw);
			m_AttitudeEdit.SetWindowTextW(attitudeStr);

			if (m_resetHead)
				m_syncIMUState = 2;

			m_errorCount = 0;
			return true;
		}
		else
		{
			// 정보를 못받으면, 연결을 끊는다.
			++m_errorCount;
			if (m_errorCount > 5)
				Stop();
		}
	}
	else if (m_syncIMUState == 2)
	{
		SendCommand(cController::Get()->GetSerial(), MSP_CUBE_RESETHEAD);
		m_resetHead = false;
		m_syncIMUState = 0;
	}

	return false;
}


void CCubeSerialView::OnBnClickedButtonResetHeading()
{
	m_resetHead = true;
	cController::Get()->GetCubeFlight().ResetHeading();
}


void CCubeSerialView::OnBnClickedCheckShowthrust()
{
	UpdateData();

	cController::Get()->GetCubeFlight().SetRenderOption(
		m_IsShowThrust ? true : false,
		m_IsShowIdealThrust ? true : false,
		m_IsShowCubeThrust? true : false);
}


void CCubeSerialView::OnBnClickedCheckShowidealthrust()
{
	UpdateData();

	cController::Get()->GetCubeFlight().SetRenderOption(
		m_IsShowThrust ? true : false,
		m_IsShowIdealThrust ? true : false,
		m_IsShowCubeThrust ? true : false);
}

void CCubeSerialView::OnBnClickedCheckShowcubethrust()
{
	UpdateData();

	cController::Get()->GetCubeFlight().SetRenderOption(
		m_IsShowThrust ? true : false,
		m_IsShowIdealThrust ? true : false,
		m_IsShowCubeThrust ? true : false);
}
