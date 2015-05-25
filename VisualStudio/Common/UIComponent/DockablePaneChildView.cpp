// DockablePaneChildView.cpp : implementation file
//

#include "stdafx.h"
//#include "CubeMonitor.h"
#include "DockablePaneChildView.h"
#include "afxdialogex.h"
#include "controller.h"


// CDockablePaneChildView dialog
CDockablePaneChildView::CDockablePaneChildView(UINT id, CWnd* pParent /*=NULL*/)
	: CDialogEx(id, pParent)
{
}

CDockablePaneChildView::~CDockablePaneChildView()
{
}

void CDockablePaneChildView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDockablePaneChildView, CDialogEx)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDockablePaneChildView message handlers
BOOL CDockablePaneChildView::OnInitDialog()
{
	__super::OnInitDialog();

	cController::Get()->AddUpdateObserver(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CDockablePaneChildView::OnEraseBkgnd(CDC* pDC)
{
	return __super::OnEraseBkgnd(pDC);
	//return TRUE;
}
