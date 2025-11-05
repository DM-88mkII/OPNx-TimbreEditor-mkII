



#include "pch.h"
#include "afxdialogex.h"
#include "ListBoxT.h"
#include "BatchDlg.h"
#include "TimbreDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



template <class T>
CListBoxT<T>::~CListBoxT()
{
}



template <class T>
CListBoxT<T>::CListBoxT(CListBox &rCListBox)
:This(rCListBox)
,aaPair(1)
,iPage(0)
{
}



template <class T>
void CListBoxT<T>::AddPage(int Count)
{
	aaPair.emplace_back(std::vector<std::pair<CString, sptr<T>>>(Count));
}



template <class T>
bool CListBoxT<T>::SetPage(int iPage)
{
	if (IsValidPage(iPage)){
		this->iPage = iPage;
		
		This.ResetContent();
		
		int i = 0;
		auto& aPair = Pair();
		for (auto& Pair : aPair){
			This.AddString(Prefix(i) + Pair.first);
			i++;
		}
		return true;
	}
	return false;
}



template <class T>
int CListBoxT<T>::GetPage()
{
	return iPage;
}



template <class T>
void CListBoxT<T>::AddPrefix(CString& rString)
{
	aPrefix.emplace_back(rString);
}



template <class T>
CString CListBoxT<T>::Prefix(int i)
{
	if (i >= 0 && i < aPrefix.size()){
		return aPrefix[i];
	}
	return CString();
}



template <class T>
std::vector<std::pair<CString, sptr<T>>>& CListBoxT<T>::Pair()
{
	return aaPair[iPage];
}



template <class T>
bool CListBoxT<T>::IsValidPage(int iPage)
{
	return (iPage >= 0 && iPage < aaPair.size());
}



template <class T>
bool CListBoxT<T>::IsValidIndex(int i)
{
	return (::IsWindow(This.GetSafeHwnd()) && i >= 0 && i < GetCount());
}



template <class T>
void CListBoxT<T>::SetFocus()
{
	This.SetFocus();
}



template <class T>
void CListBoxT<T>::ResetContent()
{
	auto& aPair = Pair();
	aPair.clear();
	return This.ResetContent();
}



template <class T>
int CListBoxT<T>::GetCount()
{
	return This.GetCount();
}



template <class T>
int CListBoxT<T>::SetTopIndex(int nIndex)
{
	return This.SetTopIndex(nIndex);
}



template <class T>
int CListBoxT<T>::GetCurSel()
{
	return This.GetCurSel();
}



template <class T>
int CListBoxT<T>::SetCurSel(int i)
{
	return This.SetCurSel(i);
}



template <class T>
void CListBoxT<T>::GetText(int i, CString& rString)
{
	if (IsValidIndex(i)){
		auto& aPair = Pair();
		rString = aPair[i].first;
	}
}



template <class T>
void CListBoxT<T>::SetText(int i, CString& rString)
{
	auto pData = GetDataPtr(i);
	DeleteString(i);
	InsertString(i, rString);
	SetDataPtr(i, pData);
}



template <class T>
void CListBoxT<T>::SetText(int iPage, int i, CString& rString)
{
	if (IsValidPage(iPage) && IsValidIndex(i)){
		auto& Pair = aaPair[iPage][i];
		Pair.first = rString;
		
		if (this->iPage == iPage){
			This.DeleteString(i);
			This.InsertString(i, Prefix(i) + rString);
		}
	}
}



template <class T>
int CListBoxT<T>::AddString(CString& rString)
{
	auto& aPair = Pair();
	aPair.emplace_back(rString, std::make_shared<T>());
	
	auto i = GetCount();
	return This.AddString(Prefix(i) + rString);
}



template <class T>
int CListBoxT<T>::InsertString(int i, CString& rString)
{
	auto& aPair = Pair();
	aPair.insert((aPair.begin() + i), {rString, std::make_shared<T>()});
	return This.InsertString(i, Prefix(i) + rString);
}



template <class T>
int CListBoxT<T>::DeleteString(int i)
{
	auto& aPair = Pair();
	aPair.erase(aPair.begin() + i);
	return This.DeleteString(i);
}



template <class T>
int CListBoxT<T>::FindString(int i, CString& rString)
{
	auto& aPair = Pair();
	if (i < 0){
		i = 0;
		for (auto& Pair : aPair){
			if (rString == Pair.first) return i;
			i++;
		}
	} else {
		if (IsValidIndex(i)){
			for (auto itr = aPair.begin() + i; itr != aPair.end(); itr++){
				if (rString == itr->first) return i;
				i++;
			}
		}
	}
	return LB_ERR;
}



template <class T>
sptr<T> CListBoxT<T>::GetDataPtr(int i)
{
	if (IsValidIndex(i)){
		auto& aPair = Pair();
		return aPair[i].second;
	}
	return nullptr;
}



template <class T>
sptr<T> CListBoxT<T>::GetDataPtr(int iPage, int i)
{
	if (IsValidPage(iPage) && IsValidIndex(i)){
		return aaPair[iPage][i].second;
	}
	return nullptr;
}



template <class T>
bool CListBoxT<T>::SetDataPtr(int i, sptr<T> pData)
{
	if (IsValidIndex(i)){
		auto& aPair = Pair();
		aPair[i].second = pData;
		return true;
	}
	return false;
}



template <class T>
bool CListBoxT<T>::SetDataPtr(int iPage, int i, sptr<T> pData)
{
	if (IsValidPage(iPage) && IsValidIndex(i)){
		aaPair[iPage][i].second = pData;
		return true;
	}
	return false;
}



template <class T>
int CListBoxT<T>::FindDataPtr(int i, sptr<T> pData)
{
	auto& aPair = Pair();
	if (i < 0){
		i = 0;
		for (auto& Pair : aPair){
			if (pData == Pair.second) return i;
			i++;
		}
	} else {
		if (IsValidIndex(i)){
			for (auto itr = aPair.begin() + i; itr != aPair.end(); itr++){
				if (pData == itr->second) return i;
				i++;
			}
		}
	}
	return LB_ERR;
}



template <class T>
int CListBoxT<T>::FindDataPtr(int i, T* pData)
{
	auto& aPair = Pair();
	if (i < 0){
		i = 0;
		for (auto& Pair : aPair){
			if (pData == Pair.second.get()) return i;
			i++;
		}
	} else {
		if (IsValidIndex(i)){
			for (auto itr = aPair.begin() + i; itr != aPair.end(); itr++){
				if (pData == itr->second.get()) return i;
				i++;
			}
		}
	}
	return LB_ERR;
}



template <class T>
void CListBoxT<T>::Erase(sptr<T> pData)
{
	for (auto& aPair : aaPair){
		int i = 0;
		for (auto& Pair : aPair){
			if (pData == Pair.second){
				Pair.first = CString();
				Pair.second = std::make_shared<T>();
				
				CString String;
				DeleteString(i);
				InsertString(i, String);
			}
			i++;
		}
	}
}



template CListBoxT</*sptr*/CBatch>;
template CListBoxT</*sptr*/CTimbre>;
