// C3DPane.cpp : implementation file
//

#include "stdafx.h"
#include "PID3D2.h"
#include "C3DPane.h"
#include "C3DPhysxPIDView.h"


// C3DPane
C3DPane::C3DPane()
{
}

C3DPane::~C3DPane()
{
}


BEGIN_MESSAGE_MAP(C3DPane, CDockablePaneBase)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// C3DPane message handlers

// CCube3DView message handlers
int C3DPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePaneBase::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_view = new C3DPhysxPIDView();
	m_view->Create(IDD_DIALOG_TEMP, this);
	m_view->ShowWindow(SW_SHOW);
	SetChildView(m_view);

	return 0;
}
