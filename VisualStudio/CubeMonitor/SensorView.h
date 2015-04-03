#pragma once

#include "DockablePaneBase.h"

// CSensorView
class CSensorForm;
class CSensorView : public CDockablePaneBase
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
};

