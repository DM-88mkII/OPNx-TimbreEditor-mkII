
#pragma once



class CListBoxCustom : public CListBox
{
	public:
		virtual ~CListBoxCustom(){}
		
		CListBoxCustom(){}
	
	protected:
		virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
		virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
		
		afx_msg void OnSetFocus(CWnd* pOldWnd);
		afx_msg void OnKillFocus(CWnd* pNewWnd);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		
		DECLARE_MESSAGE_MAP()
};
