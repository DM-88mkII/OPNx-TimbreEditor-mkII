



#include "pch.h"
#include "framework.h"
#include "FmStation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



BEGIN_MESSAGE_MAP(CFmStationApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()



CFmStationApp theApp;



CFmStationApp::CFmStationApp()
{
}



BOOL CFmStationApp::InitInstance()
{
	CWinApp::InitInstance();
	
	CShellManager *pShellManager = new CShellManager;
	
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
	
	SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));
	
	CFmStationDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK){
	} else if (nResponse == IDCANCEL){
	} else if (nResponse == -1){
		TRACE(traceAppMsg, 0, "警告: ダイアログの作成に失敗しました。アプリケーションは予期せずに終了します。\n");
		TRACE(traceAppMsg, 0, "警告: ダイアログで MFC コントロールを使用している場合、#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS を指定できません。\n");
	}
	
	if (pShellManager != nullptr){
		delete pShellManager;
	}
	
	#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)//[
	ControlBarCleanUp();
	#endif//]
	
	return FALSE;
}



int CFmStationApp::GetValue(CString Key, int Default)
{
	return GetProfileInt(_T("Settings"), Key, Default);
}



bool CFmStationApp::SetValue(CString Key, int Value)
{
	return WriteProfileInt(_T("Settings"), Key, Value);
}



CString CFmStationApp::GetString(CString Key, CString Default)
{
	return GetProfileString(_T("Settings"), Key, Default);
}



bool CFmStationApp::SetString(CString Key, CString String)
{
	return WriteProfileString(_T("Settings"), Key, String);
}
