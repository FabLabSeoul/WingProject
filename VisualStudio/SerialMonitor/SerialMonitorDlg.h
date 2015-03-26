
// SerialMonitorDlg.h : header file
//

#pragma once
#include "Serial.h"
#include "afxwin.h"
#include "afxcmn.h"

// CSerialMonitorDlg dialog
class CSerialMonitorDlg : public CDialogEx
{
// Construction
public:
	CSerialMonitorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SERIALMONITOR_DIALOG };

	void MainLoop();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void Process(float deltaT);
	int CSerialMonitorDlg::AppendToLogAndScroll(CString str, COLORREF color);
	int CSerialMonitorDlg::GetNumVisibleLines(CRichEditCtrl* pCtrl);


// Implementation
protected:
	HICON m_hIcon;
	bool m_loop;
	CSerial m_Serial;
	bool m_isConnect;
	bool m_isFastMode;

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
	CComboBox m_PortCombobox;
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
};
