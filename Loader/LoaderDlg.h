
// LoaderDlg.h : ͷ�ļ�
//

#pragma once
#include "mscomm1.h"
#include "FlashDownloadDlg.h"
#include "FlashUploadDlg.h"
#include "RFloadDlg.h"
#include "afxcmn.h"

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
	CMscomm1 m_MSComm;
	CFlashDownloadDlg m_flashDownloadDlg;
	CFlashUploadDlg m_flashUploadDlg;
	CRFloadDlg m_rfloadDlg;
	CTabCtrl m_tab;
	CDialog* pDialog[3];
	int m_CurTabSel;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
