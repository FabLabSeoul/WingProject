
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "CubeMonitor.h"

#include "MainFrm.h"
#include "ConnectionDialog.h"
#include "SerialEditorView.h"
#include "SerialGraphForm.h"
#include "CubeSerialView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define CREATE_DOCKPANE(CLASS, DOCKNAME, PANE_ID, VAR)\
{\
	CDockablePaneBase *pane = new CDockablePaneBase();\
	if (!pane->Create(DOCKNAME, this, CRect(0, 0, 460, 500), TRUE, PANE_ID, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))\
				{\
		TRACE0("Failed to create pane window\n");\
		return FALSE;\
				}\
	VAR = new CLASS(pane);\
	VAR->Create(CLASS::IDD, pane);\
	VAR->ShowWindow(SW_SHOW);\
	pane->SetChildView(VAR);\
	m_viewList.push_back(pane);\
	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(theApp.m_bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);\
	pane->SetIcon(hClassViewIcon, FALSE);\
}




// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
	// 모든 뷰들을 제거한다.
	for each (auto view in m_viewList)
	{
		SAFE_DELETE(view);
	}

	cController::Release();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	//GetDockingManager()->DisableRestoreDockState();
	//CDockingManager::SetDockingMode(DT_IMMEDIATE);

	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	//{
	//	TRACE0("Failed to create toolbar\n");
	//	return -1;      // fail to create
	//}

	//CString strToolBarName;
	//bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	//ASSERT(bNameValid);
	//m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	//ASSERT(bNameValid);
	//m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Allow user-defined toolbars operations:
	//InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

// 	if (!m_wndStatusBar.Create(this))
// 	{
// 		TRACE0("Failed to create status bar\n");
// 		return -1;      // fail to create
// 	}
// 	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	
	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
//	DockPane(&m_wndToolBar);


	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Load menu item image (not placed on any standard toolbars):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// create docking windows
	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

	
	for each (auto &view in m_viewList)
		view->EnableDocking(CBRS_ALIGN_ANY);

	DockPane(m_wndCube3DView);
	//DockPane(m_wndFileView);
	CDockablePane* pTabbedBar = NULL;
	for each (auto &view in m_viewList)
	{
		if (view != m_wndCube3DView)
			view->AttachToTabWnd(m_wndCube3DView, DM_SHOW, TRUE, &pTabbedBar);
	}


	//m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndOutput);
	//m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndProperties);

	// set the visual manager and style based on persisted value
	//OnApplicationLook(theApp.m_nAppLook);
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
	CDockingManager::SetDockingMode(DT_SMART);
//	m_wndOutput.UpdateFonts();
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);
	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
	//


	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// load user-defined toolbar images
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}


	/*
	// enable menu personalization (most-recently used commands)
	// TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);
	*/

