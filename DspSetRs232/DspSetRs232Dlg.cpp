
// DspSetRs232Dlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "DspSetRs232.h"
#include "DspSetRs232Dlg.h"
#include "afxdialogex.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDspSetRs232Dlg* pGlobalDlg;

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDspSetRs232Dlg 대화 상자




CDspSetRs232Dlg::CDspSetRs232Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDspSetRs232Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	pGlobalDlg = this;
	m_sReceive = "";
	m_nSelTab = 0;
	m_nTab = 0;
	m_nPort = 1;

	m_nBufIdx = 0;
	m_nBufStVal = 0;
	m_nBufEdVal = 0;

	m_bTmr0 = FALSE;
	m_bInitSci = FALSE;
	m_bCommSt = FALSE;

	m_bConnected = FALSE;
	m_bEnabledCtrl = FALSE;

	for(int nMode=0; nMode<MaxMode; nMode++)
	{
		for(int nFunc=0; nFunc<5; nFunc++)
			m_nRomData[nMode][nFunc] = 0;
	}

	InitCommPc();
	ResetRecvBuf();
}

CDspSetRs232Dlg::~CDspSetRs232Dlg()
{
	if(m_bInitSci)
		m_Rs232.DestroyRs232();
	m_bTmr0 = FALSE;

}

void CDspSetRs232Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDspSetRs232Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_RECEIVEDATA, OnReceive)
	ON_BN_CLICKED(IDC_CHECK_CONNECT, &CDspSetRs232Dlg::OnBnClickedCheckConnect)
	ON_WM_TIMER()
//	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MODE, &CDspSetRs232Dlg::OnTcnSelchangeTabMode)
ON_NOTIFY(NM_CLICK, IDC_TAB_MODE, &CDspSetRs232Dlg::OnClickTabMode)
ON_BN_CLICKED(IDC_BUTTON_READ_MODE, &CDspSetRs232Dlg::OnBnClickedButtonReadMode)
ON_BN_CLICKED(IDC_BUTTON_WRITE_MODE, &CDspSetRs232Dlg::OnBnClickedButtonWriteMode)
ON_BN_CLICKED(IDC_BUTTON_RESET, &CDspSetRs232Dlg::OnBnClickedButtonReset)
ON_BN_CLICKED(IDC_BUTTON_READ_FUNC_0, &CDspSetRs232Dlg::OnBnClickedButtonReadFunc0)
ON_BN_CLICKED(IDC_BUTTON_WRITE_FUNC_0, &CDspSetRs232Dlg::OnBnClickedButtonWriteFunc0)
ON_BN_CLICKED(IDC_BUTTON_READ_FUNC_1, &CDspSetRs232Dlg::OnBnClickedButtonReadFunc1)
ON_BN_CLICKED(IDC_BUTTON_WRITE_FUNC_1, &CDspSetRs232Dlg::OnBnClickedButtonWriteFunc1)
ON_BN_CLICKED(IDC_BUTTON_READ_FUNC_2, &CDspSetRs232Dlg::OnBnClickedButtonReadFunc2)
ON_BN_CLICKED(IDC_BUTTON_WRITE_FUNC_2, &CDspSetRs232Dlg::OnBnClickedButtonWriteFunc2)
ON_BN_CLICKED(IDC_BUTTON_READ_FUNC_3, &CDspSetRs232Dlg::OnBnClickedButtonReadFunc3)
ON_BN_CLICKED(IDC_BUTTON_WRITE_FUNC_3, &CDspSetRs232Dlg::OnBnClickedButtonWriteFunc3)
ON_BN_CLICKED(IDC_BUTTON_READ_FUNC_4, &CDspSetRs232Dlg::OnBnClickedButtonReadFunc4)
ON_BN_CLICKED(IDC_BUTTON_WRITE_FUNC_4, &CDspSetRs232Dlg::OnBnClickedButtonWriteFunc4)
ON_BN_CLICKED(IDC_RADIO_AUTO, &CDspSetRs232Dlg::OnBnClickedRadioAuto)
ON_BN_CLICKED(IDC_RADIO_CLOSE, &CDspSetRs232Dlg::OnBnClickedRadioClose)
ON_BN_CLICKED(IDC_RADIO_OPEN, &CDspSetRs232Dlg::OnBnClickedRadioOpen)
ON_BN_CLICKED(IDC_BUTTON_READ_FUNC_5, &CDspSetRs232Dlg::OnBnClickedButtonReadFunc5)
ON_BN_CLICKED(IDC_BUTTON_WRITE_FUNC_5, &CDspSetRs232Dlg::OnBnClickedButtonWriteFunc5)
ON_BN_CLICKED(IDC_BUTTON_READ_FUNC_6, &CDspSetRs232Dlg::OnBnClickedButtonReadFunc6)
ON_BN_CLICKED(IDC_BUTTON_WRITE_FUNC_6, &CDspSetRs232Dlg::OnBnClickedButtonWriteFunc6)
ON_BN_CLICKED(IDC_BUTTON_READ_FUNC_7, &CDspSetRs232Dlg::OnBnClickedButtonReadFunc7)
ON_BN_CLICKED(IDC_BUTTON_WRITE_FUNC_7, &CDspSetRs232Dlg::OnBnClickedButtonWriteFunc7)
ON_BN_CLICKED(IDC_BUTTON_READ_FUNC_8, &CDspSetRs232Dlg::OnBnClickedButtonReadFunc8)
ON_BN_CLICKED(IDC_BUTTON_WRITE_FUNC_8, &CDspSetRs232Dlg::OnBnClickedButtonWriteFunc8)
ON_BN_CLICKED(IDC_CHECK_REPEAT_TEST, &CDspSetRs232Dlg::OnBnClickedCheckRepeatTest)
END_MESSAGE_MAP()


// CDspSetRs232Dlg 메시지 처리기

BOOL CDspSetRs232Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	//InitTab();
	GetDlgItem(IDC_TAB_MODE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_SEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_MODE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_READ_MODE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_WRITE_MODE)->ShowWindow(SW_HIDE);

	EnableCtrl(FALSE);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDspSetRs232Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CDspSetRs232Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CDspSetRs232Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CDspSetRs232Dlg::InitSci(int nPort)
{
	BOOL bRtn;

	int nBaudRate = 38400;
	//	int nBaudRate = 115200;
	// 	int nBaudRate = 230400;

	m_Rs232.SetHwnd(this->GetSafeHwnd());
	m_Rs232.m_nPort = (BYTE)nPort;
	m_Rs232.m_dwBaudRate = (DWORD)nBaudRate;
	m_Rs232.m_bCR = TRUE;
	bRtn = m_Rs232.OpenComPort();

	return bRtn;
}

