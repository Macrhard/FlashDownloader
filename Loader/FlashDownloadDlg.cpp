// FlashDownloadDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Loader.h"
#include "FlashDownloadDlg.h"
#include "afxdialogex.h"


CFlashDownloadDlg *g_pDownloadDlg = NULL;
#define  PAGE_LEN  (1*1024)
#define  MAX_FILE  (8) //�����ص�����ļ���
CString Path[8] = { NULL };
CString Addr[8] = { NULL };


/*
generate nv file 
*/


#define  NV_KB          (1024)
#define  NV_MB          (NV_KB*NV_KB)
#define  NV_FLASH_SIZE  (1*NV_MB)                     /* flash total size = 1MB */
#define  NV_FLASH_PAGE  (4*NV_KB)                     /* flash erase size = 4KB */
#define  NV_FLASH_ADDR  (NV_FLASH_SIZE-NV_FLASH_PAGE) /* store in last page address */

typedef struct _conf_file
{
	unsigned short index;        /* 0,1..MAX_AP_NUM-1 */
	unsigned char  ssid[33];     /* 32+'\0' */
	unsigned char  pswd[33]; /* 32+'\0' */
}config_file;

#define  NV_NUM_MAX_AP     (5)
#define  NV_LEN_CONF_FILE  (sizeof(config_file))
#define  NV_LEN_MAC_ADDR   (18)
#define  NV_LEN_IP_ADDR    (16)
#define  NV_LEN_MASK_ADDR  (16)
#define  NV_LEN_GW_ADDR    (16)

typedef struct _nv_param
{
	unsigned char mac[NV_LEN_MAC_ADDR];  /* 12-34-56-78-9A-BC */
	unsigned char ip[NV_LEN_IP_ADDR];    /* 192.168.0.123     */
	unsigned char msk[NV_LEN_MASK_ADDR]; /* 255.255.255.0     */
	unsigned char gw[NV_LEN_GW_ADDR];    /* 192.168.0.1       */
	config_file conf_file[NV_NUM_MAX_AP];
}nv_param;

nv_param param;




struct _fileInfo
{
	int fileType; //fileType ��ָ�����ش��ļ������� uboot��andes xip ��
	DWORD fileAddr;
}fileInfo[MAX_FILE];

// CFlashDownloadDlg �Ի���

IMPLEMENT_DYNAMIC(CFlashDownloadDlg, CDialogEx)

CFlashDownloadDlg::CFlashDownloadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_FLASHDOWNLOAD, pParent)
{

}

CFlashDownloadDlg::~CFlashDownloadDlg()
{
}

void CFlashDownloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_check1);
	DDX_Control(pDX, IDC_CHECK2, m_check2);
	DDX_Control(pDX, IDC_CHECK3, m_check3);
	DDX_Control(pDX, IDC_CHECK4, m_check4);
	DDX_Control(pDX, IDC_CHECK5, m_check5);
	DDX_Control(pDX, IDC_CHECK6, m_check6);
	DDX_Control(pDX, IDC_CHECK7, m_check7);
	DDX_Control(pDX, IDC_CHECK8, m_check8);
	DDX_Control(pDX, IDC_EDIT_PATH2, m_path2);
	DDX_Control(pDX, IDC_EDIT_PATH1, m_path1);
	DDX_Control(pDX, IDC_EDIT_PATH3, m_path3);
	DDX_Control(pDX, IDC_EDIT_PATH4, m_path4);
	DDX_Control(pDX, IDC_EDIT_PATH5, m_path5);
	DDX_Control(pDX, IDC_EDIT_PATH6, m_path6);
	DDX_Control(pDX, IDC_EDIT_PATH7, m_path7);
	DDX_Control(pDX, IDC_EDIT_PATH8, m_path8);
	DDX_Control(pDX, IDC_LIST1, m_ListboxLog);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
}


