
// Loader.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "FlashDownloadDlg.h"
#include "FlashUploadDlg.h"
#include "RFloadDlg.h"
#include "LoaderDlg.h"
#include "MMSystem.h"

#define WM_MAIN_MSG WM_USER+0x1001
#define WM_DOWNLOAD_MSG WM_USER+0x2001

extern CLoaderDlg *g_pMainDlg;
extern CFlashDownloadDlg *g_pDownloadDlg;
// CLoaderApp: 
// �йش����ʵ�֣������ Loader.cpp
//

class CLoaderApp : public CWinApp
{
public:
	CLoaderApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CLoaderApp theApp;