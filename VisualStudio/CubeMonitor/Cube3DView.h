#pragma once

#include "DockablePaneBase.h"


// CCube3DView
class C3DDialog;
class CCube3DView : public CDockablePaneBase
{
public:
	CCube3DView();
	virtual ~CCube3DView();


protected:
	C3DDialog *m_view = NULL;


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


