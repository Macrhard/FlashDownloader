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
	DDX_Control(pDX, IDC_EDIT_ADDR, m_Addr);
	DDX_Control(pDX, IDC_EDIT_LENGTH, m_Length);
	DDX_Control(pDX, IDC_EDIT_SAVEPATH, m_SavePath);
}


BEGIN_MESSAGE_MAP(CFlashUploadDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD, &CFlashUploadDlg::OnBnClickedButtonUpload)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CFlashUploadDlg::OnBnClickedButtonOpen)
END_MESSAGE_MAP()


// CFlashUploadDlg 消息处理程序


void CFlashUploadDlg::OnBnClickedButtonUpload()
{
	// TODO: 在此添加控件通知处理程序代码
	CString addr;
	m_Addr.GetWindowTextW(addr);
	DWORD a = _tcstoul(addr, NULL, 16);
	
	CString length;
	m_Length.GetWindowTextW(length);
	int b = _tcstoul(length, NULL, 10);

	CString	path;
	m_SavePath.GetWindowTextW(path);


	CByteArray byteArray;
	byteArray.RemoveAll(); //从该数组中删除所有指针 释放所有用于指针存储所占用的内存
	byteArray.SetSize(12); //设置数组元素的个数
	byteArray.SetAt(0, (BYTE)0);  // 给数组中指定下标的元素赋值，但不能动态增长数组
	byteArray.SetAt(1, (BYTE)0);
	byteArray.SetAt(2, (BYTE)0);
	byteArray.SetAt(3, (BYTE)0);
	byteArray.SetAt(4, (BYTE)(a));
	byteArray.SetAt(5, (BYTE)(a >> 8));
	byteArray.SetAt(6, (BYTE)(a >> 16));
	byteArray.SetAt(7, (BYTE)(a >> 24));
	byteArray.SetAt(8, (BYTE)(b));
	byteArray.SetAt(9, (BYTE)(b >> 8));
	byteArray.SetAt(10, (BYTE)(b >> 16));
	byteArray.SetAt(11, (BYTE)(b >> 24));
	g_pMainDlg->m_MSComm.put_Output(COleVariant(byteArray));
}


void CFlashUploadDlg::OnBnClickedButtonOpen()
{
	// TODO: 在此添加控件通知处理程序代码
}
