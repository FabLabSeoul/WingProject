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
	m_blackBrush.CreateSolidBrush(RGB(0, 0, 0));
	m_plotPen.CreatePen(0, 1, RGB(255, 255, 0));
	m_gridPen1.CreatePen(0, 1, RGB(100, 100, 100));
	m_gridPen2.CreatePen(0, 2, RGB(100,100,100));

}

CPlotWindow::~CPlotWindow()
{
}


BEGIN_MESSAGE_MAP(CPlotWindow, CScrollView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CPlotWindow drawing

void CPlotWindow::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CPlotWindow::OnDraw(CDC* pDC)
{
	CRect cr;
	GetClientRect(cr);

	// background black
	pDC->SelectObject(m_blackBrush);
	pDC->Rectangle(cr);

	m_scaleY = (float)cr.Height() / (m_maxY - m_minY);
	m_scaleX = (float)(cr.Width()-50) / (float)m_plot.size();

	// draw x,y axis grid
	pDC->SelectObject(m_gridPen2); // center line width 2
	pDC->MoveTo(CPoint(0, cr.Height() / 2));
	pDC->LineTo(CPoint(cr.Width(), cr.Height() / 2));
	pDC->SelectObject(m_gridPen1); 
	pDC->MoveTo(CPoint(0, cr.Height() / 4 ));
	pDC->LineTo(CPoint(cr.Width(), cr.Height() / 4));
	pDC->MoveTo(CPoint(0, cr.Height()* 3 / 4));
	pDC->LineTo(CPoint(cr.Width(), cr.Height()*3 / 4));

	// draw graph
	pDC->SelectObject(m_plotPen);
	const int h = cr.Height();
	float x = 0;
	int center = cr.Height() / 2;
	for (int i = m_headIdx; i != m_tailIdx; i = ++i % m_plot.size())
	{
		int y = h - (int)((m_plot[i].second- m_minY) * m_scaleY);
		if (i == m_headIdx)
			pDC->MoveTo(CPoint((int)x, y));
		else
			pDC->LineTo(CPoint((int)x, y));
		x += m_scaleX;
	}


	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(220, 220, 220));

	// 가장 마지막 위치에 현재 값을 출력한다.
	if (m_headIdx != m_tailIdx)
	{
		int y = h - (int)((m_plot[(m_tailIdx - 1) % m_plot.size()].second - m_minY) * m_scaleY);
		if (cr.Height() < (y+20)) {
			y -= 20;
		} else {
			y += 10;
		}
		
		CString strLastVal;
		strLastVal.Format(L"%f", m_plot[(m_tailIdx - 1) % m_plot.size()].second);
		pDC->TextOutW((int)x-10, y, strLastVal);
	}

	CString strMaxY, strMinY, strCenterY;
	strMaxY.Format(L"%f", m_maxY);
	strMinY.Format(L"%f", m_minY);
	strCenterY.Format(L"%f", (m_maxY + m_minY) / 2.f);
	pDC->TextOutW(5, 0, strMaxY); // maximum y
	pDC->TextOutW(5, cr.Height() - 20, strMinY); // minimum y
	pDC->TextOutW(5, cr.Height() / 2 - 20, strCenterY); // center y
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


BOOL CPlotWindow::OnEraseBkgnd(CDC* pDC)
{
	//return CScrollView::OnEraseBkgnd(pDC);
	return TRUE;
}
