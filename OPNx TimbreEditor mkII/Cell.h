
#pragma once



#include <iterator>
#include <algorithm>



class CCell : public CStatic
{
	public:
		virtual ~CCell(){}
		
		CCell();
		
		void SetText(CString Text){ std::ranges::fill(m_aText, 0); _tcscpy_s(m_aText, std::size(m_aText), Text); }
		LPCTSTR GetText(){ return m_aText; }
		
		void SetFormat(UINT Format){ m_Format = Format; }
		void SetBackColor(COLORREF BackColor){ m_BackColor = BackColor; }
		void SetTextColor(COLORREF TextColor){ m_TextColor = TextColor; }
		
		CBitmap& GetBitmap(){ return m_CBitmap; }
	
	protected:
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnPaint();
		
		DECLARE_MESSAGE_MAP()
	
	protected:
		CFont m_Font;
		CString m_Text;
		TCHAR m_aText[128];
		UINT m_Format;
		
		COLORREF m_BackColor;
		COLORREF m_TextColor;
		
		CBitmap m_CBitmap;
};
