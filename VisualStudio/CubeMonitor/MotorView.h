#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMotorView dialog

class CMotorView : public CDockablePaneChildView
{
public:
	CMotorView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMotorView();

// Dialog Data
	enum { IDD = IDD_DIALOG_MOTOR };

	virtual void Update(const float deltaSeconds) override;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SyncMotor();

	int m_syncState;
	float m_updateIncTime;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckSyncmotor();
	afx_msg void OnBnClickedCheckSyncmotorSend();
	afx_msg void OnNMCustomdrawSlider(UINT,NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_Slider[24];
	CEdit m_Edit[24];
	BOOL m_IsSyncMotor;
	BOOL m_IsMotorSyncSend;
};
