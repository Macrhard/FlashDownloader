// RFloadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Loader.h"
#include "RFloadDlg.h"
#include "afxdialogex.h"

#define CONTINUOUS_MODE _T("continuous")
#define PACKET_MODE_TWENTY	_T("packet-20M")
#define PACKET_MODE_FORTY	_T("packet-40M")

/*测试程序使用的全局变量*/

long int pre_val, pre_sync, pre_xcr, pre_sat, pre_sig, pre_err;
long int recv_val, sync_val, xcr_val, sat_val, sig_val, err_val;

// CRFloadDlg 对话框


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
	DDX_Control(pDX, IDC_COMBO_CONFIG, m_Config);
}


BEGIN_MESSAGE_MAP(CRFloadDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_RXTEST_START, &CRFloadDlg::OnBnClickedButtonRxtestStart)
	ON_BN_CLICKED(IDC_BUTTON_RXTEST_STOP, &CRFloadDlg::OnBnClickedButtonRxtestStop)
	ON_BN_CLICKED(IDC_BUTTON_TXTEST_SEND, &CRFloadDlg::OnBnClickedButtonTxtestSend)
	ON_BN_CLICKED(IDC_BUTTON_TXTEST_STOP, &CRFloadDlg::OnBnClickedButtonTxtestStop)
END_MESSAGE_MAP()


// CRFloadDlg 消息处理程序


/*
RX 测试程序模块
*/

void CRFloadDlg::OnBnClickedButtonRxtestStart()
{
	 pre_val = 0, pre_sync = 0, pre_xcr = 0, pre_sat = 0, pre_sig = 0, pre_err = 0;
	 recv_val = 0, sync_val = 0, xcr_val = 0, sat_val = 0, sig_val = 0, err_val = 0;
	// TODO: 在此添加控件通知处理程序代码
	m_ListboxLog.ResetContent();
	pUartThread = AfxBeginThread(RepTestStart, this, THREAD_PRIORITY_NORMAL, 0, 0);
}

UINT CRFloadDlg::RepTestStart(LPVOID pParam)
{
	
	CRFloadDlg *ptr = (CRFloadDlg*)pParam;
	//g_pMainDlg->rxdata = {0};
	//rf ch (1`15)
	CString rfch;
	int RfchIndex = ptr->m_ComboBoxRFCH.GetCurSel();
	ptr->m_ComboBoxRFCH.GetLBText(RfchIndex, rfch);
	ptr->m_ListboxLog.AddString(_T("+   RF Channel 是：") + rfch);
	ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
	rfch = _T("rf ch ") + rfch + _T("\r");
	g_pMainDlg->m_MSComm.put_Output(COleVariant(rfch));
	Sleep(50);
	//WaitForSingleObject(g_pMainDlg->SPRecEvent, 5000);
	//如果rxdata 未初始化 会产生bug 即 串口如果不发东西过来
	memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);

	//获取 format 值
	CString format;
	int FormatIndex = ptr->m_ComboBoxFormat.GetCurSel();
	ptr->m_ComboBoxFormat.GetLBText(FormatIndex, format);
	ptr->m_ListboxLog.AddString(_T("+ Format 是：") + format);
	ptr->m_ListboxLog.SetCurSel(ptr->m_ListboxLog.GetCount() - 1);
	CString temp;
	//获取num_pkt 值
	CString num_pkt_cstr;
	ptr->m_NUM_PKT.GetWindowTextW(num_pkt_cstr);
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
	//}

	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x00A040D0 4\r")));
	Sleep(100);
	/*if (WaitForSingleObject(ptr->SPRecEvent, 5000) == WAIT_OBJECT_0)
	{*/
		pre_sync = ptr->TestRecv(g_pMainDlg->rxdata);
		memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
	//}

	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x00A040B8 4\r\n")));
	Sleep(30);

	/*if (WaitForSingleObject(ptr->SPRecEvent, 5000) == WAIT_OBJECT_0)
	{*/
		pre_xcr = ptr->TestRecv(g_pMainDlg->rxdata);
		memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);

	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x00A040C0 4\r\n")));
	Sleep(100);
	/*if (WaitForSingleObject(ptr->SPRecEvent, 5000) == WAIT_OBJECT_0)
	{*/
		pre_sat = ptr->TestRecv(g_pMainDlg->rxdata);
		memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
	//}

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
	// TODO: 在此添加控件通知处理程序代码
	pUartThread = AfxBeginThread(PreTestStop, this, THREAD_PRIORITY_NORMAL, 0, 0);
}

