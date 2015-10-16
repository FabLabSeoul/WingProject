#pragma once


// CReceiverView dialog

class CReceiverView : public CDockablePaneChildView
{
public:
	CReceiverView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CReceiverView();

// Dialog Data
	enum { IDD = IDD_DIALOG_RECEIVER };

	virtual void Update(const float deltaSeconds) override;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SyncReceiver();

	int m_syncState;
	float m_updateIncTime;
	int m_errorCount;
	Quaternion m_q;
	Quaternion m_qr;
	Quaternion m_qp;
	Quaternion m_qy;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	BOOL m_IsSyncReceiverRcv;
	afx_msg void OnBnClickedCheckSyncReceiverRcv();
	afx_msg void OnNMCustomdrawSlider(UINT, NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_Slider[8];
	CEdit m_Edit[8];
	BOOL m_IsSyncReceiverSend;
	afx_msg void OnBnClickedCheckSyncReceiverSend();
};
