



#include "pch.h"
#include "FmStation.h"
#include "afxdialogex.h"
#include "DrumTab.h"



IMPLEMENT_DYNAMIC(CDrumTab, CDialogEx)



CDrumTab::~CDrumTab()
{
}



CDrumTab::CDrumTab(CWnd* pParent)
:CDialogEx(IDD_DRUM_TAB, pParent)
{
	m_pDefaultBatch = std::make_shared<CBatch>();
	
	nlohmann::json aj[] = {
R"({
	"Information":{
		"ChipType":"YM2203",
		"Summary":"Timbre for OPN Series"
	},
	"Meta":{
		"Application":"OPNx-TimbreEditor",
		"Version":"0.0.0"
	},
	"Timbre":{
		"Control":{
			"ALG":2,
			"EN":1,
			"FB":4,
			"FDE":1,
			"FDT":0,
			"FIX":1,
			"KMH":128,
			"KML":0,
			"KT":108,
			"NUM":0,
			"SE":1
		},
		"Operators":{
			"AR":[21,20,22,22],
			"DR":[0,0,0,14],
			"DT":[0,0,0,0],
			"EN":[1,1,1,1],
			"FDE":[1,1,1,1],
			"KS":[0,0,0,0],
			"MT":[0,4,2,1],
			"RR":[0,0,0,8],
			"SE_FDT":[0,-7,0,0],
			"SE_FIX":[0,1,1,0],
			"SE_KT":[0,16,0,0],
			"SL":[0,0,0,2],
			"SR":[0,0,0,17],
			"SSG":[0,0,0,0],
			"TL":[2,9,17,28]
		}
	}
})"_json,
	};
	for (auto j: aj){
		CIntermediate v;
		v.from_json(j);
		auto pTimbre = std::make_shared<CTimbre>();
		pTimbre->SetIntermediate(v);
		m_pDefaultBatch->apTimbre.push_back(pTimbre);
	}
}



void CDrumTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DRUM_LIST_NOTE, m_CListBoxNote);
	DDX_Control(pDX, IDC_DRUM_LIST_BATCH, m_CListBoxBatch);
	DDX_Control(pDX, IDC_DRUM_LIST_TIMBRE, m_CListBoxTimbre);
	DDX_Control(pDX, IDC_DRUM_STATIC_TIMBRE, m_CStaticTimbre);
	DDX_Control(pDX, IDC_DRUM_SPIN_PROGRAM, m_CSpinButtonCtrlProgram);
	DDX_Control(pDX, IDC_DRUM_SPIN_BATCH_ORDER, m_CSpinButtonCtrlBatch);
	DDX_Control(pDX, IDC_DRUM_SPIN_TIMBRE_ORDER, m_CSpinButtonCtrlTimbre);
	DDX_Control(pDX, IDC_DRUM_EDIT_PROGRAM, m_CEditProgram);
}



BEGIN_MESSAGE_MAP(CDrumTab, CDialogEx)
	ON_BN_CLICKED(IDC_DRUM_BUTTON_NOTE_ADD, &CDrumTab::OnBnClickedDrumButtonNoteAdd)
	ON_BN_CLICKED(IDC_DRUM_BUTTON_NOTE_REMOVE, &CDrumTab::OnBnClickedDrumButtonNoteRemove)
	ON_BN_CLICKED(IDC_DRUM_BUTTON_BATCH_ADD, &CDrumTab::OnBnClickedDrumButtonBatchAdd)
	ON_BN_CLICKED(IDC_DRUM_BUTTON_BATCH_DELETE, &CDrumTab::OnBnClickedDrumButtonBatchDelete)
	ON_BN_CLICKED(IDC_DRUM_BUTTON_TIMBRE_ADD, &CDrumTab::OnBnClickedDrumButtonTimbreAdd)
	ON_BN_CLICKED(IDC_DRUM_BUTTON_TIMBRE_DELETE, &CDrumTab::OnBnClickedDrumButtonTimbreDelete)
	ON_LBN_SELCHANGE(IDC_DRUM_LIST_NOTE, &CDrumTab::OnLbnSelchangeDrumListNote)
	ON_LBN_SELCHANGE(IDC_DRUM_LIST_BATCH, &CDrumTab::OnLbnSelchangeDrumListBatch)
	ON_LBN_SELCHANGE(IDC_DRUM_LIST_TIMBRE, &CDrumTab::OnLbnSelchangeDrumListTimbre)
	ON_NOTIFY(UDN_DELTAPOS, IDC_DRUM_SPIN_PROGRAM, &CDrumTab::OnDeltaposDrumSpinProgram)
	ON_NOTIFY(UDN_DELTAPOS, IDC_DRUM_SPIN_BATCH_ORDER, &CDrumTab::OnDeltaposDrumSpinBatchOrder)
	ON_NOTIFY(UDN_DELTAPOS, IDC_DRUM_SPIN_TIMBRE_ORDER, &CDrumTab::OnDeltaposDrumSpinTimbreOrder)
	ON_LBN_DBLCLK(IDC_DRUM_LIST_NOTE, &CDrumTab::OnLbnDblclkDrumListNote)
	ON_LBN_DBLCLK(IDC_DRUM_LIST_BATCH, &CDrumTab::OnLbnDblclkDrumListBatch)
	ON_LBN_DBLCLK(IDC_DRUM_LIST_TIMBRE, &CDrumTab::OnLbnDblclkDrumListTimbre)
	ON_BN_CLICKED(IDC_DRUM_BUTTON_BATCH_DUP, &CDrumTab::OnBnClickedDrumButtonBatchDup)
	ON_BN_CLICKED(IDC_DRUM_BUTTON_TIMBRE_DUP, &CDrumTab::OnBnClickedDrumButtonTimbreDup)
