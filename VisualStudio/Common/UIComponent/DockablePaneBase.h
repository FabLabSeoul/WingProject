#pragma once

#include "SerialObserver.h"

// CDockablePaneBase

class CDockablePaneChildView;
class CDockablePaneBase : public CDockablePane
											,public iSerialObserver
{
public:
	CDockablePaneBase();
	CDockablePaneBase(CDockablePaneChildView *childView, UINT resourceID);
	virtual ~CDockablePaneBase();

	void SetChildView(CDockablePaneChildView *dlg);
	CDockablePaneChildView* GetChildView();

	// iSeralObserver interface
	virtual void UpdateSerial(char *buffer) override;


protected:
	UINT m_dlgResourceID = 0;
	CDockablePaneChildView *m_dlg = NULL;

	DECLARE_MESSAGE_MAP()


public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


inline void CDockablePaneBase::SetChildView(CDockablePaneChildView *dlg) { m_dlg = dlg; }
inline CDockablePaneChildView* CDockablePaneBase::GetChildView() { return m_dlg;  }
