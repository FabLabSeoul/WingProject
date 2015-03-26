
// SerialMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SerialMonitor.h"
#include "SerialMonitorDlg.h"
#include "afxdialogex.h"
#include <MMSystem.h>
#include <fstream>
#include "Serial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CSerialMonitorDlg dialog
CSerialMonitorDlg::CSerialMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSerialMonitorDlg::IDD, pParent)
	, m_FastModeButton(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_loop = true;
	m_isConnect = false;
	m_isFastMode = false;
}

void CSerialMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PORT, m_PortCombobox);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_BaudrateCombobox);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_ConnectButton);
	DDX_Control(pDX, ID_EDIT_SERIAL, m_Editor);
	DDX_Control(pDX, IDC_BUTTON_DISCONNECT, m_DisconnectButton);
	DDX_Control(pDX, IDC_CHECK_AUTOSCROLL, m_AutoScrollButton);
	DDX_Control(pDX, IDC_STATIC_FAST_TEXT, m_FastReceiveText);
	DDX_Control(pDX, IDC_RADIO1, m_NormalModeButton);
}

BEGIN_MESSAGE_MAP(CSerialMonitorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSerialMonitorDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSerialMonitorDlg::OnBnClickedCancel)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CSerialMonitorDlg::OnBnClickedButtonConnect)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_AUTOSCROLL, &CSerialMonitorDlg::OnBnClickedCheckAutoscroll)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CSerialMonitorDlg::OnBnClickedButtonDisconnect)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CSerialMonitorDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_RADIO1, &CSerialMonitorDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CSerialMonitorDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CSerialMonitorDlg message handlers

BOOL CSerialMonitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_DisconnectButton.EnableWindow(FALSE);
	m_AutoScrollButton.SetCheck(1);
	m_NormalModeButton.SetCheck(1);


	for (int i = 0; i < 20; ++i)
	{
		TCHAR comName[32];
		wsprintf(comName, L"COM %d", i + 1);
		m_PortCombobox.InsertString(m_PortCombobox.GetCount(), comName);
	}
	m_PortCombobox.SetCurSel(0);

	int baudRate[] = {9600, 14400, 19200, 38400, 56000, 57600, 115200};
	for (int i = 0; i < ARRAYSIZE(baudRate); ++i)
	{
		TCHAR brateName[32];
		wsprintf(brateName, L"%d", baudRate[i]);
		m_BaudrateCombobox.InsertString(m_BaudrateCombobox.GetCount(), brateName);
	}
	m_BaudrateCombobox.SetCurSel(0);

	// 환경설정 파일이 있으면 읽어온다.
	// portnumber, baudrate를 저장한다.
	std::ifstream iif("serialmonitor_conf.txt");
	if (iif.is_open())
	{
		int portNumberIndex, baudRateIndex;
		iif >> portNumberIndex;
		iif >> baudRateIndex;
		m_PortCombobox.SetCurSel(portNumberIndex);
		m_BaudrateCombobox.SetCurSel(baudRateIndex);
	}


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSerialMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSerialMonitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSerialMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSerialMonitorDlg::OnBnClickedOk()
{
//	CDialogEx::OnOK();
}


void CSerialMonitorDlg::OnBnClickedCancel()
{
	m_loop = false;
	CDialogEx::OnCancel();
}


BOOL CSerialMonitorDlg::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	return CDialogEx::OnWndMsg(message, wParam, lParam, pResult);
}



void CSerialMonitorDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
}


// WindowProcedure MainLoop
void CSerialMonitorDlg::MainLoop()
{
	while (m_loop)
	{
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0))
			{
				break;
			}
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		const int curT = timeGetTime();
		static int oldT = curT;
		const int elapseT = curT - oldT;
		const float t = elapseT * 0.001f;
		oldT = curT;

		Process(t);

		Sleep(0);
	}
}


