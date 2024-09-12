// MyTab.cpp : implementation file
//

#include "stdafx.h"
#include "DspSetRs232.h"
#include "MyTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTab

CMyTab::CMyTab()
{
	m_hParentWnd = NULL;
	m_crText = GetSysColor(COLOR_WINDOWTEXT);
	m_hBrush = NULL;

	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT),sizeof(m_lf),&m_lf);

	m_bFont = FALSE;
	BOOL bCr = m_font.CreateFontIndirect(&m_lf);
	if(bCr)
		m_bFont = TRUE;

	m_bState = FALSE;
	m_hCursor = NULL;

	m_ntabName = 0;
	m_bSetFont = FALSE;

	m_nTab = 0;
	m_bClicked = FALSE;
	m_crBkgnd = NULL;
	m_crText = NULL;
}

CMyTab::~CMyTab()
{

	if(m_bFont)
	{
		if(m_font.DeleteObject())
			m_bFont = FALSE;
	}

// 	if(::DeleteObject(m_hBrush))
// 		m_hBrush = FALSE;
}


BEGIN_MESSAGE_MAP(CMyTab, CTabCtrl)
	//{{AFX_MSG_MAP(CMyTab)
//	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTab message handlers


void CMyTab::SetHwnd(HWND hParentWnd, int nCtrlID)
{
	m_hParentWnd = hParentWnd;
	m_nCtrlID = nCtrlID;
}

void CMyTab::AddText(const CString& strText)
{
	m_strItem[m_ntabName] = strText;
	int nSize = strText.GetLength();
	char *tabName = new char[nSize+1];	//(char*)calloc(nSize+1, sizeof(char));
	strcpy(tabName, strText);

	TC_ITEM item;
	item.mask = TCIF_TEXT;
	item.pszText = tabName;
	InsertItem(m_ntabName, &item);
	m_ntabName++;

	delete tabName;

	return;
}

void CMyTab::SetTextColor(COLORREF crText)
{
	m_crText = crText;
	RedrawWindow();
	return;
}

void CMyTab::SetTextColor(int nItem, COLORREF crText)
{
	m_pCrText[nItem] = crText;
	RedrawWindow();
	return;
}

void CMyTab::SetFontBold(BOOL bBold)
{	
	m_bBold = bBold;
	m_lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;
	ReconstructFont();
	RedrawWindow();
	return;
}

void CMyTab::SetFontUnderline(BOOL bSet)
{	
	m_lf.lfUnderline = bSet;
	ReconstructFont();
	RedrawWindow();
	return;
}

void CMyTab::SetFontItalic(BOOL bSet)
{
	m_lf.lfItalic = bSet;
	ReconstructFont();
	RedrawWindow();
	return;	
}

void CMyTab::SetSunken(BOOL bSet)
{
	if (!bSet)
		ModifyStyleEx(WS_EX_STATICEDGE,0,SWP_DRAWFRAME);
	else
		ModifyStyleEx(0,WS_EX_STATICEDGE,SWP_DRAWFRAME);
		
	return;	
}

void CMyTab::SetBorder(BOOL bSet)
{
	if (!bSet)
		ModifyStyle(WS_BORDER,0,SWP_DRAWFRAME);
	else
		ModifyStyle(0,WS_BORDER,SWP_DRAWFRAME);
		
	return;	
}

void CMyTab::SetFontSize(int nSize)
{
	m_fontSize = nSize;
	nSize*=-1;
	m_lf.lfHeight = nSize;
	ReconstructFont();
	RedrawWindow();
	return;
}

void CMyTab::SetFontSize(int nSizeH, int nSizeW)
{
	m_lf.lfHeight = (long)nSizeH;
	m_lf.lfWidth = (long)nSizeW;
	ReconstructFont();
	RedrawWindow();
	return;
}

void CMyTab::SetBkColor(COLORREF crBkgnd)
{
	m_crBkgnd = crBkgnd;
	
	RedrawWindow();
	return;
}

void CMyTab::SetColorCtrl(COLORREF crCtrl)
{
	m_crCtrl = crCtrl;
	
	RedrawWindow();
	return;
}

void CMyTab::SetBkColor(int nItem, COLORREF crBkgnd)
{
	m_pCrBkgnd[nItem] = crBkgnd;
	
	RedrawWindow();
	return;
}

void CMyTab::SetFontName(const CString& strFont)
{	
	m_fontName = strFont;
	strcpy(m_lf.lfFaceName,strFont);
	ReconstructFont();
	RedrawWindow();
	return;
}

void CMyTab::ReconstructFont()
{
	if(m_bFont)
	{
		if(m_font.DeleteObject())
			m_bFont = FALSE;
	}
	
	BOOL bCreated = FALSE;
	if(!m_bFont)
	{
		bCreated = m_font.CreateFontIndirect(&m_lf);
		if(bCreated)
		{
			m_bFont = TRUE;
			m_bSetFont = FALSE;
			SetFont((CFont*)&m_font, TRUE);
		}
	}

	ASSERT(bCreated);
}

