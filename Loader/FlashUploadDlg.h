#pragma once


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
};