END_MESSAGE_MAP()



BOOL CDrumTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();//call DoDataExchange()
	
	m_pNoteList = std::make_unique<CListBoxNote>(m_CListBoxNote);
	m_pBatchList = std::make_unique<CListBoxBatch>(m_CListBoxBatch);
	m_pTimbreList = std::make_unique<CListBoxTimbre>(m_CListBoxTimbre);
	
	auto& NoteList = *m_pNoteList;
	for (int iNote = 0; iNote < MidiNotes; iNote++){
		CString Prefix;
		CString String;
		Prefix.Format(_T("%3d : "), iNote);
		NoteList.AddPrefix(Prefix);
		NoteList.InsertString(iNote, String);
	}
	NoteList.SetCurSel(0);
	
	for (int iProgram = 1; iProgram < MidiPrograms; iProgram++){
		NoteList.AddPage(MidiNotes);
	}
	
	{	// 
		UDACCEL aACCEL[]={
			{/*sec*/0,/*step*/1},
			{/*sec*/1,/*step*/10},
		};
		m_CSpinButtonCtrlProgram.SetAccel(std::size(aACCEL), aACCEL);
		m_CSpinButtonCtrlProgram.SetRange(0, MidiPrograms-1);
	}
	
	{	// 
		UDACCEL aACCEL[]={
			{/*sec*/0,/*step*/1},
		};
		m_CSpinButtonCtrlBatch.SetAccel(std::size(aACCEL), aACCEL);
	}
	
	{	// 
		UDACCEL aACCEL[]={
			{/*sec*/0,/*step*/1},
		};
		m_CSpinButtonCtrlTimbre.SetAccel(std::size(aACCEL), aACCEL);
	}
	
	return FALSE;
}



BOOL CDrumTab::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message){
		case WM_KEYDOWN:{
			auto bControl = (GetKeyState(VK_LCONTROL) < 0) | (GetKeyState(VK_RCONTROL) < 0);
			if (bControl){
				switch (pMsg->wParam){
					case 'W':{			theApp.GetFmStationDlg().WavRecord();	return TRUE;	}
					
					case VK_INSERT:{	theApp.GetFmStationDlg().PreviewUp();	return TRUE;	}
					case VK_DELETE:{	theApp.GetFmStationDlg().PreviewDown();	return TRUE;	}
				}
			} else {
				switch (pMsg->wParam){
					case VK_RETURN:{
						auto pFocusCWnd = GetFocus();
						if (pFocusCWnd == &m_CListBoxNote) OnLbnDblclkDrumListNote();
						if (pFocusCWnd == &m_CListBoxBatch) OnLbnDblclkDrumListBatch();
						if (pFocusCWnd == &m_CListBoxTimbre) OnLbnDblclkDrumListTimbre();
						return TRUE;
					}
					case VK_ESCAPE:{
						return TRUE;
					}
					case VK_INSERT:{	GetFmStationDlg().VolumeUp();	return TRUE;	}
					case VK_DELETE:{	GetFmStationDlg().VolumeDown();	return TRUE;	}
				}
			}
			break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}



CFmStationDlg& CDrumTab::GetFmStationDlg()
{
	return theApp.GetFmStationDlg();
}



void CDrumTab::UpdateTimbre(int iBatch)
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	TimbreList.ResetContent();
	
	if (iBatch >= 0){
		auto pBatch = BatchList.GetDataPtr(iBatch);
		if (pBatch){
			auto& Batch = *pBatch;
			auto& apTimbre = Batch.apTimbre;
			
			int iTimbre = 0;
			for (auto pTimbre : apTimbre){
				TimbreList.AddString(pTimbre->Name);
				TimbreList.SetDataPtr(iTimbre, pTimbre);
				iTimbre++;
			}
			
			m_CStaticTimbre.SetWindowText(Batch.Name);
		}
	}
	
	TimbreList.SetCurSel(0);
}



