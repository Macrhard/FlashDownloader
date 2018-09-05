#pragma once
#include "afxwin.h"


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
public:
	CWinThread* pUartThread;
	afx_msg void OnBnClickedButtonRxtestStart();
	static UINT TestStartCmd(LPVOID pParam);
	afx_msg void OnBnClickedButtonRxtestStop();
	int TestRecv(BYTE rxdata[]);
	CComboBox m_ComboBoxFormat;
	CComboBox m_ComboBoxRFCH;
	CEdit m_NUM_PKT;
	CListBox m_ListboxLog;
	static UINT PreTestStop(LPVOID pParam);
	void PreTestResult();


	CString GetComboBoxCstring(CComboBox * comboBoxName);
	afx_msg void OnBnClickedButtonTxtestSend();
	afx_msg void OnBnClickedButtonTxtestStop();
	static UINT TxtetsStopCmd(LPVOID pParam);
	static UINT TxTestContinuousMode(LPVOID pParam);
	static UINT TxTestPacketMode_TwentyM(LPVOID pParam);
	static UINT TxTestPacketMode_FortyM(LPVOID pParam);
	CComboBox m_TxGain;
	CComboBox m_Mode;
	CComboBox m_Ht;
	CComboBox m_NonHt;
	CComboBox m_ElevenB;
	void PrintLog();
	//CComboBox m_Config;
	
};
