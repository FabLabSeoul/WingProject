
// SerialMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SerialMonitor.h"
#include "SerialMonitorDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include "Serial.h"
#include "GraphDialog.h"
#include "stringfunc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSerialMonitorDlg *g_MainDlg;



// CSerialMonitorDlg dialog
CSerialMonitorDlg::CSerialMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSerialMonitorDlg::IDD, pParent)
	, m_ModeSlect(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_loop = true;
	m_isConnect = false;
	//m_isFastMode = false;
	m_isShowGraphWnd = false;
	g_MainDlg = this;
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
	DDX_Control(pDX, IDC_CHECK_TOPMOST, m_TopMostButton);
	DDX_Control(pDX, IDC_CHECK_SHOW1LINE, m_Show1LineButton);
	DDX_Control(pDX, IDC_STATIC_1, m_Static1);
	DDX_Control(pDX, IDC_STATIC_2, m_Static2);
	DDX_Control(pDX, IDC_BUTTON_CLEAR, m_ClearButton);
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
	ON_BN_CLICKED(IDC_CHECK_TOPMOST, &CSerialMonitorDlg::OnBnClickedCheckTopmost)
	ON_BN_CLICKED(IDC_CHECK_SHOW1LINE, &CSerialMonitorDlg::OnBnClickedCheckShow1line)
	ON_BN_CLICKED(IDC_BUTTON_GRAPH, &CSerialMonitorDlg::OnBnClickedButtonGraph)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_RADIO_MEM, &CSerialMonitorDlg::OnBnClickedRadioMem)
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
	m_Editor.SetBackgroundColor(FALSE, RGB(0, 0, 0));

	m_graphDlg = new CGraphDialog(this);
	m_graphDlg->Create(CGraphDialog::IDD, this);


	m_PortCombobox.InitList();

	int baudRate[] = { 9600, 14400, 19200, 38400, 56000, 57600, 115200 };
	for (int i = 0; i < ARRAYSIZE(baudRate); ++i)
	{
		TCHAR brateName[32];
		wsprintf(brateName, L"%d", baudRate[i]);
		m_BaudrateCombobox.InsertString(m_BaudrateCombobox.GetCount(), brateName);
	}
	m_BaudrateCombobox.SetCurSel(0);


	// ȯ�漳�� ������ ������ �о�´�.
	// portnumber, baudrate, window width, window height �� �����´�.
	std::ifstream iif("serialmonitor_conf.txt");
	if (iif.is_open())
	{
		iif >> m_PortNumber;
		iif >> m_BaudrateIndex;
		m_PortCombobox.InitList(m_PortNumber);
		m_BaudrateCombobox.SetCurSel(m_BaudrateIndex);

		int left, top, width, height;
		iif >> left >> top >> width >> height;
		if ((left > 0) && (top > 0) && (width > 0) && (height > 0))
		{
			CRect rect;
			GetWindowRect(rect);
			rect.left = left;
			rect.top = top;
			rect.right = left + width;
			rect.bottom = top + height;
			MoveWindow(rect);

			// ȭ���� ������ show 1 line ����� �Ǵ��Ѵ�.
			if (height < 80)
			{
				m_Show1LineButton.SetCheck(1);
				OnBnClickedCheckShow1line();
			}


			// Plot Command Editor Text�� �о�´�. ȯ������ ������ �д´�.
			std::string strCmd((std::istreambuf_iterator<char>(iif)), std::istreambuf_iterator<char>());
			common::trim(strCmd);
			if (!strCmd.empty())
			{
				wstring wstrCmd = common::str2wstr(strCmd);
				m_graphDlg->m_CommandEditor.SetWindowText(wstrCmd.c_str());
			}
		}
	}

	// �׷���â�� Plot Command Editor ��ɾ �����Ѵ�.
	m_graphDlg->ProcessPlotCommand();

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
	if (m_graphDlg)
	{
		m_graphDlg->DestroyWindow();
		delete m_graphDlg;
		m_graphDlg = NULL;
	}

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
		const int deltaT = curT - oldT;
		const float t = deltaT * 0.001f;
		oldT = curT;

		Process(t);

		// �׷��� ȭ���� �����Ѵ�.
		if (deltaT > 0)
		{
			if (m_graphDlg)
				m_graphDlg->DrawGraph(t);
		}
		//Sleep(0);
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

	// ������ �ʹ� ������ �����.
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
void CSerialMonitorDlg::Process(float deltaSeconds)
{
	if (!m_isConnect)
	{
		// ��Ʈ�� ����Ǿ� ���� �ʴٸ�, CPU �δ��� ���̱� ���� ���� ������ �Ѿ��.
		Sleep(10);
		return;
	}

	string buff;
	if (!m_Serial.ReadStringUntil('\n', buff))
	{
		// ���� �߻�. �ø��� ��Ʈ ����� ���´�.
		OnBnClickedButtonDisconnect();
		return;
	}

	if (buff.empty())
		return;

	CString str = common::str2wstr(buff).c_str();

	if (MODE_FAST1 == m_mode)
	{
		m_FastReceiveText.SetWindowTextW(str);
			
		// �ø��������� �׷��� â�� �����Ѵ�.
		if (m_isShowGraphWnd)
			m_graphDlg->SetString(buff.c_str());
	}
	else
	{
		if (MODE_MEMORY == m_mode)
		{
			//CString numStr;
			string numStr;
			numStr.reserve(str.GetLength() * 8);
			for (int i = 0; i < str.GetLength(); ++i)
			{
				numStr += common::format("%3d", (int)str[i]);
				if (i != str.GetLength() - 1)
				{
					numStr += ", ";
				}
			}
			str = numStr.c_str();
			str += "\n";
		}

		// �Ϲ� ��� ���.

		// ���๮�ڰ� �� ����, ���� ������ ���� ���� ������Ʈ �ȴ�.
		AppendToLogAndScroll(str, RGB(200, 200, 200));

		// �ø��� ������ �׷��� â�� �����Ѵ�.
		if (m_isShowGraphWnd)
			m_graphDlg->SetString(buff.c_str());
	}

	++m_RxCounter;
	SetWindowTextW(common::formatw("SerialMonitor [COM %d] Rx=%d", m_PortNumber, m_RxCounter).c_str());
}


