



#include "pch.h"
#include "StaticList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



template <int ROW_NUM, int COLUMN_NUM>
CStaticList<ROW_NUM, COLUMN_NUM>::CStaticList()
{
}



template <int ROW_NUM, int COLUMN_NUM>
void CStaticList<ROW_NUM, COLUMN_NUM>::Create(int x, int y, int Height, std::array<ColumnParam, COLUMN_NUM> aColumnParam, CWnd* pParentWnd)
{
	m_aColumnParam = aColumnParam;
	
	{	// Row
		int iRow = 0;
		int t = y;
		for (auto& Row: m_aRow){
			int iColum = 0;
			int l = x;
			int r = 0;
			int b = t + Height;
			for (auto& ColumnInfo: Row.aColumnInfo){
				ColumnInfo.iRow = iRow;
				ColumnInfo.iColum = iColum;
				ColumnInfo.pParam = &m_aColumnParam[iColum];
				
				auto& ColumParam = *ColumnInfo.pParam;
				r = l + ColumParam.Width;
				ColumnInfo.Create(ColumParam.dwStyle, CRect(l, t, r, b), pParentWnd);
				l = r;
				
				iColum++;
			}
			t = b;
			
			iRow++;
		}
	}
}



template class CStaticList<8, 7>;
template class CStaticList<16, 10>;
template class CStaticList<16, 11>;
