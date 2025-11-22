



#include "pch.h"
#include "./Cell.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CCell::CCell()
:m_Format(DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX | DT_VCENTER | DT_CENTER)//DT_LEFT, DT_RIGHT,
{
	m_Font.CreatePointFont(90, _T("Consolas"));//m_Font.CreatePointFont(90, _T("MS UI Gothic"));
	m_BackColor = ::GetSysColor(COLOR_WINDOW);
	m_TextColor = ::GetSysColor(COLOR_MENUTEXT);
}



BEGIN_MESSAGE_MAP(CCell, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()



BOOL CCell::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;//CDialogEx::OnEraseBkgnd(pDC);
}



void CCell::OnPaint()
{
	CPaintDC dc(this);
	
	CRect rect;
	GetClientRect(&rect);
	
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	
	switch (GetStyle() & SS_TYPEMASK){
		case SS_LEFT:
		case SS_CENTER:
		case SS_RIGHT:
		{
			CBitmap bmp;
			bmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
			
			CBitmap* pOldBmp = memDC.SelectObject(&bmp);
			if (pOldBmp){
				memDC.FillSolidRect(rect, m_BackColor);
				
				CFont* pOldFont = (m_Font.GetSafeHandle())? memDC.SelectObject(&m_Font): NULL;
				if (pOldFont){
					memDC.SetBkMode(TRANSPARENT);
					memDC.SetTextColor(m_TextColor);
					memDC.DrawText(GetText(), -1, &rect, m_Format);
					memDC.SelectObject(pOldFont);
				}
				
				while (!dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY));
				
				memDC.SelectObject(pOldBmp);
			}
			break;
		}
		case SS_ICON:{
			break;
		}
		case SS_BITMAP:{
			if (m_CBitmap.GetSafeHandle()){
				CBitmap* pOldBmp = memDC.SelectObject(&m_CBitmap);
				if (pOldBmp){
					BITMAP bmp;
					m_CBitmap.GetBitmap(&bmp);
					
					while (!dc.BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, &memDC, 0, 0, SRCCOPY));
					
					memDC.SelectObject(pOldBmp);
				}
			}
			break;
		}
		case SS_OWNERDRAW:{
			break;
		}
		default:{
			break;
		}
	}
}
