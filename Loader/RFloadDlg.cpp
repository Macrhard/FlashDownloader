// RFloadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Loader.h"
#include "RFloadDlg.h"
#include "afxdialogex.h"


// CRFloadDlg 对话框

IMPLEMENT_DYNAMIC(CRFloadDlg, CDialogEx)

CRFloadDlg::CRFloadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_RFLOAD, pParent)
{

}

CRFloadDlg::~CRFloadDlg()
{
}

void CRFloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRFloadDlg, CDialogEx)
END_MESSAGE_MAP()


// CRFloadDlg 消息处理程序
