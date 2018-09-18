#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CFlashUploadDlg �Ի���

class CFlashUploadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFlashUploadDlg)

public:
	CFlashUploadDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFlashUploadDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FLASHUPLOAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CWinThread* pUartThread;
	afx_msg void OnBnClickedButtonUpload();
	afx_msg void OnBnClickedButtonOpen();
	void ReceiveCode(int upFileLength);
	static UINT UartUpload(LPVOID pParam);
	CEdit m_Addr;
	CEdit m_Length;
	CEdit m_SavePath;
	//CListBox m_UploadListLogBox;	
	CProgressCtrl m_UploadProgress;
	CListBox m_UploadListLogBox;
};