LRESULT CDspSetRs232Dlg::OnReceive(WPARAM wp, LPARAM lp)
{
	char pBuf[MAX_CHAR];
	int nLen = (int)lp;
	if(nLen > MAXBLOCK)
	{
		AfxMessageBox("RS232 Buffer over MAXBLOCK");
		return 0;
	}

	int i, k;
	int nIdx=0, nVal=0;
	for(i=0; i<nLen; i++)
	{
		if(m_nBufIdx >= MAX_CHAR)
			m_nBufIdx = 0;
		m_cBuf[m_nBufIdx++] = m_Rs232.m_cRxBuffer[i];


		if(m_Rs232.m_cRxBuffer[i] == ':')
		{
			m_nBufStVal = m_nBufIdx-1;
			m_bCommSt = TRUE;
		}
		else if(m_Rs232.m_cRxBuffer[i] == ';')
		{
			if(m_bCommSt)
			{
				m_nBufEdVal = m_nBufIdx;
				int nDataLen = m_nBufEdVal - m_nBufStVal;
				if(nDataLen < 0)
					nDataLen += MAX_CHAR;

				for(k=0; k<nDataLen; k++)
				{
					int idx = m_nBufStVal + k;
					if(idx >= MAX_CHAR)
						idx -= MAX_CHAR;
					pBuf[k] = m_cBuf[idx];
				}
				pBuf[k] = '\0';

				ParsingComm(pBuf, nDataLen); //



				//nVal = atoi(pBuf);

				//if(m_nBufIdx >= BUF_NUM)
				//	m_nBufIdx = 0;

				//if(nVal >=0 && nVal <= 4095 && nDataLen <= 4)
				//	m_nBuf[m_nBufIdx++] = nVal;

				//  				if(atoi(pBuf)<100 || atoi(pBuf)>3500)
				//  					int nBreak = 0;
			}

			m_bCommSt = FALSE;
		}


	}

	return 0L;
}

int CDspSetRs232Dlg::GetData(char *pBuf, int &nIdx)
{
	char cSerialInput;
	int i, nReturnVal, nSign, nStIdx;
	nReturnVal= CMD_NONE;

	cSerialInput = (char)pBuf[nIdx++];
	m_nWatchDogComm++;

	if(cSerialInput >= '0' && cSerialInput <= '9')
	{
		m_pData[m_nDataIdx] = cSerialInput - '0';
		m_nDataIdx++;
	}
	else if(cSerialInput == '-' || cSerialInput == '+')
	{
		if(cSerialInput == '-')
		{
			m_pData[m_nDataIdx] = cSerialInput;
			m_nDataIdx++;
		}
	}
	else if(cSerialInput == ',' || cSerialInput == ';') //(g_cSerialInput == 59) // 59 is ';'
	{
		nReturnVal = 0;
		nSign = 1;
		nStIdx = 0;
		if(m_pData[0] == '-')
		{
			nSign = -1;
			nStIdx = 1;
		}
		for(i = nStIdx ; i < m_nDataIdx ; i++)
			nReturnVal += m_pData[i] *  pow(10.0,(m_nDataIdx-1-i));		
		nReturnVal *= nSign;

		// Data가 정상적으로 입력되었으면,...
		m_nData = (long)nReturnVal;
		return 0;
	}
	else
		return CMD_ERROR;

	return nReturnVal;
}

void CDspSetRs232Dlg::InitCommPc()
{
	int i;

	m_nStepParsingComm = 0;
	m_nDataIdx = 0;
	m_nData = (long)CMD_NONE;
	m_nWatchDogComm = 0;
	m_nMode = 0;
	m_nFunc = 0;
	m_nEncData = 0;

	for( i=0; i<MaxMode; i++)
	{
		m_pData[i] = 0;

		if(i<MaxFunc)
		{
			m_BufRead[i] = 0;
			m_BufWrite[i] = 0;
		}
	}
}

void CDspSetRs232Dlg::ExeCmd(int nCmd)
{
	CString sDisp="";
	BOOL pFunc[MaxFunc]={0};
	int i=0;

	switch(nCmd)
	{
	case IsConnected:
		m_bConnected = TRUE;
		break;
	case EncData:
		sDisp.Format("%d", m_nEncData);
		GetDlgItem(IDC_STATIC_ENC)->SetWindowText(sDisp);
		break;
	case ReadArrayData:
		// display m_nRomData[m_nMode][0] ~ [8]
		for(i=0; i<MaxFunc; i++)
			pFunc[i] = TRUE;
		DispModeInfo(m_nMode, pFunc);
		break;
	case ReadData:
		pFunc[m_nFunc] = TRUE;
		DispModeInfo(m_nMode, pFunc);
		break;
	case WriteArrayData:
		break;
	case WriteData:
		break;
	case Auto:
		((CButton*)GetDlgItem(IDC_RADIO_AUTO))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_CLOSE))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_OPEN))->SetCheck(FALSE);
		break;
	case Close:
		((CButton*)GetDlgItem(IDC_RADIO_AUTO))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_CLOSE))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_OPEN))->SetCheck(FALSE);
		break;
	case Open:
		((CButton*)GetDlgItem(IDC_RADIO_AUTO))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_CLOSE))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_OPEN))->SetCheck(TRUE);
		break;
	}

}

