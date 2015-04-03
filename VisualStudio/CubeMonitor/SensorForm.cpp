// SensorForm.cpp : implementation file
//

#include "stdafx.h"
#include "CubeMonitor.h"
#include "SensorForm.h"
#include "afxdialogex.h"


// CSensorForm dialog
CSensorForm::CSensorForm(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CSensorForm::IDD, pParent)
{

}

CSensorForm::~CSensorForm()
{
}

void CSensorForm::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSensorForm, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CSensorForm::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSensorForm::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSensorForm message handlers


void CSensorForm::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDockablePaneChildView::OnOK();
}


void CSensorForm::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//CDockablePaneChildView::OnCancel();
}
