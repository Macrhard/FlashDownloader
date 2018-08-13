#pragma once


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
};