void CDspSetRs232Dlg::ParsingComm(char *pBuf, int nDataLen)
{
	int nGetData, nParse=0;
	char cSerialInput;

	m_nStepParsingComm = 0;

	while(nDataLen >= nParse)
	{
		switch(m_nStepParsingComm)
		{
		case 0:	
			InitCommPc();
			m_nStepParsingComm++;
			break;
		case 1:	
			cSerialInput = (char)pBuf[nParse++];	
			if(cSerialInput == ':') // Wait for ':'
			{
				m_nWatchDogComm = 0;
				m_nStepParsingComm++;
			}
			break;
		case 2:
			cSerialInput = (char)pBuf[nParse++];
			m_nWatchDogComm++;
			if(cSerialInput == 'Y' || cSerialInput == 'y')
			{
				m_nWatchDogComm = 0;
				m_nStepParsingComm = 30; // Jump to case 30:		// in case 'Y'
			}
			else if(cSerialInput == 'E' || cSerialInput == 'e')
			{
				m_nWatchDogComm = 0;
				m_nStepParsingComm = 100; // Jump to case 100:
			}
			else if(cSerialInput == 'R' || cSerialInput == 'r')
			{
				m_nWatchDogComm = 0;
				m_nStepParsingComm = 40; // Jump to case 40:
			}
			else if(cSerialInput == 'A' || cSerialInput == 'a')
			{
				m_nWatchDogComm = 0;
				m_nStepParsingComm = 110; // Jump to case 110:
			}
			else if(cSerialInput == 'C' || cSerialInput == 'c')
			{
				m_nWatchDogComm = 0;
				m_nStepParsingComm = 120; // Jump to case 120:
			}
			else if(cSerialInput == 'O' || cSerialInput == 'o')
			{
				m_nWatchDogComm = 0;
				m_nStepParsingComm = 130; // Jump to case 130:
			}
			//else if(cSerialInput == 'W' || cSerialInput == 'w')
			//{
			//	m_nWatchDogComm = 0;
			//	m_nStepParsingComm++;
			//}
			else
			{
				m_nStepParsingComm=0;
				return;
			}

			if(m_nWatchDogComm > WatchDogComm)
			{
				m_nStepParsingComm=0;
				return;
			}
			break;

		//case 3:	
		//	cSerialInput = (char)pBuf[nParse++];
		//	m_nWatchDogComm++;	
		//	if(cSerialInput == 'A' || cSerialInput == 'a')
		//	{
		//		m_nWatchDogComm = 0;
		//		m_nStepParsingComm = 10;								// in case 'WA'
		//	}
		//	else if(cSerialInput == ',')
		//	{
		//		m_nWatchDogComm = 0;
		//		m_nStepParsingComm = 20; // Jump to case 20:		// in case 'W'
		//	}
		//	else
		//	{
		//		m_nStepParsingComm=0;
		//		return;
		//	}

		//	if(m_nWatchDogComm > WatchDogComm)
		//	{
		//		m_nStepParsingComm=0;
		//		return;
		//	}
		//	break;

		//case 10:								// in case 'WA'
		//	cSerialInput = (char)pBuf[nParse++];
		//	m_nWatchDogComm++;
		//	if(cSerialInput == ',')
		//	{
		//		m_nWatchDogComm = 0;
		//		m_nDataIdx = 0;
		//		m_nData = (long)CMD_NONE;
		//		m_nStepParsingComm++;
		//	}
		//	else
		//	{
		//		m_nStepParsingComm=0;
		//		return;
		//	}

		//	if(m_nWatchDogComm > WatchDogComm)
		//	{
		//		m_nStepParsingComm=0;
		//		return;
		//	}
		//	break;

		//case 11:
		//	nGetData = GetData(pBuf, nParse);
		//	if(0 == nGetData)
		//	{
		//		m_nMode = m_nData;

		//		m_nWatchDogComm = 0;
		//		m_nDataIdx = 0;
		//		m_nData = (long)CMD_NONE;
		//		m_nStepParsingComm++;
		//	}
		//	else if(CMD_ERROR == nGetData)
		//	{
		//		m_nStepParsingComm=0;
		//		return;
		//	}

		//	if(m_nWatchDogComm > WatchDogComm)
		//	{
		//		m_nStepParsingComm=0;
		//		return;
		//	}
		//	break;

		//case 12:
		//	nGetData = GetData(pBuf, nParse);
		//	if(0 == nGetData)
		//	{
		//		m_BufWrite[0] = m_nData;

		//		m_nWatchDogComm = 0;
		//		m_nDataIdx = 0;
		//		m_nData = (long)CMD_NONE;
		//		m_nStepParsingComm++;
		//	}
		//	else if(CMD_ERROR == nGetData)
		//	{
		//		m_nStepParsingComm=0;
		//		return;
		//	}

		//	if(m_nWatchDogComm > WatchDogComm)
		//	{
		//		m_nStepParsingComm=0;
		//		return;
		//	}
		//	break;

		//case 13:
		//	nGetData = GetData(pBuf, nParse);
		//	if(0 == nGetData)
		//	{
		//		m_BufWrite[1] = m_nData;

		//		m_nWatchDogComm = 0;
		//		m_nDataIdx = 0;
		//		m_nData = (long)CMD_NONE;
		//		m_nStepParsingComm++;
		//	}
		//	else if(CMD_ERROR == nGetData)
		//	{
		//		m_nStepParsingComm=0;
		//		return;
		//	}

		//	if(m_nWatchDogComm > WatchDogComm)
		//	{
		//		m_nStepParsingComm=0;
		//		return;
		//	}
		//	break;

		//case 14:
		//	nGetData = GetData(pBuf, nParse);
		//	if(0 == nGetData)
		//	{
		//		m_BufWrite[2] = m_nData;

		//		m_nWatchDogComm = 0;
		//		m_nDataIdx = 0;
		//		m_nData = (long)CMD_NONE;
		//		m_nStepParsingComm++;
		//	}
		//	else if(CMD_ERROR == nGetData)
		//	{
		//		m_nStepParsingComm=0;
		//		return;
		//	}

		//	if(m_nWatchDogComm > WatchDogComm)
		//	{
		//		m_nStepParsingComm=0;
		//		return;
		//	}
		//	break;

		//case 15:
		//	nGetData = GetData(pBuf, nParse);
		//	if(0 == nGetData)
		//	{
		//		m_BufWrite[3] = m_nData;

		//		m_nWatchDogComm = 0;
		//		m_nDataIdx = 0;
		//		m_nData = (long)CMD_NONE;
		//		m_nStepParsingComm++;
		//	}
		//	else if(CMD_ERROR == nGetData)
		//	{
		//		m_nStepParsingComm=0;
		//		return;
		//	}

		//	if(m_nWatchDogComm > WatchDogComm)
		//	{
		//		m_nStepParsingComm=0;
		//		return;
		//	}
		//	break;

		//case 16:
		//	nGetData = GetData(pBuf, nParse);
		//	if(0 == nGetData)
		//	{
		//		m_BufWrite[4] = m_nData;

		//		m_nWatchDogComm = 0;
		//		m_nDataIdx = 0;
		//		m_nData = (long)CMD_NONE;
		//		m_nStepParsingComm++;
		//	}
		//	else if(CMD_ERROR == nGetData)
		//	{
		//		m_nStepParsingComm=0;
		//		return;
		//	}

		//	if(m_nWatchDogComm > WatchDogComm)
		//	{
		//		m_nStepParsingComm=0;
		//		return;
		//	}
		//	break;

		//case 17:
		//	ExeCmd(WriteArrayData);
		//	m_nStepParsingComm=0;
		//	return;

		//case 20:								// in case 'W'
		//	nGetData = GetData(pBuf, nParse);
		//	if(0 == nGetData)
		//	{
		//		m_nMode = m_nData;

		//		m_nWatchDogComm = 0;
		//		m_nDataIdx = 0;
		//		m_nData = (long)CMD_NONE;
		//		m_nStepParsingComm++;
		//	}
		//	else if(CMD_ERROR == nGetData)
		//	{
		//		m_nStepParsingComm=0;
		//		return;
		//	}

		//	if(m_nWatchDogComm > WatchDogComm)
		//	{
		//		m_nStepParsingComm=0;
		//		return;
		//	}
		//	break;

		//case 21:							
		//	nGetData = GetData(pBuf, nParse);
		//	if(0 == nGetData)
		//	{
		//		m_nFunc = m_nData;

		//		m_nWatchDogComm = 0;
		//		m_nDataIdx = 0;
		//		m_nData = (long)CMD_NONE;
		//		m_nStepParsingComm++;
		//	}
		//	else if(CMD_ERROR == nGetData)
		//	{
		//		m_nStepParsingComm=0;
		//		return;
		//	}

		//	if(m_nWatchDogComm > WatchDogComm)
		//	{
		//		m_nStepParsingComm=0;
		//		return;
		//	}
		//	break;

		//case 22:							
		//	nGetData = GetData(pBuf, nParse);
		//	if(0 == nGetData)
		//	{
		//		m_BufWrite[0] = m_nData;

		//		m_nWatchDogComm = 0;
		//		m_nDataIdx = 0;
		//		m_nData = (long)CMD_NONE;
		//		m_nStepParsingComm++;
		//	}
		//	else if(CMD_ERROR == nGetData)
		//	{
		//		m_nStepParsingComm=0;
		//		return;
		//	}

		//	if(m_nWatchDogComm > WatchDogComm)
		//	{
		//		m_nStepParsingComm=0;
		//		return;
		//	}
		//	break;

		//case 23:
		//	ExeCmd(WriteData);
		//	m_nStepParsingComm=0;
		//	return;

		case 30:								// in case 'Y'
			cSerialInput = (char)pBuf[nParse++];
			m_nWatchDogComm++;
			if(cSerialInput == ';')
			{
				m_nWatchDogComm = 0;
				m_nStepParsingComm++;
			}

			if(m_nWatchDogComm > WatchDogComm)
			{
				m_nStepParsingComm=0;
				return;
			}
			break;

		case 31:
			ExeCmd(IsConnected);
			m_nStepParsingComm=0;
			return;

		case 40:	
			cSerialInput = (char)pBuf[nParse++];
			m_nWatchDogComm++;	
			if(cSerialInput == 'A' || cSerialInput == 'a')
			{
				m_nWatchDogComm = 0;
				m_nStepParsingComm = 50;								// in case 'RA'
			}
			else if(cSerialInput == ',')
			{
				m_nWatchDogComm = 0;
				m_nStepParsingComm = 70; // Jump to case 20:		// in case 'R'
			}
			else
			{
				m_nStepParsingComm=0;
				return;
			}

			if(m_nWatchDogComm > WatchDogComm)
			{
				m_nStepParsingComm=0;
				return;
			}
			break;

		case 50:								// in case 'RA'
			cSerialInput = (char)pBuf[nParse++];
			m_nWatchDogComm++;
			if(cSerialInput == ',')
			{
				m_nWatchDogComm = 0;
				m_nDataIdx = 0;
				m_nData = (long)CMD_NONE;
				m_nStepParsingComm++;
			}
			else
			{
				m_nStepParsingComm=0;
				return;
			}

			if(m_nWatchDogComm > WatchDogComm)
			{
				m_nStepParsingComm=0;
				return;
			}
			break;

		case 51:										// in case 'RA'
			nGetData = GetData(pBuf, nParse);
			if(0 == nGetData)
			{
//				m_nMode = m_nData;

				m_nWatchDogComm = 0;
				m_nDataIdx = 0;
				m_nData = (long)CMD_NONE;
				m_nStepParsingComm++;
			}
			else if(CMD_ERROR == nGetData)
			{
				m_nStepParsingComm=0;
				return;
			}

			if(m_nWatchDogComm > WatchDogComm)
			{
				m_nStepParsingComm=0;
				return;
			}
			break;

		case 52:							
			nGetData = GetData(pBuf, nParse);
			if(0 == nGetData)
			{
				m_nRomData[m_nMode][0] = m_nData;

				m_nWatchDogComm = 0;
				m_nDataIdx = 0;
				m_nData = (long)CMD_NONE;
				m_nStepParsingComm++;
			}
			else if(CMD_ERROR == nGetData)
			{
				m_nStepParsingComm=0;
				return;
			}

			if(m_nWatchDogComm > WatchDogComm)
			{
				m_nStepParsingComm=0;
				return;
			}
			break;

		case 53:							
			nGetData = GetData(pBuf, nParse);
			if(0 == nGetData)
			{
				m_nRomData[m_nMode][1] = m_nData;

				m_nWatchDogComm = 0;
				m_nDataIdx = 0;
				m_nData = (long)CMD_NONE;
				m_nStepParsingComm++;
			}
			else if(CMD_ERROR == nGetData)
			{
				m_nStepParsingComm=0;
				return;
			}

			if(m_nWatchDogComm > WatchDogComm)
			{
				m_nStepParsingComm=0;
				return;
			}
			break;

		case 54:							
			nGetData = GetData(pBuf, nParse);
			if(0 == nGetData)
			{
				m_nRomData[m_nMode][2] = m_nData;

				m_nWatchDogComm = 0;
				m_nDataIdx = 0;
				m_nData = (long)CMD_NONE;
				m_nStepParsingComm++;
			}
			else if(CMD_ERROR == nGetData)
			{
				m_nStepParsingComm=0;
				return;
			}

			if(m_nWatchDogComm > WatchDogComm)
			{
				m_nStepParsingComm=0;
				return;
			}
			break;

		case 55:							
			nGetData = GetData(pBuf, nParse);
			if(0 == nGetData)
			{
				m_nRomData[m_nMode][3] = m_nData;

				m_nWatchDogComm = 0;
				m_nDataIdx = 0;
				m_nData = (long)CMD_NONE;
				m_nStepParsingComm++;
			}
			else if(CMD_ERROR == nGetData)
			{
				m_nStepParsingComm=0;
				return;
			}

			if(m_nWatchDogComm > WatchDogComm)
			{
				m_nStepParsingComm=0;
				return;
			}
			break;

		case 56:							
			nGetData = GetData(pBuf, nParse);
			if(0 == nGetData)
			{
				m_nRomData[m_nMode][4] = m_nData;

				m_nWatchDogComm = 0;
				m_nDataIdx = 0;
				m_nData = (long)CMD_NONE;
				m_nStepParsingComm++;
			}
			else if(CMD_ERROR == nGetData)
			{
				m_nStepParsingComm=0;
				return;
			}

			if(m_nWatchDogComm > WatchDogComm)
			{
				m_nStepParsingComm=0;
				return;
			}
			break;

		case 57:
			nGetData = GetData(pBuf, nParse);
			if (0 == nGetData)
			{
				m_nRomData[m_nMode][5] = m_nData;

				m_nWatchDogComm = 0;
				m_nDataIdx = 0;
				m_nData = (long)CMD_NONE;
				m_nStepParsingComm++;
			}
			else if (CMD_ERROR == nGetData)
			{
				m_nStepParsingComm = 0;
				return;
			}

			if (m_nWatchDogComm > WatchDogComm)
			{
				m_nStepParsingComm = 0;
				return;
			}
			break;

		case 58:
			nGetData = GetData(pBuf, nParse);
			if (0 == nGetData)
			{
				m_nRomData[m_nMode][6] = m_nData;

				m_nWatchDogComm = 0;
				m_nDataIdx = 0;
				m_nData = (long)CMD_NONE;
				m_nStepParsingComm++;
			}
			else if (CMD_ERROR == nGetData)
			{
				m_nStepParsingComm = 0;
				return;
			}

			if (m_nWatchDogComm > WatchDogComm)
			{
				m_nStepParsingComm = 0;
				return;
			}
			break;

		case 59:
			nGetData = GetData(pBuf, nParse);
			if (0 == nGetData)
			{
				m_nRomData[m_nMode][7] = m_nData;

				m_nWatchDogComm = 0;
				m_nDataIdx = 0;
				m_nData = (long)CMD_NONE;
				m_nStepParsingComm++;
			}
			else if (CMD_ERROR == nGetData)
			{
				m_nStepParsingComm = 0;
				return;
			}

			if (m_nWatchDogComm > WatchDogComm)
			{
				m_nStepParsingComm = 0;
				return;
			}
			break;

		case 60:
			nGetData = GetData(pBuf, nParse);
			if (0 == nGetData)
			{
				m_nRomData[m_nMode][8] = m_nData;

				m_nWatchDogComm = 0;
				m_nDataIdx = 0;
				m_nData = (long)CMD_NONE;
				m_nStepParsingComm++;
			}
			else if (CMD_ERROR == nGetData)
			{
				m_nStepParsingComm = 0;
				return;
			}

			if (m_nWatchDogComm > WatchDogComm)
			{
				m_nStepParsingComm = 0;
				return;
			}
			break;

		case 61:
			ExeCmd(ReadArrayData);
			m_nStepParsingComm=0;
			return;

		case 70:										// in case 'R'
			nGetData = GetData(pBuf, nParse);
			if(0 == nGetData)
			{
//				m_nMode = m_nData;

				m_nWatchDogComm = 0;
				m_nDataIdx = 0;
				m_nData = (long)CMD_NONE;
				m_nStepParsingComm++;
			}
			else if(CMD_ERROR == nGetData)
			{
				m_nStepParsingComm=0;
				return;
			}

			if(m_nWatchDogComm > WatchDogComm)
			{
				m_nStepParsingComm=0;
				return;
			}
			break;

		case 71:							
			nGetData = GetData(pBuf, nParse);
			if(0 == nGetData)
			{
				m_nFunc = m_nData;

				m_nWatchDogComm = 0;
				m_nDataIdx = 0;
				m_nData = (long)CMD_NONE;
				m_nStepParsingComm++;
			}
			else if(CMD_ERROR == nGetData)
			{
				m_nStepParsingComm=0;
				return;
			}

			if(m_nWatchDogComm > WatchDogComm)
			{
				m_nStepParsingComm=0;
				return;
			}
			break;

		case 72:
			nGetData = GetData(pBuf, nParse);
			if(0 == nGetData)
			{
				m_nRomData[m_nMode][m_nFunc] = m_nData;

				m_nWatchDogComm = 0;
				m_nDataIdx = 0;
				m_nData = (long)CMD_NONE;
				m_nStepParsingComm++;
			}
			else if(CMD_ERROR == nGetData)
			{
				m_nStepParsingComm=0;
				return;
			}

			if(m_nWatchDogComm > WatchDogComm)
			{
				m_nStepParsingComm=0;
				return;
			}
			break;

		case 73:
			ExeCmd(ReadData);
			m_nStepParsingComm=0;
			return;

		case 100:								// in case 'E'
			cSerialInput = (char)pBuf[nParse++];
			m_nWatchDogComm++;
			if(cSerialInput == ',')
			{
				m_nWatchDogComm = 0;
				m_nDataIdx = 0;
				m_nData = (long)CMD_NONE;
				m_nStepParsingComm++;
			}
			else
			{
				m_nStepParsingComm=0;
				return;
			}

			if(m_nWatchDogComm > WatchDogComm)
			{
				m_nStepParsingComm=0;
				return;
			}
			break;

		case 101:							
			nGetData = GetData(pBuf, nParse);
			if(0 == nGetData)
			{
				m_nEncData = m_nData;

				m_nWatchDogComm = 0;
				m_nDataIdx = 0;
				m_nData = (long)CMD_NONE;
				m_nStepParsingComm++;
			}
			else if(CMD_ERROR == nGetData)
			{
				m_nStepParsingComm=0;
				return;
			}

			if(m_nWatchDogComm > WatchDogComm)
			{
				m_nStepParsingComm=0;
				return;
			}
			break;

		case 102:
			ExeCmd(EncData);
			m_nStepParsingComm=0;
			return;

		case 110: 
			cSerialInput = (char)pBuf[nParse++];
			m_nWatchDogComm++;	
			if(cSerialInput == 'T' || cSerialInput == 't')
			{
				m_nWatchDogComm = 0;
				m_nStepParsingComm++;								// in case 'AT'
			}
			else
			{
				m_nStepParsingComm=0;
				return;
			}

			if(m_nWatchDogComm > WatchDogComm)
			{
				m_nStepParsingComm=0;
				return;
			}
			break;

		case 111:								
			cSerialInput = (char)pBuf[nParse++];
			m_nWatchDogComm++;
			if(cSerialInput == ';')
			{
				m_nWatchDogComm = 0;
				m_nDataIdx = 0;
				m_nData = (long)CMD_NONE;
				m_nStepParsingComm++;
			}
			else
			{
				m_nStepParsingComm=0;
				return;
			}

			if(m_nWatchDogComm > WatchDogComm)
			{
				m_nStepParsingComm=0;
				return;
			}
			break;

		case 112:
			ExeCmd(Auto);
			m_nStepParsingComm=0;
			return;

		case 120: 
			cSerialInput = (char)pBuf[nParse++];
			m_nWatchDogComm++;	
			if(cSerialInput == 'L' || cSerialInput == 'l')
			{
				m_nWatchDogComm = 0;
				m_nStepParsingComm++;								// in case 'CL'
			}
			else
			{
				m_nStepParsingComm=0;
				return;
			}

			if(m_nWatchDogComm > WatchDogComm)
			{
				m_nStepParsingComm=0;
				return;
			}
			break;

		case 121:								
			cSerialInput = (char)pBuf[nParse++];
			m_nWatchDogComm++;
			if(cSerialInput == ';')
			{
				m_nWatchDogComm = 0;
				m_nDataIdx = 0;
				m_nData = (long)CMD_NONE;
				m_nStepParsingComm++;
			}
			else
			{
				m_nStepParsingComm=0;
				return;
			}

			if(m_nWatchDogComm > WatchDogComm)
			{
				m_nStepParsingComm=0;
				return;
			}
			break;

		case 122:
			ExeCmd(Close);
			m_nStepParsingComm=0;
			return;

		case 130: 
			cSerialInput = (char)pBuf[nParse++];
			m_nWatchDogComm++;	
			if(cSerialInput == 'P' || cSerialInput == 'p')
			{
				m_nWatchDogComm = 0;
				m_nStepParsingComm++;								// in case 'OP'
			}
			else
			{
				m_nStepParsingComm=0;
				return;
			}

			if(m_nWatchDogComm > WatchDogComm)
			{
				m_nStepParsingComm=0;
				return;
			}
			break;

		case 131:								
			cSerialInput = (char)pBuf[nParse++];
			m_nWatchDogComm++;
			if(cSerialInput == ';')
			{
				m_nWatchDogComm = 0;
				m_nDataIdx = 0;
				m_nData = (long)CMD_NONE;
				m_nStepParsingComm++;
			}
			else
			{
				m_nStepParsingComm=0;
				return;
			}

			if(m_nWatchDogComm > WatchDogComm)
			{
				m_nStepParsingComm=0;
				return;
			}
			break;

		case 132:
			ExeCmd(Open);
			m_nStepParsingComm=0;
			return;

		}
	}
}

