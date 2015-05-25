// SensorView.cpp : implementation file
//

#include "stdafx.h"
#include "CubeMonitor.h"
#include "SensorPane.h"
#include "SensorView.h"


// CSensorView
CSensorPane::CSensorPane() 
{

}

CSensorPane::~CSensorPane()
{
}


BEGIN_MESSAGE_MAP(CSensorPane, CDockablePaneBase)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CSensorView message handlers
int CSensorPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePaneBase::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_view = new CSensorView();
	m_view->Create(CSensorView::IDD, this);
	m_view->ShowWindow(SW_SHOW);
	SetChildView(m_view);

	return 0;
}