BEGIN_MESSAGE_MAP(CFlashDownloadDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OPNE1, &CFlashDownloadDlg::OnBnClickedButtonOpne1)
	ON_BN_CLICKED(IDC_BUTTON_OPEN2, &CFlashDownloadDlg::OnBnClickedButtonOpen2)
	ON_BN_CLICKED(IDC_BUTTON_OPEN3, &CFlashDownloadDlg::OnBnClickedButtonOpen3)
	ON_BN_CLICKED(IDC_BUTTON_OPEN4, &CFlashDownloadDlg::OnBnClickedButtonOpen4)
	ON_BN_CLICKED(IDC_BUTTON_OPEN5, &CFlashDownloadDlg::OnBnClickedButtonOpen5)
	ON_BN_CLICKED(IDC_BUTTON_OPEN6, &CFlashDownloadDlg::OnBnClickedButtonOpen6)
	ON_BN_CLICKED(IDC_BUTTON_OPEN7, &CFlashDownloadDlg::OnBnClickedButtonOpen7)
	ON_BN_CLICKED(IDC_BUTTON_OPEN8, &CFlashDownloadDlg::OnBnClickedButtonOpen8)
	ON_BN_CLICKED(IDC_BUTTON_CLN1, &CFlashDownloadDlg::OnBnClickedButtonCln1)
	ON_BN_CLICKED(IDC_BUTTON_CLN2, &CFlashDownloadDlg::OnBnClickedButtonCln2)
	ON_BN_CLICKED(IDC_BUTTON_CLN3, &CFlashDownloadDlg::OnBnClickedButtonCln3)
	ON_BN_CLICKED(IDC_BUTTON_CLN4, &CFlashDownloadDlg::OnBnClickedButtonCln4)
	ON_BN_CLICKED(IDC_BUTTON_CLN5, &CFlashDownloadDlg::OnBnClickedButtonCln5)
	ON_BN_CLICKED(IDC_BUTTON_CLN6, &CFlashDownloadDlg::OnBnClickedButtonCln6)
	ON_BN_CLICKED(IDC_BUTTON_CLN7, &CFlashDownloadDlg::OnBnClickedButtonCln7)
	ON_BN_CLICKED(IDC_BUTTON_CLN8, &CFlashDownloadDlg::OnBnClickedButtonCln8)
	ON_BN_CLICKED(IDC_BUTTON_CLNALL, &CFlashDownloadDlg::OnBnClickedButtonClnall)
	ON_BN_CLICKED(IDC_BUTTON_COMBINE, &CFlashDownloadDlg::OnBnClickedButtonCombine)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD, &CFlashDownloadDlg::OnBnClickedButtonDownload)
	//�Զ�����Ϣ
	ON_MESSAGE(WM_DOWNLOAD_MSG, &CFlashDownloadDlg::OnDownloadMsg)
	ON_BN_CLICKED(IDC_BUTTON_GENERATE_NV, &CFlashDownloadDlg::OnBnClickedButtonGenerateNv)
END_MESSAGE_MAP()

// CFlashDownloadDlg ��Ϣ�������

void CFlashDownloadDlg::OnBnClickedButtonOpne1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SelcetFile(0, IDC_EDIT_PATH1, IDC_EDIT_ADDR1);
	m_check1.SetCheck(1);
}


void CFlashDownloadDlg::OnBnClickedButtonOpen2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SelcetFile(1, IDC_EDIT_PATH2, IDC_EDIT_ADDR2);
	m_check2.SetCheck(1);
}


void CFlashDownloadDlg::OnBnClickedButtonOpen3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SelcetFile(2, IDC_EDIT_PATH3, IDC_EDIT_ADDR3);
	m_check3.SetCheck(1);
}


void CFlashDownloadDlg::OnBnClickedButtonOpen4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SelcetFile(3, IDC_EDIT_PATH4, IDC_EDIT_ADDR4);
	m_check4.SetCheck(1);
}


void CFlashDownloadDlg::OnBnClickedButtonOpen5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SelcetFile(4, IDC_EDIT_PATH5, IDC_EDIT_ADDR5);
	m_check5.SetCheck(1);
}


void CFlashDownloadDlg::OnBnClickedButtonOpen6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SelcetFile(5, IDC_EDIT_PATH6, IDC_EDIT_ADDR6);
	m_check6.SetCheck(1);
}


void CFlashDownloadDlg::OnBnClickedButtonOpen7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SelcetFile(6, IDC_EDIT_PATH7, IDC_EDIT_ADDR7);
	m_check7.SetCheck(1);
}


void CFlashDownloadDlg::OnBnClickedButtonOpen8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SelcetFile(7, IDC_EDIT_PATH8, IDC_EDIT_ADDR8);
	m_check8.SetCheck(1);
}


void CFlashDownloadDlg::OnBnClickedButtonCln1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_check1.SetCheck(0);
	Path[0] = "";
	SetDlgItemText(IDC_EDIT_PATH1,_T(""));
	SetDlgItemText(IDC_EDIT_ADDR1, _T(""));
}


void CFlashDownloadDlg::OnBnClickedButtonCln2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_check2.SetCheck(0);
	Path[1] = "";
	SetDlgItemText(IDC_EDIT_PATH2, _T(""));
	SetDlgItemText(IDC_EDIT_ADDR2, _T(""));
}


void CFlashDownloadDlg::OnBnClickedButtonCln3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_check3.SetCheck(0);
	Path[2] = "";
	SetDlgItemText(IDC_EDIT_PATH3, _T(""));
	SetDlgItemText(IDC_EDIT_ADDR3, _T(""));

}


void CFlashDownloadDlg::OnBnClickedButtonCln4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_check4.SetCheck(0);
	Path[3] = "";
	SetDlgItemText(IDC_EDIT_PATH4, _T(""));
	SetDlgItemText(IDC_EDIT_ADDR4, _T(""));
}


