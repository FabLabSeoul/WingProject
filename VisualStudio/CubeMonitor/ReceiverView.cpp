// ReceiverView.cpp : implementation file
//

#include "stdafx.h"
#include "CubeMonitor.h"
#include "ReceiverView.h"
#include "afxdialogex.h"



// CReceiverView dialog

CReceiverView::CReceiverView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CReceiverView::IDD, pParent)
	, m_IsSyncReceiverRcv(FALSE)
	, m_syncState(0)
	, m_updateIncTime(0)
	, m_IsSyncReceiverSend(FALSE)
	, m_errorCount(0)
{

}

CReceiverView::~CReceiverView()
{
}

void CReceiverView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	for (int i = 0; i < 8; ++i)
		DDX_Control(pDX, IDC_SLIDER1 + i, m_Slider[i]);
	for (int i = 0; i < 8; ++i)
		DDX_Control(pDX, IDC_EDIT1 + i, m_Edit[i]);
	DDX_Check(pDX, IDC_CHECK_SYNC_RECEIVER_RCV, m_IsSyncReceiverRcv);
	DDX_Check(pDX, IDC_CHECK_SYNC_RECEIVER_SEND, m_IsSyncReceiverSend);
}


BEGIN_MESSAGE_MAP(CReceiverView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CReceiverView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CReceiverView::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_SYNC_RECEIVER_RCV, &CReceiverView::OnBnClickedCheckSyncReceiverRcv)
	ON_NOTIFY_RANGE(NM_CUSTOMDRAW, IDC_SLIDER1, IDC_SLIDER8, &CReceiverView::OnNMCustomdrawSlider)
	ON_BN_CLICKED(IDC_CHECK_SYNC_RECEIVER_SEND, &CReceiverView::OnBnClickedCheckSyncReceiverSend)
END_MESSAGE_MAP()


// CReceiverView message handlers


void CReceiverView::OnBnClickedOk()
{
}
void CReceiverView::OnBnClickedCancel()
{
}


void CReceiverView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePaneChildView::OnSize(nType, cx, cy);

}


BOOL CReceiverView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	for (int i = 0; i < 8; ++i)
	{
		m_Slider[i].SetRange(0, 3000);
	}

	return TRUE;
}


void CReceiverView::OnBnClickedCheckSyncReceiverRcv()
{
	UpdateData();

	if (m_IsSyncReceiverRcv)
	{
		// 시리얼포트가 열려 있지않으면, 모터싱크를 하지 않는다.
		if (!cController::Get()->GetSerial().IsOpened())
		{
			m_IsSyncReceiverRcv = FALSE;
			UpdateData(FALSE);
		}
	}

	if (m_IsSyncReceiverRcv)
	{
		m_syncState = 0;
		m_errorCount = 0;
		SetBackgroundColor(g_blueColor);
	}
	else
	{
		SetBackgroundColor(g_grayColor);
	}
}


void CReceiverView::OnBnClickedCheckSyncReceiverSend()
{
}


void CReceiverView::OnNMCustomdrawSlider(UINT Id, NMHDR *pNMHDR, LRESULT *pResult)
{
	const int index = Id - IDC_SLIDER1;
	const int pos = m_Slider[index].GetPos();

	CString str;
	str.Format(L"%d", pos);
	m_Edit[index].SetWindowTextW(str);
}


void CReceiverView::Update(const float deltaSeconds)
{
	RET(!m_IsSyncReceiverRcv);
	RET(!IsWindowVisible());

	m_updateIncTime += deltaSeconds;
	if (m_updateIncTime < 0.03f)
		return;
	m_updateIncTime = 0;

	SyncReceiver();
}


