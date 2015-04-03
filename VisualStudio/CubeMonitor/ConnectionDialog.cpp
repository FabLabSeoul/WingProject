// ConnectionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CubeMonitor.h"
#include "ConnectionDialog.h"
#include "afxdialogex.h"
#include "controller.h"


// CConnectionDialog dialog

IMPLEMENT_DYNAMIC(CConnectionDialog, CDialogEx)

CConnectionDialog::CConnectionDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConnectionDialog::IDD, pParent)
{

}

CConnectionDialog::~CConnectionDialog()
{
}

void CConnectionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PORT, m_PortCombobox);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_BaudrateCombobox);
}


BEGIN_MESSAGE_MAP(CConnectionDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CConnectionDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CConnectionDialog::OnBnClickedCancel)
	ON_BN_CLICKED(ID_CONNECT, &CConnectionDialog::OnBnClickedConnect)
END_MESSAGE_MAP()


// CConnectionDialog message handlers

BOOL CConnectionDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_PortCombobox.InitList();

	int baudRate[] = { 9600, 14400, 19200, 38400, 56000, 57600, 115200 };
	for (int i = 0; i < ARRAYSIZE(baudRate); ++i)
	{
		TCHAR brateName[32];
		wsprintf(brateName, L"%d", baudRate[i]);
		m_BaudrateCombobox.InsertString(m_BaudrateCombobox.GetCount(), brateName);
	}
	m_BaudrateCombobox.SetCurSel(0);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CConnectionDialog::OnBnClickedOk()
{
	//CDialogEx::OnOK();
}


void CConnectionDialog::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}


// 큐브와 시리얼포트로 연결한다.
void CConnectionDialog::OnBnClickedConnect()
{
	CString baudRate;
	m_BaudrateCombobox.GetWindowTextW(baudRate);

	if (cController::Get()->ConnectSerial(m_PortCombobox.GetPortNum(), _wtoi(baudRate)))
	{
		EndDialog(0);
	}
	else
	{
		CString portName;
		m_PortCombobox.GetWindowTextW(portName);
		AfxMessageBox(L"Connect Error!! [" + portName + L"]");
	}
}