void CFlashDownloadDlg::OnBnClickedButtonCln5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_check5.SetCheck(0);
	Path[4] = "";
	SetDlgItemText(IDC_EDIT_PATH5, _T(""));
	SetDlgItemText(IDC_EDIT_ADDR5, _T(""));
}


void CFlashDownloadDlg::OnBnClickedButtonCln6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_check6.SetCheck(0);
	Path[5] = "";
	SetDlgItemText(IDC_EDIT_PATH6, _T(""));
	SetDlgItemText(IDC_EDIT_ADDR6, _T(""));
}


void CFlashDownloadDlg::OnBnClickedButtonCln7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_check7.SetCheck(0);
	Path[6] = "";
	SetDlgItemText(IDC_EDIT_PATH7, _T(""));
	SetDlgItemText(IDC_EDIT_ADDR7, _T(""));
}


void CFlashDownloadDlg::OnBnClickedButtonCln8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_check8.SetCheck(0);
	Path[7] = "";
	SetDlgItemText(IDC_EDIT_PATH8, _T(""));
	SetDlgItemText(IDC_EDIT_ADDR8, _T(""));
}

/*
Function by self
*/
void CFlashDownloadDlg::SelcetFile(int index, int pathID, int addrID)
{
	CString szFilters = _T("�������ļ�(*.bin)|*.bin||");
	CFileDialog Filedlg(1, 0, 0, OFN_HIDEREADONLY | OFN_EXPLORER, szFilters, 0);
	if (Filedlg.DoModal() == IDOK)
	{
		CString path = NULL;
		CString option = NULL;
		path = Filedlg.GetPathName();
		option = Filedlg.GetFileTitle();
		option.MakeLower();
		if (option.Find(_T("uboot")) != -1)
		{
			SetDlgItemText(pathID, path);
			fileInfo[index].fileType = g_pMainDlg->UBOOT;
			Addr[index] = GetConfigInfo(_T("address"), _T("uboot"));
			SetDlgItemText(addrID, Addr[index]);
			fileInfo[index].fileAddr = _tcstoul(Addr[index], NULL, 16);
		}
		else if (option.Find(_T("andes")) != -1)
		{
			SetDlgItemText(pathID, path);
			fileInfo[index].fileType = g_pMainDlg->ANDES;
			Addr[index] = GetConfigInfo(_T("address"), _T("andes"));
			SetDlgItemText(addrID, Addr[index]);
			fileInfo[index].fileAddr = _tcstoul(Addr[index], NULL, 16);
		}
		else if (option.Find(_T("andes1")) != -1)
		{
			SetDlgItemText(pathID, path);
			fileInfo[index].fileType = g_pMainDlg->ANDES1;
			Addr[index] = GetConfigInfo(_T("address"), _T("andes1"));
			SetDlgItemText(addrID, Addr[index]);
			fileInfo[index].fileAddr = _tcstoul(Addr[index], NULL, 16);
		}
		else if (option.Find(_T("nv")) != -1)
		{
			SetDlgItemText(pathID, path);
			fileInfo[index].fileType = g_pMainDlg->NV;
			Addr[index] = GetConfigInfo(_T("address"), _T("nv"));
			SetDlgItemText(addrID, Addr[index]);
			fileInfo[index].fileAddr = _tcstoul(Addr[index], NULL, 16);
		}
		else
		{
			MessageBox(_T("File type isn't match, please check it"));
			return;
		}
	}
}

//��ȡsystem.ini�ļ�����Ҫ����Ϣ�����ص����ô�
CString CFlashDownloadDlg::GetConfigInfo(CString title, CString option)
{

	//�����������һ�������ж��Ƿ�����ļ�system.ini �����ڽ��ᰴ��Ĭ�ϸ�ʽ����һ��
	CString buff;
	GetPrivateProfileString(title, option, CString("NULL"), buff.GetBuffer(MAX_PATH), MAX_PATH, _T(".\\system.ini"));
	return buff;
}

void ThrowException(int index)
{
	if (index == 0)
	{
		//MessageBox(_T("File type isn't match, please check it"));
	}
}

void CFlashDownloadDlg::OnBnClickedButtonClnall()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnBnClickedButtonCln1();
	OnBnClickedButtonCln2();
	OnBnClickedButtonCln3();
	OnBnClickedButtonCln4();
	OnBnClickedButtonCln5();
	OnBnClickedButtonCln6();
	OnBnClickedButtonCln7();
	OnBnClickedButtonCln8();
}


