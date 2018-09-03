
// LoaderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Loader.h"
//#include "LoaderDlg.h"
#include "afxdialogex.h"
#include "mscomm1.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//CFlashDownloadDlg *downloadChildDlg = new CFlashDownloadDlg();
//状态栏数组

CLoaderDlg *g_pMainDlg = NULL;


static UINT indicators[] =
{
	ID_INDICATOR_COM,
	ID_INDICATOR_SET,
	ID_INDICATOR_DATE,
	ID_INDICATOR_WEEK,
	ID_INDICATOR_TIME,
	ID_INDICATOR_FLASHSIZE,
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
	oldComNum = 0;
	g_pMainDlg = this;
	//默认波特率57600
	comBaudRate = _T("57600");
	
}

void CLoaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM1, m_MSComm);
	DDX_Control(pDX, IDC_TAB1, m_tab);
	DDX_Control(pDX, IDC_COMBO_COM, m_ComboBoxCom);
	DDX_Control(pDX, IDC_COMBO_BAUD, m_ComboBoxBaud);
	DDX_Control(pDX, IDC_COMBO_FLASHSIZE, m_FlashSize);
}

//消息映射宏
BEGIN_MESSAGE_MAP(CLoaderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CLoaderDlg::OnTcnSelchangeTab1)
	ON_WM_TIMER()

	ON_WM_DEVICECHANGE()
	ON_CBN_SELCHANGE(IDC_COMBO_BAUD, &CLoaderDlg::OnCbnSelchangeComboBaud)
	ON_CBN_SELCHANGE(IDC_COMBO_COM, &CLoaderDlg::OnCbnSelchangeComboCom)
	ON_CBN_SELCHANGE(IDC_COMBO_FLASHSIZE, &CLoaderDlg::OnCbnSelchangeComboFlashsize)
	
	//自定义消息
	ON_MESSAGE(WM_MAIN_MSG, &CLoaderDlg::OnMainMsg)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_COM, &CLoaderDlg::OnBnClickedButtonCloseCom)
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
		m_tab.InsertItem(2, _T("RF Test"));

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
		m_StatusBar.SetPaneInfo(0, ID_INDICATOR_COM, SBPS_POPOUT, 50);//SBPS_STRETCH
		m_StatusBar.SetPaneInfo(1, ID_INDICATOR_SET, SBPS_POPOUT, 200);//SBPS_STRETCH
		m_StatusBar.SetPaneInfo(2, ID_INDICATOR_DATE, SBPS_NORMAL, 80);//SBPS_NORMAL
		m_StatusBar.SetPaneInfo(3, ID_INDICATOR_WEEK, SBPS_POPOUT, 85);//SBPS_NOBORDERS
		m_StatusBar.SetPaneInfo(4, ID_INDICATOR_TIME, SBPS_POPOUT,70);//SBPS_POPOUT
		m_StatusBar.SetPaneInfo(5, ID_INDICATOR_FLASHSIZE, SBPS_POPOUT, 160);//SBPS_STRETCH

		RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 130); //放置位置
		m_StatusBar.SetPaneText(0, _T("COM??"));
		m_StatusBar.SetPaneText(1, comBaudRate + _T(", None, 8bit, 1stop"));
		m_StatusBar.SetPaneText(5, _T("FlashSize??"));
		//创建定时器
		SetDateTime();
		//启动定时器
		SetTimer(1, 1000, NULL);//1000ms
	}

	TraversalCom();
	//波特率默认显示索引为5的项 57600
	m_ComboBoxBaud.SetCurSel(5);

	UartState = UART_IDLE;
	UartResp = OK;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//CFlashDownloadDlg *g_pDownloadDlg = &(g_pMainDlg->m_flashDownloadDlg);

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


/*
状态栏相关函数
*/
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
//设置定时器显示时间
void CLoaderDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString str;
	str = ((CTime)(CTime::GetCurrentTime())).Format(_T("%H:%M:%S"));
	m_StatusBar.SetPaneText(4, str);
	CDialogEx::OnTimer(nIDEvent);
}


/*
串口相关函数
*/

