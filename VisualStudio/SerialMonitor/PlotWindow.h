#pragma once



// CPlotWindow view

class CPlotWindow : public CScrollView
{
public:
	CPlotWindow();
	virtual ~CPlotWindow();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	bool SetPlot(const float x_range, const float y_range,
		const float x_visble_range, const float y_visible_range, const DWORD flags);

	void SetPlotXY(const float x, const float y);


protected:
	float m_xRange;
	float m_yRange;
	float m_xVisibleRange;
	float m_yVisibleRange;
	DWORD m_flags;

	vector<pair<float,float>> m_plot; //x,y
	float m_maxX;
	float m_minX;
	float m_maxY;
	float m_minY;
	float m_scaleY;
	float m_scaleX;

	int m_headIdx;
	int m_tailIdx;

	CBrush m_blackBrush;
	CPen m_plotPen;
	CPen m_gridPen1; // line width 1
	CPen m_gridPen2; // line width 2

	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


