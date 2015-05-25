#pragma once


// C3DPane

class C3DPhysxView;
class C3DPane : public CDockablePaneBase
{
public:
	C3DPane();
	virtual ~C3DPane();

	C3DPhysxView *m_view;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

