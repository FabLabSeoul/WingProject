#pragma once
#include "afxwin.h"
#include "ComPortCombo.h"


// CConnectionDialog dialog

class CConnectionDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CConnectionDialog)

public:
	CConnectionDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConnectionDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONNECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedConnect();
	CComPortCombo m_PortCombobox;
	CComboBox m_BaudrateCombobox;
	virtual BOOL OnInitDialog();
};
