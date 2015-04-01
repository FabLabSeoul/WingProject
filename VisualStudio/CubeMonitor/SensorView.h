#pragma once

static const int ID_VIEW_SENSOR = 10001;

// CSensorView
class CSensorForm;
class CSensorView : public CDockablePane
{
public:
	CSensorView();
	virtual ~CSensorView();


protected:
	CSensorForm *m_view;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


