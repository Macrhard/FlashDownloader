#pragma once


// CFlashDownloadDlg �Ի���

class CFlashDownloadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFlashDownloadDlg)

public:
	CFlashDownloadDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFlashDownloadDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FLASHDOWNLOAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
