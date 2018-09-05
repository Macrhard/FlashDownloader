// RFloadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Loader.h"
#include "RFloadDlg.h"
#include "afxdialogex.h"
#include "exception"
#define CONTINUOUS_MODE _T("continuous")
#define PACKET_MODE_TWENTY	_T("packet-20M")
#define PACKET_MODE_FORTY	_T("packet-40M")

using namespace std;
/*测试程序使用的全局变量*/

long int pre_val, pre_sync, pre_xcr, pre_sat, pre_sig, pre_err;
long int recv_val, sync_val, xcr_val, sat_val, sig_val, err_val;

// CRFloadDlg 对话框

BOOL rxTestStatus = 0;
BOOL txTestStatus = 0;
IMPLEMENT_DYNAMIC(CRFloadDlg, CDialogEx)

CRFloadDlg::CRFloadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_RFLOAD, pParent)
{

}

CRFloadDlg::~CRFloadDlg()
{
}

void CRFloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_FORMAT, m_ComboBoxFormat);
	DDX_Control(pDX, IDC_COMBO_RFCH, m_ComboBoxRFCH);
	DDX_Control(pDX, IDC_EDIT_NUM_PKT, m_NUM_PKT);
	DDX_Control(pDX, IDC_LIST_LOG, m_ListboxLog);
	DDX_Control(pDX, IDC_COMBO_TXGAIN, m_TxGain);
	DDX_Control(pDX, IDC_COMBOTXMODE, m_Mode);
	DDX_Control(pDX, IDC_COMBO_RX_HT, m_Ht);
	DDX_Control(pDX, IDC_COMBO_RX_NONHT, m_NonHt);
	DDX_Control(pDX, IDC_COMBO_RX_11B, m_ElevenB);
	//DDX_Control(pDX, IDC_COMBO_CONFIG, m_Config);

}


BEGIN_MESSAGE_MAP(CRFloadDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_RXTEST_START, &CRFloadDlg::OnBnClickedButtonRxtestStart)
	ON_BN_CLICKED(IDC_BUTTON_RXTEST_STOP, &CRFloadDlg::OnBnClickedButtonRxtestStop)
	ON_BN_CLICKED(IDC_BUTTON_TXTEST_SEND, &CRFloadDlg::OnBnClickedButtonTxtestSend)
	ON_BN_CLICKED(IDC_BUTTON_TXTEST_STOP, &CRFloadDlg::OnBnClickedButtonTxtestStop)
	ON_CBN_SELCHANGE(IDC_COMBO_FORMAT, &CRFloadDlg::OnCbnSelchangeComboFormat)
END_MESSAGE_MAP()


// CRFloadDlg 消息处理程序


/*
RX 测试程序模块
*/

void CRFloadDlg::OnBnClickedButtonRxtestStart()
{
	pre_val = 0, pre_sync = 0, pre_xcr = 0, pre_sat = 0, pre_sig = 0, pre_err = 0;
	recv_val = 0, sync_val = 0, xcr_val = 0, sat_val = 0, sig_val = 0, err_val = 0;
	m_ListboxLog.ResetContent();
	if (g_pMainDlg->oldComNum == 0)
	{
		MessageBox(_T("Please selcet a appropriate serial port"), _T("Tips"), MB_OK | MB_ICONWARNING);
		return;
	}
	pUartThread = AfxBeginThread(RxTestStartCmd, this, THREAD_PRIORITY_NORMAL, 0, 0);
}

