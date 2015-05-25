#pragma once


// CMachineView dialog

class CMachineView : public CDockablePaneChildView
{
public:
	CMachineView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMachineView();

// Dialog Data
	enum { IDD = IDD_DIALOG_MACHINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonPidControl();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
