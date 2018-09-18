// FlashUploadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Loader.h"
#include "FlashUploadDlg.h"
#include "afxdialogex.h"


CFlashUploadDlg *g_pUploadDlg = NULL;
// CFlashUploadDlg 对话框
//HWND hWnd = ::FindWindow(NULL, _T("Upload"));
//g_pUploadDlg = (CFlashUploadDlg*)FromHandle(hWnd);


IMPLEMENT_DYNAMIC(CFlashUploadDlg, CDialogEx)

CFlashUploadDlg::CFlashUploadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_FLASHUPLOAD, pParent)
{
	
}

CFlashUploadDlg::~CFlashUploadDlg()
{
}

void CFlashUploadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ADDR, m_Addr);
	DDX_Control(pDX, IDC_EDIT_LENGTH, m_Length);
	DDX_Control(pDX, IDC_EDIT_SAVEPATH, m_SavePath);
	DDX_Control(pDX, IDC_PROGRESS1, m_UploadProgress);
	DDX_Control(pDX, IDC_UPLOAD_LIST_LOG, m_UploadListLogBox);
}


BEGIN_MESSAGE_MAP(CFlashUploadDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD, &CFlashUploadDlg::OnBnClickedButtonUpload)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CFlashUploadDlg::OnBnClickedButtonOpen)
END_MESSAGE_MAP()


// CFlashUploadDlg 消息处理程序


void CFlashUploadDlg::OnBnClickedButtonUpload()
{
	g_pMainDlg->ComEvent.ResetEvent();
	m_UploadListLogBox.ResetContent();//清空Log框中的内容
	g_pMainDlg->LoadType = g_pMainDlg->Upload;
	pUartThread = AfxBeginThread(UartUpload, this, THREAD_PRIORITY_NORMAL, 0, 0);
}


UINT CFlashUploadDlg::UartUpload(LPVOID pParam)
{
	
	WaitForSingleObject(g_pMainDlg->ComEvent, 5000); //等待log

	CByteArray changeBaudSync;
	changeBaudSync.SetSize(1);
	CFlashUploadDlg *ptr = (CFlashUploadDlg*)pParam;
	try {

		if (g_pMainDlg->oldComNum == 0)
		{
			throw _T("Please selcet a appropriate serial port");
		}

		int n = 0;
		while (++n)
		{
			g_pMainDlg->ComEvent.ResetEvent();
			g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("cnys")));  //57600	115200
			ptr->m_UploadListLogBox.AddString(_T("+ Handshaking ..."));
			ptr->m_UploadListLogBox.SetCurSel(ptr->m_UploadListLogBox.GetCount() - 1);
			if (WaitForSingleObject(g_pMainDlg->ComEvent, 5000) == WAIT_OBJECT_0)
			{
				break;
			}
			else
			{
				if (n > 3)
				{
					ptr->m_UploadListLogBox.AddString(_T("+ Stop upload, serial port timeout"));
					ptr->m_UploadListLogBox.SetCurSel(ptr->m_UploadListLogBox.GetCount() - 1);
					throw _T("Sorry, serial port timeout");
				}
				ptr->m_UploadListLogBox.AddString(_T("+ Handshaking......"));
				ptr->m_UploadListLogBox.SetCurSel(ptr->m_UploadListLogBox.GetCount() - 1);
			}
		}

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
		if (WaitForSingleObject(g_pMainDlg->ComEvent, 5000) == WAIT_OBJECT_0 && g_pMainDlg->rxdata[0] != 0x03)
		{
			throw _T("Change baud rate failed");
		}
		g_pMainDlg->ComEvent.ResetEvent();
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("cnys")));
		if (WaitForSingleObject(g_pMainDlg->ComEvent, 5000) == WAIT_OBJECT_0 && g_pMainDlg->rxdata[0] != 0x02)
		{
			throw _T("Handshake failed");
		}

		//发送上载信息
		CString addr;
		ptr->m_Addr.GetWindowTextW(addr);
		DWORD upAddr = _tcstoul(addr, NULL, 16);
		CString length;
		ptr->m_Length.GetWindowTextW(length);
		int upFileLength = _tcstoul(length, NULL, 10);
		CString	path;
		ptr->m_SavePath.GetWindowTextW(path);
		CByteArray byteArray;
		byteArray.RemoveAll();
		byteArray.SetSize(12);
		byteArray.SetAt(0, (BYTE)0);
		byteArray.SetAt(1, (BYTE)0);
		byteArray.SetAt(2, (BYTE)0);
		byteArray.SetAt(3, (BYTE)128);
		byteArray.SetAt(4, (BYTE)(upAddr));
		byteArray.SetAt(5, (BYTE)(upAddr >> 8));
		byteArray.SetAt(6, (BYTE)(upAddr >> 16));
		byteArray.SetAt(7, (BYTE)(upAddr >> 24));
		byteArray.SetAt(8, (BYTE)(upFileLength));
		byteArray.SetAt(9, (BYTE)(upFileLength >> 8));
		byteArray.SetAt(10, (BYTE)(upFileLength >> 16));
		byteArray.SetAt(11, (BYTE)(upFileLength >> 24));
		g_pMainDlg->ComEvent.ResetEvent();
		g_pMainDlg->m_MSComm.put_Output(COleVariant(byteArray));
		ptr->m_UploadListLogBox.AddString(_T("+ Start uplogading"));
		ptr->m_UploadListLogBox.SetCurSel(ptr->m_UploadListLogBox.GetCount() - 1);

		g_pMainDlg->ComEvent.ResetEvent();
		if (WaitForSingleObject(g_pMainDlg->ComEvent, 50000) != WAIT_OBJECT_0)
		{
			throw _T("+ Time out");
		}
		if (g_pMainDlg->rxdata[0] == 0xff)
		{
			throw _T("+ Upload infomation erro");
		}
		g_pMainDlg->ComEvent.ResetEvent();
		CByteArray zeroByte;
		zeroByte.SetSize(1);
		zeroByte.SetAt(0, 0);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(zeroByte));
		ptr->ReceiveCode(upFileLength);
	}
	catch (const WCHAR * msg)
	{
		ptr->m_UploadListLogBox.AddString(msg);
		ptr->m_UploadListLogBox.SetCurSel(ptr->m_UploadListLogBox.GetCount() - 1);
		ptr->MessageBox(msg, _T("Tips"), MB_OK | MB_ICONWARNING);
		return 0;
	}
	ptr->m_UploadListLogBox.AddString(_T("+ Upload Complete!!!"));
	ptr->m_UploadListLogBox.SetCurSel(ptr->m_UploadListLogBox.GetCount() - 1);
	return 0;
}


