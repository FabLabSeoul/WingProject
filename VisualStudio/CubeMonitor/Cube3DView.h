#pragma once


static const int ID_VIEW_CUBE3D = 10000;

// CCube3DView
class C3DView;
class CCube3DView : public CDockablePane
{

public:
	CCube3DView();
	virtual ~CCube3DView();


protected:
	C3DView *m_view;


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