sptr<CBatch> CDrumTab::GetProgram(int iProgram, int iNote)
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	sptr<CBatch> pBatch;
	pBatch = (pBatch.get() && pBatch->pUpdateBatchTab)? pBatch: NoteList.GetDataPtr(iProgram, iNote);
	pBatch = (pBatch.get() && pBatch->pUpdateBatchTab)? pBatch: NoteList.GetDataPtr(0x00, iNote);
	pBatch = (pBatch.get() && pBatch->pUpdateBatchTab)? pBatch: m_pDefaultBatch;
	return pBatch;
}



void CDrumTab::RemoveNote(int iNote)
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	CString String;
	NoteList.DeleteString(iNote);
	NoteList.InsertString(iNote, String);
	NoteList.SetCurSel(iNote);
}



void CDrumTab::AddNote(int iNote, int iBatch)
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto pBatch = BatchList.GetDataPtr(iBatch);
	//pBatch = (pBatch)? pBatch: m_pDefaultBatch;
	
	NoteList.DeleteString(iNote);
	NoteList.InsertString(iNote, pBatch->Name);
	NoteList.SetDataPtr(iNote, pBatch);
}



void CDrumTab::AddBatch(int iBatch, CString Name)
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	BatchList.InsertString(iBatch, Name);
	BatchList.SetCurSel(iBatch);
	BatchList.SetFocus();
	
	auto pBatch = std::make_shared<CBatch>(this, Name);
	BatchList.SetDataPtr(iBatch, pBatch);
}



void CDrumTab::AddTimbre(int iBatch, int iTimbre, CString Name)
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	TimbreList.InsertString(iTimbre, Name);
	TimbreList.SetCurSel(iTimbre);
	
	auto pTimbre = std::make_shared<CTimbre>(this, Name);
	TimbreList.SetDataPtr(iTimbre, pTimbre);
	
	auto& Batch = *BatchList.GetDataPtr(iBatch);
	auto& apTimbre = Batch.apTimbre;
	apTimbre.insert((apTimbre.begin() + iTimbre), pTimbre);
}



