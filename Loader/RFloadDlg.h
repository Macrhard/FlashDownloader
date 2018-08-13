#pragma once


// CRFloadDlg 对话框

class CRFloadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRFloadDlg)

public:
	CRFloadDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRFloadDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RFLOAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