void CReceiverView::SyncReceiver()
{
	RET(!m_IsSyncReceiverRcv);
	const int MAX_ERROR_CNT = 5;

	if (m_syncState == 0)
	{
		SendCommand(cController::Get()->GetSerial(), MSP_RC);
		m_syncState = 1;
	}
	else if (m_syncState == 1)
	{
		unsigned char buffer[16];
		if (RecvCommand(cController::Get()->GetSerial(), MSP_RC, buffer, sizeof(buffer)) > 0)
		{
			for (int i = 0; i < 8; ++i)
			{
				int data = *(short*)&buffer[i*2]; // +- 1800

				CString str;
				str.Format(L"%d", data);
				m_Edit[i].SetWindowTextW(str);
				m_Slider[i].SetPos(data);
			}

			// Rotation
			// roll, pitch 값으로 큐브를 회전시키는 벡터를 생성한다.
			const int roll = *(short*)&buffer[0]; // 988 ~ 1500 ~ 2010
			const int pitch = *(short*)&buffer[2]; // 988 ~ 1500 ~ 2010
			const float rot_x = (float)(roll - 1500);// / (float)(2010 - 988);
			const float rot_y = (float)(pitch - 1500);// / (float)(2010 - 988);
			Vector3 rotation(rot_x, rot_y, 0);

			// Move
			const int up = *(short*)&buffer[6]; // 988 ~ 1500 ~ 2010
			const int right = *(short*)&buffer[4]; // 988 ~ 1500 ~ 2010
			const int forward = *(short*)&buffer[10]; // 988 ~ 1500 ~ 2010
			const float mov_x = (float)(right - 1500);// / (float)(2010 - 988);
			const float mov_y = (float)(up - 1500);// / (float)(2010 - 988);
			const float mov_z = (float)(forward - 1500);// / (float)(2010 - 988);
			Vector3 move(mov_x, mov_y, mov_z);

			cController::Get()->GetCubeFlight().Thrust(-rotation, move);

			m_syncState = 3;
		}
		else
		{
			++m_errorCount;
			if (m_errorCount > MAX_ERROR_CNT)
			{
				m_IsSyncReceiverRcv = FALSE;
				SetBackgroundColor(g_grayColor);
				UpdateData(FALSE);
			}
		}
		
	}
	else if (m_syncState == 3)
	{
		SendCommand(cController::Get()->GetSerial(), MSP_ATTITUDE);
		m_syncState = 4;
	}
	else if (m_syncState == 4)
	{
		BYTE buffer[64];
		const int len = RecvCommand(cController::Get()->GetSerial(), MSP_ATTITUDE, buffer, sizeof(buffer));
		if (len > 0)
		{
			// 자세정보 업데이트
			int roll = *(short*)&buffer[0]; // +- 1800
			int pitch = *(short*)&buffer[2]; // +- 860
			int yaw = *(short*)&buffer[4]; // 0 ~ 360

			Quaternion qr, qp, qy;
			qr.Euler(Vector3(0, 0, -roll*0.1f));
			qp.Euler(Vector3(pitch*0.1f, 0, 0));
			qy.Euler(Vector3(0, (float)yaw, 0));

			Quaternion q = qy * qp * qr;
			m_q = q;
			m_qr = qr;
			m_qp = qp;
			m_qy = qy;
			cController::Get()->GetCubeFlight().m_tm = q.GetMatrix();

			m_errorCount = 0;
			m_syncState = 5;
		}
		else
		{
			++m_errorCount;
			if (m_errorCount > MAX_ERROR_CNT)
			{
				m_IsSyncReceiverRcv = FALSE;
				SetBackgroundColor(g_grayColor);
				UpdateData(FALSE);
			}
		}
	}
	else if (m_syncState == 5)
	{
		SendCommand(cController::Get()->GetSerial(), MSP_CUBE_MOTOR);
		m_syncState = 6;
	}
	else if (m_syncState == 6)
	{
		unsigned char buffer[24];
		if (RecvCommand(cController::Get()->GetSerial(), MSP_CUBE_MOTOR, buffer, sizeof(buffer)) > 0)
		{
			cController::Get()->GetCubeFlight().CubeThrust(buffer);
			m_errorCount = 0;
			m_syncState = 0;
		}
		else
		{
			++m_errorCount;
			if (m_errorCount > MAX_ERROR_CNT)
			{
				m_IsSyncReceiverRcv = FALSE;
				SetBackgroundColor(g_grayColor);
				UpdateData(FALSE);
			}
		}
	}
	else if (m_syncState == 7)
	{
		SendCommand(cController::Get()->GetSerial(), MSP_CUBE_DEBUG);
		m_syncState = 8;
	}
	else if (m_syncState == 8)
	{
		unsigned char buffer[128];
		if (RecvCommand(cController::Get()->GetSerial(), MSP_CUBE_DEBUG, buffer, sizeof(buffer)) > 0)
		{
			Vector3 v;
			int idx = 0;
			v.x = *(float*)&buffer[idx];
			v.y = *(float*)&buffer[idx+=4];
			v.z = *(float*)&buffer[idx += 4];

			const Matrix44 tm = cController::Get()->GetCubeFlight().m_tm *
				cController::Get()->GetCubeFlight().m_offset;
			Vector3 dir = Vector3(0, 0, 1).MultiplyNormal(tm);
			dir.Normalize();

			Quaternion q;
			q.x = *(float*)&buffer[idx+=4];
			q.y = *(float*)&buffer[idx+= 4];
			q.z = *(float*)&buffer[idx += 4];
			q.w = *(float*)&buffer[idx += 4];

			Quaternion qr;
			qr.x = *(float*)&buffer[idx += 4];
			qr.y = *(float*)&buffer[idx += 4];
			qr.z = *(float*)&buffer[idx += 4];
			qr.w = *(float*)&buffer[idx += 4];

			Quaternion qp;
			qp.x = *(float*)&buffer[idx += 4];
			qp.y = *(float*)&buffer[idx += 4];
			qp.z = *(float*)&buffer[idx += 4];
			qp.w = *(float*)&buffer[idx += 4];

			Quaternion qy;
			qy.x = *(float*)&buffer[idx += 4];
			qy.y = *(float*)&buffer[idx += 4];
			qy.z = *(float*)&buffer[idx += 4];
			qy.w = *(float*)&buffer[idx += 4];

			m_errorCount = 0;
			m_syncState = 0;
		}
		else
		{
			++m_errorCount;
			if (m_errorCount > MAX_ERROR_CNT)
			{
				m_IsSyncReceiverRcv = FALSE;
				SetBackgroundColor(g_grayColor);
				UpdateData(FALSE);
			}
		}

	}


}

