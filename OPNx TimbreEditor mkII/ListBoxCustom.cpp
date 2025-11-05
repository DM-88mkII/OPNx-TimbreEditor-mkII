



#include "pch.h"
#include "afxdialogex.h"
#include "ListBoxCustom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



BEGIN_MESSAGE_MAP(CListBoxCustom, CListBox)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
//	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



void CListBoxCustom::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	#if true//[
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	int nIndex = lpDrawItemStruct->itemID;
	if (nIndex < 0) return;
	
	CString strText;
	GetText(nIndex, strText);
	
	BOOL bSelected = (lpDrawItemStruct->itemState & ODS_SELECTED);
	BOOL bFocus = (GetFocus() == this);
	
	COLORREF crBack, crText;
	crText = (bSelected)? GetSysColor(COLOR_3DFACE): GetSysColor(COLOR_WINDOWTEXT);
	crBack = (bSelected)? (bFocus)? GetSysColor(COLOR_MENUHILIGHT): RGB(0x78,0x78,0x78): GetSysColor(COLOR_WINDOW);
	
	pDC->SetBkColor(crBack);
	pDC->SetTextColor(crText);
	
	CBrush brush(crBack);
	pDC->FillRect(&lpDrawItemStruct->rcItem, &brush);
	
	CRect rcItem = lpDrawItemStruct->rcItem;
	rcItem.left += 4;
	
	pDC->DrawText(strText, rcItem, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	
	if (lpDrawItemStruct->itemState & ODS_FOCUS) pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
	#else//][
	if (!lpDrawItemStruct || lpDrawItemStruct->itemID == -1) return;
	
	BOOL bSelected = (lpDrawItemStruct->itemState & ODS_SELECTED);
	BOOL bFocus = (GetFocus() == this);
	
	COLORREF crBack, crText;
	crText = (bSelected)? GetSysColor(COLOR_3DFACE): GetSysColor(COLOR_WINDOWTEXT);
	crBack = (bSelected)? (bFocus)? GetSysColor(COLOR_MENUHILIGHT): RGB(0x78,0x78,0x78): GetSysColor(COLOR_WINDOW);
	
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rcItem = lpDrawItemStruct->rcItem;
	
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rcItem.Width(), rcItem.Height());
	
	CBitmap* pOldBmp = memDC.SelectObject(&bmp);
	
	memDC.FillSolidRect(rcItem, crBack);
	
	CFont* pOldFont = nullptr;
	CFont* pFont = GetFont();
	if (pFont) pOldFont = memDC.SelectObject(pFont);
	{	// 
		CString text;
		GetText(lpDrawItemStruct->itemID, text);
		memDC.SetBkMode(TRANSPARENT);
		memDC.SetTextColor(crText);
		//memDC.TextOut(rcItem.left + 4, rcItem.top + 2, text);
		rcItem.left += 4;
		memDC.DrawText(text, rcItem, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}
	if (pOldFont) memDC.SelectObject(pOldFont);
	
	pDC->BitBlt(
		rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height(),
		&memDC, 0/*rcItem.left*/, 0/*rcItem.top*/, SRCCOPY
	);
	
	memDC.SelectObject(pOldBmp);
	#endif//]
}



void CListBoxCustom::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 18;
}



void CListBoxCustom::OnSetFocus(CWnd* pOldWnd)
{
	CListBox::OnSetFocus(pOldWnd);
	Invalidate();
}



void CListBoxCustom::OnKillFocus(CWnd* pNewWnd)
{
	CListBox::OnKillFocus(pNewWnd);
	Invalidate();
}



BOOL CListBoxCustom::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;//CDialogEx::OnEraseBkgnd(pDC);
}