UINT CRFloadDlg::RxTestStartCmd(LPVOID pParam)
{
	try {
		CRFloadDlg *ptr = (CRFloadDlg*)pParam;
		CString rfchCstring = ptr->GetComboBoxCstring(&ptr->m_ComboBoxRFCH, _T("RF Channel"));
		rfchCstring.Delete(rfchCstring.Find(_T("-")), 5);
		ptr->m_ListboxLog.AddString(_T("+   RF Channel 是：") + rfchCstring);
		ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
		rfchCstring = _T("rf ch ") + rfchCstring + _T("\r");
		g_pMainDlg->m_MSComm.put_Output(COleVariant(rfchCstring));
		Sleep(50);
		memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);

		//获取 format 值
		CString format = ptr->GetComboBoxCstring(&ptr->m_ComboBoxFormat,_T("FORMAT"));
		ptr->m_ListboxLog.AddString(_T("+ Format 是：") + format);
		ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
		CString temp;
		//获取num_pkt 值
		CString num_pkt_cstr;
		ptr->m_NUM_PKT.GetWindowTextW(num_pkt_cstr);
		if (num_pkt_cstr == _T(""))
		{
			ptr->MessageBox(_T("PACKET NUM 未填入参数"), _T("Tips"), MB_OK | MB_ICONWARNING);
			return 0;
		}
		int num_pkt = _tcstoul(num_pkt_cstr, NULL, 10);
		ptr->m_ListboxLog.AddString(_T("+ PKT Number 是：") + num_pkt_cstr);
		ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
		memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("write 0x00900358 0x10\r")));
		Sleep(25);
		memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x009004B8 4\r")));
		Sleep(30);
		pre_val = ptr->TestRecv(g_pMainDlg->rxdata);

		temp.Format(_T("%d"), pre_val);
		ptr->m_ListboxLog.AddString(temp);


		memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x00A040D0 4\r")));
		Sleep(100);
		pre_sync = ptr->TestRecv(g_pMainDlg->rxdata);
		memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);

		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x00A040B8 4\r\n")));
		Sleep(30);


		pre_xcr = ptr->TestRecv(g_pMainDlg->rxdata);
		memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);

		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x00A040C0 4\r\n")));
		Sleep(100);
	
		pre_sat = ptr->TestRecv(g_pMainDlg->rxdata);
		memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);

		switch (_tcstoul(format, NULL, 10))
		{
		case 0:	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x00A040F0 4\r\n")));
			Sleep(100);
			pre_sig = ptr->TestRecv(g_pMainDlg->rxdata);
			memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
			break;
		case 1:	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x00A04100 4\r\n")));
			Sleep(100);
			pre_sig = ptr->TestRecv(g_pMainDlg->rxdata);
			memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
			break;
		case 4:	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x00A040E8 4\r\n")));
			Sleep(100);
			pre_sig = ptr->TestRecv(g_pMainDlg->rxdata);
			memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
			break;
		default: g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x00A040F0 4\r\n")));
			Sleep(100);

			//WaitForSingleObject(ptr->SPRecEvent, 500000);
			pre_sig = ptr->TestRecv(g_pMainDlg->rxdata);
			memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
			break;
		}

		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("write 0x00900358 0x11\r")));
		Sleep(100);

		//WaitForSingleObject(ptr->SPRecEvent, 50000);
		memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);

		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x009004B8 4\r\n")));
		Sleep(100);

		//WaitForSingleObject(ptr->SPRecEvent, 500000);
		pre_err = ptr->TestRecv(g_pMainDlg->rxdata);
		memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
	}
	catch (...) {
		return 0;
	}
	rxTestStatus = 1;
	return 0;

}

//接收测试时串口回复的内容
int CRFloadDlg::TestRecv(BYTE rxdata[])
{
	int i, head = 0;
	BYTE temp[8] = { 0 };
	for (i = 0; i < g_pMainDlg->uartLen; i++)
	{
		if (rxdata[i] == ':')
		{
			head = i + 2;
		}

	}
	memcpy(temp, &rxdata[head], 8);
	CString hexNumStr;
	hexNumStr = temp;
	i = _tcstoul(hexNumStr, NULL, 16);
	return i;
}

void CRFloadDlg::OnBnClickedButtonRxtestStop()
{
	if (!rxTestStatus)
	{
		MessageBox(_T("Please click start in the first"), _T("Tips"), MB_OK | MB_ICONWARNING);
		return;
	}
	// TODO: 在此添加控件通知处理程序代码
	if (g_pMainDlg->oldComNum == 0)
	{
		MessageBox(_T("Please selcet a appropriate serial port"), _T("Tips"), MB_OK | MB_ICONWARNING);
		return;
	}
	pUartThread = AfxBeginThread(RxTestStopCmd, this, THREAD_PRIORITY_NORMAL, 0, 0);
}

