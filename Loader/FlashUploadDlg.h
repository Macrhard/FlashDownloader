#pragma once
#include "afxwin.h"


// CFlashUploadDlg 对话框

class CFlashUploadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFlashUploadDlg)

public:
	CFlashUploadDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFlashUploadDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FLASHUPLOAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonUpload();
	CEdit m_Addr;
	CEdit m_Length;
	CEdit m_SavePath;
};
