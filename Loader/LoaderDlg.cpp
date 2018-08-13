
// LoaderDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Loader.h"
#include "LoaderDlg.h"
#include "afxdialogex.h"
#include "mscomm1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
	m_tab.InsertItem(0, _T("FlashDownload"));	//insert the first table
	m_tab.InsertItem(1, _T("FlashUpload"));
	m_tab.InsertItem(2, _T("RFload"));

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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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