void CFlashDownloadDlg::OnBnClickedButtonCombine()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char* combineFile = (char*) malloc(0x1048510);//
	DWORD head = 0;
	GetFilePath();
	for (int i = 0; i < 8; i++)
	{
		if (Path[i] == "")
		{
			continue;
		}
		head = ReadFile(Path[i], i, combineFile,head);
	}
	CFile combineBinfile(_T(".\\multibinCombineFile.bin"), CFile::modeCreate|CFile::modeWrite);
	combineBinfile.Write(combineFile,head);
	combineBinfile.Close();
	free(combineFile);
}

DWORD CFlashDownloadDlg::ReadFile(CString filePath,int addIndex,char* combinFile, DWORD head)
{
	
	BYTE *fileBuffer = NULL;
	DWORD fileLength;
	int fileType;
	fileType = ReturnFileType(filePath);

	CFile binfileStream(filePath, CFile::modeRead | CFile::shareDenyWrite);
	fileLength = binfileStream.GetLength();
	fileBuffer = new BYTE[fileLength];
	binfileStream.Read(fileBuffer, fileLength);
	binfileStream.Close();
	if (fileBuffer == NULL)
	{
		//TODO error deal
		ThrowException(0);
	}


	BYTE myByteArray[12] = { 0 };
	myByteArray[0] = (BYTE)(fileType);
	myByteArray[1] = 0;
	myByteArray[2] = 0;
	myByteArray[3] = 0;
	myByteArray[4] = (BYTE)(fileInfo[addIndex].fileAddr);
	myByteArray[5] = (BYTE)((fileInfo[addIndex].fileAddr)>>8);
	myByteArray[6] = (BYTE)((fileInfo[addIndex].fileAddr)>>16);
	myByteArray[7] = (BYTE)((fileInfo[addIndex].fileAddr)>>24);
	myByteArray[8] = (BYTE)(fileLength);
	myByteArray[9] = (BYTE)(fileLength >> 8);
	myByteArray[10] = (BYTE)(fileLength >> 16);
	myByteArray[10] = (BYTE)(fileLength >> 24);
	memcpy((combinFile+head), myByteArray,12);
	memcpy(((combinFile + head) + 12), fileBuffer, fileLength);
	head = head + 12 + fileLength;
	return head;

}



//������ʾ��·���Ի����в��Ҵ򹴵��ļ�·�� ��Path[]��
void CFlashDownloadDlg::GetFilePath(void)
{
	if (m_check1.GetCheck() == 1)
	{


		m_path1.GetWindowText(Path[0]);
	}
	else
	{
		Path[0] = "";
	}

	if (m_check2.GetCheck() == 1)
	{
		m_path2.GetWindowText(Path[1]);
	}
	else
	{
		Path[1] = "";
	}

	if (m_check3.GetCheck() == 1)
	{
		m_path3.GetWindowText(Path[2]);
	}
	else
	{
		Path[2] = "";
	}

	if (m_check4.GetCheck() == 1)
	{
		m_path4.GetWindowText(Path[3]);
	}
	else
	{
		Path[3] = "";
	}

	if (m_check5.GetCheck() == 1)
	{
		m_path5.GetWindowText(Path[4]);
	}
	else
	{
		Path[4] = "";
	}

	if (m_check6.GetCheck() == 1)
	{
		m_path6.GetWindowText(Path[5]);
	}
	else
	{
		Path[5] = "";
	}

	if (m_check7.GetCheck() == 1)
	{
		m_path7.GetWindowText(Path[6]);
	}
	else
	{
		Path[6] = "";
	}

	if (m_check8.GetCheck() == 1)
	{
		m_path8.GetWindowText(Path[7]);
	}
	else
	{
		Path[7] = "";
	}
}

//�����ļ�����
int CFlashDownloadDlg::ReturnFileType(CString filePath)
{
	filePath.MakeLower();
	
	if (filePath.Find(_T("uboot")) != -1)
	{
		return g_pMainDlg->UBOOT;
	}
	else if (filePath.Find(_T("andes")) != -1)
	{
		if (filePath.Find(_T("andes1")) != -1)
		{
			return g_pMainDlg->ANDES1;
		}
		else
		{
			return g_pMainDlg->ANDES;

		}
	}
	else if(filePath.Find(_T("nv")) != -1)
	{
		return g_pMainDlg->NV;
	}
	return 0;
}

void CFlashDownloadDlg::ThrowTips(int tipsIndex)
{
	switch (tipsIndex)
	{
	case 1:
		MessageBox(_T("��ѡ��Ҫ���ص��ļ�"), _T("Tips"), MB_OK | MB_ICONWARNING);
		EnableWindow();
		break;
	default:
		MessageBox(_T("δ֪����"), _T("Tips"), MB_OK | MB_ICONWARNING);
		EnableWindow();
		break;
	}
}

