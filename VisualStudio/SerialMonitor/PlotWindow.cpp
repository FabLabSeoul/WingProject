// PlotWindow.cpp : implementation file
//

#include "stdafx.h"
#include "SerialMonitor.h"
#include "PlotWindow.h"


// CPlotWindow
CPlotWindow::CPlotWindow()
{
	m_headIdx = 0;
	m_tailIdx = 0;

}

CPlotWindow::~CPlotWindow()
{
}


BEGIN_MESSAGE_MAP(CPlotWindow, CScrollView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CPlotWindow drawing

void CPlotWindow::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CPlotWindow::OnDraw(CDC* pDC)
{
	//CDocument* pDoc = GetDocument();
	// TODO: add draw code here

	CRect cr;
	GetClientRect(cr);


	CString strMaxY, strMinY, strCenterY;
	strMaxY.Format(L"%f", m_maxY);
	strMinY.Format(L"%f", m_minY);
	strCenterY.Format(L"%f", (m_maxY+m_minY)/2.f);
	pDC->TextOutW(5, 0, strMaxY);
	pDC->TextOutW(5, cr.Height()-20, strMinY);
	pDC->TextOutW(5, cr.Height()/2 - 20, strCenterY);

	m_scaleY = (float)cr.Height() / (m_maxY - m_minY);
	m_scaleX = (float)(cr.Width()-50) / (float)m_plot.size();

	// draw x,y axis
	pDC->MoveTo(CPoint(0, cr.Height() / 2));
	pDC->LineTo(CPoint(cr.Width(), cr.Height() / 2));

	// draw graph
	const int h = cr.Height();
	float x = 0;
	int center = cr.Height() / 2;
	pDC->MoveTo(CPoint(0, cr.Height() / 2));
	for (int i = m_headIdx; i != m_tailIdx; i = ++i % m_plot.size())
	{
		int y = h - (int)((m_plot[i].second- m_minY) * m_scaleY);
		pDC->LineTo(CPoint((int)x, y));
		x += m_scaleX;
	}
}


// CPlotWindow diagnostics

#ifdef _DEBUG
void CPlotWindow::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CPlotWindow::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPlotWindow message handlers
bool CPlotWindow::SetPlot(const float x_range, const float y_range,
	const float x_visble_range, const float y_visible_range, const DWORD flags)
{
	m_xRange = x_range;
	m_yRange = y_range;
	m_xVisibleRange = x_visble_range;
	m_yVisibleRange = y_visible_range;
	m_flags = flags;

	//m_plot.clear();
	m_plot.resize(512, pair<float,float>(0.f,0.f));

	m_maxX = -FLT_MAX;
	m_minX = FLT_MAX;
	m_maxY = -FLT_MAX;
	m_minY = FLT_MAX;
	m_scaleY = 1.f;

	return true;
}



void CPlotWindow::SetPlotXY(const float x, const float y)
{
	// 그래프 정보 추가.
	m_plot[m_tailIdx] = pair<float, float>(x, y);
	m_tailIdx = ++m_tailIdx % m_plot.size();
	if (m_headIdx == m_tailIdx)
		m_headIdx = ++m_headIdx % m_plot.size();

	m_maxX = max(x, m_maxX);
	m_minX = min(x, m_minX);
	m_maxY = max(y, m_maxY);
	m_minY = min(y, m_minY);

	InvalidateRect(NULL);
}


void CPlotWindow::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);
}
