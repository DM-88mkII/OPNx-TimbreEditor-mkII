



#include "pch.h"
#include "afxdialogex.h"
#include "FmStation.h"
#include "FmStationDlg.h"
#include "BatchDlg.h"



IMPLEMENT_DYNAMIC(CBatchDlg, CDialogEx)



CBatchDlg::~CBatchDlg()
{
	BatchCallback();
}



CBatchDlg::CBatchDlg(CWnd* pParent)
:CDialogEx(IDD_BATCH_DIALOG, pParent)
{
	m_Octave = 5;
	std::ranges::fill(m_aNote, -1);
}



void CBatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BATCHDLG_EDIT_NAME, m_CEditName);
	DDX_Control(pDX, IDC_BATCH_LOG, m_CStaticLog);
}



BEGIN_MESSAGE_MAP(CBatchDlg, CDialogEx)
	ON_COMMAND(IDOK, OnOK)
	ON_COMMAND(IDCANCEL, OnCancel)
	ON_WM_CLOSE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()



BOOL CBatchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();//call DoDataExchange()
	
	int width = 525;
	int height = 32;
	
	CRect DlgRect;
	GetWindowRect(&DlgRect);
	SetWindowPos(NULL, DlgRect.left, DlgRect.top, width, DlgRect.Height(), (SWP_NOZORDER | SWP_NOMOVE));
	
	m_CEditName.SetLimitText(32);
	
	CRect KeyboardRect;
	m_CKeyboard.SubclassDlgItem(IDC_BATCHDLG_STATIC_KEYBOARD, this);
	m_CKeyboard.GetWindowRect(&KeyboardRect); ScreenToClient(&KeyboardRect);
	m_CKeyboard.SetWindowPos(NULL, KeyboardRect.left, KeyboardRect.top, width, height, (SWP_NOZORDER | SWP_NOMOVE));
	m_CKeyboard.GetWindowRect(&KeyboardRect); ScreenToClient(&KeyboardRect);
	m_CKeyboard.ModifyStyle(0, SS_NOTIFY);
	m_CKeyboard.Initialize(width, height);
	m_CKeyboard.SetCallback(this);
	
	CRect Rect(0, 0, width, KeyboardRect.bottom);
	auto dwStyle = GetStyle();
	auto dwExStyle = GetExStyle();
	auto hasMenu = (GetMenu() != NULL);
	::AdjustWindowRectEx(&Rect, dwStyle, hasMenu, dwExStyle);
	
	GetWindowRect(&DlgRect);
	SetWindowPos(NULL, DlgRect.left, DlgRect.top, Rect.Width(), Rect.Height(), (SWP_NOZORDER | SWP_NOMOVE));
	
	return TRUE;
}