void CFlashDownloadDlg::OnBnClickedButtonDownload()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_pMainDlg->DownloadEvent.ResetEvent();
	g_pMainDlg->LoadType = g_pMainDlg->Download;
	GetFilePath();
	int fileCount = 0;
	for (int cnt = 0; cnt < 8; cnt++)
	{
		if (Path[cnt] != "")
		{
			fileCount++;
		}
	}
	if (fileCount < 1)
	{
		MessageBox(_T("Please selcet the file to download"), _T("Tips"), MB_OK | MB_ICONWARNING);
		return;
	}
	if (g_pMainDlg->oldComNum == 0)
	{
		MessageBox(_T("Please selcet a appropriate serial port"), _T("Tips"), MB_OK | MB_ICONWARNING);
		return;
	}
	DisableWindow();
	pUartThread = AfxBeginThread(UartDownload, this, THREAD_PRIORITY_NORMAL, 0, 0);
}
//CLoaderDlg *pMainDlg = (CLoaderDlg*)GetParent()->GetParent();

//����ֳ��ֳ��ĺ��� ����Ҫ��д
UINT CFlashDownloadDlg::UartDownload(LPVOID pParam)
{
	CFlashDownloadDlg *ptr = (CFlashDownloadDlg*)pParam;

	ptr->m_ListboxLog.ResetContent();//���Log���е�����
	ptr->m_ListboxLog.AddString(_T("+ Start downloading"));
	ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);

	CByteArray byteArray;
	int loop = 0;
	do {
			int j = 0;
			if (g_pMainDlg->UartState == g_pMainDlg->UART_IDLE)
			{
				g_pMainDlg->UartState = g_pMainDlg->UART_SYNC;
				int n = 0;
				while (++n)
				{
					g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("cnys")));
					if (WaitForSingleObject(g_pMainDlg->DownloadEvent, 5000) == WAIT_OBJECT_0)
					{
						break;
					}
					else
					{
						if (n > 3)
						{
							ptr->m_ListboxLog.AddString(_T("+ Stop upload, serial port timeout"));
							ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
							AfxMessageBox(_T("Sorry, serial port timeout"));
							goto error_return;
						}
						ptr->m_ListboxLog.AddString(_T("+ Handshaking......"));
						ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
					}
				}
				switch (g_pMainDlg->UartResp)  //�ȴ�MCU�ظ� �ж�Ҫ�����ļ�����
				{
				case (1):
					loop = 1;
					ptr->m_ListboxLog.AddString(_T("+ Handshake successfull"));
					ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
					ptr->m_ListboxLog.AddString(_T("+ MCU in bootrom mode, uboot code will be downloaded"));
					ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
					break;
				case 2:
					loop = 0;
					ptr->m_ListboxLog.AddString(_T("+ Handshake successfull"));
					ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
					ptr->m_ListboxLog.AddString(_T("+ MCU in uboot mode, code will be downloaded"));
					ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
					break;
				case 3:
				case 4:
				case 5:
				case 6:
				case 0:
				case 0xff:
				default:
					ptr->m_ListboxLog.AddString(_T("+ Stop download, handshake failed"));
					ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
					AfxMessageBox(_T("Sorry, handshake failed"));
					goto error_return;
				}
			}


			if (loop == 1)
			{
				j = ptr->FindFile(_T("uboot"));
				goto bootromdown;
			}
			for (; j < 8; j++)
			{

				if (Path[j] == "")
				{
					continue;
				}
			bootromdown:
				BYTE *fileBuf = NULL;
				DWORD fileLen = 0;
				if (g_pMainDlg->UartState == g_pMainDlg->UART_SYNC)
				{
					g_pMainDlg->UartState = g_pMainDlg->UART_ADDR;

					CString filePath;
					filePath = Path[j];
					CFile binfile(filePath,CFile::modeRead | CFile::shareDenyWrite); //��ֻ��ģʽ�� �Ӷ�д��
					fileLen = binfile.GetLength(); //�õ��ļ��ֽ��� �߼��������ֽڱ�ʾ 
					fileBuf = new BYTE[fileLen];
					ptr->SendFileInfo(fileLen, j);
					binfile.Read(fileBuf, fileLen);
					binfile.Close(); //�ر��ļ���
					if (fileBuf == NULL)
					{
						ptr->m_ListboxLog.AddString(_T("+ Stop download, apply for memory failed"));
						ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
						AfxMessageBox(_T("Apply for memory failed"));
						binfile.Close();
						goto error_return;
					}
					if (WaitForSingleObject(g_pMainDlg->DownloadEvent, INFINITE) != WAIT_OBJECT_0)
					{
						ptr->m_ListboxLog.AddString(_T("+ Stop download serial port error"));
						ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
						AfxMessageBox(_T("serial port time out"));
						delete[] fileBuf;
						fileBuf = NULL;
						goto error_return;
					}

					if (g_pMainDlg->UartResp != g_pMainDlg->OK)
					{
						ptr->m_ListboxLog.AddString(_T("+ ��ֹ����, Ӧ�����"));
						ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
						AfxMessageBox(_T("Response error"));
						delete[] fileBuf;
						fileBuf = NULL;
						goto error_return;
					}
				}
				if (g_pMainDlg->UartState == g_pMainDlg->UART_ADDR)
				{
					g_pMainDlg->UartState = g_pMainDlg->UART_CODE;
					ptr->m_Progress.SetPos(0);
					ptr->m_Progress.SetRange32(0, fileLen);
					if ((enum _UartResponse)(fileInfo[j].fileType) == g_pMainDlg->UBOOT)
					{
						ptr->m_ListboxLog.AddString(_T("+ uboot code is being downloaded"));
						ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
					}
						
					else if ((enum _UartResponse)(fileInfo[j].fileType) == g_pMainDlg->ANDES)
					{
						ptr->m_ListboxLog.AddString(_T("+ andes code is being downloaded"));
						ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
					}
						
					else if ((enum _UartResponse)(fileInfo[j].fileType) == g_pMainDlg->ANDES1)
					{
						ptr->m_ListboxLog.AddString(_T("+ andes1 code is being downloaded"));
						ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
					}
					else
					{
						ptr->m_ListboxLog.AddString(_T("+ ��������δ֪������......"));
						ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
					}
						
					ptr->m_Progress.SetPos(0);
					ptr->SendFile(fileBuf, fileLen);

				}
				if (loop == 1)
				{
					g_pMainDlg->UartState = g_pMainDlg->UART_IDLE;
					j = 0;
					break;
				}
				else
				{
					g_pMainDlg->UartState = g_pMainDlg->UART_SYNC;
				}
			}
	} while (loop > 0);

