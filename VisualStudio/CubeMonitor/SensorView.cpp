// SensorView.cpp : implementation file
//

#include "stdafx.h"
#include "CubeMonitor.h"
#include "SensorView.h"
#include "SensorForm.h"


// CSensorView
CSensorView::CSensorView() 
{

}

CSensorView::~CSensorView()
{
}


BEGIN_MESSAGE_MAP(CSensorView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CSensorView message handlers
int CSensorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_view = new CSensorForm();
	m_view->Create(CSensorForm::IDD, this);
	m_view->ShowWindow(SW_SHOW);
	//m_view.ShowWindow(SW_SHOW);
	//m_view = new CSensorFormView();
	//m_view->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 100, 100), this, 100, NULL);

	return 0;
}


void CSensorView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (m_view && GetSafeHwnd())
	{
		m_view->MoveWindow(CRect(0, 0, cx, cy));
	}
}
