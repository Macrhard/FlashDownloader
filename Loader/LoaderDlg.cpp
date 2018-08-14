
// LoaderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Loader.h"
#include "LoaderDlg.h"
#include "afxdialogex.h"
#include "mscomm1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//状态栏数组
static UINT indicators[] =
{
	ID_INDICATOR_COM,
	ID_INDICATOR_SET,
	ID_INDICATOR_DATE,
	ID_INDICATOR_WEEK,
	ID_INDICATOR_TIME,
};

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLoaderDlg 对话框



CLoaderDlg::CLoaderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOADER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM1, m_MSComm);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(CLoaderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CLoaderDlg::OnTcnSelchangeTab1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CLoaderDlg 消息处理程序

BOOL CLoaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//初始化三个子对话框
	{
		m_tab.InsertItem(0, _T("FlashDownload"));	//insert the first table
		m_tab.InsertItem(1, _T("FlashUpload"));
		m_tab.InsertItem(2, _T("RFload"));

		m_flashDownloadDlg.Create(IDD_DIALOG_FLASHDOWNLOAD, &m_tab);//creat the first table
		m_flashUploadDlg.Create(IDD_DIALOG_FLASHUPLOAD, &m_tab);
		m_rfloadDlg.Create(IDD_DIALOG_RFLOAD, &m_tab);

		CRect tabRect;
		m_tab.GetClientRect(&tabRect); //获取标签客户区tabRect
										//调整tabRect 使其覆盖范围适合放置标签
		tabRect.left += 1;				//left 指定了矩形的左上角的x坐标。
		tabRect.top += 20;				//top 指定了矩形的左上角的y坐标。
		tabRect.right -= 2;				//right 指定了矩形的右下角的x坐标。
		tabRect.bottom -= 0;			//bottom 指定了矩形的右下角的y坐标。

		m_flashDownloadDlg.MoveWindow(&tabRect);
		m_flashUploadDlg.MoveWindow(&tabRect);
		m_rfloadDlg.MoveWindow(&tabRect);

		pDialog[0] = &m_flashDownloadDlg;
		pDialog[1] = &m_flashUploadDlg;
		pDialog[2] = &m_rfloadDlg;
		//默认显示flashload 页面
		pDialog[0]->ShowWindow(SW_SHOW);
		pDialog[1]->ShowWindow(SW_HIDE);
		pDialog[2]->ShowWindow(SW_HIDE);
		m_CurTabSel = 0;
	}

	//初始化状态栏
	{
		m_StatusBar.Create(this);
		m_StatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));
		m_StatusBar.SetPaneInfo(0, ID_INDICATOR_COM, SBPS_NORMAL, 60);//SBPS_STRETCH
		m_StatusBar.SetPaneInfo(1, ID_INDICATOR_SET, SBPS_STRETCH, 0);//SBPS_STRETCH
		m_StatusBar.SetPaneInfo(2, ID_INDICATOR_DATE, SBPS_NORMAL, 120);//SBPS_NORMAL
		m_StatusBar.SetPaneInfo(3, ID_INDICATOR_WEEK, SBPS_NORMAL, 60);//SBPS_NOBORDERS
		m_StatusBar.SetPaneInfo(4, ID_INDICATOR_TIME, SBPS_NORMAL, 70);//SBPS_POPOUT
		RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0); //放置位置
		m_StatusBar.SetPaneText(0, _T("COM??"));
		//m_StatusBar.SetPaneText(1, comBaudRate + _T(", None, 8bit, 1stop"));
		SetDateTime();
		SetTimer(1, 1000, NULL);
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CLoaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLoaderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CLoaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLoaderDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	pDialog[m_CurTabSel]->ShowWindow(SW_HIDE);
	m_CurTabSel = m_tab.GetCurSel();
	pDialog[m_CurTabSel]->ShowWindow(SW_SHOW);
	*pResult = 0;
}

//显示日期
void CLoaderDlg::SetDateTime()
{
	CString str;
	int year, month, day, week;

	year = CTime::GetCurrentTime().GetYear();
	month = CTime::GetCurrentTime().GetMonth();
	day = CTime::GetCurrentTime().GetDay();
	week = CTime::GetCurrentTime().GetDayOfWeek();
	str.Format(_T("%d-%d-%d"), year, month, day, week);
	m_StatusBar.SetPaneText(2, str);

	switch (week)
	{
	case 1: str = _T("Sunday"); break;
	case 2: str = _T("Monday"); break;
	case 3: str = _T("Tuesday"); break;
	case 4: str = _T("Wednesday"); break;
	case 5: str = _T("Thursday"); break;
	case 6: str = _T("Friday"); break;
	case 7: str = _T("Saturday"); break;
	default:str = _T("****"); break;
	}
	m_StatusBar.SetPaneText(3, str);
}


void CLoaderDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString str;
	str = ((CTime)(CTime::GetCurrentTime())).Format(_T("%H:%M:%S"));
	m_StatusBar.SetPaneText(4, str);

	CDialogEx::OnTimer(nIDEvent);
}
