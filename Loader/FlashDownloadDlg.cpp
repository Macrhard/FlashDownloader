// FlashDownloadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Loader.h"
#include "FlashDownloadDlg.h"
#include "afxdialogex.h"


#define  PAGE_LEN  (1*1024)
#define  MAX_FILE  (8) //可下载的最大文件数
CString Path[8] = { NULL };
CString Addr[8] = { NULL };

enum _UartResponse
{
	OK = 0x00,
	UBOOT = 0x01,	 /* uboot       */
	ANDES = 0x02,  /* adnes */
	XIP = 0x03,    /* xip     */
	ANDES1 = 0x04, /* andes1   */
	XIP1 = 0x05,	/*xip1*/
	NV = 0x06,
	FAIL = 0xFF
}UartResp, fileType;
enum _UartStatus
{
	UART_IDLE = 0,
	UART_SYNC = 1,
	UART_ADDR = 2,
	UART_CODE = 3
}UartState;
enum _LoadType
{
	Download = 0,
	Upload = 1,
	SYNC = 3,
	StartUpload = 4
}LoadType;

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
	//  DDX_Control(pDX, IDC_EDIT_PATH1, m_path);
	DDX_Control(pDX, IDC_EDIT_PATH2, m_path2);
	//  DDX_Control(pDX, IDC_EDIT_PATH1, path1);
	DDX_Control(pDX, IDC_EDIT_PATH1, m_path1);
	DDX_Control(pDX, IDC_EDIT_PATH3, m_path3);
	DDX_Control(pDX, IDC_EDIT_PATH4, m_path4);
	DDX_Control(pDX, IDC_EDIT_PATH5, m_path5);
	DDX_Control(pDX, IDC_EDIT_PATH6, m_path6);
	DDX_Control(pDX, IDC_EDIT_PATH7, m_path7);
	DDX_Control(pDX, IDC_EDIT_PATH8, m_path8);
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
			fileInfo[index].fileType = UBOOT;
			Addr[index] = GetConfigInfo(_T("address"), _T("uboot"));
			SetDlgItemText(addrID, Addr[index]);
			fileInfo[index].fileAddr = _tcstoul(Addr[index], NULL, 16);
		}
		else if (option.Find(_T("andes")) != -1)
		{
			SetDlgItemText(pathID, path);
			fileInfo[index].fileType = ANDES;
			Addr[index] = GetConfigInfo(_T("address"), _T("andes"));
			SetDlgItemText(addrID, Addr[index]);
			fileInfo[index].fileAddr = _tcstoul(Addr[index], NULL, 16);
		}
		else if (option.Find(_T("xip")) != -1)
		{
			SetDlgItemText(pathID, path);
			fileInfo[index].fileType = XIP;
			Addr[index] = GetConfigInfo(_T("address"), _T("xip"));
			SetDlgItemText(addrID, Addr[index]);
			fileInfo[index].fileAddr = _tcstoul(Addr[index], NULL, 16);
		}
		else if (option.Find(_T("andes1")) != -1)
		{
			SetDlgItemText(pathID, path);
			fileInfo[index].fileType = ANDES1;
			Addr[index] = GetConfigInfo(_T("address"), _T("andes1"));
			SetDlgItemText(addrID, Addr[index]);
			fileInfo[index].fileAddr = _tcstoul(Addr[index], NULL, 16);
		}
		else if (option.Find(_T("xip1")) != -1)
		{
			SetDlgItemText(pathID, path);
			fileInfo[index].fileType = XIP1;
			Addr[index] = GetConfigInfo(_T("address"), _T("xip1"));
			SetDlgItemText(addrID, Addr[index]);
			fileInfo[index].fileAddr = _tcstoul(Addr[index], NULL, 16);
		}
		else if (option.Find(_T("nv")) != -1)
		{
			SetDlgItemText(pathID, path);
			fileInfo[index].fileType = NV;
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
CString CFlashDownloadDlg::GetConfigInfo(CString title, CString option)
{

	//将在这里添加一个函数判断是否存在文件system.ini 不存在将会按照默认格式创建一个
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
	CFile combineBinfile(_T("f:\\multibinCombineFile.bin"), CFile::modeCreate|CFile::modeWrite);
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
		return UBOOT;
	}
	else if (filePath.Find(_T("andes")) != -1)
	{
		if (filePath.Find(_T("andes1")) != -1)
		{
			return ANDES1;
		}
		else
		{
			return ANDES;

		}
	}
	else if (filePath.Find(_T("xip")) != -1)
	{
		if (filePath.Find(_T("xip1")) != -1)
		{
			return ANDES1;
		}
		else
		{
			return XIP;

		}
	}
	else if(filePath.Find(_T("nv")) != -1)
	{
		return NV;
	}

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