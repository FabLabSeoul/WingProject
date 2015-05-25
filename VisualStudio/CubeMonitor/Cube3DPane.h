#pragma once



// CCube3DView
class C3DView;
class CCube3DPane : public CDockablePaneBase
{
public:
	CCube3DPane();
	virtual ~CCube3DPane();


protected:
	C3DView *m_view = NULL;


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