void CDrumTab::Load(nlohmann::json& j)
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	{	// 
		auto nBatch = BatchList.GetCount();
		for (int iBatch = 0; iBatch < nBatch; iBatch++){
			auto& Batch = *BatchList.GetDataPtr(iBatch);
			Batch.DlgCancel();
			
			for (auto& pTimbre : Batch.apTimbre){
				pTimbre->DlgCancel();
			}
		}
		
		NoteList.ResetContent();
		BatchList.ResetContent();
		TimbreList.ResetContent();
		
		for (int iNote = 0; iNote < MidiNotes; iNote++){
			CString String;
			NoteList.InsertString(iNote, String);
		}
	}
	
	{	// 
		nlohmann::json jDrum = j;
		
		{	// 
			int iBatch = 0;
			for (auto& jBatch : jDrum.at("Batchs")){
				CString Name(jBatch.at("Name").get<std::string>().c_str());
				//TRACE(_T("Batch[%d] %s\n"), iBatch, Name);
				AddBatch(iBatch, Name);
				
				int iTimbre = 0;
				for (auto& jTimbre : jBatch.at("Timbres")){
					CString Name(jTimbre.at("Name").get<std::string>().c_str());
					//TRACE(_T("Timbre[%d] %s\n"), iTimbre, Name);
					AddTimbre(iBatch, iTimbre, Name);
					
					iTimbre++;
				}
				
				auto& Batch = *BatchList.GetDataPtr(iBatch);
				Batch.Load(jBatch);
				
				BatchList.SetCurSel(iBatch);
				UpdateTimbre(iBatch);
				
				iBatch++;
			}
		}
		
		{	// 
			m_CEditProgram.SetWindowText(_T("0"));
			
			for (int iProgram = 0; iProgram < MidiPrograms; iProgram++){
				for (int iNote = 0; iNote < MidiNotes; iNote++){
					CString String;
					NoteList.SetText(iProgram, iNote, String);
					NoteList.SetDataPtr(iProgram, iNote, std::make_shared<CBatch>());
				}
			}
			
			for (auto& jProgram : jDrum.at("Programs").items()){
				int iProgram = std::stoi(jProgram.key().c_str());
				if (iProgram >= 0 && iProgram < MidiPrograms){
					int iNote = 0;
					for (auto& jNote : jProgram.value()){
						auto iBatch = jNote.get<int>();
						if (iBatch >= 0){
							auto pBatch = BatchList.GetDataPtr(iBatch);
							NoteList.SetText(iProgram, iNote, pBatch->Name);
							NoteList.SetDataPtr(iProgram, iNote, pBatch);
						}
						iNote++;
					}
				}
			}
		}
		
		{	// 
			NoteList.SetTopIndex(0);	NoteList.SetCurSel(0);
			BatchList.SetTopIndex(0);	BatchList.SetCurSel(0);		UpdateTimbre(0);
			TimbreList.SetTopIndex(0);	TimbreList.SetCurSel(0);
		}
	}
}



nlohmann::json CDrumTab::Save()
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	std::array<std::array<int, MidiNotes>, MidiPrograms> aaiProgram;
	for (auto& aiProgram: aaiProgram) std::ranges::fill(aiProgram, -1);
	
	for (int iProgram = 0; iProgram < MidiPrograms; iProgram++){
		for (int iNote = 0; iNote < MidiNotes; iNote++){
			auto pNoteBatch = NoteList.GetDataPtr(iProgram, iNote);
			auto iBatch = BatchList.FindDataPtr(-1, pNoteBatch);
			aaiProgram[iProgram][iNote] = iBatch;
		}
	}
	
	nlohmann::json jDrum;
	
	int iProgram = 0;
	auto jProgram = nlohmann::json::object();
	for (auto& aiProgram: aaiProgram){
		auto bSave = false;
		for (auto& iProgram: aiProgram){
			bSave |= (iProgram >= 0);
		}
		if (bSave){
			auto jNotes = nlohmann::json::array();
			for (auto& iProgram: aiProgram){
				jNotes.push_back(iProgram);
			}
			jProgram[std::to_string(iProgram)] = jNotes;
		}
		
		iProgram++;
	}
	jDrum["Programs"] = jProgram;
	
	auto jBatchs = nlohmann::json::array();
	auto nBatch = BatchList.GetCount();
	for (int iBatch = 0; iBatch < nBatch; iBatch++){
		auto pBatch = BatchList.GetDataPtr(iBatch);
		jBatchs.push_back(pBatch->Save());
	}
	jDrum["Batchs"] = jBatchs;
	
	return jDrum;
}



void CDrumTab::Reset()
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto nBatch = BatchList.GetCount();
	for (int iBatch = 0; iBatch < nBatch; iBatch++){
		auto pBatch = BatchList.GetDataPtr(iBatch);
		if (pBatch->pBatchDlg) pBatch->pBatchDlg->ResetKeyboard();
		
		for (auto& pTimbre : pBatch->apTimbre){
			if (pTimbre->pTimbreDlg) pTimbre->pTimbreDlg->ResetKeyboard();
		}
	}
}



void CDrumTab::UpdateBatch(CBatch* pBatch)
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iBatch = BatchList.FindDataPtr(-1, pBatch);
	if (iBatch >= 0){
		BatchList.SetText(iBatch, pBatch->Name);
		BatchList.SetCurSel(iBatch);
		
		auto nNote = NoteList.GetCount();
		for (int iNote = 0; iNote < nNote; iNote++){
			auto pNoteBatch = NoteList.GetDataPtr(iNote);
			if (pNoteBatch.get() == pBatch){
				NoteList.SetText(iNote, pBatch->Name);
			}
		}
	}
}



void CDrumTab::UpdateTimbre(CTimbre* pTimbre)
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iTimbre = TimbreList.FindDataPtr(-1, pTimbre);
	if (iTimbre >= 0){
		TimbreList.SetText(iTimbre, pTimbre->Name);
		TimbreList.SetCurSel(iTimbre);
	}
}



