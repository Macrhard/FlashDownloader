// FlashDownloadDlg.cpp : 实现文件
//

/*
To be or not to be that is a question:
Whether it's nobler in the mind to suffer
The slings and arrows of outrageous fortune
Or to take arms against sea of troubles
And by opposing end them. To die--to sleep
No more and by a sleep to say we end the heart-ache
And the thousand natural shocks
That flesh is to heir 'Tis a consummation
Devoutly to be wish'd. To die,to sleep
To sleep perchance to dream - ay, there's the rub
*/


#include "stdafx.h"
#include "Loader.h"
#include "FlashDownloadDlg.h"
#include "afxdialogex.h"
#include "malloc.h"

#define BOOTROM_STAT 0x01
#define FLASH_STAT 0x02

using namespace std;

CFlashDownloadDlg *g_pDownloadDlg = NULL;
#define  PAGE_LEN  (1*1024)
#define  MAX_FILE  (8) //可下载的最大文件数
CString Path[8] = { NULL };
CString Addr[8] = { NULL };

BYTE **pCode;
BYTE **pInfo;
BYTE ubootFlag = 0;
int totalFileCount = 0;
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
		else if (option.Find(_T("multibin")) != -1)
		{
			SetDlgItemText(pathID, path);
			fileInfo[index].fileType = g_pMainDlg->COMBINE;
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
	CString buff;
	GetPrivateProfileString(title, option, CString("NULL"), buff.GetBuffer(MAX_PATH), MAX_PATH, _T(".\\system.ini"));
	return CStringA(buff);
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
	CFile combineBinfile(_T(".\\multibin.bin"), CFile::modeCreate|CFile::modeWrite);
	combineBinfile.Write(combineFile,head);
	combineBinfile.Close();
	free(combineFile);
	m_ListboxLog.AddString(_T("multibin.bin has been created and filepath is .\\multibin.bin"));
	m_ListboxLog.SetCurSel(m_ListboxLog.GetCount() - 1);
}

