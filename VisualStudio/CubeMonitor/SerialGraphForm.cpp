// SerialGraphForm.cpp : implementation file
//

#include "stdafx.h"
#include "CubeMonitor.h"
#include "SerialGraphForm.h"
#include "afxdialogex.h"
//#include "PlotWindow.h"



// CSerialGraphForm dialog
CSerialGraphForm::CSerialGraphForm(CWnd* pParent /*=NULL*/)
: CDockablePaneChildView(CSerialGraphForm::IDD, pParent)
{
}

CSerialGraphForm::~CSerialGraphForm()
{
}

void CSerialGraphForm::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_COMMAND, m_CommandEditor);
	DDX_Control(pDX, IDC_BUTTON_UPDATE, m_UpdateButton);
	DDX_Control(pDX, IDC_CHECK_FIXEDMODE, m_FixedModeButton);
	DDX_Control(pDX, IDC_STATIC1, m_Static1);
	DDX_Control(pDX, IDC_BUTTON_HELP, m_HelpButton);
}

BEGIN_ANCHOR_MAP(CSerialGraphForm)
	ANCHOR_MAP_ENTRY(IDC_EDIT_COMMAND, ANF_LEFT | ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_UPDATE, ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_HELP, ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_CHECK_FIXEDMODE, ANF_LEFT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_STATIC1, ANF_LEFT | ANF_BOTTOM)
END_ANCHOR_MAP()


BEGIN_MESSAGE_MAP(CSerialGraphForm, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CSerialGraphForm::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSerialGraphForm::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CSerialGraphForm::OnBnClickedButtonUpdate)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_CHECK_FIXEDMODE, &CSerialGraphForm::OnBnClickedCheckFixedmode)
	ON_BN_CLICKED(IDC_BUTTON_HELP, &CSerialGraphForm::OnBnClickedButtonHelp)
END_MESSAGE_MAP()


// CSerialGraphForm message handlers


void CSerialGraphForm::OnBnClickedOk()
{
}


void CSerialGraphForm::OnBnClickedCancel()
{
}


BOOL CSerialGraphForm::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	InitAnchors();

	m_CommandEditor.SetWindowTextW(
		//		L"plot1 = 0,0,0,0,0\n\rstring1 = %*s %f, %f, %*f");
L"plot1 = 0, 0, 0, 0, 0\r\n\
string1 = %*s %f, %f, %*f\r\n\
plot2 = 0, 0, 0, 0, 0\r\n\
string2 = %*s %f, %*f, %f\r\n\
plot3 = 0, 0, 0, 0, 0\r\n\
string3 = %*s %f, %*f, %*f, %f\r\n");

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSerialGraphForm::OnSize(UINT nType, int cx, int cy)
{
	CDockablePaneChildView::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	HandleAnchors(&rcWnd);

	CalcGraphWindowSize();
}



CPlotWindow* CSerialGraphForm::CreatePlotWindow(const int plotIndex)
{
	CRect rect;
	GetClientRect(rect);

	CPlotWindow *plot = new CPlotWindow();
	BOOL result = plot->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW | WS_CHILD,
		CRect(0, 0, rect.Width(), 100), this, AFX_IDW_PANE_FIRST);

	plot->SetScrollSizes(MM_TEXT, CSize(rect.Width() - 30, 100));
	plot->ShowWindow(SW_SHOW);

	plot->SetPlot(0, 0, 0, 0, 0);

	return plot;
}


// void CSerialGraphForm::OnClose()
// {
// 	// ȭ���� ��ĥ �� ȣ��ȴ�. Caption X button click
// 	g_MainDlg->ShowGraphWindow(false);
// 
// 	CDialogEx::OnClose();
// }


// str ������ �׷����� ������Ʈ �Ѵ�.
// str: ��Ʈ�� �迭
void CSerialGraphForm::SetString(const char *str)
{
	if (m_plotWindows.empty())
		return;

	for each (auto &plot in m_plotWindows)
	{
		float x, y;
		const int ret = sscanf_s(str, plot.scanString.c_str(), &x, &y);
		if (ret >= 2)
			plot.wnd->SetPlotXY(x, y);
	}
}