void CDrumTab::OnBnClickedDrumButtonNoteAdd()
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iBatch = BatchList.GetCurSel();
	if (iBatch >= 0){
		auto iNote = NoteList.GetCurSel();
		iNote = (iNote < 0)? 0: iNote;
		
		AddNote(iNote, iBatch);
		NoteList.SetTopIndex(iNote);
		NoteList.SetFocus();
		NoteList.SetCurSel(iNote);
	}
}



void CDrumTab::OnBnClickedDrumButtonNoteRemove()
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iNote = NoteList.GetCurSel();
	if (iNote >= 0) RemoveNote(iNote);
	
	OnLbnSelchangeDrumListNote();
}



void CDrumTab::OnBnClickedDrumButtonBatchAdd()
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iBatch = BatchList.GetCurSel();
	iBatch = (iBatch < 0)? 0: (iBatch + 1);
	
	CString Name;
	Name.Format(_T("Batch : %d"), rand());
	AddBatch(iBatch, Name);
	{	// 
		UpdateTimbre(iBatch);
		OnBnClickedDrumButtonTimbreAdd();
		UpdateTimbre(iBatch);
	}
	BatchList.SetTopIndex(iBatch);
	BatchList.SetCurSel(iBatch);
	BatchList.SetFocus();
}



void CDrumTab::OnBnClickedDrumButtonBatchDelete()
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iBatch = BatchList.GetCurSel();
	if (iBatch >= 0){
		auto pBatch = BatchList.GetDataPtr(iBatch);
		
		pBatch->DlgCancel();
		for (auto& pTimbre : pBatch->apTimbre){
			pTimbre->DlgCancel();
			
			auto& FmStationDlg = theApp.GetFmStationDlg();
			for (int MidiChannel = 0; MidiChannel < MidiChannels; MidiChannel++){
				for (int MidiNote = 0; MidiNote < MidiNotes; MidiNote++){
					while (pTimbre->MidiNoteOff(MidiChannel, MidiNote, 0)){
						FmStationDlg.MidiNoteOff(MidiChannel, MidiNote, 0, pTimbre);
					}
				}
			}
		}
		
		NoteList.Erase(pBatch);
		
		BatchList.DeleteString(iBatch);
		
		auto nBatch = BatchList.GetCount();
		iBatch = (iBatch < (nBatch - 1))? iBatch: (nBatch - 1);
		BatchList.SetCurSel(iBatch);
		BatchList.SetFocus();
		UpdateTimbre(iBatch);
	}
}



void CDrumTab::OnBnClickedDrumButtonTimbreAdd()
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iBatch = BatchList.GetCurSel();
	if (iBatch >= 0){
		auto iTimbre = TimbreList.GetCurSel();
		iTimbre = (iTimbre < 0)? 0: (iTimbre + 1);
		
		CString Name;
		Name.Format(_T("Timbre : %d"), rand());
		AddTimbre(iBatch, iTimbre, Name);
		TimbreList.SetTopIndex(iTimbre);
		TimbreList.SetCurSel(iTimbre);
		TimbreList.SetFocus();
	}
}



void CDrumTab::OnBnClickedDrumButtonTimbreDelete()
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iBatch = BatchList.GetCurSel();
	auto iTimbre = TimbreList.GetCurSel();
	if (iBatch >= 0 && iTimbre >= 0){
		auto& Batch = *BatchList.GetDataPtr(iBatch);
		auto pTimbre = Batch.apTimbre[iTimbre];
		
		pTimbre->DlgCancel();
		
		{	// 
			auto& FmStationDlg = theApp.GetFmStationDlg();
			for (int MidiChannel = 0; MidiChannel < MidiChannels; MidiChannel++){
				for (int MidiNote = 0; MidiNote < MidiNotes; MidiNote++){
					while (pTimbre->MidiNoteOff(MidiChannel, MidiNote, 0)){
						FmStationDlg.MidiNoteOff(MidiChannel, MidiNote, 0, pTimbre);
					}
				}
			}
		}
		
		auto& apTimbre = Batch.apTimbre;
		apTimbre.erase(apTimbre.begin() + iTimbre);
		
		TimbreList.DeleteString(iTimbre);
		
		auto nTimbre = TimbreList.GetCount();
		iTimbre = (iTimbre < (nTimbre - 1))? iTimbre: (nTimbre - 1);
		TimbreList.SetCurSel(iTimbre);
		TimbreList.SetFocus();
	}
}