UINT CRFloadDlg::RxTestStopCmd(LPVOID pParam)
{
	CRFloadDlg *ptr = (CRFloadDlg*)pParam;

	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("write 0x00900358 0x10\r\n")));
	Sleep(100);
	memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x009004B8 4\r\n")));
	Sleep(100);

	recv_val = ptr->TestRecv(g_pMainDlg->rxdata);
		
	CString temp;
	temp.Format(_T("%d"), recv_val);
	ptr->m_ListboxLog.AddString(temp);
	memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);


	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x00A040D0 4\r\n")));
	Sleep(100);

	sync_val = ptr->TestRecv(g_pMainDlg->rxdata);
	memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
	

	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x00A040B8 4\r\n")));
	Sleep(100);
	xcr_val = ptr->TestRecv(g_pMainDlg->rxdata);
	memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);


	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x00A040C0 4\r\n")));
	Sleep(100);

	sat_val = ptr->TestRecv(g_pMainDlg->rxdata);
	memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);

	CString format;
	int FormatIndex = ptr->m_ComboBoxFormat.GetCurSel();
	ptr->m_ComboBoxFormat.GetLBText(FormatIndex, format);
	switch (_tcstoul(format, NULL, 10))
	{
	case 0:	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x00A040F0 4\r\n")));
		Sleep(100);
		sig_val = ptr->TestRecv(g_pMainDlg->rxdata);
		memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
		break;
	case 1: g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x00A04100 4\r\n")));
		Sleep(100);
		sig_val = ptr->TestRecv(g_pMainDlg->rxdata);

		memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
		break;
	case 4:	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x00A040E8 4\r\n")));
		Sleep(100);
		sig_val = ptr->TestRecv(g_pMainDlg->rxdata);

		memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
		break;
	default: g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x00A040F0 4\r\n")));
		Sleep(100);

		sig_val = ptr->TestRecv(g_pMainDlg->rxdata);

		memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
		break;
	}

	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("write 0x00900358 0x11\r")));
	Sleep(100);
	memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x009004B8 4\r\n")));
	Sleep(100);
	err_val = ptr->TestRecv(g_pMainDlg->rxdata);
	memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
	ptr->RxTestResult();
	return 0;
}

void CRFloadDlg::RxTestResult()
{
	unsigned int  recv_pkt, sync_ok, xcr_ok, sat_ok, sig_ok, err_pkt;
	CString PER_cstr, recv_pkt_cstr, sync_ok_cstr, xcr_ok_cstr, sat_ok_cstr, sig_ok_cstr, err_pkt_cstr;
	float PER;
	recv_pkt = recv_val - pre_val;
	sync_ok = sync_val - pre_sync;
	xcr_ok = xcr_val - pre_xcr;
	sat_ok = sat_val - pre_sat;
	sig_ok = sig_val - pre_sig;
	err_pkt = err_val - pre_err;

	//PER
	PER = (1000 - (float)recv_pkt) / 10;
	PER_cstr.Format(_T("%.2f"), PER);
	m_ListboxLog.AddString(_T("PER: ") + PER_cstr + _T("%"));
	m_ListboxLog.SetCurSel(m_ListboxLog.GetCount() - 1);

	//recv_pkt
	recv_pkt_cstr.Format(_T("%d"), recv_pkt);
	m_ListboxLog.AddString(_T("recv_pkt: ") + recv_pkt_cstr);
	m_ListboxLog.SetCurSel(m_ListboxLog.GetCount() - 1);

	//sync_ok
	sync_ok_cstr.Format(_T("%d"), sync_ok);
	m_ListboxLog.AddString(_T("sync_ok:  ") + sync_ok_cstr);
	m_ListboxLog.SetCurSel(m_ListboxLog.GetCount() - 1);

	//xcr_ok
	xcr_ok_cstr.Format(_T("%d"), xcr_ok);
	m_ListboxLog.AddString(_T("xcr_ok:   ") + xcr_ok_cstr);
	m_ListboxLog.SetCurSel(m_ListboxLog.GetCount() - 1);

	//sat_ok
	sat_ok_cstr.Format(_T("%d"), sat_ok);
	m_ListboxLog.AddString(_T("sat_ok:   ") + sat_ok_cstr);
	m_ListboxLog.SetCurSel(m_ListboxLog.GetCount() - 1);

	//sig_ok
	sig_ok_cstr.Format(_T("%d"), sig_ok);
	m_ListboxLog.AddString(_T("sig_ok:   ") + sig_ok_cstr);
	m_ListboxLog.SetCurSel(m_ListboxLog.GetCount() - 1);

	//err_pkt
	err_pkt_cstr.Format(_T("%d"), err_pkt);
	m_ListboxLog.AddString(_T("err_pkt:  ") + err_pkt_cstr);
	m_ListboxLog.SetCurSel(m_ListboxLog.GetCount() - 1);

}


