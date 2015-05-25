#pragma once



// CPlotWindow view

class CPlotWindow : public CScrollView
{
	DECLARE_DYNCREATE(CPlotWindow)
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
		const float x_visble_range, const float y_visible_range, const DWORD flags,
		const int plotCount=1);

	void SetPlotXY(const float x, const float y, const int plotIndex=0);
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
	float m_incSeconds = 0; // Frame per Seconds 를 맞추기 위해 증가된 시간을 저장한다.
	float m_startTime = 0; // 그래프가 그려지기 시작한 시간. 처음 SetPlotXY()에서 초기화 된다. Seconds 단위.

	// plot array
	struct sPlotData {
		vector<std::pair<float, float>> xy;
		int headIdx = 0;
		int tailIdx = 0;
		int renderStartIndex = 0;
		//CPen pen;
	};

	//vector<std::pair<float, float>> m_plot; //x,y
	//int m_headIdx = 0;
	//int m_tailIdx = 0;
	vector<sPlotData> m_plots;
	//int m_renderStartIndex = 0; // 출력되는 그래프의 첫번째 인덱스



	bool m_isFixedPlot = false;
	CBrush m_blackBrush;
	CPen m_plotPen;
	CPen m_plotPens[4];
	CPen m_gridPen1; // line width 1
	CPen m_gridPen2; // line width 2

	std::pair<float, float> GetHeadValue(const u_int plotIndex=0) const;
	std::pair<float, float> GetTailValue(const u_int plotIndex = 0) const;
	int GetDrawStartIndex(const u_int plotIndex, const int currentIndex, const float drawTimeRange);

	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


inline void CPlotWindow::SetFixedWidthMode(const bool isFixedWidth) { m_isFixedPlot = isFixedWidth; }