{
	//ȫ��������Ϸ���12�ֽ�0
	byteArray.RemoveAll();
	byteArray.SetSize(12);
	for (int ii = 0; ii < 12; ii++)
	{
		byteArray.SetAt(ii, 0);
	}
	g_pMainDlg->m_MSComm.put_Output(COleVariant(byteArray));
	while (g_pMainDlg->m_MSComm.get_OutBufferCount());
	ptr->m_ListboxLog.AddString(_T("+ The download is complete"));
	ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
	g_pMainDlg->OnBnClickedButtonCloseCom();
}

	error_return:
	ptr->EnableWindow();
	g_pMainDlg->UartState = g_pMainDlg->UART_IDLE;
	g_pMainDlg->UartResp = g_pMainDlg->OK;
	
	return 0;
}

afx_msg LRESULT CFlashDownloadDlg::OnDownloadMsg(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CFlashDownloadDlg::SendFileInfo(DWORD fileLen, int j)
{

	CByteArray byteArray;

	//�õ�Ҫ���͵�12���ֽڵ��ļ���Ϣ
	//ǰ4�ֽڱ�ʾ����ģʽ
	//�м�4�ֽڱ�ʾ��ַ
	//���4�ֽڱ�ʾ�ļ��ֽڳ���
	//binfile.Read(fileBuf, fileLen);//���ṩ������ ������ļ��ж�ȡ������ �����ļ����ݴ浽�ڴ��� fileLen���Զ�ȡ������ַ���
	byteArray.RemoveAll(); //�Ӹ�������ɾ������ָ�� �ͷ���������ָ��洢��ռ�õ��ڴ�
	byteArray.SetSize(12); //��������Ԫ�صĸ���
	byteArray.SetAt(0, (BYTE)(fileInfo[j].fileType));  // ��������ָ���±��Ԫ�ظ�ֵ�������ܶ�̬��������
	byteArray.SetAt(1, (BYTE)0);
	byteArray.SetAt(2, (BYTE)0);
	byteArray.SetAt(3, (BYTE)0);
	byteArray.SetAt(4, (BYTE)(fileInfo[j].fileAddr));
	byteArray.SetAt(5, (BYTE)(fileInfo[j].fileAddr >> 8));
	byteArray.SetAt(6, (BYTE)(fileInfo[j].fileAddr >> 16));
	byteArray.SetAt(7, (BYTE)(fileInfo[j].fileAddr >> 24));
	byteArray.SetAt(8, (BYTE)(fileLen));
	byteArray.SetAt(9, (BYTE)(fileLen >> 8));
	byteArray.SetAt(10, (BYTE)(fileLen >> 16));
	byteArray.SetAt(11, (BYTE)(fileLen >> 24));
	g_pMainDlg->m_MSComm.put_Output(COleVariant(byteArray));
	m_ListboxLog.AddString(_T("+ File information has been sent to the MCU"));
	m_ListboxLog.SetCurSel(m_ListboxLog.GetCount() - 1);
}

void CFlashDownloadDlg::SendFile(BYTE *fileBuf, DWORD fileLen)
{
	CByteArray byteArray;
	DWORD len = 0;
	while (len < fileLen)
	{
		int block = (fileLen - len) < PAGE_LEN ? (fileLen - len) : PAGE_LEN;

		byteArray.RemoveAll();
		byteArray.SetSize(block);
		for (int i = 0; i < block; i++, len++)
		{
			byteArray.SetAt(i, fileBuf[len]);
		}
		g_pMainDlg->m_MSComm.put_Output(COleVariant(byteArray));
		if (WaitForSingleObject(g_pMainDlg->DownloadEvent, INFINITE) != WAIT_OBJECT_0) //INFINITE
		{
			m_ListboxLog.AddString(_T("+ ��ֹ����, ���ڴ���"));
			m_ListboxLog.SetCurSel(m_ListboxLog.GetCount() - 1);
			AfxMessageBox(_T("Serial port time out"));
			delete[] fileBuf;
			fileBuf = NULL;
			return;
		}
		m_Progress.SetStep(block);
		m_Progress.StepIt();
		if (g_pMainDlg->UartResp != g_pMainDlg->OK)
		{
			m_ListboxLog.AddString(_T("+ ��ֹ����, Ӧ�����"));
			m_ListboxLog.SetCurSel(m_ListboxLog.GetCount() - 1);
			AfxMessageBox(_T("Response error"));
			delete[] fileBuf;
			fileBuf = NULL;
			return;
		}
	}
	delete[] fileBuf;
	fileBuf = NULL;
}

int CFlashDownloadDlg::FindFile(CString fileName)
{
	if (Path[0].Find(fileName) != -1)
	{
		return 0;
	}
	if (Path[1].Find(fileName) != -1)
	{
		return 1;
	}
	if (Path[2].Find(fileName) != -1)
	{
		return 2;
	}
	if (Path[3].Find(fileName) != -1)
	{
		return 3;
	}
	if (Path[4].Find(fileName) != -1)
	{
		return 4;
	}
	if (Path[5].Find(fileName) != -1)
	{
		return 5;
	}
	if (Path[6].Find(fileName) != -1)
	{
		return 6;
	}
	if (Path[7].Find(fileName) != -1)
	{
		return 7;
	}
	else
	{
		return -1;
	}
}

void CFlashDownloadDlg::DisableWindow(void)
{
	GetDlgItem(IDC_BUTTON_DOWNLOAD)->EnableWindow(FALSE); //�ر����ذ�ť��ʹ֮���ɰ���
	g_pMainDlg->GetDlgItem(IDC_COMBO_COM)->EnableWindow(FALSE);  //�ر�COM���б��ʹ֮���ɸ���ѡ��
	GetDlgItem(IDC_EDIT_PATH1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PATH2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PATH3)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PATH4)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PATH5)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PATH6)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PATH7)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PATH8)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_ADDR1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_ADDR2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_ADDR3)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_ADDR4)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_ADDR5)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_ADDR6)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_ADDR7)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_ADDR8)->EnableWindow(FALSE);
	g_pMainDlg->GetDlgItem(IDC_COMBO_FLASHSIZE)->EnableWindow(FALSE);
	g_pMainDlg->GetDlgItem(IDC_COMBO_BAUD)->EnableWindow(FALSE);
	g_pMainDlg->GetDlgItem(IDC_BUTTON_CLOSE_COM)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_COMBINE)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK2)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK3)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK4)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK5)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK6)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK7)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK8)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CLNALL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_COMBINE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OPNE1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OPEN2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OPEN3)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OPEN4)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OPEN5)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OPEN6)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OPEN7)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OPEN8)->EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_CLN1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CLN2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CLN3)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CLN4)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CLN5)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CLN6)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CLN7)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CLN8)->EnableWindow(FALSE);

}

