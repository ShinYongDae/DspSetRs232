#if !defined(AFX_MYTAB_H__5408F965_9CCF_4100_B847_0D810904686A__INCLUDED_)
#define AFX_MYTAB_H__5408F965_9CCF_4100_B847_0D810904686A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyTab.h : header file
//

#define MAX_TAB_ITEM	10

/////////////////////////////////////////////////////////////////////////////
// CMyTab window

class CMyTab : public CTabCtrl
{
	int m_nTab;
	BOOL m_bFont, m_bSetFont, m_bClicked;
	COLORREF	m_crCtrl;
	COLORREF	m_crText;
	COLORREF	m_pCrText[MAX_TAB_ITEM];
	COLORREF	m_crBkgnd;
	COLORREF	m_pCrBkgnd[MAX_TAB_ITEM];
	HBRUSH		m_hBrush;
//	HBRUSH		m_hwndBrush;
	LOGFONT		m_lf;
	CFont		m_font;
	int			m_fontSize;
	CString		m_fontName;
	BOOL		m_bBold;
	CString		m_strText;
	BOOL		m_bState;
	HCURSOR		m_hCursor;
	CRect		m_rtItem[MAX_TAB_ITEM];
	CString		m_strItem[MAX_TAB_ITEM];

//	char **m_tabName;
	int m_ntabName;

	void ReconstructFont();

// Construction
public:
	CMyTab();

// Attributes
public:
	HWND m_hParentWnd;
	int	m_nCtrlID;

// Operations
public:
	void SetHwnd(HWND hParentWnd, int nCtrlID);

	COLORREF GetBkColor() {return m_crBkgnd;}
	COLORREF GetTextColor(){return m_crText;}
//	CString GetText(){return m_strText;}
	BOOL GetFontBold(){return m_bBold;}
	CString GetFontName(){return m_fontName;}
	int GetFontSize(){return m_fontSize;}
	BOOL GetClicked(){return m_bClicked;}

	void SetClicked(BOOL bOn){m_bClicked = bOn;}
	void SetBkColor(COLORREF crBkgnd=RGB_LT_GREEN);
	void SetColorCtrl(COLORREF crCtrl=RGB_FRAME);
	void SetBkColor(int nItem=0, COLORREF crBkgnd=RGB_LT_GREEN);
	void SetTextColor(COLORREF crText=RGB_RED);
	void SetTextColor(int nItem=0, COLORREF crText=RGB_RED);
	void AddText(const CString& strText);
//	void SetText(const CString& strText);
	void SetFontBold(BOOL bBold);
	void SetFontName(const CString& strFont);
	void SetFontUnderline(BOOL bSet);
	void SetFontItalic(BOOL bSet);
	void SetFontSize(int nSize);
	void SetFontSize(int nSizeH, int nSizeW);
	void SetSunken(BOOL bSet);
	void SetBorder(BOOL bSet);
	void Refresh();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTab)
	protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyTab();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyTab)
//	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTAB_H__5408F965_9CCF_4100_B847_0D810904686A__INCLUDED_)