BOOL CBatchDlg::PreTranslateMessage(MSG* pMsg)
{
	auto pFocusWnd = GetFocus();
	if (pFocusWnd && pFocusWnd->IsKindOf(RUNTIME_CLASS(CEdit))){
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB){
			m_CKeyboard.SetFocus();
			return TRUE;
		}
		return CDialogEx::PreTranslateMessage(pMsg);
	}
	
	switch (pMsg->message){
		case WM_KEYDOWN:{
			auto bControl = (GetKeyState(VK_LCONTROL) < 0) | (GetKeyState(VK_RCONTROL) < 0);
			if (bControl){
				switch (pMsg->wParam){
					case 'W':{			theApp.GetFmStationDlg().WavRecord();	return TRUE;	}
					
					case VK_INSERT:{	PreviewUp();	return TRUE;	}
					case VK_DELETE:{	PreviewDown();	return TRUE;	}
				}
			} else {
				switch (pMsg->wParam){
					case VK_RETURN:{	return TRUE;	}
					case VK_ESCAPE:{	return TRUE;	}
					
					case VK_HOME:{		m_Octave += (m_Octave < 10)? 1: 0;	Log(_T("o{}"), m_Octave-1);	return TRUE;	}
					case VK_END:{		m_Octave -= (m_Octave >  0)? 1: 0;	Log(_T("o{}"), m_Octave-1);	return TRUE;	}
					
					case VK_INSERT:{	VolumeUp();		return TRUE;	}
					case VK_DELETE:{	VolumeDown();	return TRUE;	}
					
					case 'Z':{			Play(0, _T("c"));	return TRUE;	}
					case 'S':{			Play(1, _T("c+"));	return TRUE;	}
					case 'X':{			Play(2, _T("d"));	return TRUE;	}
					case 'D':{			Play(3, _T("d+"));	return TRUE;	}
					case 'C':{			Play(4, _T("e"));	return TRUE;	}
					case 'V':{			Play(5, _T("f"));	return TRUE;	}
					case 'G':{			Play(6, _T("f+"));	return TRUE;	}
					case 'B':{			Play(7, _T("g"));	return TRUE;	}
					case 'H':{			Play(8, _T("g+"));	return TRUE;	}
					case 'N':{			Play(9, _T("a"));	return TRUE;	}
					case 'J':{			Play(10, _T("a+"));	return TRUE;	}
					case 'M':{			Play(11, _T("b"));	return TRUE;	}
					case VK_OEM_COMMA:{	Play(12, _T("c"));	return TRUE;	}
					
					case VK_TAB:{		this->SetFocus();	return TRUE;	}
				}
			}
			break;
		}
		case WM_KEYUP:{
			{	// 
				switch (pMsg->wParam){
					case 'Z':{			Stop(0);	return TRUE;	}
					case 'S':{			Stop(1);	return TRUE;	}
					case 'X':{			Stop(2);	return TRUE;	}
					case 'D':{			Stop(3);	return TRUE;	}
					case 'C':{			Stop(4);	return TRUE;	}
					case 'V':{			Stop(5);	return TRUE;	}
					case 'G':{			Stop(6);	return TRUE;	}
					case 'B':{			Stop(7);	return TRUE;	}
					case 'H':{			Stop(8);	return TRUE;	}
					case 'N':{			Stop(9);	return TRUE;	}
					case 'J':{			Stop(10);	return TRUE;	}
					case 'M':{			Stop(11);	return TRUE;	}
					case VK_OEM_COMMA:{	Stop(12);	return TRUE;	}
				}
			}
			break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}



void CBatchDlg::OnOK()
{
	BatchCallback(true);
	DestroyWindow();
}



void CBatchDlg::OnCancel()
{
	BatchCallback();
	DestroyWindow();
}



void CBatchDlg::OnClose()
{
	BatchCallback(true);
	DestroyWindow();
}



void CBatchDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState, pWndOther, bMinimized);
	
	switch (nState){
		case WA_INACTIVE:{
			for (int Key = 0; Key <= 12; Key++) Stop(Key);
			break;
		}
		case WA_ACTIVE:
		case WA_CLICKACTIVE:
		{
			break;
		}
	}
}



void CBatchDlg::NoteOn(int MidiNote)
{
	if (m_pBatch){
		auto MidiChannel = theApp.GetFmStationDlg().GetPreview();
		m_pBatch->MidiNoteOn(MidiChannel, MidiNote, 127);
	}
}



void CBatchDlg::NoteOff(int MidiNote)
{
	if (m_pBatch){
		auto MidiChannel = theApp.GetFmStationDlg().GetPreview();
		m_pBatch->MidiNoteOff(MidiChannel, MidiNote, 0);
	}
}



void CBatchDlg::ResetKeyboard()
{
	if (!::IsWindow(m_CKeyboard.GetSafeHwnd())) return;
	m_CKeyboard.Reset();
	m_CKeyboard.Invalidate();
}



void CBatchDlg::UpdateKeyboard(int MidiNote, bool bNoteState)
{
	if (!::IsWindow(m_CKeyboard.GetSafeHwnd())) return;
	m_CKeyboard.UpdateKeyboard(MidiNote, bNoteState);
	m_CKeyboard.Invalidate();
}



void CBatchDlg::BatchCallback(bool bResult)
{
	if (m_pBatch){
		m_pBatch->DlgCallback(bResult);
		m_pBatch.reset();
	}
}



CFmStationDlg& CBatchDlg::GetFmStationDlg()
{
	return theApp.GetFmStationDlg();
}



CSettingTab& CBatchDlg::GetSettingTab()
{
	return GetFmStationDlg().GetSettingTab();
}



void CBatchDlg::SetBatch(sptr<CBatch> pBatch)
{
	m_pBatch = pBatch;
	if (::IsWindow(m_CEditName.GetSafeHwnd())){
		m_CEditName.SetWindowText(m_pBatch->Name);
		m_CKeyboard.SetFocus();//m_CEditName.SetSel(0, -1);
		
		SetWindowText(m_pBatch->Name);
	}
}



void CBatchDlg::GetName(CString& Name)
{
	if (::IsWindow(m_CEditName.GetSafeHwnd())){
		m_CEditName.GetWindowText(Name);
	}
}



template<class... Args> void CBatchDlg::Log(std::wformat_string<Args...> fmt, Args&& ... args)
{
	m_CStaticLog.SetWindowText((LPCTSTR)std::format(fmt, std::forward<Args>(args)...).c_str());
}



void CBatchDlg::Play(int Key, CString KeyName)
{
	auto NewNote = Key + (m_Octave * 12);
	Log(_T("o{}{}({})"), ((NewNote / 12) - 1), (LPCTSTR)KeyName, NewNote);
	if (m_aNote[Key] != NewNote){
		Stop(Key);
		
		m_aNote[Key] = NewNote;
		
		NoteOn(NewNote);
		m_CKeyboard.UpdateKeyboard(NewNote, true);
		m_CKeyboard.Invalidate();
	}
}



