
#pragma once



#include <array>
#include "Cell.h"



template <int ROW_NUM, int COLUMN_NUM>
class CStaticList
{
	public:
		struct ColumnParam
		{
			DWORD dwStyle;
			int Width;
			
			ColumnParam():dwStyle(0),Width(0){}
		};
		
		struct ColumnInfo
		{
			int iRow;
			int iColum;
			ColumnParam* pParam;
			CCell Cell;
			
			ColumnInfo():iRow(-1),iColum(-1),pParam(NULL){}
			
			void Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd)
			{
				Cell.Create(NULL, dwStyle, rect, pParentWnd);
			}
		};
		
		struct RowInfo
		{
			std::array<ColumnInfo, COLUMN_NUM> aColumnInfo;
			
			RowInfo(){}
			
			std::array<ColumnInfo, COLUMN_NUM>& GetColumns(){ return aColumnInfo; }
		};
	
	public:
		static constexpr int RowNum = ROW_NUM;
		static constexpr int ColumnNum = COLUMN_NUM;
	
	public:
		virtual ~CStaticList(){}
		
		CStaticList();
		
		void Create(int x, int y, int Height, std::array<ColumnParam, COLUMN_NUM> aColumParam, CWnd* pParentWnd);
		
		std::array<RowInfo, ROW_NUM>& GetRows(){ return m_aRow; }
		
		void Invalidate(int iRow, int iColum){ m_aRow[iRow].aColumnInfo[iColum].Cell.Invalidate(); }
	
	protected:
		std::array<ColumnParam, COLUMN_NUM> m_aColumnParam;
		
		std::array<RowInfo, ROW_NUM> m_aRow;
};
