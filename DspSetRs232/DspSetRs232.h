
// DspSetRs232.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.


// CDspSetRs232App:
// 이 클래스의 구현에 대해서는 DspSetRs232.cpp을 참조하십시오.
//
// Define Color

#define RGB_BLACK       RGB(0,0,0)
#define RGB_WHITE       RGB(255,255,255)
#define RGB_GRAY        RGB(0x80, 0x80, 0x80)
#define RGB_RED         RGB(255,0,0)
#define RGB_DARKRED     RGB(207,0,0)
#define RGB_LTRED       RGB(255,128,128)
#define RGB_YELLOW      RGB(255,255,0)
#define RGB_DARKYELLOW  RGB(128,128,0)
#define RGB_GREEN       RGB(0,255,0)
#define RGB_LTGREEN     RGB(0,255,128)
#define RGB_DARKGREEN   RGB(0,155,75)
#define RGB_CYAN        RGB(0,255,255)
#define RGB_BLUE        RGB(0,0,255)
#define RGB_LTBLUE		RGB(82, 200, 200)
#define RGB_CLOUDBLUE	RGB(128,184,223)
#define RGB_MAGENTA     RGB(255,0,255)
#define RGB_DARKMAGENTA RGB(193,0,130)
#define RGB_WHGRAY      RGB(218,218,218)
#define RGB_LTGRAY      RGB(192,192,192)
#define RGB_DARKGRAY    RGB(48,48,48)
#define RGB_DARKCYAN    RGB(0,128,128)
#define RGB_LTCYAN      RGB(128,255,255)
#define RGB_BOON        RGB(255,0,128)
#define RGB_DARKBOON    RGB(64,0,128)
#define RGB_DARKBLUE    RGB(0,0,128)
#define RGB_PINK        RGB(255,0,128)
#define RGB_SKYBLUE		RGB(128,255,255)
#define RGB_PURPLE		RGB(128,0,255)
#define RGB_YELLOWGREEN RGB(128,255,0)
#define RGB_WHYELLOW    RGB(255,255,128)
#define RGB_ORANGE      RGB(255,128,0)
#define RGB_DARKORANGE  RGB(220,70,0)
#define RGB_CHESTNUT	RGB(128,64,64)
#define RGB_NAVY		RGB(0,0,128)
#define RGB_LLTGREEN    RGB(128,255,128)
#define RGB_LTPURPLE    RGB(255,128,255)
#define RGB_LT_ORANGE   RGB(255,128,64)
#define RGB_DARKFINGER	RGB(128,0,128)
#define RGB_DLG_FRM		RGB(192,192,192)
#define RGB_DARKBROWN	RGB(64,0,0)
#define RGB_BROWN       RGB(100,0,0)
#define RGB_DIALOG		RGB(200,200,192)
#define RGB_POSTIT		RGB(240,233,134)
#define RGB_LTYELLOW	RGB(254,254,190)
#define RGB_DARK_BLUE	RGB(68,136,204)
#define RGB_FRAME		RGB(212,208,200)

#define RGB_DARK_GREEN	RGB(0,125,60)
#define RGB_LT_PURPLE	RGB(179,179,255)
#define RGB_LT_YELLOW	RGB(255,255,150)
#define RGB_LT_PINK		RGB(255,200,200)
#define RGB_LT_GREEN	RGB(195,255,195)
#define RGB_LT_BLUE		RGB(210,255,255)
#define RGB_WH_ORANGE	RGB(255,220,190)

#define WM_MYTAB_CLICKED				(WM_USER+0)
#define	WM_RECEIVEDATA					(WM_USER+1)

class CDspSetRs232App : public CWinApp
{

public:
	CDspSetRs232App();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

// 구현입니다.

	DECLARE_MESSAGE_MAP()
};

extern CDspSetRs232App theApp;