void CDspSetRs232Dlg::InitTab()
{
	m_Tab.SubclassDlgItem(IDC_TAB_MODE, this);
	m_Tab.SetHwnd(this->GetSafeHwnd(), IDC_TAB_MODE);
	m_Tab.AddText("Mode 1");
	m_Tab.AddText("Mode 2");
	m_Tab.AddText("Mode 3");
	m_Tab.AddText("Mode 4");
	m_Tab.AddText("Mode 5");
	m_Tab.AddText("Mode 6");
	m_Tab.AddText("Mode 7");
	m_Tab.AddText("Mode 8");
	m_Tab.AddText("Mode 9");
	m_Tab.SetBorder(FALSE);
	m_Tab.SetFontBold(FALSE);
	m_Tab.SetFontName("굴림");
	m_Tab.SetFontSize(16, 7);
	m_Tab.SetCurSel(m_nSelTab);
	m_Tab.HighlightItem(m_nSelTab,TRUE);
}

void CDspSetRs232Dlg::EnableCtrl(BOOL bEnable)
{
	m_bEnabledCtrl = bEnable;

	GetDlgItem(IDC_EDIT_COM_PORT)->EnableWindow(!bEnable);

	GetDlgItem(IDC_EDIT_MODE)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_FUNC_0)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_FUNC_1)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_FUNC_2)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_FUNC_3)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_FUNC_4)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_FUNC_5)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_FUNC_6)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_FUNC_7)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_FUNC_8)->EnableWindow(bEnable);

	GetDlgItem(IDC_BUTTON_READ_MODE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_WRITE_MODE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_READ_FUNC_0)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_WRITE_FUNC_0)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_READ_FUNC_1)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_WRITE_FUNC_1)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_READ_FUNC_2)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_WRITE_FUNC_2)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_READ_FUNC_3)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_WRITE_FUNC_3)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_READ_FUNC_4)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_WRITE_FUNC_4)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_READ_FUNC_5)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_WRITE_FUNC_5)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_READ_FUNC_6)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_WRITE_FUNC_6)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_READ_FUNC_7)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_WRITE_FUNC_7)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_READ_FUNC_8)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_WRITE_FUNC_8)->EnableWindow(bEnable);
}