// ��Ʈ�� str���� plot ������ �м��ؼ� SPlotInfo ����ü�� ������ �����ϰ� �����Ѵ�.
// ��Ʈ�� �м��� ������ ����� false�� �����ϰ� ����ȴ�.
bool CSerialGraphForm::ParsePlotInfo(const int plotIndex, const wstring &str, SPlotInfo &out)
{
	CString plotStr, strStr;
	plotStr.Format(L"plot%d =", plotIndex + 1);
	strStr.Format(L"string%d =", plotIndex + 1);

	wstring plotParameters = ParseKeyValue(str, plotStr.GetBuffer());
	if (plotParameters.empty())
		return false;

	wstring strParameters = ParseKeyValue(str, strStr.GetBuffer());
	if (strParameters.empty())
		return false;

	vector<wstring> plotParams;
	common::wtokenizer(plotParameters, L",", L"", plotParams);
	if (plotParams.size() < 5)
		return false;

	out.xRange = (float)_wtof(plotParams[0].c_str());
	out.yRange = (float)_wtof(plotParams[1].c_str());
	out.xVisibleRange = (float)_wtof(plotParams[2].c_str());
	out.yVisibleRange = (float)_wtof(plotParams[3].c_str());
	out.flags = _wtoi(plotParams[4].c_str());

	out.scanString = wstr2str(strParameters);

	return true;
}


// key = value1, value2, value3 ������ ��Ʈ���� �Է¹޾�
// value1, value2 ~~  ���ڿ��� �����Ѵ�.
// �����ϸ� �� ���ڿ��� �����Ѵ�.
wstring CSerialGraphForm::ParseKeyValue(const wstring &str, const wstring &key)
{
	const int idx = str.find(key);
	if (idx == string::npos)
		return L"";

	const int beginIdx = str.find(L"=", idx);
	if (beginIdx == string::npos)
		return L"";

	const int endIdx = str.find(L"\n", idx);

	wstring ret = str.substr(beginIdx + 1, endIdx - beginIdx);
	return ret;
}


// �׷���â ũ�⿡ ���� plot window ũ�⸦ �����Ѵ�.
void CSerialGraphForm::CalcGraphWindowSize()
{
	if (GetSafeHwnd() == NULL)
		return;
	if (m_CommandEditor.GetSafeHwnd() == NULL)
		return;

	CRect cr;
	GetClientRect(cr);
	CRect editCr;
	m_CommandEditor.GetWindowRect(editCr);
	CRect btnR;
	m_UpdateButton.GetClientRect(btnR);


	// ȭ�鿡 �������� plot ������ ���Ѵ�.
	int plotWindowCount = 0;
	for each (auto &plot in m_plotWindows)
	{
		if (plot.wnd->IsWindowVisible())
			++plotWindowCount;
	}
	if (plotWindowCount <= 0)
		plotWindowCount = m_plotWindows.size(); // ó�� ���α׷��� ���۵� ��, 0 �� �ȴ�.

	const int plotSize = plotWindowCount;
	const int totalHeight = cr.Height() - editCr.Height() - btnR.Height() - 10;
	const int plotH = totalHeight / plotSize;

	int plotT = 0;
	for each (auto &it in m_plotWindows)
	{
		if (it.wnd)
			it.wnd->MoveWindow(CRect(0, plotT, cr.Width(), plotT + plotH));
		plotT += plotH;
	}

	// ���â �� Update��ư ��ġ�� ���� �ٲ��.
	m_CommandEditor.MoveWindow(CRect(0, cr.bottom - editCr.Height() - 5, cr.right, cr.bottom - 5));

	RedrawWindow();
}


// void CSerialGraphForm::OnBnClickedButtonHelp()
// {
// 	AfxMessageBox(
// L"�׷����� Fast 1 line ����� ���� �����Ѵ�.\n\n\
// plot = x-range, y-range, x-visible-range, y-visible-range, option \n\
// 	- x-range: 0 - auto \n\
// 	- y-range : 0 - auto \n\
// 	- x-visible - range : 0 - auto \n\
// 	- y-visible - range : 0 - auto \n\
// 	- option : \n\
// 		- x, y scroll \n\
// 		- line weight\n\
// \n\
// string = sscanf �� ���ڷ� �� �Ľ� ����\n\
// \n\
// ����)\n\
// 	- Serial Data\n\
// 		- Start, 1234, 1.11, 2.34, 3.33, 4.44\n\
// \n\
// 	- Plot Command Script\n\
// 		plot1 = 0, 0, 0, 0, 0\n\
// 		string1 = %*s %f, %f, %*f, %*f, %*f\n\
// \n\
// 		plot2 = 0, 0, 0, 0, 0\n\
// 		string2 = %*s %f, %*f, %f, %*f, %*f\n\
// ");
// }