// http://www.codeproject.com/Articles/12093/Using-RichEditCtrl-to-Display-Formatted-Logs
int CSerialMonitorDlg::AppendToLogAndScroll(CString str, COLORREF color)
{
	long nVisible = 0;
	long nInsertionPoint = 0;
	CHARFORMAT cf;

	// Initialize character format structure
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.dwEffects = 0; // To disable CFE_AUTOCOLOR

	cf.crTextColor = color;

	// Set insertion point to end of text
	nInsertionPoint = m_Editor.GetWindowTextLength();
	m_Editor.SetSel(nInsertionPoint, -1);

	// Set the character format
	m_Editor.SetSelectionCharFormat(cf);

	// Replace selection. Because we have nothing
	// selected, this will simply insert
	// the string at the current caret position.
	m_Editor.ReplaceSel(str);

	// Get number of currently visible lines or maximum number of visible lines
	// (We must call GetNumVisibleLines() before the first call to LineScroll()!)
	nVisible = GetNumVisibleLines(&m_Editor);

	// Now this is the fix of CRichEditCtrl's abnormal behaviour when used
	// in an application not based on dialogs. Checking the focus prevents
	// us from scrolling when the CRichEditCtrl does so automatically,
	// even though ES_AUTOxSCROLL style is NOT set.
	if (m_AutoScrollButton.GetCheck() && &m_Editor != m_Editor.GetFocus())
	{
		m_Editor.LineScroll(INT_MAX);
		m_Editor.LineScroll(1 - nVisible);
	}

	// 내용이 너무 많으면 지운다.
	const int maximumLine = 100;
	if (m_Editor.GetLineCount() > maximumLine)
	{
		long nFirstChar = m_Editor.CharFromPos(CPoint(0, 0));
		m_Editor.SetSel(0, nFirstChar);
		m_Editor.ReplaceSel(L"");
	}

	return 0;
}


//http://www.codeproject.com/Articles/12093/Using-RichEditCtrl-to-Display-Formatted-Logs
int CSerialMonitorDlg::GetNumVisibleLines(CRichEditCtrl* pCtrl)
{
	CRect rect;
	long nFirstChar, nLastChar;
	long nFirstLine, nLastLine;

	// Get client rect of rich edit control
	pCtrl->GetClientRect(rect);

	// Get character index close to upper left corner
	nFirstChar = pCtrl->CharFromPos(CPoint(0, 0));

	// Get character index close to lower right corner
	nLastChar = pCtrl->CharFromPos(CPoint(rect.right, rect.bottom));
	if (nLastChar < 0)
	{
		nLastChar = pCtrl->GetTextLength();
	}

	// Convert to lines
	nFirstLine = pCtrl->LineFromChar(nFirstChar);
	nLastLine = pCtrl->LineFromChar(nLastChar);

	return (nLastLine - nFirstLine);
}


// Serial Transmitt/Receive Process
void CSerialMonitorDlg::Process(float deltaT)
{
	if (!m_isConnect)
		return;

	TCHAR data = 0;
	if (0 >= m_Serial.ReadData(&data, 1))
		return;

	if (m_isFastMode)
	{
		static CString str;
		str += data;
		// CR Return 일 때만 업데이트 된다.
		if (data == '\n')
		{
			m_FastReceiveText.SetWindowTextW(str);
			str = "";
		}
	}
	else
	{
		CString str;
		str += data;
		AppendToLogAndScroll(str, RGB(0,0,0));
	}

}


// Connect Serial port
void CSerialMonitorDlg::OnBnClickedButtonConnect()
{
	if (m_isConnect)
		return;

	CString port;
	m_PortCombobox.GetWindowTextW(port);
	CString baudRate;
	m_BaudrateCombobox.GetWindowTextW(baudRate);

	int portNumber = 0;
	const int spaceIdx = port.Find(L" ");
	if (spaceIdx > 0)
		portNumber = _wtoi(&port.GetBuffer()[spaceIdx]);

	// Open Serial Port
	if (m_Serial.Open(portNumber, _wtoi(baudRate)))
	{
		m_isConnect = true;
		m_ConnectButton.EnableWindow(FALSE);
		m_DisconnectButton.EnableWindow(TRUE);

		// portnumber, baudrate를 저장한다.
		std::ofstream of("serialmonitor_conf.txt");
		if (of.is_open())
		{
			of << m_PortCombobox.GetCurSel() << std::endl << m_BaudrateCombobox.GetCurSel();
		}
	}
	else
	{
		AfxMessageBox(L"Connect Error!! [" + port + L"]");
	}
}


void CSerialMonitorDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	
}


void CSerialMonitorDlg::OnBnClickedCheckAutoscroll()
{

}


void CSerialMonitorDlg::OnBnClickedButtonDisconnect()
{
	if (m_isConnect)
	{
		m_isConnect = false;
		m_Serial.Close();
		m_ConnectButton.EnableWindow(TRUE);
		m_DisconnectButton.EnableWindow(FALSE);
	}
}


void CSerialMonitorDlg::OnBnClickedButtonClear()
{
	m_Editor.SetSel(0, m_Editor.GetTextLength());
	m_Editor.ReplaceSel(L"");
}


void CSerialMonitorDlg::OnBnClickedRadio1()
{
	m_isFastMode = false;
}


void CSerialMonitorDlg::OnBnClickedRadio2()
{
	m_isFastMode = true;
}