void CDspSetRs232Dlg::OnBnClickedCheckConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( ( (CButton*)GetDlgItem(IDC_CHECK_CONNECT) )->GetCheck() )
	{
		CString sVal;
		GetDlgItem(IDC_EDIT_COM_PORT)->GetWindowText(sVal);

		if(sVal.IsEmpty())
		{
			AfxMessageBox("Not exist Port number!!!");
			( (CButton*)GetDlgItem(IDC_CHECK_CONNECT) )->SetCheck(FALSE);
			return;
		}
		m_nPort = atoi(sVal);
		m_bInitSci = InitSci(m_nPort);

		if(!m_bInitSci)
			AfxMessageBox("Failed to Initialize Serial Com.");
		else
		{
			m_bTmr0 = TRUE;
			SetTimer(0, 10, NULL);
		}
	}
	else
	{
		if(m_bInitSci)
		{
			m_bTmr0 = FALSE;
			EnableCtrl(FALSE);
			m_Rs232.CloseConnection();
		}
	}
}


void CDspSetRs232Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString sSend;
	char pSend[MAX_PATH]={0};
	BOOL pFunc[MaxFunc]={0};
	int i=0;

	if(nIDEvent == 0)
	{
		KillTimer(0);

		//ResetRecvBuf();
		sSend.Format(":C;");
		strcpy(pSend, sSend);
		m_Rs232.WriteRs232Block(pSend, sSend.GetLength());

		if(m_bTmr0)
			SetTimer(1, 50, NULL);
	}

	if(nIDEvent == 1)
	{
		KillTimer(1);

		if(m_bConnected && !m_bEnabledCtrl)
		{
			ReloadEep();
			if(m_bTmr0)
				SetTimer(2, 100, NULL);
		}
		//else if(!m_bConnected && m_bEnabledCtrl)
		//{
		//	m_bTmr0 = FALSE;
		//	EnableCtrl(FALSE);
		//}
		else
		{
			if(m_bTmr0)
				SetTimer(1, 50, NULL);
		}
	}

	if(nIDEvent == 2)
	{
		KillTimer(2);
		EnableCtrl(TRUE);
		if(m_bTmr0)
			SetTimer(3, 50, NULL);
	}

	if(nIDEvent == 3)
	{
		KillTimer(3);
		sSend.Format(":Q;");
		strcpy(pSend, sSend);
		m_Rs232.WriteRs232Block(pSend, sSend.GetLength());
		if(m_bTmr0)
			SetTimer(4, 50, NULL);
	}

	if(nIDEvent == 4)
	{
		KillTimer(4);
//		m_nMode = m_nTab + 1;
		for(i=0; i<MaxFunc; i++)
			pFunc[i] = TRUE;
		DispModeInfo(m_nMode, pFunc);
		if(m_bTmr0)
			SetTimer(5, 50, NULL);
	}

	if(nIDEvent == 5)
	{
		KillTimer(5);

		if(!m_bConnected && m_bEnabledCtrl)
		{
			m_bTmr0 = FALSE;
			EnableCtrl(FALSE);
		}

		if(m_bTmr0)
			SetTimer(5, 50, NULL);
	}

	if(nIDEvent == 100)
	{
		KillTimer(100);
		sSend.Format(":Q;");
		strcpy(pSend, sSend);
		m_Rs232.WriteRs232Block(pSend, sSend.GetLength());
	}

	if(nIDEvent == 200)
	{
		KillTimer(200);
		sSend.Format(":ra,0;");
		strcpy(pSend, sSend);
		m_Rs232.WriteRs232Block(pSend, sSend.GetLength());

		//if(m_nCntLoad < MaxMode)
		//{
		//	sSend.Format(":ra,%d;", m_nCntLoad++);
		//	strcpy(pSend, sSend);
		//	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());
		//
		//	SetTimer(200, 300, NULL);
		//}
		//else
		//{
		//	m_nMode = m_nTab + 1;
		//	for(i=0; i<5; i++)
		//		pFunc[i] = TRUE;
		//	DispModeInfo(m_nMode, pFunc);
		//}
	}

	CDialogEx::OnTimer(nIDEvent);
}


