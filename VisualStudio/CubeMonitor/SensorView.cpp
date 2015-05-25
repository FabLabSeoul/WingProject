// SensorForm.cpp : implementation file
//

#include "stdafx.h"
#include "CubeMonitor.h"
#include "SensorView.h"
#include "afxdialogex.h"


// CSensorForm dialog
CSensorView::CSensorView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CSensorView::IDD, pParent)
{

}

CSensorView::~CSensorView()
{
}

void CSensorView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSensorView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CSensorView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSensorView::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSensorForm message handlers


void CSensorView::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDockablePaneChildView::OnOK();
}


void CSensorView::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//CDockablePaneChildView::OnCancel();
}
