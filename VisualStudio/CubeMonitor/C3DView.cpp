// C3DView.cpp : implementation file
//

#include "stdafx.h"
#include "CubeMonitor.h"
#include "C3DView.h"


// C3DView
C3DView::C3DView()
{

}

C3DView::~C3DView()
{
}

BEGIN_MESSAGE_MAP(C3DView, CView)
END_MESSAGE_MAP()


// C3DView drawing

void C3DView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// C3DView diagnostics

#ifdef _DEBUG
void C3DView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void C3DView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// C3DView message handlers
