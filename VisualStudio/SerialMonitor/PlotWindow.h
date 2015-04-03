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
	void DrawPlot(const float deltaSeconds);
	void SetFixedWidthMode(const bool isFixedWidth);


protected:
	// Plot variable
	float m_xRange;
	float m_yRange;
	float m_xVisibleRange;
	float m_yVisibleRange;
	DWORD m_flags;

	float m_maxX;
	float m_minX;
	float m_maxY;
	float m_minY;
	float m_scaleY;
	float m_scaleX;

	// plot sync
	float m_updateTime = 0; // seconds
	float m_incSeconds = 0; // Frame per Seconds �� ���߱� ���� ������ �ð��� �����Ѵ�.
	float m_startTime = 0; // �׷����� �׷����� ������ �ð�. ó�� SetPlotXY()���� �ʱ�ȭ �ȴ�. Seconds ����.

	// plot array
	vector<pair<float, float>> m_plot; //x,y
	int m_headIdx = 0;
	int m_tailIdx = 0;

	bool m_isFixedPlot = false;
	int m_renderStartIndex = 0; // ��µǴ� �׷����� ù��° �ε���
	CBrush m_blackBrush;
	CPen m_plotPen;
	CPen m_gridPen1; // line width 1
	CPen m_gridPen2; // line width 2

	pair<float, float> GetHeadValue() const;
	pair<float, float> GetTailValue() const;
	int GetDrawStartIndex(const int currentIndex, const float drawTimeRange);

	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


inline void CPlotWindow::SetFixedWidthMode(const bool isFixedWidth) { m_isFixedPlot = isFixedWidth; }
