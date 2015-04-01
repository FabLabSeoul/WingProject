// Cube3DView.cpp : implementation file
//

#include "stdafx.h"
#include "CubeMonitor.h"
#include "Cube3DView.h"
#include "C3DDialog.h"


// CCube3DView
CCube3DView::CCube3DView()
{

}

CCube3DView::~CCube3DView()
{
	SAFE_DELETE(m_view);
}


BEGIN_MESSAGE_MAP(CCube3DView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CCube3DView message handlers
int CCube3DView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_view = new C3DDialog();
	m_view->Create(C3DDialog::IDD, this);
	m_view->ShowWindow(SW_SHOW);

	return 0;
}


void CCube3DView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (m_view && GetSafeHwnd())
	{
		m_view->MoveWindow(CRect(0, 0, cx, cy));
	}
}

