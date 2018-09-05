// FlashDownloadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Loader.h"
#include "FlashDownloadDlg.h"
#include "afxdialogex.h"


CFlashDownloadDlg *g_pDownloadDlg = NULL;
#define  PAGE_LEN  (1*1024)
#define  MAX_FILE  (8) //可下载的最大文件数
CString Path[8] = { NULL };
CString Addr[8] = { NULL };


struct _fileInfo
{
	int fileType; //fileType 来指定下载打文件类型如 uboot，andes xip 等
	DWORD fileAddr;
}fileInfo[MAX_FILE];

// CFlashDownloadDlg 对话框

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
	//自定义消息
	ON_MESSAGE(WM_DOWNLOAD_MSG, &CFlashDownloadDlg::OnDownloadMsg)
	ON_BN_CLICKED(IDC_BUTTON_GENERATE_NV, &CFlashDownloadDlg::OnBnClickedButtonGenerateNv)
END_MESSAGE_MAP()

// CFlashDownloadDlg 消息处理程序

void CFlashDownloadDlg::OnBnClickedButtonOpne1()
{
	// TODO: 在此添加控件通知处理程序代码
	SelcetFile(0, IDC_EDIT_PATH1, IDC_EDIT_ADDR1);
	m_check1.SetCheck(1);
}


void CFlashDownloadDlg::OnBnClickedButtonOpen2()
{
	// TODO: 在此添加控件通知处理程序代码
	SelcetFile(1, IDC_EDIT_PATH2, IDC_EDIT_ADDR2);
	m_check2.SetCheck(1);
}


void CFlashDownloadDlg::OnBnClickedButtonOpen3()
{
	// TODO: 在此添加控件通知处理程序代码
	SelcetFile(2, IDC_EDIT_PATH3, IDC_EDIT_ADDR3);
	m_check3.SetCheck(1);
}


void CFlashDownloadDlg::OnBnClickedButtonOpen4()
{
	// TODO: 在此添加控件通知处理程序代码
	SelcetFile(3, IDC_EDIT_PATH4, IDC_EDIT_ADDR4);
	m_check4.SetCheck(1);
}


void CFlashDownloadDlg::OnBnClickedButtonOpen5()
{
	// TODO: 在此添加控件通知处理程序代码
	SelcetFile(4, IDC_EDIT_PATH5, IDC_EDIT_ADDR5);
	m_check5.SetCheck(1);
}


void CFlashDownloadDlg::OnBnClickedButtonOpen6()
{
	// TODO: 在此添加控件通知处理程序代码
	SelcetFile(5, IDC_EDIT_PATH6, IDC_EDIT_ADDR6);
	m_check6.SetCheck(1);
}


void CFlashDownloadDlg::OnBnClickedButtonOpen7()
{
	// TODO: 在此添加控件通知处理程序代码
	SelcetFile(6, IDC_EDIT_PATH7, IDC_EDIT_ADDR7);
	m_check7.SetCheck(1);
}


void CFlashDownloadDlg::OnBnClickedButtonOpen8()
{
	// TODO: 在此添加控件通知处理程序代码
	SelcetFile(7, IDC_EDIT_PATH8, IDC_EDIT_ADDR8);
	m_check8.SetCheck(1);
}


void CFlashDownloadDlg::OnBnClickedButtonCln1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_check1.SetCheck(0);
	Path[0] = "";
	SetDlgItemText(IDC_EDIT_PATH1,_T(""));
	SetDlgItemText(IDC_EDIT_ADDR1, _T(""));
}


void CFlashDownloadDlg::OnBnClickedButtonCln2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_check2.SetCheck(0);
	Path[1] = "";
	SetDlgItemText(IDC_EDIT_PATH2, _T(""));
	SetDlgItemText(IDC_EDIT_ADDR2, _T(""));
}


void CFlashDownloadDlg::OnBnClickedButtonCln3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_check3.SetCheck(0);
	Path[2] = "";
	SetDlgItemText(IDC_EDIT_PATH3, _T(""));
	SetDlgItemText(IDC_EDIT_ADDR3, _T(""));

}


void CFlashDownloadDlg::OnBnClickedButtonCln4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_check4.SetCheck(0);
	Path[3] = "";
	SetDlgItemText(IDC_EDIT_PATH4, _T(""));
	SetDlgItemText(IDC_EDIT_ADDR4, _T(""));
}


void CFlashDownloadDlg::OnBnClickedButtonCln5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_check5.SetCheck(0);
	Path[4] = "";
	SetDlgItemText(IDC_EDIT_PATH5, _T(""));
	SetDlgItemText(IDC_EDIT_ADDR5, _T(""));
}


void CFlashDownloadDlg::OnBnClickedButtonCln6()
{
	// TODO: 在此添加控件通知处理程序代码
	m_check6.SetCheck(0);
	Path[5] = "";
	SetDlgItemText(IDC_EDIT_PATH6, _T(""));
	SetDlgItemText(IDC_EDIT_ADDR6, _T(""));
}


