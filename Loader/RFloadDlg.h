#pragma once


// CRFloadDlg �Ի���

class CRFloadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRFloadDlg)

public:
	CRFloadDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRFloadDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RFLOAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
