
// LoaderDlg.h : 头文件
//

#pragma once
#include "mscomm1.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "Dbt.h"
//#include "FlashDownloadDlg.h"
//#include "FlashUploadDlg.h"
//#include "RFloadDlg.h"

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
	CStatusBar	m_StatusBar;
	CMscomm1 m_MSComm;
	LONG uartLen;
	BYTE rxdata[1024];
	CFlashDownloadDlg m_flashDownloadDlg;
	CFlashUploadDlg m_flashUploadDlg;
	CRFloadDlg m_rfloadDlg;

	CTabCtrl m_tab;
	CDialog* pDialog[3];
	int m_CurTabSel;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD dwData);
	void SetDateTime();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void SendSncy(void)
	{
		m_MSComm.put_Output(COleVariant(_T("cnys")));

	}
	
	void TraversalCom(void);
	CComboBox m_ComboBoxCom;
	CComboBox m_ComboBoxBaud;
	CComboBox m_FlashSize;
	CString comBaudRate;
	int oldComNum;
	afx_msg void OnCbnSelchangeComboBaud();
	afx_msg void OnCbnSelchangeComboCom();
	afx_msg void OnCbnSelchangeComboFlashsize();
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm1();
protected:
	afx_msg LRESULT OnMainMsg(WPARAM wParam, LPARAM lParam);
};