//遍历串口
void CLoaderDlg::TraversalCom(void)
{
#if 0

	int total, num, place;
	num = m_ComboBoxCom.GetCount();
	for (int i = num - 1; i > 0; i--)
	{

		m_ComboBoxCom.DeleteString(i);
	}

	CArray<SSerInfo, SSerInfo&> asi;
	EnumSerialPorts(asi, FALSE);
	total = asi.GetSize();

	if (total > 0)
	{
		CString str;
		for (int i = 0; i<total; i++)
		{
			str = asi[i].strFriendlyName;
			place = str.Find(_T("COM"));
			num = _ttoi(str.Mid(place + 3, 2));
			if (num > 16)
			{
				continue;
			}
			if (num > 9)
			{
				str = str.Mid(place, 5);
			}
			else
			{
				str = str.Mid(place, 4);
			}

			if (asi[i].strFriendlyName.Find(_T("USB")) != -1)
			{
				str += _T("-USB");
			}
			m_ComboBoxCom.AddString(str);
		}
	}
	else
	{
		m_ComboBoxCom.AddString(_T("没有可用的串口"));
	}

	m_ComboBoxCom.SetCurSel(0);

#else

	int nCount = m_ComboBoxCom.GetCount();
	for (int i = nCount - 1; i > 0; i--)
	{
		m_ComboBoxCom.DeleteString(i);
	}
	HKEY    hKey;
	CString str;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Hardware\\DeviceMap\\SerialComm"), NULL, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		TCHAR szPortName[256], szComName[256];
		DWORD dwLong, dwSize;
		nCount = 0;
		while (1)
		{
			dwLong = dwSize = 256;
			if (RegEnumValue(hKey, nCount, szPortName, &dwLong, NULL, NULL, (PUCHAR)szComName, &dwSize) == ERROR_NO_MORE_ITEMS)
				break;

			str.Format(_T("%s"), szComName);
			m_ComboBoxCom.AddString(szComName);
			nCount++;
		}
		RegCloseKey(hKey);
	}
	else
	{
		m_ComboBoxCom.AddString(_T("没有可用的串口"));
	}
#endif
}
//更换设备重新遍历串口
BOOL CLoaderDlg::OnDeviceChange(UINT nEventType, DWORD dwData)
{
	//https://blog.csdn.net/dinjay/article/details/38320619
	switch (nEventType)
	{
	case DBT_DEVICEREMOVECOMPLETE:
	case DBT_DEVICEARRIVAL:
		TraversalCom();
		break;
	default:
		break;
	}
	return TRUE;
}
//串口组合框处理函数
void CLoaderDlg::OnCbnSelchangeComboCom()
{
	// TODO: 在此添加控件通知处理程序代码
	//把原先使用的串口关闭
	if (oldComNum)
	{
		m_MSComm.put_PortOpen(FALSE);
	}
	CString strCom;
	m_ComboBoxCom.GetLBText(m_ComboBoxCom.GetCurSel(), strCom);
	if (strCom.Find(_T("COM")) == -1)
	{
		MessageBox(_T("Illegal serial port, please check it"), _T("Tips"), MB_OK | MB_ICONWARNING);
		oldComNum = 0;
		m_StatusBar.SetPaneText(0, _T("COM??"));
		return;
	}
	short newComNum = _ttoi(strCom.Mid(3));
	m_MSComm.put_CommPort(newComNum);
	if (m_MSComm.get_PortOpen())
	{
		MessageBox(_T("The serial port is occupied, please check it"), _T("友情提示"), MB_OK | MB_ICONWARNING);
		m_MSComm.put_PortOpen(FALSE);
		m_StatusBar.SetPaneText(0, _T("COM??"));
		oldComNum = 0;
		return;
	}

	//设置串口参数
	m_MSComm.put_Settings(comBaudRate + _T(",n,8,1")); //波特率 无校验 数据位 停止位
	m_MSComm.put_InputMode(1); //输入方式为二级制方式
	m_MSComm.put_InputLen(0); //设置当前缓冲区长度 
	m_MSComm.put_InBufferSize(1024); //设置输入缓冲区
	m_MSComm.put_InBufferCount(0);
	m_MSComm.put_OutBufferSize(1024); //设置输出缓冲区
	m_MSComm.put_OutBufferCount(0);
	m_MSComm.put_RThreshold(1); //每当接收缓冲区有个字符则接收串口数据
	m_MSComm.put_PortOpen(TRUE); //打开串口

	m_StatusBar.SetPaneText(0, strCom);
	oldComNum = newComNum;
	/*m_MSComm.put_Output(COleVariant(_T("cnys")));*/
	/*downloadChildDlg->OnBnClickedButtonDownload.ChildoldComNum = oldComNum;*/
}
//波特率组合框处理函数
void CLoaderDlg::OnCbnSelchangeComboBaud()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_ComboBoxBaud.GetCurSel();
	m_ComboBoxBaud.GetLBText(index, comBaudRate);
	//TRACE(comBaudRate);
	m_StatusBar.SetPaneText(1, comBaudRate + _T(", None, 8bit, 1stop"));
}
//FlashSize组合框处理函数
void CLoaderDlg::OnCbnSelchangeComboFlashsize()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFlashSize;
	int index = m_FlashSize.GetCurSel();
	m_FlashSize.GetLBText(index, strFlashSize);
	m_StatusBar.SetPaneText(5, _T("FlashSize:")+strFlashSize);
}