/*
TX 测试程序模块
*/

void CRFloadDlg::OnBnClickedButtonTxtestSend()
{
	if (g_pMainDlg->oldComNum == 0)
	{
		MessageBox(_T("Please selcet a appropriate serial port"), _T("Tips"), MB_OK | MB_ICONWARNING);
		return;
	}
	try {
			m_ListboxLog.ResetContent();
			CString modeCstring = GetComboBoxCstring(&m_Mode, _T("MODE"));

			if (modeCstring == CONTINUOUS_MODE)
			{
				pUartThread = AfxBeginThread(TxTestContinuousMode, this, THREAD_PRIORITY_NORMAL, 0, 0);
			}
			else if (modeCstring == PACKET_MODE_TWENTY)
			{
				pUartThread = AfxBeginThread(TxTestPacketMode_TwentyM, this, THREAD_PRIORITY_NORMAL, 0, 0);
			}
			else if (modeCstring == PACKET_MODE_FORTY)
			{
				pUartThread = AfxBeginThread(TxTestPacketMode_FortyM, this, THREAD_PRIORITY_NORMAL, 0, 0);
			}
			else
			{
				MessageBox(_T("Please select a test mode"), _T("Tips"), MB_OK | MB_ICONWARNING);
			}
	}
	catch (...)
	{
		return;
	}
}

UINT CRFloadDlg::TxTestContinuousMode(LPVOID pParam)
{
	try 
	{
		CRFloadDlg *ptr = (CRFloadDlg*)pParam;
		CString channelCstring = ptr->GetComboBoxCstring(&ptr->m_ComboBoxRFCH,_T("RF Channel"));
		if (channelCstring == _T("nothing"))
		{
			ptr->MessageBox(_T("未配置Channel"), _T("Tips"), MB_OK | MB_ICONWARNING);
			return 0;
		}
		channelCstring.Delete(channelCstring.Find(_T("-")), 5);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("rf ch ")+channelCstring+_T("\r")));
		Sleep(500);
		ptr->PrintLog();
		CString txGainCstring = ptr->GetComboBoxCstring(&ptr->m_TxGain,_T("TX GAIN"));
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("rf txgain ") + txGainCstring + _T("\r")));
		Sleep(100);
		ptr->PrintLog();
		CString formatCstring = ptr->GetComboBoxCstring(&ptr->m_ComboBoxFormat,_T("FORMAT"));
		//0-nonht;1-ht;4-11b;
		if (formatCstring == _T("0-nonht"))
		{
			CString nonhtCstring = ptr->GetComboBoxCstring(&ptr->m_NonHt,_T("NONHT"));
			g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("phy romgen start nonht ") + nonhtCstring + _T("\r")));
			Sleep(50);
			ptr->PrintLog();
		}
		else if(formatCstring == _T("1-ht"))
		{
			CString htCstring = ptr->GetComboBoxCstring(&ptr->m_Ht,_T("HT"));
			g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("phy romgen start ht ") + htCstring + _T("\r")));
			Sleep(50);
			ptr->PrintLog();
		}
		else if(formatCstring == _T("4-11b"))
		{
			CString elevenBCstring = ptr->GetComboBoxCstring(&ptr->m_ElevenB,_T("11b"));
			g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("phy romgen start 11b ") + elevenBCstring + _T("\r")));
			Sleep(50);
			ptr->PrintLog();
	}

	}
	catch (...)
	{
		return 0;
	}
	txTestStatus = 1;
	return 0;
}

