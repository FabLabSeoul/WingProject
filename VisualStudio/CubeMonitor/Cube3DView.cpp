// Cube3DView.cpp : implementation file
//

#include "stdafx.h"
#include "CubeMonitor.h"
#include "Cube3DView.h"
#include "C3DView.h"


// CCube3DView

//IMPLEMENT_DYNAMIC(CCube3DView, CDockablePane)

CCube3DView::CCube3DView()
	: m_view(NULL)
{

}

CCube3DView::~CCube3DView()
{
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

	m_view = new C3DView();
	m_view->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 100,100), this, 10);

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
