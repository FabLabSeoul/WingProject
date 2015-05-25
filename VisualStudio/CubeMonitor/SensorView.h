#pragma once

//#include "DockablePaneChildView.h"

// CSensorForm dialog

class CSensorView : public CDockablePaneChildView
{
public:
	CSensorView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSensorView();

// Dialog Data
	enum { IDD = IDD_DIALOG_SENSOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