void CDrumTab::OnLbnSelchangeDrumListNote()
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iNote = NoteList.GetCurSel();
	if (iNote >= 0){
		auto pNoteBatch = NoteList.GetDataPtr(iNote);
		auto iBatch = BatchList.FindDataPtr(-1, pNoteBatch);
		if (iBatch != LB_ERR){
			BatchList.SetCurSel(iBatch);
			UpdateTimbre(iBatch);
		}
	}
}



void CDrumTab::OnLbnSelchangeDrumListBatch()
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iBatch = BatchList.GetCurSel();
	UpdateTimbre(iBatch);
}



void CDrumTab::OnLbnSelchangeDrumListTimbre()
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iTimbre = TimbreList.GetCurSel();
}



void CDrumTab::OnDeltaposDrumSpinProgram(NMHDR* pNMHDR, LRESULT* pResult)
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	auto iProgram = m_CSpinButtonCtrlProgram.GetPos() + pNMUpDown->iDelta;
	NoteList.SetPage(iProgram);
	*pResult = 0;
}



void CDrumTab::OnDeltaposDrumSpinBatchOrder(NMHDR* pNMHDR, LRESULT* pResult)
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iBatch = BatchList.GetCurSel();
	auto nBatch = BatchList.GetCount();
	if (nBatch > 0){
		CString String;
		
		LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
		switch (pNMUpDown->iDelta){
			case -1:{//Up
				if (iBatch > 0){
					auto iInsert = iBatch - 1;
					
					auto pBatch = BatchList.GetDataPtr(iBatch);
					BatchList.GetText(iBatch, String);
					BatchList.DeleteString(iBatch);
					
					BatchList.InsertString(iInsert, String);
					BatchList.SetDataPtr(iInsert, pBatch);
					BatchList.SetCurSel(iInsert);
				}
				break;
			}
			case 1:{//Down
				if (iBatch < (nBatch - 1)){
					auto iInsert = iBatch + 1;
					
					auto pBatch = BatchList.GetDataPtr(iBatch);
					BatchList.GetText(iBatch, String);
					BatchList.DeleteString(iBatch);
					
					BatchList.InsertString(iInsert, String);
					BatchList.SetDataPtr(iInsert, pBatch);
					BatchList.SetCurSel(iInsert);
				}
				break;
			}
		}
	}
	*pResult = 0;
}



void CDrumTab::OnDeltaposDrumSpinTimbreOrder(NMHDR* pNMHDR, LRESULT* pResult)
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iBatch = BatchList.GetCurSel();
	auto iTimbre = TimbreList.GetCurSel();
	auto nTimbre = TimbreList.GetCount();
	if (iBatch >= 0 && nTimbre > 0){
		auto& Batch = *BatchList.GetDataPtr(iBatch);
		CString String;
		
		LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
		switch (pNMUpDown->iDelta){
			case -1:{//Up
				if (iTimbre > 0){
					auto iInsert = iTimbre - 1;
					
					{	// 
						auto pTimbre = TimbreList.GetDataPtr(iTimbre);
						TimbreList.GetText(iTimbre, String);
						TimbreList.DeleteString(iTimbre);
						
						TimbreList.InsertString(iInsert, String);
						TimbreList.SetDataPtr(iInsert, pTimbre);
						TimbreList.SetCurSel(iInsert);
					}
					
					{	// 
						auto& apTimbre = Batch.apTimbre;
						auto pTimbre = apTimbre[iTimbre];
						apTimbre.erase(apTimbre.begin() + iTimbre);
						apTimbre.insert((apTimbre.begin() + iInsert), pTimbre);
					}
				}
				break;
			}
			case 1:{//Down
				if (iTimbre < (nTimbre - 1)){
					auto iInsert = iTimbre + 1;
					
					{	// 
						auto pTimbre = TimbreList.GetDataPtr(iTimbre);
						TimbreList.GetText(iTimbre, String);
						TimbreList.DeleteString(iTimbre);
						
						TimbreList.InsertString(iInsert, String);
						TimbreList.SetDataPtr(iInsert, pTimbre);
						TimbreList.SetCurSel(iInsert);
					}
					
					{	// 
						auto& apTimbre = Batch.apTimbre;
						auto pTimbre = apTimbre[iTimbre];
						apTimbre.erase(apTimbre.begin() + iTimbre);
						apTimbre.insert((apTimbre.begin() + iInsert), pTimbre);
					}
				}
				break;
			}
		}
	}
	*pResult = 0;
}