afx_msg LRESULT CLoaderDlg::OnMainMsg(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

//串口事件处理程序
BEGIN_EVENTSINK_MAP(CLoaderDlg, CDialogEx)
	ON_EVENT(CLoaderDlg, IDC_MSCOMM1, 1, CLoaderDlg::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()
void CLoaderDlg::OnCommMscomm1()
{
	g_pDownloadDlg = &(g_pMainDlg->m_flashDownloadDlg);
	// TODO: 在此处添加消息处理程序代码
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	LONG  k;

	if (m_MSComm.get_CommEvent() == 2) 
	{
		variant_inp = m_MSComm.get_Input(); 
		safearray_inp = variant_inp; 
		uartLen = safearray_inp.GetOneDimSize();
		for (k = 0; k < uartLen; k++)
		{
			safearray_inp.GetElement(&k, rxdata + k); 
		}
		m_MSComm.put_InBufferCount(0);
		k = 0x5AA56996;
		BYTE log = 0;
		BYTE logLen = 0;

		//判断上下载模式
		if (LoadType == Download)
		{
			while (log < uartLen)
			{
				//打印log信息
				if ((rxdata[log] == 'L'))
				{
					logLen = rxdata[log + 1];
					rxdata[log + logLen] = '\0';
					rxdata[log + 0] = '-';
					rxdata[log + 1] = ' ';
					
					g_pDownloadDlg->m_ListboxLog.AddString((CString)(rxdata + log));
					log += logLen + 2;
				}
				else
				{
					k = rxdata[log];
					log += 1;
				}
			}
			if (k != 0x5AA56996)
			{
				if ((k == 0xFF) || (k < 0x05))
				{
					switch (UartState)
					{
					case UART_IDLE:
						break;
					case UART_SYNC:
					case UART_ADDR:
					case UART_CODE:
						UartResp = (enum _UartResponse)k;
						DownloadEvent.SetEvent();
						return;
					default:
						break;
					}
				}
			}
		}
		//判断是上载的起始阶段还是 接收阶段
		if (LoadType == Upload)
		{
			if (rxdata[0] == 2)
			{
				LoadType = SYNC;
				UploadEvent.SetEvent();
				return;
			}
		}
		else
		{
			//loadType = StartUpload;  //1
			UploadEvent.SetEvent();
		}
	}
	
}

void CLoaderDlg::OnBnClickedButtonCloseCom()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_MSComm.get_PortOpen()) //如果串口是打开的返回 1 ，关闭返回 0    则关闭串口 
	{
		m_MSComm.put_PortOpen(FALSE);
		oldComNum = 0;
		m_StatusBar.SetPaneText(0, _T("COM??"));
		m_ComboBoxCom.SetCurSel(0); //让ComboBox_Com显示第0项内容
	}
}
