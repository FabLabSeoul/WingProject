
// SerialMonitor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SerialMonitor.h"
#include "SerialMonitorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSerialMonitorApp

BEGIN_MESSAGE_MAP(CSerialMonitorApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSerialMonitorApp construction

CSerialMonitorApp::CSerialMonitorApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CSerialMonitorApp object

CSerialMonitorApp theApp;


// CSerialMonitorApp initialization

BOOL CSerialMonitorApp::InitInstance()
{
	AfxInitRichEdit2();

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	CShellManager *pShellManager = new CShellManager;
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CSerialMonitorDlg *dlg = new CSerialMonitorDlg();
	m_pMainWnd = dlg;
	dlg->Create(CSerialMonitorDlg::IDD, NULL);
	dlg->ShowWindow(SW_SHOW);
	dlg->MainLoop();


	// 위에서 만든 셸 관리자를 삭제합니다.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	delete dlg;
	return FALSE;
}

