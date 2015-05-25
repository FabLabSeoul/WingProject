// PlotWindow.cpp : implementation file
//

#include "stdafx.h"
//#include "CubeMonitor.h"
#include "PlotWindow.h"


IMPLEMENT_DYNCREATE(CPlotWindow, CScrollView)

// CPlotWindow
CPlotWindow::CPlotWindow()
{
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


// �׷��� ���.
void CPlotWindow::OnDraw(CDC* pDC)
{
	CRect cr;
	GetClientRect(cr);

	// background black
	pDC->SelectObject(m_blackBrush);
	pDC->Rectangle(cr);

	// draw x,y axis grid
	pDC->SelectObject(m_gridPen2); // center line width 2
	pDC->MoveTo(CPoint(0, cr.Height() / 2));
	pDC->LineTo(CPoint(cr.Width(), cr.Height() / 2));
	pDC->SelectObject(m_gridPen1); 
	pDC->MoveTo(CPoint(0, cr.Height() / 4 ));
	pDC->LineTo(CPoint(cr.Width(), cr.Height() / 4));
	pDC->MoveTo(CPoint(0, cr.Height()* 3 / 4));
	pDC->LineTo(CPoint(cr.Width(), cr.Height()*3 / 4));

	// draw time line grid
	float TOTAL_DRAW_TIMELINE = 0;
	int oneSecondsWidth = 0; // 1�ʴ� �ȼ� ����

	// 1�ʴ� �ȼ����� ������ ����� ���.
	if (m_isFixedPlot)
	{
		oneSecondsWidth = 100; // 1�ʴ� �ȼ� ������ 100���� ������Ų��.
		TOTAL_DRAW_TIMELINE = (float)cr.Width() / (float)oneSecondsWidth;
		m_renderStartIndex = GetDrawStartIndex(m_renderStartIndex, TOTAL_DRAW_TIMELINE);
	}
	else
	{ // ȭ�鿡 ����� �׷����� �ð��� ������ų ���.
		TOTAL_DRAW_TIMELINE = 5.f; // �� ȭ�鿡 5���� �׷����� ����ϰ� �Ѵ�.
		m_renderStartIndex = GetDrawStartIndex(m_renderStartIndex, TOTAL_DRAW_TIMELINE);
		oneSecondsWidth = (int)((float)cr.Width() / TOTAL_DRAW_TIMELINE); // 1�ʴ� �ȼ� ����
	}

	const float timeElapse = m_plot[ m_renderStartIndex].first - m_startTime; // ������ �ð���ŭ timeline�� �̵���Ų��.

	// ���̰� ���� �ð� ���ݸ�ŭ �ȼ��� ����Ѵ�. 1�� ������ ���ݸ� �˸�ȴ�.
	const int offsetX = ((int)(timeElapse * (float)oneSecondsWidth)) % oneSecondsWidth; 
	const int timeLineCount = cr.Width() / oneSecondsWidth;
	for (int i = 1; i <= timeLineCount+1; ++i)
	{
		pDC->MoveTo(CPoint((i * oneSecondsWidth) - offsetX, 0));
		pDC->LineTo(CPoint((i * oneSecondsWidth) - offsetX, cr.Height()));
	}

	m_scaleY = (float)cr.Height() / (m_maxY - m_minY);
	m_scaleX = oneSecondsWidth / 1.f; // 1�� �� �ȼ��� //(float)(cr.Width() - 50) / (float)m_plot.size();

	// draw graph
	pDC->SelectObject(m_plotPen);
	const int h = cr.Height();
	int lastX = 0;
	for (int i = m_renderStartIndex; i != m_tailIdx; i = ++i % m_plot.size())
	{
		const int x = (int)((m_plot[i].first - m_plot[m_renderStartIndex].first) * m_scaleX) - 50;
		const int y = h - (int)((m_plot[i].second- m_minY) * m_scaleY);
		lastX = x;

		if (i == m_renderStartIndex)
			pDC->MoveTo(CPoint(x, y));
		else
			pDC->LineTo(CPoint(x, y));
	}


	// ���� ���. �ִ�, �ּ�, �߰� ��, ���� ��.
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(220, 220, 220));

	// ���� ������ ��ġ�� ���� ���� ����Ѵ�.
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
		pDC->TextOutW(lastX-10, y, strLastVal);
	}

	CString strMaxY, strMinY, strCenterY;
	strMaxY.Format(L"%f", m_maxY);
	strMinY.Format(L"%f", m_minY);
	strCenterY.Format(L"%f", (m_maxY + m_minY) / 2.f);
	pDC->TextOutW(5, 0, strMaxY); // maximum y
	pDC->TextOutW(5, cr.Height() - 20, strMinY); // minimum y
	pDC->TextOutW(5, cr.Height() / 2 - 20, strCenterY); // center y
}