void CMyTab::Refresh()
{
	RedrawWindow();
}



void CMyTab::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CTabCtrl::PreSubclassWindow();
//	ModifyStyle(0, TCS_OWNERDRAWFIXED);
}


void CMyTab::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect = lpDrawItemStruct->rcItem;
	rect.top += ::GetSystemMetrics(SM_CYEDGE);

// 	if(m_fontSize > 10)
// 	{
// 		rect.top -= (m_fontSize-10);
// 		rect.bottom += (m_fontSize-10);
// 		rect.left -= (m_fontSize-10);
// 		rect.right += (m_fontSize-10);
// 	}

	int nTabIndex = lpDrawItemStruct->itemID;
	
	if (nTabIndex < 0) return;

	m_rtItem[nTabIndex] = rect;
	int nCurSel = GetCurSel();
	BOOL bSelected = (nTabIndex == nCurSel);

// 	COLORREF crSelected = m_bColorSelected ? m_crSelected : GetSysColor(COLOR_BTNTEXT);
// 	COLORREF crNormal  = m_bColorNormal   ? m_crNormal   : GetSysColor(COLOR_BTNTEXT);
// 	COLORREF crDisabled = m_bColorDisabled ? m_crDisabled : GetSysColor(COLOR_GRAYTEXT);

	COLORREF crNormalText  = GetSysColor(COLOR_GRAYTEXT);
	COLORREF crNormalBk  = RGB_FRAME;
	COLORREF crSelectedText = m_crText ? m_crText : m_pCrText[nCurSel];
	COLORREF crSelectedBk = m_crBkgnd ? m_crBkgnd : m_pCrBkgnd[nCurSel];

	char label[64];
	TC_ITEM item;
	item.mask = TCIF_TEXT|TCIF_IMAGE;
	item.pszText = label;     
	item.cchTextMax = 63;    	
	if (!GetItem(nTabIndex, &item))
		return;

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	if (!pDC)
		return;

	int nSavedDC = pDC->SaveDC();

	CRect rectItem;
	POINT pt;

	GetItemRect(nTabIndex, &rectItem);
	GetCursorPos(&pt);
	ScreenToClient(&pt);

// 	if (rectItem.PtInRect(pt))
// 		m_iIndexMouseOver = nTabIndex;



	pDC->SetBkMode(TRANSPARENT);
//	pDC->FillSolidRect(rect, ::GetSysColor(COLOR_BTNFACE));
//	pDC->FillSolidRect(rect, m_crBkgnd);

	CRect WndRect;
	GetClientRect(&WndRect);
	if (!bSelected)
		pDC->FillSolidRect(WndRect, m_crCtrl);

	//**  Draw the image
	CImageList* pImageList = GetImageList();
	if (pImageList && item.iImage >= 0) 
	{

		rect.left += pDC->GetTextExtent(_T(" ")).cx;

		IMAGEINFO info;
		pImageList->GetImageInfo(item.iImage, &info);
		CRect ImageRect(info.rcImage);
		int nYpos = rect.top;

		pImageList->Draw(pDC, item.iImage, CPoint(rect.left, nYpos), ILD_TRANSPARENT);
		rect.left += ImageRect.Width();
	}


//	CFont *pOldFont = pDC->SelectObject(&m_font);
	// Set Font........
//  	if(m_bFont && !m_bSetFont)
// 	{
// 		SetFont((CFont*)&m_font, TRUE);
// 		m_bSetFont = TRUE;
// 	}


	if (bSelected)
	{
		pDC->SetTextColor(crSelectedText);
		pDC->FillSolidRect(rect, crSelectedBk);

		rect.top -= ::GetSystemMetrics(SM_CYEDGE);
		pDC->DrawText(label, rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	}
	else
	{
		for(int nCol=0; nCol<m_ntabName; nCol++)
		{
			CRect rt = m_rtItem[nCol];
			pDC->SetTextColor(crNormalText);
			rt.bottom += (::GetSystemMetrics(SM_CYEDGE))*2;
			pDC->FillSolidRect(rt, crNormalBk);

			rt.top -= ::GetSystemMetrics(SM_CYEDGE);
			pDC->DrawText(m_strItem[nCol], rt, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
		}
	}


//	pDC->SelectObject(pOldFont);

	pDC->RestoreDC(nSavedDC);
	
}

// void CMyTab::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
// {
// 	// TODO: Add your control notification handler code here
// 	m_bClicked = TRUE;
// 	::PostMessage(m_hParentWnd, WM_MYTAB_CLICKED, (WPARAM)NULL, (LPARAM)m_nCtrlID);
// 	
// 	*pResult = 0;
// }