// Connect Serial port
void CSerialMonitorDlg::OnBnClickedButtonConnect()
{
	if (m_isConnect)
		return;

	const int portNumber = m_PortCombobox.GetPortNum();
	CString baudRate;
	m_BaudrateCombobox.GetWindowTextW(baudRate);

	// Open Serial Port
	if (m_Serial.Open(portNumber, _wtoi(baudRate)))
	{
		m_isConnect = true;
		m_RxCounter = 0;
		m_ConnectButton.EnableWindow(FALSE);
		m_DisconnectButton.EnableWindow(TRUE);
		m_PortNumber = portNumber;
		m_BaudrateIndex = m_BaudrateCombobox.GetCurSel();
	}
	else
	{
		CString portName;
		m_PortCombobox.GetWindowTextW(portName);
		AfxMessageBox(L"Connect Error!! [" + portName + L"]");
	}
}


void CSerialMonitorDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_Editor.GetSafeHwnd())
	{
		CRect rect;
		m_Editor.GetWindowRect(rect);
		
		ScreenToClient(rect);

		rect.right = rect.left + cx - 20;
		rect.bottom = cy - 13;
		m_Editor.MoveWindow(rect);
	}	
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
	//m_isFastMode = false;
	m_mode = MODE_NORMAL;
}


void CSerialMonitorDlg::OnBnClickedRadio2()
{
	//m_isFastMode = true;
	m_mode = MODE_FAST1;
}

void CSerialMonitorDlg::OnBnClickedRadioMem()
{
	m_mode = MODE_MEMORY;
}



void CSerialMonitorDlg::OnBnClickedCheckTopmost()
{
	const bool isTopmost = m_TopMostButton.GetCheck() > 0;
	::SetWindowPos(GetSafeHwnd(),
		isTopmost ? HWND_TOPMOST : HWND_NOTOPMOST,
		0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSIZE);
}


