#pragma once

#include "UpdateObserver.h"


// CDockablePaneChildView dialog

class CDockablePaneChildView : public CDialogEx
												 , public iUpdateObserver
{
public:
	CDockablePaneChildView(UINT id, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDockablePaneChildView();

	virtual void UpdateSerial(char *buffer) {}
	virtual void Update(const float deltaSeconds) {}


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
