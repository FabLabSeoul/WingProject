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
}


BEGIN_MESSAGE_MAP(CCube3DView, CDockablePaneBase)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CCube3DView message handlers
int CCube3DView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePaneBase::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_view = new C3DDialog();
	m_view->Create(C3DDialog::IDD, this);
	m_view->ShowWindow(SW_SHOW);
	SetChildView(m_view);

	return 0;
}