void CSerialMonitorDlg::OnBnClickedCheckShow1line()
{
	static CRect oldCheckBoxPos(0,0,0,0);
	static CRect oldReceiveStaticPos(0, 0, 0, 0);
	if (oldCheckBoxPos.IsRectEmpty())
	{
		// ���� ��ġ�� �����Ѵ�.
		m_Show1LineButton.GetWindowRect(oldCheckBoxPos);
		m_FastReceiveText.GetWindowRect(oldReceiveStaticPos);
		ScreenToClient(oldCheckBoxPos);
		ScreenToClient(oldReceiveStaticPos);
	}

	const bool isShow1Line = m_Show1LineButton.GetCheck() > 0;
	if (isShow1Line)
	{
		// üũ�ڽ�, �ؽ�Ʈ ��ġ �̵�
		m_Show1LineButton.MoveWindow(CRect(5, 0, 5+oldCheckBoxPos.Width(), oldCheckBoxPos.Height()));		
		m_FastReceiveText.MoveWindow(CRect(5, 18, 5+oldReceiveStaticPos.Width(), 18+oldReceiveStaticPos.Height()));
	}
	else
	{
		// üũ�ڽ�, �ؽ�Ʈ ��ġ �̵�
		m_Show1LineButton.MoveWindow(oldCheckBoxPos);
		m_FastReceiveText.MoveWindow(oldReceiveStaticPos);
	}

	if (isShow1Line)
	{
		// ��� ���� ��ư�� Fast mode�� �ٲ۴�.
		//m_isFastMode = true;
		m_mode = MODE_FAST1;
		CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
	}

	const DWORD otherControlsShowFlag = isShow1Line ? SW_HIDE : SW_SHOW;
	m_PortCombobox.ShowWindow(otherControlsShowFlag);
	m_BaudrateCombobox.ShowWindow(otherControlsShowFlag);
	m_ConnectButton.ShowWindow(otherControlsShowFlag); 
	m_DisconnectButton.ShowWindow(otherControlsShowFlag);
	m_ClearButton.ShowWindow(otherControlsShowFlag);

	m_Static1.ShowWindow(otherControlsShowFlag);
	m_Static2.ShowWindow(otherControlsShowFlag);


	// ȭ�� ũ�⸦ �ٲ۴�.
	static CRect oldWindowPos(0, 0, 0, 0);
	if (isShow1Line)
	{
		// ȭ�� ����� ���� ���� �ٴ�(bottom)�� �������� ���δ�.
		GetWindowRect(oldWindowPos);
		MoveWindow(CRect(oldWindowPos.left, oldWindowPos.bottom-75, oldWindowPos.right,
			oldWindowPos.bottom));

		// ���� âũ�Ⱑ �۰� �Ǿ��ٸ�, ���Ƿ� ���̸� �ø���.
		if (oldWindowPos.Height() < 80)
		{
			// �ٴ��� �������� �ø���.
			oldWindowPos.top = oldWindowPos.bottom - 200;
		}
	}
	else
	{
		// ���� âũ��� �ǵ�����.
		MoveWindow(oldWindowPos);
	}
}


void CSerialMonitorDlg::OnBnClickedButtonGraph()
{
	if (m_graphDlg)
	{
		m_graphDlg->ShowWindow(SW_SHOW);
		m_isShowGraphWnd = true;
	}
}


// â�� ���� ��, ȣ��ȴ�. �޸𸮰� ���ŵǱ� ��.
void CSerialMonitorDlg::OnClose()
{
	SaveConfigFile();

	CDialogEx::OnClose();
}


void CSerialMonitorDlg::SaveConfigFile()
{
	// ȯ�漳�� ���Ͽ� ����� ����� �����Ѵ�.
	// portnumber, baudrate, window width, window height, plot command script
	// �� �����Ѵ�.
	std::ofstream of("serialmonitor_conf.txt");
	if (of.is_open())
	{
		CRect rect;
		GetWindowRect(rect);

		of << m_PortCombobox.GetPortNum() << std::endl << m_BaudrateCombobox.GetCurSel() << std::endl;
		of << rect.left << std::endl << rect.top << std::endl;
		of << rect.Width() << std::endl << rect.Height() << std::endl;

		// Plot Command Editor Text ����
		CString cmdText;
		m_graphDlg->m_CommandEditor.GetWindowText(cmdText);
		string strCmd = common::wstr2str((LPCTSTR)cmdText);
		of << strCmd.c_str() << std::endl;
	}
}