DWORD CFlashDownloadDlg::ReadFile(CString filePath,int addIndex,char* combinFile, DWORD head)
{
	
	
	DWORD fileLength;
	int fileType;
	fileType = ReturnFileType(filePath);

	CFile binfileStream(filePath, CFile::modeRead | CFile::shareDenyWrite);
	fileLength = binfileStream.GetLength();
	BYTE *fileBuffer = NULL;
	fileBuffer = new BYTE[fileLength];
	binfileStream.Read(fileBuffer, fileLength);
	binfileStream.Close();
	if (fileBuffer == NULL)
	{
		MessageBox(_T("Failed to apply memory for combining files "));
		return 0;
	}


	CByteArray fileInfoArray;
	fileInfoArray.SetSize(12);
	fileInfoArray.SetAt(0,(BYTE)(fileType));
	fileInfoArray.SetAt(1, (BYTE)0);
	fileInfoArray.SetAt(2, (BYTE)0);
	fileInfoArray.SetAt(3, (BYTE)0);
	fileInfoArray.SetAt(4, (BYTE)(fileInfo[addIndex].fileAddr));
	fileInfoArray.SetAt(5, (BYTE)(fileInfo[addIndex].fileAddr >> 8));
	fileInfoArray.SetAt(6, (BYTE)(fileInfo[addIndex].fileAddr >> 16));
	fileInfoArray.SetAt(7, (BYTE)(fileInfo[addIndex].fileAddr >> 24));
	fileInfoArray.SetAt(8, (BYTE)(fileLength));
	fileInfoArray.SetAt(9, (BYTE)(fileLength >> 8));
	fileInfoArray.SetAt(10, (BYTE)(fileLength >> 16));
	fileInfoArray.SetAt(11, (BYTE)(fileLength >> 24));
	memcpy((combinFile+head), fileInfoArray.GetData(),12);
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
		Path[0].MakeLower();
		CString addr;
		GetDlgItemText(IDC_EDIT_ADDR1, addr);
		fileInfo[0].fileAddr = _tcstoul(addr, NULL, 16);
	}
	else
	{
		Path[0] = "";
	}

	if (m_check2.GetCheck() == 1)
	{
		m_path2.GetWindowText(Path[1]);
		Path[1].MakeLower();
		CString addr;
		GetDlgItemText(IDC_EDIT_ADDR2, addr);
		fileInfo[1].fileAddr = _tcstoul(addr, NULL, 16);
	}
	else
	{
		Path[1] = "";
	}

	if (m_check3.GetCheck() == 1)
	{
		m_path3.GetWindowText(Path[2]);
		Path[2].MakeLower();
		CString addr;
		GetDlgItemText(IDC_EDIT_ADDR3, addr);
		fileInfo[2].fileAddr = _tcstoul(addr, NULL, 16);
	}
	else
	{
		Path[2] = "";
	}

	if (m_check4.GetCheck() == 1)
	{
		m_path4.GetWindowText(Path[3]);
		Path[3].MakeLower();
		CString addr;
		GetDlgItemText(IDC_EDIT_ADDR4, addr);
		fileInfo[3].fileAddr = _tcstoul(addr, NULL, 16);
	}
	else
	{
		Path[3] = "";
	}

	if (m_check5.GetCheck() == 1)
	{
		m_path5.GetWindowText(Path[4]);
		Path[4].MakeLower();
		CString addr;
		GetDlgItemText(IDC_EDIT_ADDR5, addr);
		fileInfo[4].fileAddr = _tcstoul(addr, NULL, 16);
	}
	else
	{
		Path[4] = "";
	}

	if (m_check6.GetCheck() == 1)
	{
		m_path6.GetWindowText(Path[5]);
		Path[5].MakeLower();
		CString addr;
		GetDlgItemText(IDC_EDIT_ADDR6, addr);
		fileInfo[5].fileAddr = _tcstoul(addr, NULL, 16);
	}
	else
	{
		Path[5] = "";
	}

	if (m_check7.GetCheck() == 1)
	{
		m_path7.GetWindowText(Path[6]);
		Path[6].MakeLower();
		CString addr;
		GetDlgItemText(IDC_EDIT_ADDR7, addr);
		fileInfo[6].fileAddr = _tcstoul(addr, NULL, 16);
	}
	else
	{
		Path[6] = "";
	}

	if (m_check8.GetCheck() == 1)
	{
		m_path8.GetWindowText(Path[7]);
		Path[7].MakeLower();
		CString addr;
		GetDlgItemText(IDC_EDIT_ADDR8, addr);
		fileInfo[7].fileAddr = _tcstoul(addr, NULL, 16);
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

void CFlashDownloadDlg::OnBnClickedButtonDownload()
{
	g_pDownloadDlg = this;
	// TODO: 在此添加控件通知处理程序代码
	g_pMainDlg->ComEvent.ResetEvent();
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
	pCode = (BYTE**)malloc(sizeof(BYTE*) * 20);
	pInfo = (BYTE**)malloc(sizeof(BYTE*) * 20);
	DisableWindow();
	ReadFileToMemory();
	pUartThread = AfxBeginThread(UartDownload, this, THREAD_PRIORITY_NORMAL, 0, 0);
}

//这个又长又臭的函数 迟早要重写
//已经重写
UINT CFlashDownloadDlg::UartDownload(LPVOID pParam)
{
	Sleep(300);
	CFlashDownloadDlg *ptr = (CFlashDownloadDlg*)pParam;
	ptr->m_ListboxLog.ResetContent();//清空Log框中的内容
	ptr->m_ListboxLog.AddString(_T("+ Start downlogading"));
	ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
	CByteArray changeBaudSync;
	changeBaudSync.SetSize(1);
	int loopFlag = 1;
	CByteArray byteArrayEnd;
	byteArrayEnd.SetSize(12);

	try
	{
		while (loopFlag == BOOTROM_STAT)
		{
			int n = 0;
			while (++n)
			{
				g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("cnys")));  //57600	115200
				if (WaitForSingleObject(g_pMainDlg->ComEvent, 1000) == WAIT_OBJECT_0)
				{
					break;
				}
				else
				{
					if (n > 20)
					{
						throw _T("+ Stop upload Sorry, serial port timeout");
					}
					ptr->m_ListboxLog.AddString(_T("+ Handshaking......"));
					ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
				}
			}
			switch (g_pMainDlg->UartStatus)  //等待MCU回复 判断要下载文件类型
			{
			case BOOTROM_STAT:
				loopFlag = BOOTROM_STAT;
				ptr->m_ListboxLog.AddString(_T("+ Handshake successfull"));
				ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
				ptr->m_ListboxLog.AddString(_T("+ MCU in bootrom mode, uboot code will be downloaded"));
				ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
				break;
			case FLASH_STAT:
				loopFlag = FLASH_STAT;
				ptr->m_ListboxLog.AddString(_T("+ Handshake successfull"));
				ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
				ptr->m_ListboxLog.AddString(_T("+ MCU in uboot mode, code will be downloaded"));
				ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
				break;
			default:
				ptr->m_ListboxLog.AddString(_T("+ Stop download, handshake failed"));
				ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
				//AfxMessageBox(_T("Sorry, handshake failed"));
				throw _T("Sorry, handshake failed");
			}

			//BOOTROM_STAT 阶段 查找uboot并进行下载
			if (loopFlag == BOOTROM_STAT/*&& (BYTE(*pInfo[ubootFlag]) == 0x1)*/)
			{
				if ((BYTE(*pInfo[ubootFlag]) == 0x1))
				{
					ptr->SendFileInfo(ubootFlag);
					if ((WaitForSingleObject(g_pMainDlg->ComEvent, 15000) == WAIT_OBJECT_0) && (g_pMainDlg->UartStatus == 0x00))
					{
						ptr->SendFile(pCode[ubootFlag], ptr->GetFileType((BYTE)*pInfo[ubootFlag]));
						continue;
					}
					else
					{
						throw _T("### Response erro");
					}
				}
				else
				{
					throw _T("### Starting from bootrom requires uboot file");
				}
			}

			if (loopFlag == FLASH_STAT)
			{
				if (g_pMainDlg->m_ComboBoxBaud.GetCurSel() == 1)
				{
					g_pMainDlg->ComEvent.ResetEvent();
					changeBaudSync.SetAt(0, 1);
					g_pMainDlg->m_MSComm.put_Output(COleVariant(changeBaudSync));
					g_pMainDlg->m_MSComm.put_Settings(_T("115200,n,8,1"));
				}
				else
				{
					g_pMainDlg->ComEvent.ResetEvent();
					changeBaudSync.SetAt(0, 2);
					g_pMainDlg->m_MSComm.put_Output(COleVariant(changeBaudSync));
				}
				if ((WaitForSingleObject(g_pMainDlg->ComEvent, 15000) != WAIT_OBJECT_0) || (g_pMainDlg->UartStatus != 0x03))
				{
					throw _T("Change baud rate failed");
				}
			}
			if (loopFlag == FLASH_STAT)
			{
				g_pMainDlg->ComEvent.ResetEvent();
				g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("cnys")));
				if ((WaitForSingleObject(g_pMainDlg->ComEvent, 15000) != WAIT_OBJECT_0) || (g_pMainDlg->UartStatus != 0x02))
				{
					throw _T("Sorry, handshake failed, after change baud rate ");
				}
			}

			for (int i = 0; i < totalFileCount; i++)
			{
				ptr->SendFileInfo(i);
				if ((WaitForSingleObject(g_pMainDlg->ComEvent, 180000) != WAIT_OBJECT_0) || (g_pMainDlg->UartStatus != 0x00))
				{
					throw _T("### Sorry time out");
				}
				ptr->SendFile(pCode[i], ptr->GetFileType((BYTE)*pInfo[i]));
			}
		}
		g_pMainDlg->m_MSComm.put_Output(COleVariant(byteArrayEnd));
		free(pCode);
		free(pInfo);
		ptr->m_ListboxLog.AddString(_T("+ Download completion"));
		ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
		ptr->CloseSerialPort();
		ptr->EnableWindow();
		return 0;
	}
	catch (const WCHAR * msg)
	{
		ptr->m_ListboxLog.AddString(msg);
		ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
		ptr->MessageBox(msg, _T("Tips"), MB_OK | MB_ICONWARNING);	
		free(pCode);
		free(pInfo);
		ptr->CloseSerialPort();
		ptr->EnableWindow();
		return 0;
	}
}

