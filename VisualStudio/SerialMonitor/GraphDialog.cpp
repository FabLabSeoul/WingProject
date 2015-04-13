// GraphDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SerialMonitor.h"
#include "GraphDialog.h"
#include "afxdialogex.h"
#include "PlotWindow.h"
#include "SerialMonitorDlg.h"
#include "stringfunc.h"


// CGraphDialog dialog

IMPLEMENT_DYNAMIC(CGraphDialog, CDialogEx)

CGraphDialog::CGraphDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGraphDialog::IDD, pParent)
{
	m_plotWindowCount = 0;
}

CGraphDialog::~CGraphDialog()
{
	}

void CGraphDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_COMMAND, m_CommandEditor);
	DDX_Control(pDX, IDC_BUTTON_UPDATE, m_UpdateButton);
	DDX_Control(pDX, IDC_BUTTON_HELP, m_HelpButton);
	DDX_Control(pDX, IDC_STATIC1, m_Static1);
	DDX_Control(pDX, IDC_CHECK_FIXEDMODE, m_FixedMode);
}


BEGIN_MESSAGE_MAP(CGraphDialog, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CGraphDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CGraphDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CGraphDialog::OnBnClickedButtonUpdate)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_HELP, &CGraphDialog::OnBnClickedButtonHelp)
	ON_BN_CLICKED(IDC_CHECK_FIXEDMODE, &CGraphDialog::OnBnClickedCheckFixedmode)
END_MESSAGE_MAP()


// CGraphDialog message handlers
BOOL CGraphDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_plotWindows.push_back({ CreatePlotWindow(0), "%*s %f, %*f, %f"});

	m_CommandEditor.SetWindowText(
		L"plot1 = 0,0,0,0,0 \r\n\
string1 = %*s %f, %*f, %f \r\n");
	
	CalcGraphWindowSize();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CGraphDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CGraphDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CGraphDialog::OnDestroy()
{
	CDialogEx::OnDestroy();

	for each (auto &it in m_plotWindows)
	{
		if (it.wnd)
		{
			it.wnd->DestroyWindow();
			delete it.wnd;
		}
	}
	m_plotWindows.clear();
}


CPlotWindow* CGraphDialog::CreatePlotWindow(const int plotIndex)
{
	CRect rect;
	GetClientRect(rect);

	CPlotWindow *plot = new CPlotWindow();
	BOOL result = plot->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, rect.Width(), 100), this, 0);
	plot->SetScrollSizes(MM_TEXT, CSize(rect.Width()-30, 100));
	plot->ShowWindow(SW_SHOW);

	plot->SetPlot(0, 0, 0, 0, 0);

	return plot;
}


// 그래프 창 크기가 바뀌면, Plot창도 크기가 따라 바뀐다.
void CGraphDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CalcGraphWindowSize();
}


void CGraphDialog::OnClose()
{
	// 화면이 닫칠 때 호출된다. Caption X button click
	g_MainDlg->ShowGraphWindow(false);

	CDialogEx::OnClose();
}


// str 정보로 그래프를 업데이트 한다.
// str: 스트링 배열
void CGraphDialog::SetString(const char *str)
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


// 명령창의 내용을 분석해서, 그래프 화면을 업데이트한다.
void CGraphDialog::OnBnClickedButtonUpdate()
{
	ProcessPlotCommand();

	// 업데이트 될 때마다, Plot 명령어를 저장한다.
	g_MainDlg->SaveConfigFile();
}


// 스트링 str에서 plot 정보를 분석해서 SPlotInfo 구조체에 정보를 저장하고 리턴한다.
// 스트링 분석에 문제가 생기면 false를 리턴하고 종료된다.
bool CGraphDialog::ParsePlotInfo(const int plotIndex, const wstring &str, SPlotInfo &out)
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

	out.scanString = common::wstr2str(strParameters);

	return true;
}


// key = value1, value2, value3 형태의 스트링을 입력받아
// value1, value2 ~~  문자열을 리턴한다.
// 실패하면 빈 문자열을 리턴한다.
wstring CGraphDialog::ParseKeyValue(const wstring &str, const wstring &key)
{
	const int idx = str.find(key);
	if (idx == string::npos)
		return L"";

	const int beginIdx = str.find(L"=", idx);
	if (beginIdx == string::npos)
		return L"";
	
	const int endIdx = str.find(L"\n", idx);

	wstring ret = str.substr(beginIdx+1, endIdx-beginIdx);
	return ret;
}


