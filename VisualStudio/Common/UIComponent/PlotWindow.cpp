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


// 그래프 출력.
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
	int oneSecondsWidth = 0; // 1초당 픽셀 간격

	// 1초당 픽셀폭이 고정된 모드일 경우.
	if (m_isFixedPlot)
	{
		oneSecondsWidth = 100; // 1초당 픽셀 간격을 100으로 고정시킨다.
		TOTAL_DRAW_TIMELINE = (float)cr.Width() / (float)oneSecondsWidth;
		m_renderStartIndex = GetDrawStartIndex(m_renderStartIndex, TOTAL_DRAW_TIMELINE);
	}
	else
	{ // 화면에 출력할 그래프의 시간을 고정시킬 경우.
		TOTAL_DRAW_TIMELINE = 5.f; // 한 화면에 5초의 그래프를 출력하게 한다.
		m_renderStartIndex = GetDrawStartIndex(m_renderStartIndex, TOTAL_DRAW_TIMELINE);
		oneSecondsWidth = (int)((float)cr.Width() / TOTAL_DRAW_TIMELINE); // 1초당 픽셀 간격
	}

	const float timeElapse = m_plot[ m_renderStartIndex].first - m_startTime; // 지나간 시간만큼 timeline을 이동시킨다.

	// 차이가 나는 시간 간격만큼 픽셀을 계산한다. 1초 단위의 간격만 알면된다.
	const int offsetX = ((int)(timeElapse * (float)oneSecondsWidth)) % oneSecondsWidth; 
	const int timeLineCount = cr.Width() / oneSecondsWidth;
	for (int i = 1; i <= timeLineCount+1; ++i)
	{
		pDC->MoveTo(CPoint((i * oneSecondsWidth) - offsetX, 0));
		pDC->LineTo(CPoint((i * oneSecondsWidth) - offsetX, cr.Height()));
	}

	m_scaleY = (float)cr.Height() / (m_maxY - m_minY);
	m_scaleX = oneSecondsWidth / 1.f; // 1초 당 픽셀수 //(float)(cr.Width() - 50) / (float)m_plot.size();

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


	// 정보 출력. 최대, 최소, 중간 값, 현재 값.
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


// Plot을 Frame per Seconds에 맞춰 출력하게 한다.
void CPlotWindow::DrawPlot(const float deltaSeconds)
{
	const float elapseTime = 0.03f; // 1초에 30프레임일 때, 시간 간격

	// 가장 최근에 업데이트 된 시간과 현재시간을 비교해서, 
	// 일정시간 이상 그래프가 업데이트가 되지 않았다면,
	// 가장 최근 값으로, 그래프 값을 추가한다.
	const float curT = timeGetTime() * 0.001f;
	if ((curT - m_updateTime) > elapseTime) // 30프레임주기로 그래프를 업데이트 한다.
	{
		// 가장 최근 값을 저장한다.
		SetPlotXY(curT, GetTailValue().second);
	}

	m_incSeconds += deltaSeconds;
	if (m_incSeconds > elapseTime) // 1초에 30프레임을 유지한다.
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


// 그래프 정보 추가.
void CPlotWindow::SetPlotXY(const float x, const float y)
{	
	// X축은 시간축으로 한다. 
	const float t = timeGetTime() * 0.001f;

	// Ring형 배열 구조, 추가
	m_plot[m_tailIdx] = pair<float, float>(t, y);
	m_tailIdx = ++m_tailIdx % m_plot.size();
	if (m_headIdx == m_tailIdx)
		m_headIdx = ++m_headIdx % m_plot.size();

	m_maxX = max(x, m_maxX);
	m_minX = min(x, m_minX);
	m_maxY = max(y, m_maxY);
	m_minY = min(y, m_minY);

	m_updateTime = t; // 가장 최근 값이 바뀐 시간을 저장한다.

	if (m_startTime == t) // 처음 그래프가 그려질 때, 초기화 된다.
		m_startTime = t;
}


void CPlotWindow::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);
}


BOOL CPlotWindow::OnEraseBkgnd(CDC* pDC)
{
	//return CScrollView::OnEraseBkgnd(pDC); 백그라운드 화면을 갱신하지 않는다.
	return TRUE;
}


// 가장 먼저 저장된 값을 리턴 한다. (가장 오래된 값)
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


// 가장 나중에 저장 된 값을 리턴한다. (가장 최근 값)
pair<float, float> CPlotWindow::GetTailValue() const
{
	const int idx = (m_plot.size() + m_tailIdx - 1) % m_plot.size();
	return m_plot[ idx];
}


// 현재 plot index 부터 tail 까지 쫓아가면서 drawTimeRange 범위에 안에 드는 
// 첫 번째 plot index 를 찾아 리턴한다.
// 그래프가 정확히 drawTimeRange 만큼 출력하기 위한 함수다.
int CPlotWindow::GetDrawStartIndex(const int currentIndex, const float drawTimeRange)
{
	const float tailSeconds = GetTailValue().first;

	for (int i = currentIndex; i != m_tailIdx; i = ++i % m_plot.size())
	{
		const float totalDrawTime = tailSeconds - m_plot[i].first;
		if (totalDrawTime < drawTimeRange)
			return i;
	}

	// 찾지 못했다면, 현재 인덱스를 리턴한다.
	return currentIndex;
}
