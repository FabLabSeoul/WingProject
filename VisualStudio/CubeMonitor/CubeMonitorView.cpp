
// CubeMonitorView.cpp : implementation of the CCubeMonitorView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "CubeMonitor.h"
#endif

#include "CubeMonitorDoc.h"
#include "CubeMonitorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCubeMonitorView

IMPLEMENT_DYNCREATE(CCubeMonitorView, CView)

BEGIN_MESSAGE_MAP(CCubeMonitorView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CCubeMonitorView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CCubeMonitorView construction/destruction

CCubeMonitorView::CCubeMonitorView()
{
	// TODO: add construction code here

}

CCubeMonitorView::~CCubeMonitorView()
{
}

BOOL CCubeMonitorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CCubeMonitorView drawing

void CCubeMonitorView::OnDraw(CDC* /*pDC*/)
{
	CCubeMonitorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CCubeMonitorView printing


void CCubeMonitorView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CCubeMonitorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCubeMonitorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCubeMonitorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CCubeMonitorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCubeMonitorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCubeMonitorView diagnostics

#ifdef _DEBUG
void CCubeMonitorView::AssertValid() const
{
	CView::AssertValid();
}

void CCubeMonitorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCubeMonitorDoc* CCubeMonitorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCubeMonitorDoc)));
	return (CCubeMonitorDoc*)m_pDocument;
}
#endif //_DEBUG


// CCubeMonitorView message handlers
