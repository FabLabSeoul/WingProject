// SerialEditorView.cpp : implementation file
//

#include "stdafx.h"
#include "CubeMonitor.h"
#include "SerialEditorView.h"
#include "SerialEditorForm.h"


// CSerialEditorView

CSerialEditorView::CSerialEditorView()
{

}

CSerialEditorView::~CSerialEditorView()
{
}


BEGIN_MESSAGE_MAP(CSerialEditorView, CDockablePaneBase)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CSerialEditorView message handlers
int CSerialEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePaneBase::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_view = new CSerialEditorForm(this);
	m_view->Create(CSerialEditorForm::IDD, this);
	m_view->ShowWindow(SW_SHOW);
	SetChildView(m_view);

	return 0;
}