UINT CRFloadDlg::TxTestPacketMode_TwentyM(LPVOID pParam)
{
	try {
		CRFloadDlg *ptr = (CRFloadDlg*)pParam;
		CString channelCstring = ptr->GetComboBoxCstring(&ptr->m_ComboBoxRFCH, _T("RF Channel"));
		channelCstring.Delete(channelCstring.Find(_T("-")), 5);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("rf ch ") + channelCstring + _T("\r")));
		Sleep(100);

		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("write 0xa06020 0x0\r")));
		Sleep(50);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("rf rf_bandwidth_set\r")));
		Sleep(50);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set macaddr kwon 0\r")));
		Sleep(50);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set destaddr kim 0\r")));
		Sleep(50);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set txop off\r")));
		Sleep(50);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("hif down\r")));
		Sleep(50);

		CString formatCstring = ptr->GetComboBoxCstring(&ptr->m_ComboBoxFormat, _T("FORMAT"));
		if (formatCstring == _T("0-nonht"))
		{
			g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set vif 0 format=nonht\r")));
			Sleep(50);
			CString nonht = ptr->GetComboBoxCstring(&ptr->m_NonHt, _T("NONHT"));
			g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set config 0 0 ") + nonht));
			Sleep(50);
		}
		else if (formatCstring == _T("1-ht"))
		{
			g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set vif 0 format=ht\r")));
			Sleep(50);
			CString ht = ptr->GetComboBoxCstring(&ptr->m_Ht, _T("HT"));
			g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set config 0 0 ") + ht));
			Sleep(50);
		}
		else if (formatCstring == _T("4-11b"))
		{
			g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set vif 0 format=11b\r")));
			Sleep(50);
			CString elevenB = ptr->GetComboBoxCstring(&ptr->m_ElevenB, _T("11b"));
			g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set config 0 0 ") + elevenB));
			Sleep(50);
		}

		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("write 0x900004 0x01200072\r")));
		Sleep(50);
		CString packetNumCsting;
		ptr->m_NUM_PKT.GetWindowTextW(packetNumCsting);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("test start ") + packetNumCsting + _T(" 1000 450 0 0\r")));
		Sleep(50);
	}
	catch (...)
	{
		return 0;
	}
	txTestStatus = 1;
	return 0;
}