void CDrumTab::OnLbnDblclkDrumListNote()
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iNote = NoteList.GetCurSel();
	if (iNote >= 0){
		auto pNoteBatch = NoteList.GetDataPtr(iNote);
		auto& pBatchDlg = pNoteBatch->pBatchDlg;
		if (pBatchDlg){
			if (::IsWindow(pBatchDlg->m_hWnd)){
				if (pBatchDlg->IsIconic()) pBatchDlg->ShowWindow(SW_RESTORE);
				pBatchDlg->SetForegroundWindow();
			}
		} else {
			pBatchDlg = std::make_unique<CBatchDlg>();
			pBatchDlg->Create(IDD_BATCH_DIALOG);
			pBatchDlg->ShowWindow(SW_SHOW);
			pBatchDlg->SetBatch(pNoteBatch);
		}
	}
}



void CDrumTab::OnLbnDblclkDrumListBatch()
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iBatch = BatchList.GetCurSel();
	if (iBatch >= 0){
		auto pBatch = BatchList.GetDataPtr(iBatch);
		auto& pBatchDlg = pBatch->pBatchDlg;
		if (pBatchDlg){
			if (::IsWindow(pBatchDlg->m_hWnd)){
				if (pBatchDlg->IsIconic()) pBatchDlg->ShowWindow(SW_RESTORE);
				pBatchDlg->SetForegroundWindow();
			}
		} else {
			pBatchDlg = std::make_unique<CBatchDlg>();
			pBatchDlg->Create(IDD_BATCH_DIALOG);
			pBatchDlg->ShowWindow(SW_SHOW);
			pBatchDlg->SetBatch(pBatch);
		}
	}
}



void CDrumTab::OnLbnDblclkDrumListTimbre()
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iTimbre = TimbreList.GetCurSel();
	if (iTimbre >= 0){
		auto pTimbre = TimbreList.GetDataPtr(iTimbre);
		auto& pTimbreDlg = pTimbre->pTimbreDlg;
		if (pTimbreDlg){
			if (::IsWindow(pTimbreDlg->m_hWnd)){
				if (pTimbreDlg->IsIconic()) pTimbreDlg->ShowWindow(SW_RESTORE);
				pTimbreDlg->SetForegroundWindow();
			}
		} else {
			auto iBatch = BatchList.GetCurSel();
			auto& Batch = *BatchList.GetDataPtr(iBatch);
			
			pTimbreDlg = std::make_unique<CTimbreDlg>();
			pTimbreDlg->Create(IDD_TIMBRE_DIALOG);
			pTimbreDlg->ShowWindow(SW_SHOW);
			pTimbreDlg->SetTimbre(pTimbre, Batch.Name);
		}
	}
}



void CDrumTab::OnBnClickedDrumButtonBatchDup()
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iBatchSrc = BatchList.GetCurSel();
	if (iBatchSrc >= 0){
		OnBnClickedDrumButtonBatchAdd();
		OnBnClickedDrumButtonTimbreDelete();
		
		int iTimbreDst = 0;
		auto& BatchSrc = *BatchList.GetDataPtr(iBatchSrc);
		for (auto& pTimbreSrc : BatchSrc.apTimbre){
			OnBnClickedDrumButtonTimbreAdd();
			
			auto& TimbreSrc = *pTimbreSrc;
			auto& TimbreDst = *TimbreList.GetDataPtr(iTimbreDst);
			TimbreDst.SetIntermediate(TimbreSrc.GetIntermediate());
			
			iTimbreDst++;
		}
	}
}



void CDrumTab::OnBnClickedDrumButtonTimbreDup()
{
	auto& NoteList = *m_pNoteList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iTimbreSrc = TimbreList.GetCurSel();
	if (iTimbreSrc >= 0){
		OnBnClickedDrumButtonTimbreAdd();
		
		auto iTimbreDst = TimbreList.GetCurSel();
		auto& TimbreSrc = *TimbreList.GetDataPtr(iTimbreSrc);
		auto& TimbreDst = *TimbreList.GetDataPtr(iTimbreDst);
		TimbreDst.SetIntermediate(TimbreSrc.GetIntermediate());
	}
}
