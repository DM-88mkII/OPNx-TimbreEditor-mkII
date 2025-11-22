
#pragma once



#include "Common/Common.h"



template <class T>
class CListBoxT
{
	public: ~CListBoxT();
	
	public: CListBoxT(CListBox &rCListBox);
	
	protected: CListBox& This;
	protected: std::vector<CString> aPrefix;
	protected: std::vector<std::vector<std::pair<CString, sptr<T>>>> aaPair;
	protected: int iPage;
	
	public: void AddPage(int Count);
	public: bool SetPage(int iPage);
	public: int GetPage();
	
	public: void AddPrefix(CString& rString);
	
	
	
	protected: CString Prefix(int i);
	protected: std::vector<std::pair<CString, sptr<T>>>& Pair();
	
	protected: bool IsValidPage(int iPage);
	protected: bool IsValidIndex(int i);
	
	public: void SetFocus();
	
	public: void ResetContent();
	
	public: int GetCount();
	
	public: int SetTopIndex(int nIndex);
	
	public: int GetCurSel();
	public: int SetCurSel(int i);
	
	public: void GetText(int i, CString& rString);
	public: void SetText(int i, CString& rString);
	public: void SetText(int iPage, int i, CString& rString);
	
	public: int AddString(CString& rString);
	public: int InsertString(int i, CString& rString);
	public: int DeleteString(int i);
	public: int FindString(int i, CString& rString);
	
	public: sptr<T> GetDataPtr(int i);
	public: sptr<T> GetDataPtr(int iPage, int i);
	public: bool SetDataPtr(int i, sptr<T> pData);
	public: bool SetDataPtr(int iPage, int i, sptr<T> pData);
	
	public: int FindDataPtr(int i, sptr<T> pData);
	public: int FindDataPtr(int i, T* pData);
	
	public: void Erase(sptr<T> pData);
};