// 	CConnectionDialog dlg;
// 	dlg.DoModal();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// Create class view
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);


	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(theApp.m_bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);


	//m_wndClassView = new CClassView();
	//if (!m_wndClassView->Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("Failed to create Class View window\n");
	//	return FALSE; // failed to create
	//}

	// Create file view
	//CString strFileView;
	//bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	//ASSERT(bNameValid);
	//m_wndFileView = new CFileView();
	//if (!m_wndFileView->Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("Failed to create File View window\n");
	//	return FALSE; // failed to create
	//}

	// Create cube3d view
	m_wndCube3DView = new CCube3DPane();
	if (!m_wndCube3DView->Create(L"Cube3DView", this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CUBE3D, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create cube 3d View window\n");
		return FALSE; // failed to create
	}
	m_viewList.push_back(m_wndCube3DView);

	CREATE_DOCKPANE(CCubeSerialView, L"Cube Serial View", ID_VIEW_CUBE_SERIAL, m_cubeSerialView);



// 
// 
// 	// Create sensor view
// 	m_wndSensorView = new CSensorPane();
// 	if (!m_wndSensorView->Create(L"SensorView", this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_SENSOR, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
// 	{
// 		TRACE0("Failed to create cube 3d View window\n");
// 		return FALSE; // failed to create
// 	}
// 
// 	// Create serial editor view
// 	{
// 		m_wndSerialEditorView = new CDockablePaneBase();
// 		if (!m_wndSerialEditorView->Create(L"SerialEditorView", this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_SERIAL_EDITOR, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
// 		{
// 			TRACE0("Failed to create SerialEditor View window\n");
// 			return FALSE; // failed to create
// 		}
// 
// 		CSerialEditorView *view = new CSerialEditorView(m_wndSerialEditorView);
// 		view->Create(CSerialEditorView::IDD, m_wndSerialEditorView);
// 		view->ShowWindow(SW_SHOW);
// 		m_wndSerialEditorView->SetChildView(view);
// 	}
// 
// 
// 	// Create serial graph view
// 	{
// 		m_serialGraphView = new CDockablePaneBase();
// 		if (!m_serialGraphView->Create(L"SerialGraphView", this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_SERIAL_GRAPH, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
// 		{
// 			TRACE0("Failed to create SerialGraph View window\n");
// 			return FALSE; // failed to create
// 		}
// 
// 		CSerialGraphForm *view = new CSerialGraphForm(m_serialGraphView);
// 		view->Create(CSerialGraphForm::IDD, m_serialGraphView);
// 		view->ShowWindow(SW_SHOW);
// 		m_serialGraphView->SetChildView(view);
// 	}
// 

	//m_viewList.push_back(m_wndClassView);
	//m_viewList.push_back(m_wndFileView);
// 	m_viewList.push_back(m_wndSensorView);
// 	m_viewList.push_back(m_wndSerialEditorView);
// 	m_viewList.push_back(m_serialGraphView);


	//// Create output window
	//CString strOutputWnd;
	//bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	//ASSERT(bNameValid);
	//if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("Failed to create Output window\n");
	//	return FALSE; // failed to create
	//}

	//// Create properties window
	//CString strPropertiesWnd;
	//bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	//ASSERT(bNameValid);
	//if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("Failed to create Properties window\n");
	//	return FALSE; // failed to create
	//}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndFileView->SetIcon(hFileViewIcon, FALSE);

	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndClassView->SetIcon(hClassViewIcon, FALSE);

	HICON hCube3DViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndCube3DView->SetIcon(hCube3DViewIcon, FALSE);
// 	m_wndSensorView->SetIcon(hCube3DViewIcon, FALSE);
// 	m_wndSerialEditorView->SetIcon(hCube3DViewIcon, FALSE);
// 	m_serialGraphView->SetIcon(hCube3DViewIcon, FALSE);

	//HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	//HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	//CWaitCursor wait;

	//theApp.m_nAppLook = id;

	//switch (theApp.m_nAppLook)
	//{
	//case ID_VIEW_APPLOOK_WIN_2000:
	//	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
	//	break;

	//case ID_VIEW_APPLOOK_OFF_XP:
	//	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
	//	break;

	//case ID_VIEW_APPLOOK_WIN_XP:
	//	CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
	//	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
	//	break;

	//case ID_VIEW_APPLOOK_OFF_2003:
	//	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
	//	CDockingManager::SetDockingMode(DT_SMART);
	//	break;

	//case ID_VIEW_APPLOOK_VS_2005:
	//	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
	//	CDockingManager::SetDockingMode(DT_SMART);
	//	break;

	//case ID_VIEW_APPLOOK_VS_2008:
	//	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
	//	CDockingManager::SetDockingMode(DT_SMART);
	//	break;

	//case ID_VIEW_APPLOOK_WINDOWS_7:
	//	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
	//	CDockingManager::SetDockingMode(DT_SMART);
	//	break;

	//default:
	//	switch (theApp.m_nAppLook)
	//	{
	//	case ID_VIEW_APPLOOK_OFF_2007_BLUE:
	//		CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
	//		break;

	//	case ID_VIEW_APPLOOK_OFF_2007_BLACK:
	//		CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
	//		break;

	//	case ID_VIEW_APPLOOK_OFF_2007_SILVER:
	//		CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
	//		break;

	//	case ID_VIEW_APPLOOK_OFF_2007_AQUA:
	//		CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
	//		break;
	//	}

	//	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
	//	CDockingManager::SetDockingMode(DT_SMART);
	//}

	//m_wndOutput.UpdateFonts();
	//RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	//theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// base class does the real work

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	//BOOL isDocked = FALSE;
	//CPane *parent = m_wndFileView.DockPaneStandard(isDocked);
	//m_wndClassView.UndockPane();
	//m_wndFileView.UndockPane();
	//m_wndClassView.ShowPane(TRUE, FALSE, TRUE);
	//m_wndFileView.ShowPane(TRUE, FALSE, TRUE);
	////m_wndFileView.MoveWindow(CRect(0, 0, 100, 100));
	//m_wndClassView.DockToFrameWindow(CBRS_ALIGN_LEFT, CRect(0, 0, 500, 500));
	//CDockablePane* pTabbedBar = NULL;
	//m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);

	return TRUE;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
	//m_wndOutput.UpdateFonts();
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWndEx::OnSize(nType, cx, cy);

	if (GetSafeHwnd() != NULL)
	{
		if (SIZE_MAXIMIZED == nType)
		{
			int a = 0;
		}
	}
}