void CFlashDownloadDlg::OnBnClickedButtonCln7()
{
	// TODO: 在此添加控件通知处理程序代码
	m_check7.SetCheck(0);
	Path[6] = "";
	SetDlgItemText(IDC_EDIT_PATH7, _T(""));
	SetDlgItemText(IDC_EDIT_ADDR7, _T(""));
}


void CFlashDownloadDlg::OnBnClickedButtonCln8()
{
	// TODO: 在此添加控件通知处理程序代码
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
	CString szFilters = _T("二进制文件(*.bin)|*.bin||");
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

//获取system.ini文件中需要的信息并返回到调用处
CStringA CFlashDownloadDlg:: GetConfigInfo(CString title, CString option)
{

	//将在这里添加一个函数判断是否存在文件system.ini 不存在将会按照默认格式创建一个
	//WritePrivateProfileString
	CFile finder;
	BOOL ifNotFind = finder.Open(_T(".\\system.ini"),CFile::modeRead);
	finder.Close();  //打开后必须关闭文件流，不然下面无法读取配置
	if (!ifNotFind)
	{
		WritePrivateProfileString(_T("address"), _T("uboot"), _T("0x3000"), _T(".\\system.ini"));
		WritePrivateProfileString(_T("address"), _T("nv"), _T("0x7000"), _T(".\\system.ini"));
		WritePrivateProfileString(_T("address"), _T("andes"), _T("0x8000"), _T(".\\system.ini"));
		WritePrivateProfileString(_T("address"), _T("andes1"), _T("0x84000"), _T(".\\system.ini"));
		WritePrivateProfileString(_T("config"), _T("mac"), _T("12-34-56-78-AB-CD"), _T(".\\system.ini"));
		WritePrivateProfileString(_T("config"), _T("ip"), _T("192.168.0.123"), _T(".\\system.ini"));
		WritePrivateProfileString(_T("config"), _T("mask"), _T("255.255.255.0"), _T(".\\system.ini"));
		WritePrivateProfileString(_T("config"), _T("gateway"), _T("192.168.0.1"), _T(".\\system.ini"));
		WritePrivateProfileString(_T("config"), _T("ssid1"), _T(""), _T(".\\system.ini"));
		WritePrivateProfileString(_T("config"), _T("password1"), _T(""), _T(".\\system.ini"));
		WritePrivateProfileString(_T("config"), _T("ssid2"), _T(""), _T(".\\system.ini"));
		WritePrivateProfileString(_T("config"), _T("password2"), _T(""), _T(".\\system.ini"));
		WritePrivateProfileString(_T("config"), _T("ssid3"), _T(""), _T(".\\system.ini"));
		WritePrivateProfileString(_T("config"), _T("password3"), _T(""), _T(".\\system.ini"));
		WritePrivateProfileString(_T("config"), _T("ssid4"), _T(""), _T(".\\system.ini"));
		WritePrivateProfileString(_T("config"), _T("password4"), _T(""), _T(".\\system.ini"));
		WritePrivateProfileString(_T("config"), _T("ssid5"), _T(""), _T(".\\system.ini"));
		WritePrivateProfileString(_T("config"), _T("password5"), _T(""), _T(".\\system.ini"));
	}
	CString buff;
	GetPrivateProfileString(title, option, CString("NULL"), buff.GetBuffer(MAX_PATH), MAX_PATH, _T(".\\system.ini"));
	return CStringA(buff);
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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


//保存显示在路径对话框中并且打勾的文件路径 到Path[]中
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

//返回文件类型
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
		MessageBox(_T("请选择要下载的文件"), _T("Tips"), MB_OK | MB_ICONWARNING);
		EnableWindow();
		break;
	default:
		MessageBox(_T("未知错误"), _T("Tips"), MB_OK | MB_ICONWARNING);
		EnableWindow();
		break;
	}
}

