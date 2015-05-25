#pragma once
#include "afxcmn.h"


// CMotorView dialog
class cMotor;

class CMotorView : public CDockablePaneChildView
{
public:
	CMotorView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMotorView();

// Dialog Data
	enum { IDD = IDD_DIALOG_MOTOR };

	virtual void Update(const float deltaSeconds) override;


	cMotor *m_motor; // reference

protected:
	float m_incSeconds;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdrawSliderPower(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditPower();
	afx_msg void OnNMCustomdrawSliderPowerNoise(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditPowerNoise();
	afx_msg void OnNMCustomdrawSliderDirX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderDirY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderDirZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditDirX();
	afx_msg void OnEnChangeEditDirY();
	afx_msg void OnEnChangeEditDirZ();
	afx_msg void OnNMCustomdrawSliderDirNX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderDirNY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderDirNZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditDirNX();
	afx_msg void OnEnChangeEditDirNY();
	afx_msg void OnEnChangeEditDirNZ();
	virtual BOOL OnInitDialog();
	CSliderCtrl m_sliderPower;
	CSliderCtrl m_sliderPowerNoise;
	float m_editPower;
	float m_editPowerNoise;
	CSliderCtrl m_sliderDirX;
	CSliderCtrl m_sliderDirY;
	CSliderCtrl m_sliderDirZ;
	float m_editDirX;
	float m_editDirY;
	float m_editDirZ;
	CSliderCtrl m_sliderDirNoiseX;
	CSliderCtrl m_sliderDirNoiseY;
	CSliderCtrl m_sliderDirNoiseZ;
	float m_editDirNoiseX;
	float m_editDirNoiseY;
	float m_editDirNoiseZ;
	float m_editP;
	float m_editI;
	float m_editD;
	CPlotWindow *m_plotP;
	CPlotWindow *m_plotI;
	CPlotWindow *m_plotD;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeEditP();
	afx_msg void OnEnChangeEditI();
	afx_msg void OnEnChangeEditD();
};