// 그래프창 크기에 따라 plot window 크기를 조정한다.
void CGraphDialog::CalcGraphWindowSize()
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
	CRect helpBtn;
	m_HelpButton.GetClientRect(helpBtn);
	CRect text1R;
	m_Static1.GetClientRect(text1R);
	CRect checkR;
	m_FixedMode.GetClientRect(checkR);

	// 화면에 보여지는 plot 개수를 구한다.
	int plotWindowCount = 0;
	for each (auto &plot in m_plotWindows)
	{
		if (plot.wnd->IsWindowVisible())
			++plotWindowCount;
	}
	if (plotWindowCount <= 0)
		plotWindowCount = m_plotWindows.size(); // 처음 프로그램이 시작될 때, 0 이 된다.

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

	// 명령창 과 Update버튼 위치도 따라 바뀐다.
	m_CommandEditor.MoveWindow(CRect(0, cr.bottom - editCr.Height() - 5, cr.right, cr.bottom - 5));

	m_UpdateButton.MoveWindow(CRect(
		cr.right - 5 - btnR.Width(),
		cr.bottom - editCr.Height() - btnR.Height() - 3 - 5,
		cr.right - 5,
		cr.bottom - editCr.Height() - 3 - 5));

	m_HelpButton.MoveWindow(CRect(
		cr.right - 5 - btnR.Width() - helpBtn.Width() - 5,
		cr.bottom - editCr.Height() - helpBtn.Height() - 3 - 5,
		cr.right - btnR.Width() - 5 - 5,
		cr.bottom - editCr.Height() - 3 - 5));

	m_Static1.MoveWindow(CRect(
		cr.left + 5,
		cr.bottom - editCr.Height() - text1R.Height() - 3 - 5,
		cr.left + 5 + text1R.Width(),
		cr.bottom - editCr.Height() - 3 - 5));

	m_FixedMode.MoveWindow(CRect(
		cr.left + 5 + text1R.Width() + 5,
		cr.bottom - editCr.Height() - checkR.Height() - 3 - 5,
		cr.left + 5 + text1R.Width() + checkR.Width() + 5,
		cr.bottom - editCr.Height() - 3 - 5));

	RedrawWindow();
}


void CGraphDialog::OnBnClickedButtonHelp()
{
	AfxMessageBox(
L"그래프는 Fast 1 line 모드일 때만 동작한다.\n\n\
plot = x-range, y-range, x-visible-range, y-visible-range, option \n\
	- x-range: 0 - auto \n\
	- y-range : 0 - auto \n\
	- x-visible - range : 0 - auto \n\
	- y-visible - range : 0 - auto \n\
	- option : \n\
		- x, y scroll \n\
		- line weight\n\
\n\
string = sscanf 의 인자로 들어갈 파싱 포맷\n\
\n\
예제)\n\
	- Serial Data\n\
		- Start, 1234, 1.11, 2.34, 3.33, 4.44\n\
\n\
	- Plot Command Script\n\
		plot1 = 0, 0, 0, 0, 0\n\
		string1 = %*s %f, %f, %*f, %*f, %*f\n\
\n\
		plot2 = 0, 0, 0, 0, 0\n\
		string2 = %*s %f, %*f, %f, %*f, %*f\n\
"
				);
}


// Plot Command Editor 의 명령어 문자열을 가져와 실행한다.
void CGraphDialog::ProcessPlotCommand()
{
	// 명령어 텍스트 가져옴.
	CString str;
	m_CommandEditor.GetWindowText(str);
	wstring wstr = (LPCTSTR)str;

	// plot1, string1, plot2, string2 ~~ 순서대로 읽어온다. 없다면 종료.
	vector<SPlotInfo> plotInfos;
	plotInfos.reserve(10);
	for (int i = 0; i < 10; ++i)
	{
		SPlotInfo info;
		if (!ParsePlotInfo(i, wstr, info))
			break;
		plotInfos.push_back(info);
	}

	// 명령문을 토대로 plot window를 생성한다.
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

		// 그래프를 화면에 출력.
		wnd->ShowWindow(SW_SHOW);

		// 그래프 설정.
		wnd->SetPlot(
			plotInfos[i].xRange, plotInfos[i].yRange,
			plotInfos[i].xVisibleRange, plotInfos[i].yVisibleRange, plotInfos[i].flags);

		// 그래프 파싱 스트링 설정.
		m_plotWindows[i].scanString = plotInfos[i].scanString;
	}


	// 나머지 plot window는 hide 시킨다.
	for (unsigned int i = plotInfos.size(); i < m_plotWindows.size(); ++i)
	{
		m_plotWindows[i].wnd->ShowWindow(SW_HIDE);
	}

	m_plotWindowCount = plotInfos.size();

	// 모든 계산이 끝난 후, 창크기를 재조정 한다.
	CalcGraphWindowSize();	
}


// 매프레임마다 호출되어야 한다.
// 그래프를 갱신한다.
void CGraphDialog::DrawGraph(const float deltaSeconds)
{
	for each(auto &plot in m_plotWindows)
	{
		plot.wnd->DrawPlot(deltaSeconds);
	}
}


void CGraphDialog::OnBnClickedCheckFixedmode()
{
	for each(auto &plot in m_plotWindows)
	{
		plot.wnd->SetFixedWidthMode(m_FixedMode.GetCheck() > 0);
	}
}