//void CDspSetRs232Dlg::OnTcnSelchangeTabMode(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	*pResult = 0;
//}


void CDspSetRs232Dlg::OnClickTabMode(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
/*	BOOL pFunc[5]={0};
	int i=0;
	int nTab = m_Tab.GetCurSel();

	if(nTab != m_nTab)
	{
		m_Tab.HighlightItem(m_nTab,FALSE);

		m_nTab = nTab;
		m_Tab.HighlightItem(m_nTab,TRUE);

		m_nMode = nTab + 1;
		for(i=0; i<5; i++)
			pFunc[i] = TRUE;
		DispModeInfo(m_nMode, pFunc);
	}

	*pResult = 0;*/
}

void CDspSetRs232Dlg::DispModeInfo(int nMode, BOOL *pFunc)
{
	CString sVal;

	sVal.Format("%d", m_nRomData[0][0]);
	GetDlgItem(IDC_EDIT_MODE)->SetWindowText(sVal);

	if(pFunc[0])
	{
		sVal.Format("%d", m_nRomData[nMode][0]);
		GetDlgItem(IDC_EDIT_FUNC_0)->SetWindowText(sVal);
	}
	if(pFunc[1])
	{
		sVal.Format("%d", m_nRomData[nMode][1]);
		GetDlgItem(IDC_EDIT_FUNC_1)->SetWindowText(sVal);
	}
	if(pFunc[2])
	{
		sVal.Format("%d", m_nRomData[nMode][2]);
		GetDlgItem(IDC_EDIT_FUNC_2)->SetWindowText(sVal);
	}
	if(pFunc[3])
	{
		sVal.Format("%d", m_nRomData[nMode][3]);
		GetDlgItem(IDC_EDIT_FUNC_3)->SetWindowText(sVal);
	}
	if(pFunc[4])
	{
		sVal.Format("%d", m_nRomData[nMode][4]);
		GetDlgItem(IDC_EDIT_FUNC_4)->SetWindowText(sVal);
	}
	if (pFunc[5])
	{
		sVal.Format("%d", m_nRomData[nMode][5]);
		GetDlgItem(IDC_EDIT_FUNC_5)->SetWindowText(sVal);
	}
	if (pFunc[6])
	{
		sVal.Format("%d", m_nRomData[nMode][6]);
		GetDlgItem(IDC_EDIT_FUNC_6)->SetWindowText(sVal);
	}
	if (pFunc[7])
	{
		sVal.Format("%d", m_nRomData[nMode][7]);
		GetDlgItem(IDC_EDIT_FUNC_7)->SetWindowText(sVal);
	}
	if (pFunc[8])
	{
		sVal.Format("%d", m_nRomData[nMode][8]);
		GetDlgItem(IDC_EDIT_FUNC_8)->SetWindowText(sVal);
	}
}

void CDspSetRs232Dlg::ReloadEep()
{
	m_nCntLoad = 0;
	SetTimer(200, 300, NULL);
	//CString sSend;
	//char pSend[MAX_PATH]={0};

	//for(int nMode=0; nMode<MaxMode; nMode++)
	//{
	//	sSend.Format(":ra,%d;", nMode);
	//	strcpy(pSend, sSend);
	//	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());
	//	Sleep(100);
	//}
}

