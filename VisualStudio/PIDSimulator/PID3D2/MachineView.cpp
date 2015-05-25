// MachineView.cpp : implementation file
//

#include "stdafx.h"
#include "PID3D2.h"
#include "MachineView.h"
#include "afxdialogex.h"
#include "C3DPhysxPIDView.h"


// CMachineView dialog
CMachineView::CMachineView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CMachineView::IDD, pParent)
{

}

CMachineView::~CMachineView()
{
}

void CMachineView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMachineView, CDockablePaneChildView)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CMachineView::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_PID_CONTROL, &CMachineView::OnBnClickedButtonPidControl)
	ON_BN_CLICKED(IDOK, &CMachineView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMachineView::OnBnClickedCancel)
END_MESSAGE_MAP()


// CMachineView message handlers

void CMachineView::OnBnClickedOk()
{
	// nothing~
}


void CMachineView::OnBnClickedCancel()
{
	// nothing~
}


void CMachineView::OnBnClickedButtonReset()
{
	// TODO: Add your control notification handler code here
}


void CMachineView::OnBnClickedButtonPidControl()
{
	g_physxView->m_machine.EnablePIDControl(
		!g_physxView->m_machine.m_enablePID);
}