void CFlashDownloadDlg::EnableWindow(void)
{
	GetDlgItem(IDC_BUTTON_DOWNLOAD)->EnableWindow(TRUE); //�ر����ذ�ť��ʹ֮���ɰ���
	g_pMainDlg->GetDlgItem(IDC_COMBO_COM)->EnableWindow(TRUE);  //�ر�COM���б��ʹ֮���ɸ���ѡ��
	GetDlgItem(IDC_EDIT_PATH1)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PATH2)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PATH3)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PATH4)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PATH5)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PATH6)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PATH7)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PATH8)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_ADDR1)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_ADDR2)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_ADDR3)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_ADDR4)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_ADDR5)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_ADDR6)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_ADDR7)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_ADDR8)->EnableWindow(TRUE);
	g_pMainDlg->GetDlgItem(IDC_COMBO_FLASHSIZE)->EnableWindow(TRUE);
	g_pMainDlg->GetDlgItem(IDC_COMBO_BAUD)->EnableWindow(TRUE);
	g_pMainDlg->GetDlgItem(IDC_BUTTON_CLOSE_COM)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_COMBINE)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK1)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK2)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK3)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK4)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK5)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK6)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK7)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK8)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CLNALL)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_COMBINE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_OPNE1)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_OPEN2)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_OPEN3)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_OPEN4)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_OPEN5)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_OPEN6)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_OPEN7)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_OPEN8)->EnableWindow(TRUE);

	GetDlgItem(IDC_BUTTON_CLN1)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CLN2)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CLN3)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CLN4)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CLN5)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CLN6)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CLN7)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CLN8)->EnableWindow(TRUE);
}

