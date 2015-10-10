#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CCubeSerialView dialog

class CCubeSerialView : public CDockablePaneChildView
{
public:
	CCubeSerialView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCubeSerialView();

// Dialog Data
	enum { IDD = IDD_DIALOG_CUBE_SERIAL };

	virtual void Update(const float deltaSeconds) override;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	bool SyncIMU();
	void UpdateRecvCount();
	void Start();
	void Stop();

	bool m_isStart;
	//CSerial m_serial;
	int m_serialRcvCount;
	float m_updateIncTime;
	int m_syncIMUState;
	int m_errorCount;


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CComPortCombo m_ComPort;
	CComboBox m_Baudrate;
	afx_msg void OnBnClickedButtonStart();
	virtual BOOL OnInitDialog();
	CButton m_StartButton;
	CRichEditCtrl m_RecvEdit;
	CStatic m_RcvCount;
	afx_msg void OnBnClickedButtonSend();
	CString m_SendText;
	BOOL m_IsSyncIMU;
	afx_msg void OnBnClickedCheckImu();
	CEdit m_AttitudeEdit;
	afx_msg void OnBnClickedButtonResetHeading();
	BOOL m_IsShowThrust;
	BOOL m_IsShowIdealThrust;
	afx_msg void OnBnClickedCheckShowthrust();
	afx_msg void OnBnClickedCheckShowidealthrust();
};