// Plot�� Frame per Seconds�� ���� ����ϰ� �Ѵ�.
void CPlotWindow::DrawPlot(const float deltaSeconds)
{
	const float elapseTime = 0.03f; // 1�ʿ� 30�������� ��, �ð� ����

	// ���� �ֱٿ� ������Ʈ �� �ð��� ����ð��� ���ؼ�, 
	// �����ð� �̻� �׷����� ������Ʈ�� ���� �ʾҴٸ�,
	// ���� �ֱ� ������, �׷��� ���� �߰��Ѵ�.
	const float curT = timeGetTime() * 0.001f;
	if ((curT - m_updateTime) > elapseTime) // 30�������ֱ�� �׷����� ������Ʈ �Ѵ�.
	{
		// ���� �ֱ� ���� �����Ѵ�.
		SetPlotXY(curT, GetTailValue().second);
	}

	m_incSeconds += deltaSeconds;
	if (m_incSeconds > elapseTime) // 1�ʿ� 30�������� �����Ѵ�.
	{
		InvalidateRect(NULL);
		m_incSeconds = 0;
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

	m_plot.resize(2048, pair<float,float>(0.f,0.f));

	m_maxX = -FLT_MAX;
	m_minX = FLT_MAX;
	m_maxY = -FLT_MAX;
	m_minY = FLT_MAX;
	m_scaleY = 1.f;

	return true;
}


// �׷��� ���� �߰�.
void CPlotWindow::SetPlotXY(const float x, const float y)
{	
	// X���� �ð������� �Ѵ�. 
	const float t = timeGetTime() * 0.001f;

	// Ring�� �迭 ����, �߰�
	m_plot[m_tailIdx] = pair<float, float>(t, y);
	m_tailIdx = ++m_tailIdx % m_plot.size();
	if (m_headIdx == m_tailIdx)
		m_headIdx = ++m_headIdx % m_plot.size();

	m_maxX = max(x, m_maxX);
	m_minX = min(x, m_minX);
	m_maxY = max(y, m_maxY);
	m_minY = min(y, m_minY);

	m_updateTime = t; // ���� �ֱ� ���� �ٲ� �ð��� �����Ѵ�.

	if (m_startTime == t) // ó�� �׷����� �׷��� ��, �ʱ�ȭ �ȴ�.
		m_startTime = t;
}


void CPlotWindow::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);
}


BOOL CPlotWindow::OnEraseBkgnd(CDC* pDC)
{
	//return CScrollView::OnEraseBkgnd(pDC); ��׶��� ȭ���� �������� �ʴ´�.
	return TRUE;
}


// ���� ���� ����� ���� ���� �Ѵ�. (���� ������ ��)
pair<float, float> CPlotWindow::GetHeadValue() const
{
	if (m_headIdx < m_tailIdx)
	{
		return m_plot[m_headIdx];
	}
	else
	{
		const int idx = (m_plot.size() + m_headIdx - 1) % m_plot.size();
		return m_plot[idx];
	}
}


// ���� ���߿� ���� �� ���� �����Ѵ�. (���� �ֱ� ��)
pair<float, float> CPlotWindow::GetTailValue() const
{
	const int idx = (m_plot.size() + m_tailIdx - 1) % m_plot.size();
	return m_plot[ idx];
}


// ���� plot index ���� tail ���� �Ѿư��鼭 drawTimeRange ������ �ȿ� ��� 
// ù ��° plot index �� ã�� �����Ѵ�.
// �׷����� ��Ȯ�� drawTimeRange ��ŭ ����ϱ� ���� �Լ���.
int CPlotWindow::GetDrawStartIndex(const int currentIndex, const float drawTimeRange)
{
	const float tailSeconds = GetTailValue().first;

	for (int i = currentIndex; i != m_tailIdx; i = ++i % m_plot.size())
	{
		const float totalDrawTime = tailSeconds - m_plot[i].first;
		if (totalDrawTime < drawTimeRange)
			return i;
	}

	// ã�� ���ߴٸ�, ���� �ε����� �����Ѵ�.
	return currentIndex;
}