void CBatchDlg::Stop(int Key)
{
	auto OldNote = m_aNote[Key];
	if (OldNote >= 0){
		m_aNote[Key] = -1;
		
		NoteOff(OldNote);
		m_CKeyboard.UpdateKeyboard(OldNote, false);
		m_CKeyboard.Invalidate();
	}
}



void CBatchDlg::VolumeUp()
{
	GetFmStationDlg().VolumeUp();
	
	auto& rCSettingTab = GetSettingTab();
	Log(_T("Volume x{:1.1f}"), rCSettingTab.GetVolume() * 0.1);
}



void CBatchDlg::VolumeDown()
{
	GetFmStationDlg().VolumeDown();
	
	auto& rCSettingTab = GetSettingTab();
	Log(_T("Volume x{:1.1f}"), rCSettingTab.GetVolume() * 0.1);
}



void CBatchDlg::PreviewUp()
{
	GetFmStationDlg().PreviewUp();
	
	auto& rCSettingTab = GetSettingTab();
	Log(_T("Preview ch.{}"), rCSettingTab.GetPreview() + 1);
}



void CBatchDlg::PreviewDown()
{
	GetFmStationDlg().PreviewDown();
	
	auto& rCSettingTab = GetSettingTab();
	Log(_T("Preview ch.{}"), rCSettingTab.GetPreview() + 1);
}



// CBatch

CBatch::~CBatch()
{
	DlgCancel();
}



void CBatch::DlgOK()
{
	if (pBatchDlg && ::IsWindow(pBatchDlg->GetSafeHwnd())){
		pBatchDlg->OnOK();
	}
}



void CBatch::DlgCancel()
{
	if (pBatchDlg && ::IsWindow(pBatchDlg->GetSafeHwnd())){
		pBatchDlg->OnCancel();
	}
}



void CBatch::DlgCloase()
{
	if (pBatchDlg && ::IsWindow(pBatchDlg->GetSafeHwnd())){
		pBatchDlg->OnClose();
	}
}



void CBatch::DlgCallback(bool bResult)
{
	if (pBatchDlg){
		if (bResult){
			pBatchDlg->GetName(Name);
			pUpdateBatchTab->UpdateBatch(this);
		}
		pBatchDlg.release();
	}
}



void CBatch::MidiNoteOn(i8 MidiChannel, i8 MidiNote, int MidiVelocity)
{
	auto& FmStationDlg = theApp.GetFmStationDlg();
	for (auto& pTimbre : apTimbre){
		if (pTimbre->MidiNoteOn(MidiChannel, MidiNote, MidiVelocity)){
			FmStationDlg.MidiNoteOn(MidiChannel, MidiNote, MidiVelocity, pTimbre);
			if (pBatchDlg) pBatchDlg->UpdateKeyboard(MidiNote, true);
		}
	}
}



void CBatch::MidiNoteOff(i8 MidiChannel, i8 MidiNote, int MidiVelocity)
{
	auto& FmStationDlg = theApp.GetFmStationDlg();
	for (auto& pTimbre : apTimbre){
		if (pTimbre->MidiNoteOff(MidiChannel, MidiNote, MidiVelocity)){
			FmStationDlg.MidiNoteOff(MidiChannel, MidiNote, MidiVelocity, pTimbre);
			if (pBatchDlg) pBatchDlg->UpdateKeyboard(MidiNote, false);
		}
	}
}



void CBatch::Load(nlohmann::json& jBatch)
{
	int iTimbre = 0;
	for (auto& jTimbre : jBatch.at("Timbres")){
		auto& pTimbre = apTimbre[iTimbre];
		
		auto jIntermediate = jTimbre["Timbre"];
		auto v = jIntermediate.get<CIntermediate>();
		pTimbre->SetIntermediate(v);
		
		iTimbre++;
	}
}



nlohmann::json CBatch::Save()
{
	nlohmann::json jBatch;
	
	jBatch["Name"] = (LPCTSTR)Name;
	
	auto jTimbres = nlohmann::json::array();
	for (auto& pTimbre : apTimbre){
		nlohmann::json jTimbre;
		
		jTimbre["Name"] = (LPCTSTR)pTimbre->Name;
		
		nlohmann::json jIntermediate;
		auto Intermediate = pTimbre->GetIntermediate();
		Intermediate.to_json(jIntermediate);
		jTimbre["Timbre"] = jIntermediate;
		
		jTimbres.push_back(jTimbre);
	}
	jBatch["Timbres"] = jTimbres;
	
	return jBatch;
}
