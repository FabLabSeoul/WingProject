#pragma once
#include "afxcmn.h"

//#include "bpctrlanchormap.h"
//#include "DockablePaneChildView.h"
#include "afxwin.h"

// CSerialEditorForm dialog

class CSerialEditorView : public CDockablePaneChildView
{
public:
	CSerialEditorView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSerialEditorView();

// Dialog Data
	enum { IDD = IDD_DIALOG_SERIAL_EDITOR };


protected:
	int AppendToLogAndScroll(CString str, COLORREF color);
	int GetNumVisibleLines(CRichEditCtrl* pCtrl);
	virtual void UpdateSerial(char *buffer) override;


protected:
	bool m_isFastMode = false;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_ANCHOR_MAP();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CRichEditCtrl m_Editor;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CStatic m_FastText;
	CButton m_ModeButton;
	afx_msg void OnBnClickedRadioMode();
	afx_msg void OnBnClickedRadioFast();
};
