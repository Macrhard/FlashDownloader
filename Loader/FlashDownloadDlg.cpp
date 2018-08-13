// FlashDownloadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Loader.h"
#include "FlashDownloadDlg.h"
#include "afxdialogex.h"


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
}


BEGIN_MESSAGE_MAP(CFlashDownloadDlg, CDialogEx)
END_MESSAGE_MAP()


// CFlashDownloadDlg 消息处理程序