void CFlashDownloadDlg::OnBnClickedButtonDownload()
{
	// TODO: 在此添加控件通知处理程序代码
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

//这个又长又臭的函数 迟早要重写
UINT CFlashDownloadDlg::UartDownload(LPVOID pParam)
{
	CFlashDownloadDlg *ptr = (CFlashDownloadDlg*)pParam;

	ptr->m_ListboxLog.ResetContent();//清空Log框中的内容
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
				switch (g_pMainDlg->UartResp)  //等待MCU回复 判断要下载文件类型
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
					CFile binfile(filePath,CFile::modeRead | CFile::shareDenyWrite); //以只读模式打开 加读写锁
					fileLen = binfile.GetLength(); //得到文件字节数 逻辑长度以字节表示 
					fileBuf = new BYTE[fileLen];
					ptr->SendFileInfo(fileLen, j);
					binfile.Read(fileBuf, fileLen);
					binfile.Close(); //关闭文件流
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
						ptr->m_ListboxLog.AddString(_T("+ 终止下载, 应答错误"));
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
						ptr->m_ListboxLog.AddString(_T("+ 正在下载未知代码中......"));
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
	//全部下载完毕发送12字节0
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

	//得到要传送的12个字节的文件信息
	//前4字节表示数据模式
	//中间4字节表示地址
	//最后4字节表示文件字节长度
	//binfile.Read(fileBuf, fileLen);//向提供缓冲器 存入从文件中读取的数据 即将文件数据存到内存中 fileLen可以读取的最大字符数
	byteArray.RemoveAll(); //从该数组中删除所有指针 释放所有用于指针存储所占用的内存
	byteArray.SetSize(12); //设置数组元素的个数
	byteArray.SetAt(0, (BYTE)(fileInfo[j].fileType));  // 给数组中指定下标的元素赋值，但不能动态增长数组
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
			m_ListboxLog.AddString(_T("+ 终止下载, 串口错误"));
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
			m_ListboxLog.AddString(_T("+ 终止下载, 应答错误"));
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
	GetDlgItem(IDC_BUTTON_DOWNLOAD)->EnableWindow(FALSE); //关闭下载按钮，使之不可按下
	g_pMainDlg->GetDlgItem(IDC_COMBO_COM)->EnableWindow(FALSE);  //关闭COM口列表框，使之不可更改选项
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
	GetDlgItem(IDC_BUTTON_DOWNLOAD)->EnableWindow(TRUE); //关闭下载按钮，使之不可按下
	g_pMainDlg->GetDlgItem(IDC_COMBO_COM)->EnableWindow(TRUE);  //关闭COM口列表框，使之不可更改选项
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

	CByteArray configByteArray;
	configByteArray.SetSize(568); //创建一个全为0的568字节的数组

	configCopy(&configByteArray, _T("mac"), 0);
	configCopy(&configByteArray, _T("ip"), 20);
	configCopy(&configByteArray, _T("mask"), 36);
	configCopy(&configByteArray, _T("gateway"), 52);
	configCopy(&configByteArray, _T("ssid1"), 68);
	configCopy(&configByteArray, _T("ssid2"), 168);
	configCopy(&configByteArray, _T("ssid3"), 268);
	configCopy(&configByteArray, _T("ssid4"), 368);
	configCopy(&configByteArray, _T("ssid5"), 468);

	CFile putOutNvFile(_T(".\\nv.bin"), CFile::modeCreate | CFile::modeWrite);
	putOutNvFile.Write(configByteArray.GetData(), 568);
	putOutNvFile.Close();
	m_ListboxLog.AddString(_T("The nv.bin file has been created and filepath is .\\nv.bin"));
	m_ListboxLog.SetCurSel(m_ListboxLog.GetCount() - 1);
}

void CFlashDownloadDlg::configCopy(CByteArray* configByteArray, CString option, int insertIndex)
{
	CStringA configCstring = GetConfigInfo(_T("config"),option);
	if (option == _T("ssid1"))
	{
		configByteArray->SetAt(insertIndex, (short)1);
		int nSize = configCstring.GetLength();
		memcpy(configByteArray->GetData()+insertIndex + 2, configCstring, nSize);
		CStringA passwd = GetConfigInfo(_T("config"), _T("password1"));
		nSize = passwd.GetLength();
		memcpy(configByteArray->GetData() + insertIndex +35, passwd, nSize);
		return;
	}
	if (option == _T("ssid2"))
	{
		configByteArray->SetAt(insertIndex, (short)2);
		int nSize = configCstring.GetLength();
		memcpy(configByteArray->GetData() + insertIndex + 2, configCstring, nSize);
		CStringA passwd = GetConfigInfo(_T("config"), _T("password2"));
		nSize = passwd.GetLength();
		memcpy(configByteArray->GetData() + insertIndex + 35, passwd, nSize);
		return;
	}
	if (option == _T("ssid3"))
	{
		configByteArray->SetAt(insertIndex, (short)3);
		int nSize = configCstring.GetLength();
		memcpy(configByteArray->GetData() + insertIndex + 2, configCstring, nSize);
		CStringA passwd = GetConfigInfo(_T("config"), _T("password3"));
		nSize = passwd.GetLength();
		memcpy(configByteArray->GetData() + insertIndex + 35, passwd, nSize);
		return;
	}
	if (option == _T("ssid4"))
	{
		configByteArray->SetAt(insertIndex, (short)4);
		int nSize = configCstring.GetLength();
		memcpy(configByteArray->GetData() + insertIndex + 2, configCstring, nSize);
		CStringA passwd = GetConfigInfo(_T("config"), _T("password4"));
		nSize = passwd.GetLength();
		memcpy(configByteArray->GetData() + insertIndex + 35, passwd, nSize);
		return;
	}
	if (option == _T("ssid5"))
	{
		configByteArray->SetAt(insertIndex, (short)5);
		int nSize = configCstring.GetLength();
		memcpy(configByteArray->GetData() + insertIndex + 2, configCstring, nSize);
		CStringA passwd = GetConfigInfo(_T("config"), _T("password5"));
		nSize = passwd.GetLength();
		memcpy(configByteArray->GetData() + insertIndex + 35, passwd, nSize);
		return;
	}
	int nSize = configCstring.GetLength();
	memcpy(configByteArray->GetData() + insertIndex, configCstring, nSize);

}