// Cube3DView.cpp : implementation file
//

#include "stdafx.h"
#include "CubeMonitor.h"
#include "Cube3DPane.h"


// CCube3DView
CCube3DPane::CCube3DPane()
{
}

CCube3DPane::~CCube3DPane()
{
}


BEGIN_MESSAGE_MAP(CCube3DPane, CDockablePaneBase)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CCube3DView message handlers
int CCube3DPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePaneBase::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_view = new C3DView();
	m_view->Create(IDD_DIALOG_3D, this);
	m_view->ShowWindow(SW_SHOW);
	SetChildView(m_view);

	return 0;
}