void CDspSetRs232Dlg::ResetRecvBuf()
{
	int i;
	m_nBufIdx = 0;
	for(i=0; i<MAX_CHAR; i++)
		m_cBuf[i] = 0;
}

void CDspSetRs232Dlg::OnBnClickedButtonReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sSend;
	char pSend[MAX_PATH]={0};
	sSend.Format(":rst;");
	strcpy(pSend, sSend);
	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());
}


void CDspSetRs232Dlg::OnBnClickedRadioAuto()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sSend;
	char pSend[MAX_PATH]={0};
	sSend.Format(":at;");
	strcpy(pSend, sSend);
	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());
	SetTimer(100, 500, NULL);
}

void CDspSetRs232Dlg::OnBnClickedRadioClose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sSend;
	char pSend[MAX_PATH]={0};
	sSend.Format(":cl;");
	strcpy(pSend, sSend);
	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());
	SetTimer(100, 500, NULL);
}


void CDspSetRs232Dlg::OnBnClickedRadioOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sSend;
	char pSend[MAX_PATH]={0};
	sSend.Format(":op;");
	strcpy(pSend, sSend);
	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());
	SetTimer(100, 500, NULL);
}


void CDspSetRs232Dlg::OnBnClickedButtonReadMode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sSend;
	char pSend[MAX_PATH]={0};
	int nMode = 0;
	int nFunc = 0;
	sSend.Format(":r,%d,%d;", nMode, nFunc);
	strcpy(pSend, sSend);
	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());
}

void CDspSetRs232Dlg::OnBnClickedButtonWriteMode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CString sSend;
	//char pSend[MAX_PATH]={0};
	//char pSendRtn[MAX_PATH]={0};
	//int nMode = 0;
	//int nFunc = 0;
	//int nVal = 0;
	//CString sVal="";

	//GetDlgItem(IDC_EDIT_MODE)->GetWindowText(sVal);
	//nVal = atoi(sVal);

	//sSend.Format(":w,%d,%d,%d;", nMode, nFunc, nVal);
	//strcpy(pSend, sSend);
	//m_Rs232.WriteRs232Block(pSend, sSend.GetLength());

	//Sleep(50);

	//m_nMode = nMode;
	//m_nFunc = nFunc;
	//sSend.Format(":r,%d,%d;", nMode, nFunc);
	//strcpy(pSendRtn, sSend);
	//m_Rs232.WriteRs232Block(pSendRtn, sSend.GetLength());
}


void CDspSetRs232Dlg::OnBnClickedButtonReadFunc0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sSend;
	char pSend[MAX_PATH]={0};
	//int nMode = m_nTab+1;
	int nMode = 0;
	int nFunc = 0;
	m_nMode = nMode;
	m_nFunc = nFunc;
	sSend.Format(":r,%d,%d;", nMode, nFunc);
	strcpy(pSend, sSend);
	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());
}

void CDspSetRs232Dlg::OnBnClickedButtonWriteFunc0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sSend;
	char pSend[MAX_PATH]={0};
	char pSendRtn[MAX_PATH]={0};
	//int nMode = m_nTab+1;
	int nMode = 0;
	int nFunc = 0;
	int nVal = 0;
	CString sVal="";

	GetDlgItem(IDC_EDIT_FUNC_0)->GetWindowText(sVal);
	nVal = atoi(sVal);

	sSend.Format(":w,%d,%d,%d;", nMode, nFunc, nVal);
	strcpy(pSend, sSend);
	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());

	Sleep(50);

	m_nMode = nMode;
	m_nFunc = nFunc;
	sSend.Format(":r,%d,%d;", nMode, nFunc);
	strcpy(pSendRtn, sSend);
	m_Rs232.WriteRs232Block(pSendRtn, sSend.GetLength());
}


void CDspSetRs232Dlg::OnBnClickedButtonReadFunc1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sSend;
	char pSend[MAX_PATH]={0};
//	int nMode = m_nTab+1;
	int nMode = 0;
	int nFunc = 1;
	m_nMode = nMode;
	m_nFunc = nFunc;
	sSend.Format(":r,%d,%d;", nMode, nFunc);
	strcpy(pSend, sSend);
	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());
}

void CDspSetRs232Dlg::OnBnClickedButtonWriteFunc1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sSend;
	char pSend[MAX_PATH]={0};
	char pSendRtn[MAX_PATH]={0};
//	int nMode = m_nTab+1;
	int nMode = 0;
	int nFunc = 1;
	int nVal = 0;
	CString sVal="";

	GetDlgItem(IDC_EDIT_FUNC_1)->GetWindowText(sVal);
	nVal = atoi(sVal);

	sSend.Format(":w,%d,%d,%d;", nMode, nFunc, nVal);
	strcpy(pSend, sSend);
	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());

	Sleep(50);

	m_nMode = nMode;
	m_nFunc = nFunc;
	sSend.Format(":r,%d,%d;", nMode, nFunc);
	strcpy(pSendRtn, sSend);
	m_Rs232.WriteRs232Block(pSendRtn, sSend.GetLength());
}


void CDspSetRs232Dlg::OnBnClickedButtonReadFunc2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sSend;
	char pSend[MAX_PATH]={0};
//	int nMode = m_nTab+1;
	int nMode = 0;
	int nFunc = 2;
	m_nMode = nMode;
	m_nFunc = nFunc;
	sSend.Format(":r,%d,%d;", nMode, nFunc);
	strcpy(pSend, sSend);
	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());
}

void CDspSetRs232Dlg::OnBnClickedButtonWriteFunc2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sSend;
	char pSend[MAX_PATH]={0};
	char pSendRtn[MAX_PATH]={0};
//	int nMode = m_nTab+1;
	int nMode = 0;
	int nFunc = 2;
	int nVal = 0;
	CString sVal="";

	GetDlgItem(IDC_EDIT_FUNC_2)->GetWindowText(sVal);
	nVal = atoi(sVal);

	sSend.Format(":w,%d,%d,%d;", nMode, nFunc, nVal);
	strcpy(pSend, sSend);
	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());

	Sleep(50);

	m_nMode = nMode;
	m_nFunc = nFunc;
	sSend.Format(":r,%d,%d;", nMode, nFunc);
	strcpy(pSendRtn, sSend);
	m_Rs232.WriteRs232Block(pSendRtn, sSend.GetLength());
}


void CDspSetRs232Dlg::OnBnClickedButtonReadFunc3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sSend;
	char pSend[MAX_PATH]={0};
//	int nMode = m_nTab+1;
	int nMode = 0;
	int nFunc = 3;
	m_nMode = nMode;
	m_nFunc = nFunc;
	sSend.Format(":r,%d,%d;", nMode, nFunc);
	strcpy(pSend, sSend);
	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());
}

void CDspSetRs232Dlg::OnBnClickedButtonWriteFunc3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sSend;
	char pSend[MAX_PATH]={0};
	char pSendRtn[MAX_PATH]={0};
	int nMode = m_nTab+1;
	int nFunc = 3;
	int nVal = 0;
	CString sVal="";

	GetDlgItem(IDC_EDIT_FUNC_3)->GetWindowText(sVal);
	nVal = atoi(sVal);

	sSend.Format(":w,%d,%d,%d;", nMode, nFunc, nVal);
	strcpy(pSend, sSend);
	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());

	Sleep(50);

	m_nMode = nMode;
	m_nFunc = nFunc;
	sSend.Format(":r,%d,%d;", nMode, nFunc);
	strcpy(pSendRtn, sSend);
	m_Rs232.WriteRs232Block(pSendRtn, sSend.GetLength());
}


