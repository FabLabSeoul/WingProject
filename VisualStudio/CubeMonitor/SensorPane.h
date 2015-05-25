#pragma once


// CSensorView
class CSensorView;
class CSensorPane : public CDockablePaneBase
{
public:
	CSensorPane();
	virtual ~CSensorPane();


protected:
	CSensorView *m_view;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