afx_msg LRESULT CFlashDownloadDlg::OnDownloadMsg(WPARAM wParam, LPARAM lParam)
{
	return 0;
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
	GetDlgItem(IDC_BUTTON_GENERATE_NV)->EnableWindow(FALSE);
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
	GetDlgItem(IDC_BUTTON_GENERATE_NV)->EnableWindow(TRUE);
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


void CFlashDownloadDlg::SendFileInfo(int index)
{
	CByteArray temp;
	temp.SetSize(12);
	memcpy(temp.GetData(), pInfo[index], 12);
	g_pMainDlg->m_MSComm.put_Output(COleVariant(temp));
	m_ListboxLog.AddString(_T("+ File information has been sent to the MCU"));
	m_ListboxLog.SetCurSel(m_ListboxLog.GetCount() - 1);
}
void CFlashDownloadDlg::SendFile(BYTE *pFile,CString fileType)
{
	m_ListboxLog.AddString(_T("+ Start download ")+ fileType + _T(" files"));
	m_ListboxLog.SetCurSel(m_ListboxLog.GetCount() - 1);
	CByteArray byteArray;
	DWORD len = 0, fileLen = 0;
	fileLen = _msize(pFile);
	m_Progress.SetPos(0);
	m_Progress.SetRange32(0, fileLen);
	while (len < fileLen)
	{
		int block = (fileLen - len) < PAGE_LEN ? (fileLen - len) : PAGE_LEN;

		byteArray.RemoveAll();
		byteArray.SetSize(block);
		memcpy(byteArray.GetData(), pFile+len, block);
		g_pMainDlg->ComEvent.ResetEvent();
		g_pMainDlg->m_MSComm.put_Output(COleVariant(byteArray));
		m_Progress.SetStep(block);
		m_Progress.StepIt();
		if (WaitForSingleObject(g_pMainDlg->ComEvent, 8000) != WAIT_OBJECT_0) //INFINITE
		{
			throw _T("Serial port time out");
		}
		if (g_pMainDlg->UartStatus != 0x00)
		{
			throw _T("Response error");
		}
		len = len + block;
	}

}

void CFlashDownloadDlg::ReadFileToMemory()
{
	/*int fileCount = 0;
	
	for (int cnt = 0; cnt < 8; cnt++)
	{
		if (Path[cnt] != "")
		{
			fileCount++;
		}
	}*/
	int i = 0;
	for (int j = 0; j < 8;j++)
	{
		DWORD  shiftLength = 0;
		if (Path[j] == _T(""))
		{
			continue;
		}
		CFile fileRead(Path[j], CFile::modeRead);
		DWORD fileLength = fileRead.GetLength();
		BYTE *fileBuffer = new BYTE[fileLength];
		fileRead.Read(fileBuffer, fileLength);
		if (Path[j].Find(_T("multibin")) != -1)
		{		
			UINT *fileSize = (UINT *)malloc(4), nSize = 0;
			for (; fileLength > shiftLength;)
			{
				pInfo[i] = (BYTE*)malloc(12);
				memcpy(pInfo[i], fileBuffer + shiftLength, 12);
				memcpy(fileSize, fileBuffer + shiftLength + 8, 4);
				nSize = *fileSize;
				pCode[i] = (BYTE*)malloc(nSize);
				memcpy(pCode[i], fileBuffer + shiftLength + 12, nSize);
				shiftLength = shiftLength + 12 + nSize;
				if (BYTE(*pInfo[i]) == 0x1)
				{
					ubootFlag = i;
				}
				i++;
			}
			continue;
		}
		if ((BYTE)fileInfo[j].fileType == 1)
		{
			ubootFlag = i;
		}
		CByteArray byteArray;
		byteArray.RemoveAll();
		byteArray.SetSize(12);
		byteArray.SetAt(0, (BYTE)(fileInfo[j].fileType));
		byteArray.SetAt(1, (BYTE)0);
		byteArray.SetAt(2, (BYTE)0);
		byteArray.SetAt(3, (BYTE)0);
		byteArray.SetAt(4, (BYTE)(fileInfo[j].fileAddr));
		byteArray.SetAt(5, (BYTE)(fileInfo[j].fileAddr >> 8));
		byteArray.SetAt(6, (BYTE)(fileInfo[j].fileAddr >> 16));
		byteArray.SetAt(7, (BYTE)(fileInfo[j].fileAddr >> 24));
		byteArray.SetAt(8, (BYTE)(fileLength));
		byteArray.SetAt(9, (BYTE)(fileLength >> 8));
		byteArray.SetAt(10, (BYTE)(fileLength >> 16));
		byteArray.SetAt(11, (BYTE)(fileLength >> 24));
		pInfo[i] = (BYTE*)malloc(12);
		memcpy(pInfo[i], byteArray.GetData(), 12);
		pCode[i] = (BYTE*)malloc(sizeof(BYTE)*fileLength);
		memcpy(pCode[i], fileBuffer, fileLength);
		i++;
	}
	totalFileCount = i;
}

CString CFlashDownloadDlg::GetFileType(BYTE fileID)
{
	switch (fileID)
	{
	case 1:
		return _T("uboot");
		break;
	case 2:
		return _T("nv");
		break;
	case 3:
		return _T("andes");
		break;
	case 4:
		return _T("andes1");
		break;
	default:
		return _T("Unknown");
		break;
	}
}

void CFlashDownloadDlg::CloseSerialPort()
{
	if (g_pMainDlg->m_MSComm.get_PortOpen()) //如果串口是打开的返回 1 ，关闭返回 0    则关闭串口 
	{
		g_pMainDlg->m_MSComm.put_PortOpen(FALSE);
		g_pMainDlg->oldComNum = 0;
		g_pMainDlg->m_ComboBoxCom.SetCurSel(0); //让ComboBox_Com显示第0项内容
		g_pMainDlg->m_StatusBar.SetPaneText(0, _T("COM??"));
	}
}