void CDspSetRs232Dlg::OnBnClickedButtonReadFunc4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sSend;
	char pSend[MAX_PATH]={0};
//	int nMode = m_nTab+1;
	int nMode = 0;
	int nFunc = 4;
	m_nMode = nMode;
	m_nFunc = nFunc;
	sSend.Format(":r,%d,%d;", nMode, nFunc);
	strcpy(pSend, sSend);
	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());
}

void CDspSetRs232Dlg::OnBnClickedButtonWriteFunc4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sSend;
	char pSend[MAX_PATH]={0};
	char pSendRtn[MAX_PATH]={0};
//	int nMode = m_nTab+1;
	int nMode = 0;
	int nFunc = 4;
	int nVal = 0;
	CString sVal="";

	GetDlgItem(IDC_EDIT_FUNC_4)->GetWindowText(sVal);
	nVal = atoi(sVal);

	sSend.Format(":w,%d,%d,%d;", nMode, nFunc, nVal);
	strcpy(pSend, sSend);
	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());

	Sleep(50);

	m_nMode = nMode;
	m_nFunc = nFunc;
	sSend.Format(":r,%d,%d;", nMode, nFunc);
	strcpy(pSendRtn, sSend);
	m_Rs232.WriteRs232Block(pSendRtn, sSend.GetLength());
}





void CDspSetRs232Dlg::OnBnClickedButtonReadFunc5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sSend;
	char pSend[MAX_PATH] = { 0 };
	//	int nMode = m_nTab+1;
	int nMode = 0;
	int nFunc = 5;
	m_nMode = nMode;
	m_nFunc = nFunc;
	sSend.Format(":r,%d,%d;", nMode, nFunc);
	strcpy(pSend, sSend);
	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());
}


void CDspSetRs232Dlg::OnBnClickedButtonWriteFunc5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sSend;
	char pSend[MAX_PATH] = { 0 };
	char pSendRtn[MAX_PATH] = { 0 };
	//	int nMode = m_nTab+1;
	int nMode = 0;
	int nFunc = 5;
	int nVal = 0;
	CString sVal = "";

	GetDlgItem(IDC_EDIT_FUNC_5)->GetWindowText(sVal);
	nVal = atoi(sVal);

	sSend.Format(":w,%d,%d,%d;", nMode, nFunc, nVal);
	strcpy(pSend, sSend);
	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());

	Sleep(50);

	m_nMode = nMode;
	m_nFunc = nFunc;
	sSend.Format(":r,%d,%d;", nMode, nFunc);
	strcpy(pSendRtn, sSend);
	m_Rs232.WriteRs232Block(pSendRtn, sSend.GetLength());
}


void CDspSetRs232Dlg::OnBnClickedButtonReadFunc6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sSend;
	char pSend[MAX_PATH] = { 0 };
	//	int nMode = m_nTab+1;
	int nMode = 0;
	int nFunc = 6;
	m_nMode = nMode;
	m_nFunc = nFunc;
	sSend.Format(":r,%d,%d;", nMode, nFunc);
	strcpy(pSend, sSend);
	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());
}


void CDspSetRs232Dlg::OnBnClickedButtonWriteFunc6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sSend;
	char pSend[MAX_PATH] = { 0 };
	char pSendRtn[MAX_PATH] = { 0 };
	//	int nMode = m_nTab+1;
	int nMode = 0;
	int nFunc = 6;
	int nVal = 0;
	CString sVal = "";

	GetDlgItem(IDC_EDIT_FUNC_6)->GetWindowText(sVal);
	nVal = atoi(sVal);

	sSend.Format(":w,%d,%d,%d;", nMode, nFunc, nVal);
	strcpy(pSend, sSend);
	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());

	Sleep(50);

	m_nMode = nMode;
	m_nFunc = nFunc;
	sSend.Format(":r,%d,%d;", nMode, nFunc);
	strcpy(pSendRtn, sSend);
	m_Rs232.WriteRs232Block(pSendRtn, sSend.GetLength());
}


void CDspSetRs232Dlg::OnBnClickedButtonReadFunc7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sSend;
	char pSend[MAX_PATH] = { 0 };
	//	int nMode = m_nTab+1;
	int nMode = 0;
	int nFunc = 7;
	m_nMode = nMode;
	m_nFunc = nFunc;
	sSend.Format(":r,%d,%d;", nMode, nFunc);
	strcpy(pSend, sSend);
	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());
}


void CDspSetRs232Dlg::OnBnClickedButtonWriteFunc7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sSend;
	char pSend[MAX_PATH] = { 0 };
	char pSendRtn[MAX_PATH] = { 0 };
	//	int nMode = m_nTab+1;
	int nMode = 0;
	int nFunc = 7;
	int nVal = 0;
	CString sVal = "";

	GetDlgItem(IDC_EDIT_FUNC_7)->GetWindowText(sVal);
	nVal = atoi(sVal);

	sSend.Format(":w,%d,%d,%d;", nMode, nFunc, nVal);
	strcpy(pSend, sSend);
	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());

	Sleep(50);

	m_nMode = nMode;
	m_nFunc = nFunc;
	sSend.Format(":r,%d,%d;", nMode, nFunc);
	strcpy(pSendRtn, sSend);
	m_Rs232.WriteRs232Block(pSendRtn, sSend.GetLength());
}


void CDspSetRs232Dlg::OnBnClickedButtonReadFunc8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sSend;
	char pSend[MAX_PATH] = { 0 };
	//	int nMode = m_nTab+1;
	int nMode = 0;
	int nFunc = 8;
	m_nMode = nMode;
	m_nFunc = nFunc;
	sSend.Format(":r,%d,%d;", nMode, nFunc);
	strcpy(pSend, sSend);
	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());
}


void CDspSetRs232Dlg::OnBnClickedButtonWriteFunc8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sSend;
	char pSend[MAX_PATH] = { 0 };
	char pSendRtn[MAX_PATH] = { 0 };
	//	int nMode = m_nTab+1;
	int nMode = 0;
	int nFunc = 8;
	int nVal = 0;
	CString sVal = "";

	GetDlgItem(IDC_EDIT_FUNC_8)->GetWindowText(sVal);
	nVal = atoi(sVal);

	sSend.Format(":w,%d,%d,%d;", nMode, nFunc, nVal);
	strcpy(pSend, sSend);
	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());

	Sleep(50);

	m_nMode = nMode;
	m_nFunc = nFunc;
	sSend.Format(":r,%d,%d;", nMode, nFunc);
	strcpy(pSendRtn, sSend);
	m_Rs232.WriteRs232Block(pSendRtn, sSend.GetLength());
}


void CDspSetRs232Dlg::OnBnClickedCheckRepeatTest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn = ((CButton*)GetDlgItem(IDC_CHECK_REPEAT_TEST))->GetCheck();
	CString sSend;
	char pSend[MAX_PATH] = { 0 };
	if (bOn)
	{
		sSend.Format(":test;");
	}
	else
	{
		sSend.Format(":at;");
	}
	strcpy(pSend, sSend);
	m_Rs232.WriteRs232Block(pSend, sSend.GetLength());
}
