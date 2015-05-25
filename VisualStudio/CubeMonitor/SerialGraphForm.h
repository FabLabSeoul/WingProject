#pragma once

//#include "DockablePaneChildView.h"
//#include "bpctrlanchormap.h"
#include "afxwin.h"


// CSerialGraphForm dialog
class CPlotWindow;
class CSerialGraphForm : public CDockablePaneChildView
{
public:
	CSerialGraphForm(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSerialGraphForm();

// Dialog Data
	enum { IDD = IDD_DIALOG_SERIAL_GRAPH };

	void SetString(const char *str);
	void DrawGraph(const float deltaSeconds);
	void ProcessPlotCommand();

	// SerialObserver Interface
	virtual void UpdateSerial(char *buffer) override;
	// UpdateObserver Interface
	virtual void Update(const float deltaSeconds) override;

protected:
	struct SPlot
	{
		CPlotWindow *wnd;
		string scanString;
	};

	vector<SPlot> m_plotWindows;
	int m_plotWindowCount;

	struct SPlotInfo
	{
		float xRange;
		float yRange;
		float xVisibleRange;
		float yVisibleRange;
		DWORD flags;
		string scanString;
	};
	vector<SPlotInfo> m_PlotInfos;


protected:
	CPlotWindow* CreatePlotWindow(const int plotIndex);
	bool ParsePlotInfo(const int plotIndex, const wstring &str, SPlotInfo &out);
	wstring ParseKeyValue(const wstring &str, const wstring &key);
	void CalcGraphWindowSize();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_ANCHOR_MAP();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	CEdit m_CommandEditor;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonUpdate();
	CButton m_UpdateButton;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedCheckFixedmode();
	CButton m_FixedModeButton;
	CStatic m_Static1;
	CButton m_HelpButton;
	afx_msg void OnBnClickedButtonHelp();
};
