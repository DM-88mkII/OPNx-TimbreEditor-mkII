
#pragma once



#ifndef __AFXWIN_H__//[
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif//]

#include "resource.h"
#include "FmStationDlg.h"



class CFmStationApp : public CWinApp
{
	public:
		CFmStationApp();
	
	public:
		virtual BOOL InitInstance();
		
		DECLARE_MESSAGE_MAP()
	
	public:
		CFmStationDlg& GetFmStationDlg(){ return *(CFmStationDlg*)m_pMainWnd; }
		
		int GetValue(CString Key, int Default);
		bool SetValue(CString Key, int Value);
		
		CString GetString(CString Key, CString Default);
		bool SetString(CString Key, CString String);
};

extern CFmStationApp theApp;
