// FlashUploadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Loader.h"
#include "FlashUploadDlg.h"
#include "afxdialogex.h"


// CFlashUploadDlg 对话框

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
}


BEGIN_MESSAGE_MAP(CFlashUploadDlg, CDialogEx)
END_MESSAGE_MAP()


// CFlashUploadDlg 消息处理程序
