#pragma once
#include "afxwin.h"



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
public:
	afx_msg void OnBnClickedButtonOpne1();
	afx_msg void OnBnClickedButtonOpen2();
	afx_msg void OnBnClickedButtonOpen3();
	afx_msg void OnBnClickedButtonOpen4();
	afx_msg void OnBnClickedButtonOpen5();
	afx_msg void OnBnClickedButtonOpen6();
	afx_msg void OnBnClickedButtonOpen7();
	afx_msg void OnBnClickedButtonOpen8();
	afx_msg void OnBnClickedButtonCln1();
	afx_msg void OnBnClickedButtonCln2();
	afx_msg void OnBnClickedButtonCln3();
	afx_msg void OnBnClickedButtonCln4();
	afx_msg void OnBnClickedButtonCln5();
	afx_msg void OnBnClickedButtonCln6();
	afx_msg void OnBnClickedButtonCln7();
	afx_msg void OnBnClickedButtonCln8();
	void SelcetFile(int index, int pathID, int addrID);
	CString GetConfigInfo(CString title, CString option);
	CButton m_check1;
	CButton m_check2;
	CButton m_check3;
	CButton m_check4;
	CButton m_check5;
	CButton m_check6;
	CButton m_check7;
	CButton m_check8;
	afx_msg void OnBnClickedButtonClnall();
};
