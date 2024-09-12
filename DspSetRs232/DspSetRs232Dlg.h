
// DspSetRs232Dlg.h : ��� ����
//

#pragma once

#include "MyTab.h"
#include "Rs232.h"
#define MAX_CHAR	1000


// PC Comm Command
#define IsConnected		0
#define ReadArrayData	1
#define ReadData		2
#define WriteArrayData	3
#define WriteData		4
#define EncData			5
#define Auto			6
#define Close			7
#define Open			8

#define WatchDogComm	100

#define CMD_NONE		0x7FFF
#define CMD_ERROR		0x7FFE


/******** for RomData ******************************************
Head information : Select mode [1~9], Spare, Spare, Spare, Spare
RomData[0][0] :
mode : 1 ~ 9 [ea]
RomData[mode][0] : ������ ���ܵ��� �ð� ( 30 ~ 10,000 ) [mSec]
RomData[mode][1] : ���� ���� (  1 - S Ŀ�� , 2 - T Ŀ�� ) 
RomData[mode][2] : ���� ���� Min ( -20 deg ~ +20 deg ) [degree]
RomData[mode][3] : ���� ���� Max ( -20 deg ~ +20 deg ) [degree]
RomData[mode][4] : ���ڴ� ü�͸� ���� �޽��� [pulse]
******************************************************************/
#define	CutOffTimeMin		20
#define CutOffTimeMax		10000
#define	CutOffTimeDefault	40
#define	Scurve				1
#define	Tcurve				2
#define AngleMin			-20
#define AngleMax			20
#define EncFilterDefault	2
#define	MaxMode				10	// 0 ~ 9
#define	MaxFunc				9	// 0 ~ 8


// CDspSetRs232Dlg ��ȭ ����
class CDspSetRs232Dlg : public CDialogEx
{
	CMyTab m_Tab;
	CRs232 m_Rs232;
	CString m_sReceive;
	BOOL m_bTmr0, m_bInitSci;
	int m_nSelTab, m_nPort, m_nTab;
	int m_nMode, m_nFunc;
	int m_nRomData[MaxMode][MaxFunc];
	int m_nStepParsingComm, m_nWatchDogComm;
	int m_nDataIdx, m_pData[10];
	long m_nData;
	int m_SetData[MaxMode][MaxFunc], m_BufRead[MaxFunc], m_BufWrite[MaxFunc];
	long m_nEncData;
	BOOL m_bConnected, m_bEnabledCtrl;
	char m_cBuf[MAX_CHAR];
	int m_nBufIdx, m_nBufStVal, m_nBufEdVal;
	BOOL m_bCommSt;
	int m_nCntLoad;

	void InitTab();
	BOOL InitSci(int nPort);
	void ParsingComm(char *pBuf, int nDataLen);
	int GetData(char *pBuf, int &nIdx);
	void InitCommPc();
	void ExeCmd(int nCmd);
	void EnableCtrl(BOOL bEnable);
	void DispModeInfo(int nMode, BOOL *pFunc);
	void ReloadEep();
	void ResetRecvBuf();

// �����Դϴ�.
public:
	CDspSetRs232Dlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	~CDspSetRs232Dlg();

	LRESULT OnReceive(WPARAM wp, LPARAM lp);

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DSPSETRS232_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckConnect();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg void OnTcnSelchangeTabMode(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickTabMode(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonReadMode();
	afx_msg void OnBnClickedButtonWriteMode();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonReadFunc0();
	afx_msg void OnBnClickedButtonWriteFunc0();
	afx_msg void OnBnClickedButtonReadFunc1();
	afx_msg void OnBnClickedButtonWriteFunc1();
	afx_msg void OnBnClickedButtonReadFunc2();
	afx_msg void OnBnClickedButtonWriteFunc2();
	afx_msg void OnBnClickedButtonReadFunc3();
	afx_msg void OnBnClickedButtonWriteFunc3();
	afx_msg void OnBnClickedButtonReadFunc4();
	afx_msg void OnBnClickedButtonWriteFunc4();
	afx_msg void OnBnClickedRadioAuto();
	afx_msg void OnBnClickedRadioClose();
	afx_msg void OnBnClickedRadioOpen();
	afx_msg void OnBnClickedButtonReadFunc5();
	afx_msg void OnBnClickedButtonWriteFunc5();
	afx_msg void OnBnClickedButtonReadFunc6();
	afx_msg void OnBnClickedButtonWriteFunc6();
	afx_msg void OnBnClickedButtonReadFunc7();
	afx_msg void OnBnClickedButtonWriteFunc7();
	afx_msg void OnBnClickedButtonReadFunc8();
	afx_msg void OnBnClickedButtonWriteFunc8();
	afx_msg void OnBnClickedCheckRepeatTest();
};
