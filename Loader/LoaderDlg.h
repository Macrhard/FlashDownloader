
// LoaderDlg.h : ͷ�ļ�
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

// CLoaderDlg �Ի���
class CLoaderDlg : public CDialogEx
{
// ����
public:
	CLoaderDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOADER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	enum _UartResponse
	{
		OK = 0x00,
		NV = 0x02,
		UBOOT = 0x01,	  /* uboot  */
		ANDES = 0x03,    /* adnes  */
		ANDES1 = 0x04, /* andes  */
		FAIL = 0xFF
	}UartResp, fileType;
	enum _UartStatus
	{
		UART_IDLE = 0,
		UART_SYNC = 1,
		UART_ADDR = 2,
		UART_CODE = 3
	}UartState;
	enum _LoadType
	{
		Download = 0,
		Upload = 1,
		SYNC = 3,
		StartUpload = 4
	}LoadType;

	CEvent DownloadEvent;
	CEvent UploadEvent;
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
};
