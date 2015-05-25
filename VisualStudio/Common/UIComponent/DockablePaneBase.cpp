// DockablePaneBase.cpp : implementation file
//

#include "stdafx.h"
//#include "CubeMonitor.h"
#include "DockablePaneBase.h"
#include "DockablePaneChildView.h"
#include "controller.h"
#include "SerialCommunication.h"


// CDockablePaneBase
CDockablePaneBase::CDockablePaneBase() :
m_dlg(NULL)
{
}

CDockablePaneBase::CDockablePaneBase(CDockablePaneChildView *childView, UINT resourceID)
	: m_dlg(childView)
	, m_dlgResourceID(resourceID)
{
}

CDockablePaneBase::~CDockablePaneBase()
{
	SAFE_DELETE(m_dlg);
}


BEGIN_MESSAGE_MAP(CDockablePaneBase, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_MOUSEACTIVATE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDockablePaneBase message handlers

int CDockablePaneBase::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	cController::Get()->GetSerialComm().AddObserver(this);

	if (m_dlg)
	{
		m_dlg->Create(m_dlgResourceID, this);
		m_dlg->ShowWindow(SW_SHOW);
	}

	return 0;
}


// 윈도우 크기가 바뀌면, 자식 뷰도 같이 바뀐다.
void CDockablePaneBase::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (m_dlg && GetSafeHwnd())
	{
		m_dlg->MoveWindow(CRect(0, 0, cx, cy));
	}
}


// 메뉴창 안뜨게 함.
void CDockablePaneBase::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// nothing~
}


// 시리얼을 통해 정보가 넘어오면 호출된다.
void CDockablePaneBase::UpdateSerial(char *buffer)
{
	if (m_dlg)
		m_dlg->UpdateSerial(buffer);
}


// CDockablePane 자식으로 CView가 있을 때, OnMouseActivate() 함수에서 죽는문제를 해결하기 위한 코드다.
// https://social.msdn.microsoft.com/Forums/en-US/e496c4da-221e-4c12-88d0-8b351a3a8986/cdockablepane-assertion-error-on-onmouseactivate?forum=vcgeneral
int CDockablePaneBase::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	CFrameWnd* pParentFrame = GetParentFrame();
	if ((pParentFrame == pDesktopWnd) || (pDesktopWnd->IsChild(pParentFrame)))
	{
		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
	return MA_NOACTIVATE;
}


BOOL CDockablePaneBase::OnEraseBkgnd(CDC* pDC)
{
	//return __super::OnEraseBkgnd(pDC);
	return TRUE;
}
