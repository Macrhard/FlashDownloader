
// LoaderDlg.cpp : ʵ���ļ�
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
//״̬������

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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CLoaderDlg �Ի���



CLoaderDlg::CLoaderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOADER_DIALOG, pParent)
{
	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	oldComNum = 0;
	g_pMainDlg = this;
	//Ĭ�ϲ�����57600
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

//��Ϣӳ���
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
	
	//�Զ�����Ϣ
	ON_MESSAGE(WM_MAIN_MSG, &CLoaderDlg::OnMainMsg)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_COM, &CLoaderDlg::OnBnClickedButtonCloseCom)
END_MESSAGE_MAP()


// CLoaderDlg ��Ϣ�������

BOOL CLoaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();



	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//��ʼ�������ӶԻ���
	{
		m_tab.InsertItem(0, _T("FlashDownload"));	//insert the first table
		m_tab.InsertItem(1, _T("FlashUpload"));
		m_tab.InsertItem(2, _T("RF Test"));

		m_flashDownloadDlg.Create(IDD_DIALOG_FLASHDOWNLOAD, &m_tab);//creat the first table
		m_flashUploadDlg.Create(IDD_DIALOG_FLASHUPLOAD, &m_tab);
		m_rfloadDlg.Create(IDD_DIALOG_RFLOAD, &m_tab);

		CRect tabRect;
		m_tab.GetClientRect(&tabRect); //��ȡ��ǩ�ͻ���tabRect
										//����tabRect ʹ�串�Ƿ�Χ�ʺϷ��ñ�ǩ
		tabRect.left += 1;				//left ָ���˾��ε����Ͻǵ�x���ꡣ
		tabRect.top += 20;				//top ָ���˾��ε����Ͻǵ�y���ꡣ
		tabRect.right -= 2;				//right ָ���˾��ε����½ǵ�x���ꡣ
		tabRect.bottom -= 0;			//bottom ָ���˾��ε����½ǵ�y���ꡣ

		m_flashDownloadDlg.MoveWindow(&tabRect);
		m_flashUploadDlg.MoveWindow(&tabRect);
		m_rfloadDlg.MoveWindow(&tabRect);

		pDialog[0] = &m_flashDownloadDlg;
		pDialog[1] = &m_flashUploadDlg;
		pDialog[2] = &m_rfloadDlg;
		//Ĭ����ʾflashload ҳ��
		pDialog[0]->ShowWindow(SW_SHOW);
		pDialog[1]->ShowWindow(SW_HIDE);
		pDialog[2]->ShowWindow(SW_HIDE);
		m_CurTabSel = 0;
	}

	//��ʼ��״̬��
	{
		m_StatusBar.Create(this);
		m_StatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));
		m_StatusBar.SetPaneInfo(0, ID_INDICATOR_COM, SBPS_POPOUT, 50);//SBPS_STRETCH
		m_StatusBar.SetPaneInfo(1, ID_INDICATOR_SET, SBPS_POPOUT, 200);//SBPS_STRETCH
		m_StatusBar.SetPaneInfo(2, ID_INDICATOR_DATE, SBPS_NORMAL, 80);//SBPS_NORMAL
		m_StatusBar.SetPaneInfo(3, ID_INDICATOR_WEEK, SBPS_POPOUT, 85);//SBPS_NOBORDERS
		m_StatusBar.SetPaneInfo(4, ID_INDICATOR_TIME, SBPS_POPOUT,70);//SBPS_POPOUT
		m_StatusBar.SetPaneInfo(5, ID_INDICATOR_FLASHSIZE, SBPS_POPOUT, 160);//SBPS_STRETCH

		RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 130); //����λ��
		m_StatusBar.SetPaneText(0, _T("COM??"));
		m_StatusBar.SetPaneText(1, comBaudRate + _T(", None, 8bit, 1stop"));
		m_StatusBar.SetPaneText(5, _T("FlashSize??"));
		//������ʱ��
		SetDateTime();
		//������ʱ��
		SetTimer(1, 1000, NULL);//1000ms
	}

	TraversalCom();
	//������Ĭ����ʾ����Ϊ5���� 57600
	m_ComboBoxBaud.SetCurSel(5);

	UartState = UART_IDLE;
	UartResp = OK;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLoaderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CLoaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLoaderDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	pDialog[m_CurTabSel]->ShowWindow(SW_HIDE);
	m_CurTabSel = m_tab.GetCurSel();
	pDialog[m_CurTabSel]->ShowWindow(SW_SHOW);
	*pResult = 0;
}


