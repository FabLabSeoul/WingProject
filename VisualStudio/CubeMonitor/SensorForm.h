#pragma once


// CSensorForm dialog

class CSensorForm : public CDialogEx
{
	DECLARE_DYNAMIC(CSensorForm)

public:
	CSensorForm(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSensorForm();

// Dialog Data
	enum { IDD = IDD_DIALOG_SENSOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
