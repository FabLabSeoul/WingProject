// SerialEditorForm.cpp : implementation file
//

#include "stdafx.h"
#include "CubeMonitor.h"
#include "SerialEditorView.h"
#include "afxdialogex.h"


BEGIN_ANCHOR_MAP(CSerialEditorView)
	ANCHOR_MAP_ENTRY(IDC_RICHEDIT_SERIAL, ANF_TOP | ANF_LEFT | ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_STATIC_FASTTEXT, ANF_TOP | ANF_LEFT | ANF_RIGHT )
END_ANCHOR_MAP()


// CSerialEditorForm dialog
CSerialEditorView::CSerialEditorView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CSerialEditorView::IDD, pParent)
{

}

CSerialEditorView::~CSerialEditorView()
{
}

void CSerialEditorView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT_SERIAL, m_Editor);
	DDX_Control(pDX, IDC_STATIC_FASTTEXT, m_FastText);
	DDX_Control(pDX, IDC_RADIO_MODE, m_ModeButton);
}


BEGIN_MESSAGE_MAP(CSerialEditorView, CDockablePaneChildView)
	ON_BN_CLICKED(IDCANCEL, &CSerialEditorView::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CSerialEditorView::OnBnClickedOk)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RADIO_MODE, &CSerialEditorView::OnBnClickedRadioMode)
	ON_BN_CLICKED(IDC_RADIO_FAST, &CSerialEditorView::OnBnClickedRadioFast)
END_MESSAGE_MAP()


// CSerialEditorForm message handlers
BOOL CSerialEditorView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	InitAnchors();

	m_Editor.SetBackgroundColor(FALSE, RGB(0, 0, 0));
	m_ModeButton.SetCheck(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSerialEditorView::OnBnClickedCancel()
{
//	CDockablePaneChildView::OnCancel();
}


void CSerialEditorView::OnBnClickedOk()
{
//	CDockablePaneChildView::OnOK();
}

void CSerialEditorView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePaneChildView::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	HandleAnchors(&rcWnd);
}


// http://www.codeproject.com/Articles/12093/Using-RichEditCtrl-to-Display-Formatted-Logs
int CSerialEditorView::AppendToLogAndScroll(CString str, COLORREF color)
{
	long nVisible = 0;
	long nInsertionPoint = 0;
	CHARFORMAT cf;

	// Initialize character format structure
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.dwEffects = 0; // To disable CFE_AUTOCOLOR

	cf.crTextColor = color;

	// Set insertion point to end of text
	nInsertionPoint = m_Editor.GetWindowTextLength();
	m_Editor.SetSel(nInsertionPoint, -1);

	// Set the character format
	m_Editor.SetSelectionCharFormat(cf);

	// Replace selection. Because we have nothing
	// selected, this will simply insert
	// the string at the current caret position.
	m_Editor.ReplaceSel(str);

	// Get number of currently visible lines or maximum number of visible lines
	// (We must call GetNumVisibleLines() before the first call to LineScroll()!)
	nVisible = GetNumVisibleLines(&m_Editor);

	// Now this is the fix of CRichEditCtrl's abnormal behaviour when used
	// in an application not based on dialogs. Checking the focus prevents
	// us from scrolling when the CRichEditCtrl does so automatically,
	// even though ES_AUTOxSCROLL style is NOT set.
	if (//m_AutoScrollButton.GetCheck() && 
		&m_Editor != m_Editor.GetFocus())
	{
		m_Editor.LineScroll(INT_MAX);
		m_Editor.LineScroll(1 - nVisible);
	}

	// 내용이 너무 많으면 지운다.
	const int maximumLine = 100;
	if (m_Editor.GetLineCount() > maximumLine)
	{
		long nFirstChar = m_Editor.CharFromPos(CPoint(0, 0));
		m_Editor.SetSel(0, nFirstChar);
		m_Editor.ReplaceSel(L"");
	}

	return 0;
}


//http://www.codeproject.com/Articles/12093/Using-RichEditCtrl-to-Display-Formatted-Logs
int CSerialEditorView::GetNumVisibleLines(CRichEditCtrl* pCtrl)
{
	CRect rect;
	long nFirstChar, nLastChar;
	long nFirstLine, nLastLine;

	// Get client rect of rich edit control
	pCtrl->GetClientRect(rect);

	// Get character index close to upper left corner
	nFirstChar = pCtrl->CharFromPos(CPoint(0, 0));

	// Get character index close to lower right corner
	nLastChar = pCtrl->CharFromPos(CPoint(rect.right, rect.bottom));
	if (nLastChar < 0)
	{
		nLastChar = pCtrl->GetTextLength();
	}

	// Convert to lines
	nFirstLine = pCtrl->LineFromChar(nFirstChar);
	nLastLine = pCtrl->LineFromChar(nLastChar);

	return (nLastLine - nFirstLine);
}


// 시리얼통신 정보 처리
void CSerialEditorView::UpdateSerial(char *buffer)
{
	CString str = str2wstr(buffer).c_str();

	if (m_isFastMode)
	{
		m_FastText.SetWindowTextW(str);
	}
	else
	{
		AppendToLogAndScroll(str, RGB(200, 200, 200));
	}
}


void CSerialEditorView::OnBnClickedRadioMode()
{
	m_isFastMode = false;
}


void CSerialEditorView::OnBnClickedRadioFast()
{
	m_isFastMode = true;
}