/*
״̬����غ���
*/
//��ʾ����
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
//���ö�ʱ����ʾʱ��
void CLoaderDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString str;
	str = ((CTime)(CTime::GetCurrentTime())).Format(_T("%H:%M:%S"));
	m_StatusBar.SetPaneText(4, str);
	CDialogEx::OnTimer(nIDEvent);
}


/*
������غ���
*/

//��������
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
		m_ComboBoxCom.AddString(_T("û�п��õĴ���"));
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
		m_ComboBoxCom.AddString(_T("û�п��õĴ���"));
	}
#endif
}
//�����豸���±�������
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
//������Ͽ�����
void CLoaderDlg::OnCbnSelchangeComboCom()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ԭ��ʹ�õĴ��ڹر�
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
		MessageBox(_T("The serial port is occupied, please check it"), _T("������ʾ"), MB_OK | MB_ICONWARNING);
		m_MSComm.put_PortOpen(FALSE);
		m_StatusBar.SetPaneText(0, _T("COM??"));
		oldComNum = 0;
		return;
	}

	//���ô��ڲ���
	m_MSComm.put_Settings(comBaudRate + _T(",n,8,1")); //������ ��У�� ����λ ֹͣλ
	m_MSComm.put_InputMode(1); //���뷽ʽΪ�����Ʒ�ʽ
	m_MSComm.put_InputLen(0); //���õ�ǰ���������� 
	m_MSComm.put_InBufferSize(1024); //�������뻺����
	m_MSComm.put_InBufferCount(0);
	m_MSComm.put_OutBufferSize(1024); //�������������
	m_MSComm.put_OutBufferCount(0);
	m_MSComm.put_RThreshold(1); //ÿ�����ջ������и��ַ�����մ�������
	m_MSComm.put_PortOpen(TRUE); //�򿪴���

	m_StatusBar.SetPaneText(0, strCom);
	oldComNum = newComNum;
	/*m_MSComm.put_Output(COleVariant(_T("cnys")));*/
	/*downloadChildDlg->OnBnClickedButtonDownload.ChildoldComNum = oldComNum;*/
}
//��������Ͽ�����
void CLoaderDlg::OnCbnSelchangeComboBaud()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int index = m_ComboBoxBaud.GetCurSel();
	m_ComboBoxBaud.GetLBText(index, comBaudRate);
	//TRACE(comBaudRate);
	m_StatusBar.SetPaneText(1, comBaudRate + _T(", None, 8bit, 1stop"));
}
//FlashSize��Ͽ�����
void CLoaderDlg::OnCbnSelchangeComboFlashsize()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strFlashSize;
	int index = m_FlashSize.GetCurSel();
	m_FlashSize.GetLBText(index, strFlashSize);
	m_StatusBar.SetPaneText(5, _T("FlashSize:")+strFlashSize);
}

afx_msg LRESULT CLoaderDlg::OnMainMsg(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

//�����¼��������
BEGIN_EVENTSINK_MAP(CLoaderDlg, CDialogEx)
	ON_EVENT(CLoaderDlg, IDC_MSCOMM1, 1, CLoaderDlg::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()
void CLoaderDlg::OnCommMscomm1()
{
	g_pDownloadDlg = &(g_pMainDlg->m_flashDownloadDlg);
	// TODO: �ڴ˴������Ϣ����������
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

		//�ж�������ģʽ
		if (LoadType == Download)
		{
			while (log < uartLen)
			{
				//��ӡlog��Ϣ
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
		//�ж������ص���ʼ�׶λ��� ���ս׶�
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_MSComm.get_PortOpen()) //��������Ǵ򿪵ķ��� 1 ���رշ��� 0    ��رմ��� 
	{
		m_MSComm.put_PortOpen(FALSE);
		oldComNum = 0;
		m_StatusBar.SetPaneText(0, _T("COM??"));
		m_ComboBoxCom.SetCurSel(0); //��ComboBox_Com��ʾ��0������
	}
}
