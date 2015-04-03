#pragma once
#include "afxwin.h"

class CPlotWindow;

// CGraphDialog dialog

class CGraphDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CGraphDialog)

public:
	CGraphDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGraphDialog();

	void SetString(const TCHAR *str);
	void DrawGraph(const float deltaSeconds);
	void ProcessPlotCommand();

// Dialog Data
	enum { IDD = IDD_DIALOG_GRAPH };

protected:
	struct SPlot
	{
		CPlotWindow *wnd;
		wstring scanString;
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
		CString scanString;
	};
	vector<SPlotInfo> m_PlotInfos;



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CPlotWindow* CreatePlotWindow(const int plotIndex);
	bool ParsePlotInfo(const int plotIndex, const wstring &str, SPlotInfo &out);
	wstring ParseKeyValue(const wstring &str, const wstring &key);
	void CalcGraphWindowSize();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonUpdate();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	CEdit m_CommandEditor;
	CButton m_UpdateButton;
	afx_msg void OnBnClickedButtonHelp();
	CButton m_HelpButton;
	CStatic m_Static1;
	afx_msg void OnBnClickedCheckFixedmode();
	CButton m_FixedMode;
};
