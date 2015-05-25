// SerialEditorView.cpp : implementation file
//

#include "stdafx.h"
#include "CubeMonitor.h"
#include "SerialEditorPane.h"
#include "SerialEditorView.h"


// CSerialEditorView

CSerialEditorPane::CSerialEditorPane()
{

}

CSerialEditorPane::~CSerialEditorPane()
{
}


BEGIN_MESSAGE_MAP(CSerialEditorPane, CDockablePaneBase)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CSerialEditorView message handlers
int CSerialEditorPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePaneBase::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_view = new CSerialEditorView(this);
	m_view->Create(CSerialEditorView::IDD, this);
	m_view->ShowWindow(SW_SHOW);
	SetChildView(m_view);

	return 0;
}
