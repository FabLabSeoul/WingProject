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


BEGIN_MESSAGE_MAP(CSensorView, CDockablePaneBase)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CSensorView message handlers
int CSensorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePaneBase::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_view = new CSensorForm();
	m_view->Create(CSensorForm::IDD, this);
	m_view->ShowWindow(SW_SHOW);
	SetChildView(m_view);

	return 0;
}