UINT CRFloadDlg::TxTestPacketMode_FortyM(LPVOID pParam)
{
	try {
		CRFloadDlg *ptr = (CRFloadDlg*)pParam;
		CString channelCstring = ptr->GetComboBoxCstring(&ptr->m_ComboBoxRFCH, _T("RF Channel"));
		channelCstring.Delete(channelCstring.Find(_T("-")), 5);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("rf ch ") + channelCstring + _T("\r")));
		Sleep(100);

		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("write 0xa06020 0x1\r")));
		Sleep(50);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("rf rf_bandwidth_set\r")));
		Sleep(50);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set macaddr kwon 0\r")));
		Sleep(50);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set destaddr kim 0\r")));
		Sleep(50);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set txop off\r")));
		Sleep(50);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("hif down\r")));
		Sleep(50);

		CString formatCstring = ptr->GetComboBoxCstring(&ptr->m_ComboBoxFormat, _T("FORMAT"));

		if (formatCstring == _T("0-nonht"))
		{
			g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set vif 0 format=nonht\r")));
			Sleep(50);
			CString nonht = ptr->GetComboBoxCstring(&ptr->m_NonHt, _T("NONHT"));
			g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set config 0 0 ") + nonht));
			Sleep(50);
		}
		else if (formatCstring == _T("1-ht"))
		{
			g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set vif 0 format=ht\r")));
			Sleep(50);
			CString ht = ptr->GetComboBoxCstring(&ptr->m_Ht, _T("HT"));
			g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set config 0 0 ") + ht));
			Sleep(50);
		}
		else if (formatCstring == _T("4-11b"))
		{
			g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set vif 0 format=11b\r")));
			Sleep(50);
			CString elevenB = ptr->GetComboBoxCstring(&ptr->m_ElevenB, _T("11b"));
			g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set config 0 0 ") + elevenB));
			Sleep(50);
		}

		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set vif 0 bandwidth=40m lower\r")));
		Sleep(50);
		Sleep(50);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("write 0x900004 0x01200072\r")));
		Sleep(50);
		CString packetNumCsting;
		ptr->m_NUM_PKT.GetWindowTextW(packetNumCsting);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("test start ") + packetNumCsting + _T(" 1000 450 0 0\r")));
		Sleep(50);
	}
	catch (...)
	{
		return 0;
	}
	txTestStatus = 1;
	return 0;
}


void CRFloadDlg::OnBnClickedButtonTxtestStop()
{
	if (!txTestStatus)
	{
		MessageBox(_T("Please run send script"), _T("Tips"), MB_OK | MB_ICONWARNING);
		return ;
	}
	pUartThread = AfxBeginThread(TxtetsStopCmd, this, THREAD_PRIORITY_NORMAL, 0, 0);
}

UINT CRFloadDlg::TxtetsStopCmd(LPVOID pParam)
{
	try {
		CRFloadDlg *ptr = (CRFloadDlg*)pParam;
		CString modeCstring = ptr->GetComboBoxCstring(&ptr->m_Mode, _T("MODE"));
		if (modeCstring == CONTINUOUS_MODE)
		{
			g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("phy romgen stop\r")));
			Sleep(50);
		}
		else if (modeCstring == PACKET_MODE_TWENTY)
		{
			g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("test stop\r")));
			Sleep(50);
		}
		else if (modeCstring == PACKET_MODE_FORTY)
		{
			g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("test stop\r")));
			Sleep(50);
		}
	}
	catch (...) {
		return 0;
	}
	return 0;
}

CString CRFloadDlg::GetComboBoxCstring(CComboBox *comboBoxPoint,CString comboBoxName)
{
	CString comboBoxStr;
	int index = comboBoxPoint->GetCurSel();
	if (index == 0xffffffff)
	{
		MessageBox(comboBoxName +_T("未填入参数"), _T("Tips"), MB_OK | MB_ICONWARNING);
		throw "comboBoxErro";
	}
	comboBoxPoint->GetLBText(index, comboBoxStr);
	//returnNum = _tcstoul(comboBoxStr, NULL, 10);
	return comboBoxStr;

}

void CRFloadDlg::PrintLog()
{
	CString logCstring;
	g_pMainDlg->rxdata[g_pMainDlg->uartLen] = '\0';
	logCstring = g_pMainDlg->rxdata;
	m_ListboxLog.AddString(logCstring);
	m_ListboxLog.SetCurSel(m_ListboxLog.GetCount() - 1);
	memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
}

void CRFloadDlg::OnCbnSelchangeComboFormat()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_ComboBoxFormat.GetCurSel();
	if (index == 0)
	{
		GetDlgItem(IDC_COMBO_RX_NONHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_RX_HT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_11B)->EnableWindow(FALSE);
	}
	else if(index == 1)
	{
		GetDlgItem(IDC_COMBO_RX_NONHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_HT)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_RX_11B)->EnableWindow(FALSE);
	}
	else if(index == 2)
	{
		GetDlgItem(IDC_COMBO_RX_NONHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_HT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RX_11B)->EnableWindow(TRUE);
	}
}