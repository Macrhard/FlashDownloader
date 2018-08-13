
// LoaderDlg.h : 头文件
//

#pragma once
#include "mscomm1.h"
#include "FlashDownloadDlg.h"
#include "FlashUploadDlg.h"
#include "RFloadDlg.h"
#include "afxcmn.h"

// CLoaderDlg 对话框
class CLoaderDlg : public CDialogEx
{
// 构造
public:
	CLoaderDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOADER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMscomm1 m_MSComm;
	CFlashDownloadDlg m_flashDownloadDlg;
	CFlashUploadDlg m_flashUploadDlg;
	CRFloadDlg m_rfloadDlg;
	CTabCtrl m_tab;
	CDialog* pDialog[3];
	int m_CurTabSel;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