UINT CRFloadDlg::PreTestStop(LPVOID pParam)
{
	CRFloadDlg *ptr = (CRFloadDlg*)pParam;
	CString rfch;
	//CString recv_val_cstr, recv_sync_cstr, recv_xcr_cstr, recv_sat_cstr, recv_sig_cstr, recv_err_cstr;

	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("write 0x00900358 0x10\r\n")));
	Sleep(100);
	//WaitForSingleObject(ptr->SPRecEvent, 5000);
	memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x009004B8 4\r\n")));
	Sleep(100);

	/*if (WaitForSingleObject(ptr->SPRecEvent, 10000) == WAIT_OBJECT_0)
	{*/
		recv_val = ptr->TestRecv(g_pMainDlg->rxdata);
		
		CString temp;
		temp.Format(_T("%d"), recv_val);
		ptr->m_ListboxLog.AddString(temp);

		memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
	//}

	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x00A040D0 4\r\n")));
	Sleep(100);

	/*if (WaitForSingleObject(ptr->SPRecEvent, 5000) == WAIT_OBJECT_0)
	{*/
		sync_val = ptr->TestRecv(g_pMainDlg->rxdata);
		memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
	//}

	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x00A040B8 4\r\n")));
	Sleep(100);

	//WaitForSingleObject(ptr->SPRecEvent, 500000);
	xcr_val = ptr->TestRecv(g_pMainDlg->rxdata);
	memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);


	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x00A040C0 4\r\n")));
	Sleep(100);

	//WaitForSingleObject(ptr->SPRecEvent, 500000);
	sat_val = ptr->TestRecv(g_pMainDlg->rxdata);
	memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);

	CString format;
	int FormatIndex = ptr->m_ComboBoxFormat.GetCurSel();
	ptr->m_ComboBoxFormat.GetLBText(FormatIndex, format);
	switch (_tcstoul(format, NULL, 10))
	{
	case 0:	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x00A040F0 4\r\n")));
		Sleep(100);

		//WaitForSingleObject(ptr->SPRecEvent, 500000);
		sig_val = ptr->TestRecv(g_pMainDlg->rxdata);

		memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
		break;
	case 1: g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x00A04100 4\r\n")));
		Sleep(100);

		//WaitForSingleObject(ptr->SPRecEvent, 500000);
		sig_val = ptr->TestRecv(g_pMainDlg->rxdata);

		memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
		break;
	case 4:	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x00A040E8 4\r\n")));
		Sleep(100);

		//WaitForSingleObject(ptr->SPRecEvent, 500000);
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

	//WaitForSingleObject(ptr->SPRecEvent, 50000);
	memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);

	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("read 0x009004B8 4\r\n")));
	Sleep(100);
	err_val = ptr->TestRecv(g_pMainDlg->rxdata);

	memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
	ptr->PreTestResult();
	return 0;
}

void CRFloadDlg::PreTestResult()
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
	m_ListboxLog.ResetContent();
	CString modeCstring = GetComboBoxCstring(&m_Mode);

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

}

UINT CRFloadDlg::TxTestContinuousMode(LPVOID pParam)
{
	CRFloadDlg *ptr = (CRFloadDlg*)pParam;
	CString channelCstring = ptr->GetComboBoxCstring(&ptr->m_ComboBoxRFCH);
	channelCstring.Delete(channelCstring.Find(_T("-")), 5);
	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("rf ch ")+channelCstring+_T("\r")));
	Sleep(500);
	ptr->PrintLog();
	CString txGainCstring = ptr->GetComboBoxCstring(&ptr->m_TxGain);
	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("rf txgain ") + txGainCstring + _T("\r")));
	Sleep(100);
	ptr->PrintLog();
	CString formatCstring = ptr->GetComboBoxCstring(&ptr->m_ComboBoxFormat);
	//0-nonht;1-ht;4-11b;
	if (formatCstring == _T("0-nonht"))
	{
		CString nonhtCstring = ptr->GetComboBoxCstring(&ptr->m_NonHt);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("phy romgen start nonht ") + nonhtCstring + _T("\r")));
		Sleep(50);
		ptr->PrintLog();
	}
	else if(formatCstring == _T("1-ht"))
	{
		CString htCstring = ptr->GetComboBoxCstring(&ptr->m_Ht);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("phy romgen start ht ") + htCstring + _T("\r")));
		Sleep(50);
		ptr->PrintLog();
	}
	else if(formatCstring == _T("4-11b"))
	{
		CString elevenbCstring = ptr->GetComboBoxCstring(&ptr->m_ElevenB);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("phy romgen start 11b ") + elevenbCstring + _T("\r")));
		Sleep(50);
		ptr->PrintLog();
	}

	return 0;
}

