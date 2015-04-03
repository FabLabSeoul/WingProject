#pragma once

#include "DockablePaneBase.h"

// CSerialEditorView
class CSerialEditorForm;
class CSerialEditorView : public CDockablePaneBase
{
public:
	CSerialEditorView();
	virtual ~CSerialEditorView();


protected:
	CSerialEditorForm *m_view = NULL;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
