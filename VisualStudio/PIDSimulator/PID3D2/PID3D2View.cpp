
// PID3D2View.cpp : implementation of the CPID3D2View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PID3D2.h"
#endif

#include "PID3D2Doc.h"
#include "PID3D2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPID3D2View

IMPLEMENT_DYNCREATE(CPID3D2View, CView)

BEGIN_MESSAGE_MAP(CPID3D2View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPID3D2View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CPID3D2View construction/destruction

CPID3D2View::CPID3D2View()
{
	// TODO: add construction code here

}

CPID3D2View::~CPID3D2View()
{
}

BOOL CPID3D2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CPID3D2View drawing

void CPID3D2View::OnDraw(CDC* /*pDC*/)
{
	CPID3D2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CPID3D2View printing


void CPID3D2View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPID3D2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPID3D2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPID3D2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CPID3D2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPID3D2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPID3D2View diagnostics

#ifdef _DEBUG
void CPID3D2View::AssertValid() const
{
	CView::AssertValid();
}

void CPID3D2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPID3D2Doc* CPID3D2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPID3D2Doc)));
	return (CPID3D2Doc*)m_pDocument;
}
#endif //_DEBUG


// CPID3D2View message handlers