// Plot Command Editor �� ��ɾ� ���ڿ��� ������ �����Ѵ�.
void CSerialGraphForm::ProcessPlotCommand()
{
	// ��ɾ� �ؽ�Ʈ ������.
	CString str;
	m_CommandEditor.GetWindowText(str);
	wstring wstr = (LPCTSTR)str;

	// plot1, string1, plot2, string2 ~~ ������� �о�´�. ���ٸ� ����.
	vector<SPlotInfo> plotInfos;
	plotInfos.reserve(10);
	for (int i = 0; i < 10; ++i)
	{
		SPlotInfo info;
		if (!ParsePlotInfo(i, wstr, info))
			break;
		plotInfos.push_back(info);
	}

	// ��ɹ��� ���� plot window�� �����Ѵ�.
	for (unsigned int i = 0; i < plotInfos.size(); ++i)
	{
		CPlotWindow *wnd;
		if (m_plotWindows.size() > i)
		{
			wnd = m_plotWindows[i].wnd;
		}
		else
		{
			wnd = CreatePlotWindow(i);
			m_plotWindows.push_back({ wnd, plotInfos[i].scanString });
		}

		// �׷����� ȭ�鿡 ���.
		wnd->ShowWindow(SW_SHOW);

		// �׷��� ����.
		wnd->SetPlot(
			plotInfos[i].xRange, plotInfos[i].yRange,
			plotInfos[i].xVisibleRange, plotInfos[i].yVisibleRange, plotInfos[i].flags);

		// �׷��� �Ľ� ��Ʈ�� ����.
		m_plotWindows[i].scanString = plotInfos[i].scanString;
	}


	// ������ plot window�� hide ��Ų��.
	for (unsigned int i = plotInfos.size(); i < m_plotWindows.size(); ++i)
	{
		m_plotWindows[i].wnd->ShowWindow(SW_HIDE);
	}

	m_plotWindowCount = plotInfos.size();

	// ��� ����� ���� ��, âũ�⸦ ������ �Ѵ�.
	CalcGraphWindowSize();
}


// �������Ӹ��� ȣ��Ǿ�� �Ѵ�.
// �׷����� �����Ѵ�.
void CSerialGraphForm::DrawGraph(const float deltaSeconds)
{
	for each(auto &plot in m_plotWindows)
	{
		plot.wnd->DrawPlot(deltaSeconds);
	}
}


void CSerialGraphForm::OnBnClickedButtonUpdate()
{
	ProcessPlotCommand();
	 
	// ������Ʈ �� ������, Plot ��ɾ �����Ѵ�.
	//g_MainDlg->SaveConfigFile();
}


// �ø��� ������ �Ѿ�� ��, ȣ��ȴ�.
void CSerialGraphForm::UpdateSerial(char *buffer)
{
	SetString(buffer);
}


// �������� ���� ȣ��ȴ�.
void CSerialGraphForm::Update(const float deltaSeconds)
{
	DrawGraph(deltaSeconds);
}


BOOL CSerialGraphForm::OnEraseBkgnd(CDC* pDC)
{
	// ��׶���� �������� �ʴ´�. (�ӵ��� �������ϱ�����)
	return CDockablePaneChildView::OnEraseBkgnd(pDC);
	//return TRUE;
}


void CSerialGraphForm::OnBnClickedCheckFixedmode()
{
 	for each(auto &plot in m_plotWindows)
 	{
		plot.wnd->SetFixedWidthMode(m_FixedModeButton.GetCheck() > 0);
 	}
}


void CSerialGraphForm::OnBnClickedButtonHelp()
{
 	AfxMessageBox(
 L"\n\
plot = x-range, y-range, x-visible-range, y-visible-range, option \n\
	- x-range: 0 - auto \n\
	- y-range : 0 - auto \n\
	- x-visible - range : 0 - auto \n\
	- y-visible - range : 0 - auto \n\
	- option : \n\
	 	- x, y scroll \n\
	 	- line weight\n\
\n\
string = sscanf �� ���ڷ� �� �Ľ� ����\n\
\n\
����)\n\
	- Serial Data\n\
		- Start, 1234, 1.11, 2.34, 3.33, 4.44\n\
\n\
	- Plot Command Script\n\
		plot1 = 0, 0, 0, 0, 0\n\
		string1 = %*s %f, %f, %*f, %*f, %*f\n\
\n\
		plot2 = 0, 0, 0, 0, 0\n\
		string2 = %*s %f, %*f, %f, %*f, %*f\n\
");
}
