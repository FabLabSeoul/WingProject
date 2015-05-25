#pragma once


// CSerialEditorView
class CSerialEditorView;
class CSerialEditorPane : public CDockablePaneBase
{
public:
	CSerialEditorPane();
	virtual ~CSerialEditorPane();


protected:
	CSerialEditorView *m_view = NULL;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
