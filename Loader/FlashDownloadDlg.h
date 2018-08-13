#pragma once


// CFlashDownloadDlg 对话框

class CFlashDownloadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFlashDownloadDlg)

public:
	CFlashDownloadDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFlashDownloadDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FLASHDOWNLOAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