void CFlashDownloadDlg::OnBnClickedButtonGenerateNv()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog fileDlg(TRUE, _T("txt"), NULL, OFN_HIDEREADONLY, _T("�ı��ļ�(*.txt)|*.txt||"), this);

	if (fileDlg.DoModal() == IDOK)
	{
		memset(&param, 0, sizeof(param));

		CString filePath;
		CFile   fileHandle;
		int     fileLen;
		filePath = fileDlg.GetPathName();
		fileHandle.Open(filePath, CFile::modeRead | CFile::shareDenyWrite);
		fileLen = fileHandle.GetLength();

		BYTE *fileBuf = new BYTE[fileLen];
		if (fileBuf)
		{
			fileHandle.Read(fileBuf, fileLen);
			int i, start, end;

			// mac
			for (i = 0; i<fileLen - 2; i++)
				if (fileBuf[i] == 'm' && fileBuf[i + 1] == 'a' && fileBuf[i + 2] == 'c')
					break;
			while (fileBuf[++i] != '=');
			while (fileBuf[++i] == ' ');
			start = i;
			while (fileBuf[++i] != '\r');
			end = i;
			memcpy(param.mac, fileBuf + start, end - start);

			// ip
			for (i = 0; i<fileLen - 1; i++)
				if (fileBuf[i] == 'i' && fileBuf[i + 1] == 'p')
					break;
			while (fileBuf[++i] != '=');
			while (fileBuf[++i] == ' ');
			start = i;
			while (fileBuf[++i] != '\r');
			end = i;
			memcpy(param.ip, fileBuf + start, end - start);

			// mask
			for (i = 0; i<fileLen - 3; i++)
				if (fileBuf[i] == 'm' && fileBuf[i + 1] == 'a' && fileBuf[i + 2] == 's' && fileBuf[i + 3] == 'k')
					break;
			while (fileBuf[++i] != '=');
			while (fileBuf[++i] == ' ');
			start = i;
			while (fileBuf[++i] != '\r');
			end = i;
			memcpy(param.msk, fileBuf + start, end - start);

			// gateway
			for (i = 0; i<fileLen - 3; i++)
				if (fileBuf[i] == 'g' && fileBuf[i + 1] == 'a' && fileBuf[i + 2] == 't' && fileBuf[i + 3] == 'e')
					break;
			while (fileBuf[++i] != '=');
			while (fileBuf[++i] == ' ');
			start = i;
			while (fileBuf[++i] != '\r');
			end = i;
			memcpy(param.gw, fileBuf + start, end - start);

			for (int id = 0; id<NV_NUM_MAX_AP; id++)
			{
				param.conf_file[id].index = id;

				// ssid
				for (i = 0; i<fileLen - 4; i++)
					if (fileBuf[i] == 's' && fileBuf[i + 1] == 's' && fileBuf[i + 2] == 'i' && fileBuf[i + 3] == 'd' && fileBuf[i + 4] == (id + '1'))
						break;

				while (fileBuf[i++] != '=');

				if (fileBuf[i] == ' ')
					while (fileBuf[++i] == ' ');
				start = i;

				if (fileBuf[i] != '\r')
					while (fileBuf[++i] != '\r');
				end = i;

				memcpy(param.conf_file[id].ssid, fileBuf + start, end - start);

				// password
				for (i = 0; i<fileLen - 4; i++)
					if (fileBuf[i] == 'w' && fileBuf[i + 1] == 'o' && fileBuf[i + 2] == 'r' && fileBuf[i + 3] == 'd' && fileBuf[i + 4] == (id + '1'))
						break;

				while (fileBuf[i++] != '=');

				if (fileBuf[i] == ' ')
					while (fileBuf[++i] == ' ');
				start = i;

				if (fileBuf[i] != '\r')
					while (fileBuf[++i] != '\r');
				end = i;

				memcpy(param.conf_file[id].pswd, fileBuf + start, end - start);
			}
			delete[] fileBuf;
		}

		filePath.Replace(_T("txt"), _T("bin"));
		fileHandle.Close();
		fileHandle.Open(filePath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite);
		fileHandle.Write(&param, sizeof(param));
		fileHandle.Close();

		MessageBox(filePath, _T("��ϲ����, �󼪴���"), MB_OK | MB_ICONINFORMATION);
	}
}
