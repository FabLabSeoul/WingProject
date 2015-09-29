// CubeSerialView.cpp : implementation file
//

#include "stdafx.h"
#include "CubeMonitor.h"
#include "CubeSerialView.h"
#include "afxdialogex.h"
#include "BaseFlightProtocol.h"


// CCubeSerialView dialog
CCubeSerialView::CCubeSerialView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CCubeSerialView::IDD, pParent)
	, m_isStart(false)
	, m_SendText(_T(""))
	, m_serialRcvCount(0)
	, m_updateIncTime(0)
	, m_IsSyncIMU(FALSE)
	, m_syncIMUState(0)
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
}


BEGIN_MESSAGE_MAP(CCubeSerialView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CCubeSerialView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCubeSerialView::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_START, &CCubeSerialView::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CCubeSerialView::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_CHECK_IMU, &CCubeSerialView::OnBnClickedCheckImu)
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

	const int baudRate[] = { 9600, 14400, 19200, 38400, 56000, 57600, 115200 };
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
	if (m_serial.ReadData(buffer, sizeof(buffer)) > 0)
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
		m_isStart = false;
		m_serial.Close();
		m_StartButton.SetWindowTextW(L"Start");
		SetBackgroundColor(g_grayColor);
	}
	else
	{
		const int portNumber = m_ComPort.GetPortNum();
		CString baudRate;
		m_Baudrate.GetWindowTextW(baudRate);

		if (m_serial.Open(portNumber, _wtoi(baudRate)))
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
}


// 스트링 전송 버튼 클릭
void CCubeSerialView::OnBnClickedButtonSend()
{
	RET(!m_isStart);

	UpdateData();

	string sendText = wstr2str((LPCTSTR)m_SendText);
	m_serial.SendData(sendText.c_str(), sendText.size());

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
		SendCommand(m_serial, MSP_ATTITUDE);
		m_syncIMUState = 1;
	}
	else
	{
		m_syncIMUState = 0;
		BYTE buffer[64];
		const int len = RecvCommand(m_serial, buffer, sizeof(buffer));
		if (len > 0)
		{
			// 자세정보 업데이트
			int roll = *(short*)&buffer[0]; // +- 1800
			int pitch = *(short*)&buffer[2]; // +- 860
			int yaw = *(short*)&buffer[4]; // 0 ~ 360
			//common::dbg::Print("%d %d %d", roll, pitch, yaw);

			//cController::Get()->GetCubeFlight().SetEulerAngle(roll*0.1f, yaw, pitch*0.1f);// pitch*0.1f, 0);
			Quaternion qr, qp, qy;
			qr.Euler(Vector3(roll*0.1f,0,0));
			qp.Euler(Vector3(0, 0, pitch*0.1f));
			qy.Euler(Vector3(0, (float)yaw, 0));

			Quaternion q = qy * qp * qr;
			cController::Get()->GetCubeFlight().m_tm = q.GetMatrix();

			CString attitudeStr;
			attitudeStr.Format(L"%f %f %f", roll*0.1f, pitch*0.1f, (float)yaw);
			m_AttitudeEdit.SetWindowTextW(attitudeStr);
			return true;
		}
	}

	return false;
}


// Naze32 CLI 명령 전송
void CCubeSerialView::SendCommand(CSerial &serial, const unsigned char cmd)
{
	unsigned char packet[64];
	int checksum = 0;
	int idx = 0;
	packet[idx++] = '$';
	packet[idx++] = 'M';
	packet[idx++] = '<';
	packet[idx++] = 0;
	checksum ^= 0;
	packet[idx++] = cmd;
	checksum ^= cmd;
	packet[idx++] = checksum;
	serial.SendData((char*)packet, idx);
}


// Naze32 CLI 정보 수신
// return value : 0 정보 수신중
//						   n 수신된 정보 수
//						   -1 수신 완료, 실패
int CCubeSerialView::RecvCommand(CSerial &serial, OUT unsigned char buffer[], const int maxLen)
{
	int state = 0;
	int len = 0;
	int readLen = 0;
	int msp = 0;
	int noDataCnt = 0;
	int checkSum = 0;
	while (1)
	{
		unsigned char c;
		if (serial.ReadData(&c, 1) <= 0)
		{
			Sleep(1);
			++noDataCnt;
			if (noDataCnt > 500)
				break; // exception
			continue;
		}

		switch (state)
		{
		case 0:
		{
			state = (c == '$') ? 1 : 0;
			//cout << c;
		}
		break;

		case 1:
		{
			state = (c == 'M') ? 2 : 0;
			//cout << c;
		}
		break;

		case 2:
		{
			state = (c == '>') ? 3 : 0;
			//cout << c;
		}
		break;

		case 3:
		{
			len = c;
			//cout << (int)c;
			checkSum ^= c;
			state = 4;
		}
		break;

		case 4:
		{
			msp = c;
			//cout << (int)c << " ";
			checkSum ^= c;
			state = 5;
		}
		break;

		case 5:
		{
			if (len > readLen)
			{
				checkSum ^= c;
				if (readLen < maxLen)
					buffer[readLen] = c;

				//cout << (int)c << " ";
			}
			else
			{
				if (checkSum == c)
				{
					//cout << "ok" << endl;
					return readLen; // end;
				}
				else
				{
					//cout << "error!!" << endl;
					return -1; // end;
				}
			}

			++readLen;
		}
		break;

		default:
			break;
		}
	}

	return 0;
}