void CFlashUploadDlg::OnBnClickedButtonOpen()
{
	/*HWND hWnd = ::FindWindow(_T("CFlashUploadDlg"), _T("Upload"));
	g_pUploadDlg = (CFlashUploadDlg*)FromHandle(hWnd);*/
	/*g_pUploadDlg = (CFlashUploadDlg*)AfxGetApp()->GetMainWnd();*/
	//GetSafeHwnd();
	//this->h;
	//GetDlgItem(h, IDD_DIALOG_FLASHUPLOAD);
	/*g_pUploadDlg = (CFlashUploadDlg*)CWnd::GetDlgItem(IDD_DIALOG_FLASHUPLOAD);*/

	g_pUploadDlg = this;
	CString szFilters = _T("二进制文件(*.bin)|*.bin||");
	CFileDialog Filedlg(0, _T("bin"), _T("UploadFile"), OFN_HIDEREADONLY | OFN_EXPLORER, szFilters, 0);
	if (Filedlg.DoModal() == IDOK)
	{
		CString path = NULL;
		path = Filedlg.GetPathName();
		SetDlgItemText(IDC_EDIT_SAVEPATH, path);
	}
}

void CFlashUploadDlg::ReceiveCode(int upFileLength)
{
	m_UploadProgress.SetPos(0);
	m_UploadProgress.SetRange32(0, upFileLength);
	
	CString path;
	m_SavePath.GetWindowTextW(path);
	CFile upFile(path, CFile::modeCreate | CFile::modeWrite);
	int receiveTotalLength = 0;
	while (upFileLength > receiveTotalLength)
	{
		if ((WaitForSingleObject(g_pMainDlg->ComEvent, 5000) == WAIT_OBJECT_0))
		{	
			receiveTotalLength += g_pMainDlg->uartLen;
			BYTE *fileBuffer = new BYTE[g_pMainDlg->uartLen];
			memcpy(fileBuffer, g_pMainDlg->rxdata, g_pMainDlg->uartLen);
			upFile.Write(fileBuffer, g_pMainDlg->uartLen);
			delete[] fileBuffer;
			m_UploadProgress.SetStep(g_pMainDlg->uartLen);
			m_UploadProgress.StepIt();
			m_UploadListLogBox.AddString(_T("+ 上载了一部分"));
			m_UploadListLogBox.SetCurSel(m_UploadListLogBox.GetCount() - 1);
		}
		else
		{
			throw _T("接收数据错误");
		}
	
		g_pMainDlg->ComEvent.ResetEvent();
		CByteArray zeroByte;
		zeroByte.SetSize(1);
		zeroByte.SetAt(0, 0);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(zeroByte));
	}
	upFile.Close();
}