UINT CRFloadDlg::TxTestPacketMode_TwentyM(LPVOID pParam)
{
	CRFloadDlg *ptr = (CRFloadDlg*)pParam;
	CString channelCstring = ptr->GetComboBoxCstring(&ptr->m_ComboBoxRFCH);
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

	CString formatCstring = ptr->GetComboBoxCstring(&ptr->m_ComboBoxFormat);
	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set vif 0 ") + formatCstring + _T("\r")));
	Sleep(50);

	//0-nonht;1-ht;4-11b;
	//if (formatCstring == _T("0-nonht"))
	//{
	//	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set vif 0 nonht") + formatCstring + _T("\r")));

	//	/*CString nonhtCstring = GetComboBoxCstring(&m_NonHt);
	//	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set vif 0 nonht") + nonhtCstring + _T("\r")));
	//	Sleep(50);*/
	//}
	//else if (formatCstring == _T("1-ht"))
	//{
	//	CString htCstring = GetComboBoxCstring(&m_Ht);
	//	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set vif 0 ht") + htCstring + _T("\r")));
	//	Sleep(50);
	//}
	//else if (formatCstring == _T("4-11b"))
	//{
	//	CString elevenbCstring = GetComboBoxCstring(&m_ElevenB);
	//	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set vif 0 11b") + elevenbCstring + _T("\r")));
	//	Sleep(50);
	//}
	CString configCstring = ptr->GetComboBoxCstring(&ptr->m_Config);
	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set config 0 0 ") + configCstring + _T("\r")));
	Sleep(50);
	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("write 0x900004 0x01200072\r")));
	Sleep(50);
	CString packetNumCsting;
	ptr->m_NUM_PKT.GetWindowTextW(packetNumCsting);
	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("test start ") + packetNumCsting + _T(" 1000 450 0 0\r")));
	Sleep(50);

	return 0;
}

UINT CRFloadDlg::TxTestPacketMode_FortyM(LPVOID pParam)
{
	CRFloadDlg *ptr = (CRFloadDlg*)pParam;
	CString channelCstring = ptr->GetComboBoxCstring(&ptr->m_ComboBoxRFCH);
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


	CString formatCstring = ptr->GetComboBoxCstring(&ptr->m_ComboBoxFormat);
	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set vif 0 ") + formatCstring + _T("\r")));
	Sleep(50);

	//0-nonht;1-ht;4-11b;
	/*if (formatCstring == _T("0-nonht"))
	{
		CString nonhtCstring = GetComboBoxCstring(&m_NonHt);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set vif 0 nonht") + nonhtCstring + _T("\r")));
		Sleep(50);
	}
	else if (formatCstring == _T("1-ht"))
	{
		CString htCstring = GetComboBoxCstring(&m_Ht);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set vif 0 ht") + htCstring + _T("\r")));
		Sleep(50);
	}
	else if (formatCstring == _T("4-11b"))
	{
		CString elevenbCstring = GetComboBoxCstring(&m_ElevenB);
		g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set vif 0 11b") + elevenbCstring + _T("\r")));
		Sleep(50);
	}*/

	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set vif 0 bandwidth=40m lower\r")));
	Sleep(50);
	CString configCstring = ptr->GetComboBoxCstring(&ptr->m_Config);
	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("set config 0 0 ")+configCstring+_T("\r")));
	Sleep(50);
	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("write 0x900004 0x01200072\r")));
	Sleep(50);
	CString packetNumCsting;
	ptr->m_NUM_PKT.GetWindowTextW(packetNumCsting);
	g_pMainDlg->m_MSComm.put_Output(COleVariant(_T("test start ")+ packetNumCsting+_T(" 1000 450 0 0\r")));
	Sleep(50);

	return 0;
}


void CRFloadDlg::OnBnClickedButtonTxtestStop()
{
	pUartThread = AfxBeginThread(TxtetsStopCmd, this, THREAD_PRIORITY_NORMAL, 0, 0);
}

UINT CRFloadDlg::TxtetsStopCmd(LPVOID pParam)
{
	CRFloadDlg *ptr = (CRFloadDlg*)pParam;
	CString modeCstring = ptr-> GetComboBoxCstring(&ptr->m_Mode);
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
	return 0;
}

CString CRFloadDlg::GetComboBoxCstring(CComboBox *comboBoxName)
{
	CString comboBoxStr;
	int index = comboBoxName->GetCurSel();
	if (index == 0xffffffff)
	{
		MessageBox(_T("下拉列表框未填入参数"), _T("Tips"), MB_OK | MB_ICONWARNING);
		return 0;
	}
	comboBoxName->GetLBText(index, comboBoxStr);
	//returnNum = _tcstoul(comboBoxStr, NULL, 10);
	return comboBoxStr;
}

void CRFloadDlg::PrintLog()
{
	CString logCstring;
	g_pMainDlg->rxdata[g_pMainDlg->uartLen] = '\0';
	logCstring = g_pMainDlg->rxdata;
	m_ListboxLog.AddString(/*_T("+ pre_err_cstr 是：") + */logCstring);
	m_ListboxLog.SetCurSel(m_ListboxLog.GetCount() - 1);
	memset(g_pMainDlg->rxdata, 0, g_pMainDlg->uartLen);
}
