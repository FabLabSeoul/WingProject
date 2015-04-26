
// SerialMonitorDlg.h : header file
//

#pragma once
#include "Serial.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ComPortCombo.h"
#include "BufferedSerial.h"


class CGraphDialog;

// CSerialMonitorDlg dialog
class CSerialMonitorDlg : public CDialogEx
{
// Construction
public:
	CSerialMonitorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SERIALMONITOR_DIALOG };

	void MainLoop();
	void ShowGraphWindow(const bool isShow);
	void SaveConfigFile();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void Process(float deltaSeconds);
	int CSerialMonitorDlg::AppendToLogAndScroll(CString str, COLORREF color);
	int CSerialMonitorDlg::GetNumVisibleLines(CRichEditCtrl* pCtrl);


// Implementation
protected:
	enum DISPLAY_MODE
	{
		MODE_NORMAL,
		MODE_FAST1,		// Fast 1 Line
		MODE_MEMORY,
	};

	HICON m_hIcon;
	bool m_loop;
	//CSerial m_Serial;
	CBufferedSerial m_Serial;
	bool m_isConnect;
	bool m_isFastMode;
	DISPLAY_MODE m_mode = MODE_NORMAL;
	bool m_isShowGraphWnd;
	int m_PortNumber;
	int m_BaudrateIndex;
	int m_RxCounter = 0;
	CGraphDialog *m_graphDlg; // modeless

	TCHAR m_serialBuffer[256];
	int m_currentBufferIndex = 0;


	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnDestroy();
	//CComboBox m_PortCombobox;
	CComPortCombo m_PortCombobox;
	CComboBox m_BaudrateCombobox;
	afx_msg void OnBnClickedButtonConnect();
	CButton m_ConnectButton;
	CButton m_DisconnectButton;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheckAutoscroll();
	CRichEditCtrl m_Editor;
	afx_msg void OnBnClickedButtonDisconnect();

	CButton m_AutoScrollButton;
	afx_msg void OnBnClickedButtonClear();
	CStatic m_FastReceiveText;
	CButton m_NormalModeButton;
	int m_FastModeButton;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	CButton m_TopMostButton;
	afx_msg void OnBnClickedCheckTopmost();
	CButton m_Show1LineButton;
	afx_msg void OnBnClickedCheckShow1line();
	CStatic m_Static1;
	CStatic m_Static2;
	CButton m_ClearButton;
	int m_ModeSlect;
	afx_msg void OnBnClickedButtonGraph();
	afx_msg void OnClose();
	afx_msg void OnBnClickedRadioMem();
};


inline void CSerialMonitorDlg::ShowGraphWindow(const bool isShow) {
	m_isShowGraphWnd = isShow;
}
