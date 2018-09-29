
// LoaderDlg.h : 头文件
//

#pragma once
#include "mscomm1.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "Dbt.h"
#define PAGE_LEN (1*1024)
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

	LONG uartLen;
	BYTE rxdata[PAGE_LEN];

	/*BOOL mscomState = 0;*/
	int pathCount = 0;

	enum _UartStatus
	{
		CONTINUS_STAT = 0x00,
		BOOTROM_STAT = 0x01,
		FLASH_STAT = 0x02,
		CHGEBAUD_STAT = 0x03
	}UartStatus;

	enum _UartResponse
	{
		UBOOT = 0x01,	  /* uboot  */
		NV = 0x02,
		ANDES = 0x03,    /* adnes  */
		ANDES1 = 0x04, /* andes  */
		COMBINE = 0x05
	}fileType;
	
	enum _LoadType
	{
		Download = 0,
		Upload = 1,
	}LoadType;

	CEvent ComEvent;
	CStatusBar	m_StatusBar;
	CMscomm1 m_MSComm;
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
	void TraversalCom(void);
	CComboBox m_ComboBoxCom;
	CComboBox m_ComboBoxBaud;
	CComboBox m_FlashSize;
	CString comBaudRate;
	CString strFlashSize;
	int oldComNum;
	afx_msg void OnCbnSelchangeComboBaud();
	afx_msg void OnCbnSelchangeComboCom();
	afx_msg void OnCbnSelchangeComboFlashsize();
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm1();
protected:
	afx_msg LRESULT OnMainMsg(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonCloseCom();
	CButton m_ifMostTopCheck;
	afx_msg void OnBnClickedCheck1();
	void CreateConfigFile();
};
