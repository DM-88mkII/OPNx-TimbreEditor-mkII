



#include "pch.h"
#include "FmStation.h"
#include "afxdialogex.h"
#include "InstTab.h"



IMPLEMENT_DYNAMIC(CInstTab, CDialogEx)



CInstTab::~CInstTab()
{
}



CInstTab::CInstTab(CWnd* pParent)
:CDialogEx(IDD_INST_TAB, pParent)
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
			"FB":6,
			"FDE":1,
			"FDT":-2,
			"FIX":0,
			"KMH":47,
			"KML":0,
			"KT":12,
			"NUM":0,
			"SE":0
		},
		"Operators":{
			"AR":[25,24,26,28],
			"DR":[5,5,15,6],
			"DT":[7,2,2,3],
			"EN":[1,1,1,1],
			"FDE":[1,1,1,1],
			"KS":[2,2,2,2],
			"MT":[3,9,2,1],
			"RR":[2,2,3,5],
			"SE_FDT":[0,0,0,0],
			"SE_FIX":[0,0,0,0],
			"SE_KT":[0,0,0,0],
			"SL":[4,4,1,1],
			"SR":[0,1,1,5],
			"SSG":[0,0,0,0],
			"TL":[35,38,37,15]
		}
	}
})"_json,
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
			"FB":6,
			"FDE":1,
			"FDT":-1,
			"FIX":0,
			"KMH":56,
			"KML":48,
			"KT":12,
			"NUM":0,
			"SE":0
		},
		"Operators":{
			"AR":[21,20,24,25],
			"DR":[5,5,14,6],
			"DT":[7,1,1,2],
			"EN":[1,1,1,1],
			"FDE":[1,1,1,1],
			"KS":[2,2,2,2],
			"MT":[3,9,2,1],
			"RR":[2,2,3,5],
			"SE_FDT":[0,0,0,0],
			"SE_FIX":[0,0,0,0],
			"SE_KT":[0,0,0,0],
			"SL":[4,4,1,2],
			"SR":[0,1,1,5],
			"SSG":[0,0,0,0],
			"TL":[38,43,40,15]
		}
	}
})"_json,
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
			"FB":6,
			"FDE":1,
			"FDT":0,
			"FIX":0,
			"KMH":64,
			"KML":57,
			"KT":12,
			"NUM":0,
			"SE":0
		},
		"Operators":{
			"AR":[21,20,23,24],
			"DR":[5,5,13,5],
			"DT":[7,1,0,1],
			"EN":[1,1,1,1],
			"FDE":[1,1,1,1],
			"KS":[2,2,2,2],
			"MT":[3,7,2,1],
			"RR":[2,2,3,5],
			"SE_FDT":[0,0,0,0],
			"SE_FIX":[0,0,0,0],
			"SE_KT":[0,0,0,0],
			"SL":[4,4,1,3],
			"SR":[0,1,1,4],
			"SSG":[0,0,0,0],
			"TL":[42,46,41,16]
		}
	}
})"_json,
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
			"FB":6,
			"FDE":1,
			"FDT":0,
			"FIX":0,
			"KMH":71,
			"KML":65,
			"KT":12,
			"NUM":0,
			"SE":0
		},
		"Operators":{
			"AR":[22,21,23,24],
			"DR":[4,4,12,5],
			"DT":[6,2,2,3],
			"EN":[1,1,1,1],
			"FDE":[1,1,1,1],
			"KS":[2,2,2,2],
			"MT":[3,7,2,1],
			"RR":[2,2,3,5],
			"SE_FDT":[0,0,0,0],
			"SE_FIX":[0,0,0,0],
			"SE_KT":[0,0,0,0],
			"SL":[4,4,1,4],
			"SR":[0,1,1,4],
			"SSG":[0,0,0,0],
			"TL":[49,54,44,16]
		}
	}
})"_json,
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
			"ALG":3,
			"EN":1,
			"FB":6,
			"FDE":1,
			"FDT":0,
			"FIX":0,
			"KMH":80,
			"KML":72,
			"KT":0,
			"NUM":0,
			"SE":0
		},
		"Operators":{
			"AR":[20,22,21,23],
			"DR":[4,13,4,6],
			"DT":[2,2,5,3],
			"EN":[1,1,1,1],
			"FDE":[1,1,1,1],
			"KS":[2,2,2,2],
			"MT":[10,4,6,2],
			"RR":[2,3,2,5],
			"SE_FDT":[0,0,0,0],
			"SE_FIX":[0,0,0,0],
			"SE_KT":[0,0,0,0],
			"SL":[4,1,4,5],
			"SR":[2,2,1,5],
			"SSG":[0,0,0,0],
			"TL":[58,48,55,18]
		}
	}
})"_json,
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
			"ALG":3,
			"EN":1,
			"FB":6,
			"FDE":1,
			"FDT":1,
			"FIX":0,
			"KMH":88,
			"KML":81,
			"KT":0,
			"NUM":0,
			"SE":0
		},
		"Operators":{
			"AR":[20,22,21,23],
			"DR":[4,13,4,7],
			"DT":[2,2,6,3],
			"EN":[1,1,1,1],
			"FDE":[1,1,1,1],
			"KS":[2,2,2,2],
			"MT":[4,2,4,2],
			"RR":[2,3,2,5],
			"SE_FDT":[0,0,0,0],
			"SE_FIX":[0,0,0,0],
			"SE_KT":[0,0,0,0],
			"SL":[4,1,4,6],
			"SR":[2,2,1,6],
			"SSG":[0,0,0,0],
			"TL":[55,49,58,19]
		}
	}
})"_json,
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
			"ALG":3,
			"EN":1,
			"FB":6,
			"FDE":1,
			"FDT":2,
			"FIX":0,
			"KMH":127,
			"KML":89,
			"KT":0,
			"NUM":0,
			"SE":0
		},
		"Operators":{
			"AR":[19,21,20,23],
			"DR":[5,13,5,8],
			"DT":[2,2,7,3],
			"EN":[1,1,1,1],
			"FDE":[1,1,1,1],
			"KS":[2,2,2,2],
			"MT":[2,2,4,2],
			"RR":[2,3,2,5],
			"SE_FDT":[0,0,0,0],
			"SE_FIX":[0,0,0,0],
			"SE_KT":[0,0,0,0],
			"SL":[4,1,4,7],
			"SR":[3,3,2,7],
			"SSG":[0,0,0,0],
			"TL":[46,48,65,21]
		}
	}
})"_json,
	};
	for (auto j : aj){
		CIntermediate v;
		v.from_json(j);
		auto pTimbre = std::make_shared<CTimbre>();
		pTimbre->SetIntermediate(v);
		m_pDefaultBatch->apTimbre.push_back(pTimbre);
	}
}



void CInstTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INST_LIST_PROGRAM, m_CListBoxProgram);
	DDX_Control(pDX, IDC_INST_LIST_BATCH, m_CListBoxBatch);
	DDX_Control(pDX, IDC_INST_LIST_TIMBRE, m_CListBoxTimbre);
	DDX_Control(pDX, IDC_INST_STATIC_TIMBRE, m_CStaticTimbre);
	DDX_Control(pDX, IDC_INST_SPIN_BANK, m_CSpinButtonCtrlBank);
	DDX_Control(pDX, IDC_INST_SPIN_BATCH_ORDER, m_CSpinButtonCtrlBatch);
	DDX_Control(pDX, IDC_INST_SPIN_TIMBRE_ORDER, m_CSpinButtonCtrlTimbre);
	DDX_Control(pDX, IDC_INST_EDIT_BANK, m_CEditBank);
}



BEGIN_MESSAGE_MAP(CInstTab, CDialogEx)
	ON_BN_CLICKED(IDC_INST_BUTTON_PROGRAM_ADD, &CInstTab::OnBnClickedInstButtonProgramAdd)
	ON_BN_CLICKED(IDC_INST_BUTTON_PROGRAM_REMOVE, &CInstTab::OnBnClickedInstButtonProgramRemove)
	ON_BN_CLICKED(IDC_INST_BUTTON_BATCH_ADD, &CInstTab::OnBnClickedInstButtonBatchAdd)
	ON_BN_CLICKED(IDC_INST_BUTTON_BATCH_DELETE, &CInstTab::OnBnClickedInstButtonBatchDelete)
	ON_BN_CLICKED(IDC_INST_BUTTON_TIMBRE_ADD, &CInstTab::OnBnClickedInstButtonTimbreAdd)
	ON_BN_CLICKED(IDC_INST_BUTTON_TIMBRE_DELETE, &CInstTab::OnBnClickedInstButtonTimbreDelete)
	ON_LBN_SELCHANGE(IDC_INST_LIST_PROGRAM, &CInstTab::OnLbnSelchangeInstListProgram)
	ON_LBN_SELCHANGE(IDC_INST_LIST_BATCH, &CInstTab::OnLbnSelchangeInstListBatch)
	ON_LBN_SELCHANGE(IDC_INST_LIST_TIMBRE, &CInstTab::OnLbnSelchangeInstListTimbre)
	ON_NOTIFY(UDN_DELTAPOS, IDC_INST_SPIN_BANK, &CInstTab::OnDeltaposInstSpinBank)
	ON_NOTIFY(UDN_DELTAPOS, IDC_INST_SPIN_BATCH_ORDER, &CInstTab::OnDeltaposInstSpinBatchOrder)
	ON_NOTIFY(UDN_DELTAPOS, IDC_INST_SPIN_TIMBRE_ORDER, &CInstTab::OnDeltaposInstSpinTimbreOrder)
	ON_LBN_DBLCLK(IDC_INST_LIST_PROGRAM, &CInstTab::OnLbnDblclkInstListProgram)
	ON_LBN_DBLCLK(IDC_INST_LIST_BATCH, &CInstTab::OnLbnDblclkInstListBatch)
	ON_LBN_DBLCLK(IDC_INST_LIST_TIMBRE, &CInstTab::OnLbnDblclkInstListTimbre)
	ON_BN_CLICKED(IDC_INST_BUTTON_BATCH_DUP, &CInstTab::OnBnClickedInstButtonBatchDup)
	ON_BN_CLICKED(IDC_INST_BUTTON_TIMBRE_DUP, &CInstTab::OnBnClickedInstButtonTimbreDup)
END_MESSAGE_MAP()



BOOL CInstTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();//call DoDataExchange()
	
	m_pProgramList = std::make_unique<CListBoxProgram>(m_CListBoxProgram);
	m_pBatchList = std::make_unique<CListBoxBatch>(m_CListBoxBatch);
	m_pTimbreList = std::make_unique<CListBoxTimbre>(m_CListBoxTimbre);
	
	auto& ProgramList = *m_pProgramList;
	for (int iProgram = 0; iProgram < MidiPrograms; iProgram++){
		CString Prefix;
		CString String;
		Prefix.Format(_T("%3d : "), (iProgram + 1));
		ProgramList.AddPrefix(Prefix);
		ProgramList.InsertString(iProgram, String);
	}
	ProgramList.SetCurSel(0);
	
	for (int iBank = 1; iBank < MidiBanks; iBank++){
		ProgramList.AddPage(MidiNotes);
	}
	
	{	// 
		UDACCEL aACCEL[]={
			{/*sec*/0,/*step*/1},
			{/*sec*/1,/*step*/10},
		};
		m_CSpinButtonCtrlBank.SetAccel(std::size(aACCEL), aACCEL);
		m_CSpinButtonCtrlBank.SetRange(0, MidiBanks-1);
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



BOOL CInstTab::PreTranslateMessage(MSG* pMsg)
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
						if (pFocusCWnd == &m_CListBoxProgram) OnLbnDblclkInstListProgram();
						if (pFocusCWnd == &m_CListBoxBatch) OnLbnDblclkInstListBatch();
						if (pFocusCWnd == &m_CListBoxTimbre) OnLbnDblclkInstListTimbre();
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



CFmStationDlg& CInstTab::GetFmStationDlg()
{
	return theApp.GetFmStationDlg();
}



void CInstTab::UpdateTimbre(int iBatch)
{
	auto& ProgramList = *m_pProgramList;
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



sptr<CBatch> CInstTab::GetProgram(int iBank, int iProgram)
{
	auto& ProgramList = *m_pProgramList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	sptr<CBatch> pBatch;
	pBatch = (pBatch.get() && pBatch->pUpdateBatchTab)? pBatch: ProgramList.GetDataPtr((iBank & 0x7f), iProgram);
	pBatch = (pBatch.get() && pBatch->pUpdateBatchTab)? pBatch: ProgramList.GetDataPtr(0x00, iProgram);
	pBatch = (pBatch.get() && pBatch->pUpdateBatchTab)? pBatch: m_pDefaultBatch;
	return pBatch;
}



void CInstTab::RemoveProgram(int iProgram)
{
	auto& ProgramList = *m_pProgramList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	CString String;
	ProgramList.DeleteString(iProgram);
	ProgramList.InsertString(iProgram, String);
	ProgramList.SetCurSel(iProgram);
}



void CInstTab::AddProgram(int iProgram, int iBatch)
{
	auto& ProgramList = *m_pProgramList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto pBatch = BatchList.GetDataPtr(iBatch);
	//pBatch = (pBatch)? pBatch: m_pDefaultBatch;
	
	ProgramList.DeleteString(iProgram);
	ProgramList.InsertString(iProgram, pBatch->Name);
	ProgramList.SetDataPtr(iProgram, pBatch);
}



void CInstTab::AddBatch(int iBatch, CString Name)
{
	auto& ProgramList = *m_pProgramList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	BatchList.InsertString(iBatch, Name);
	BatchList.SetCurSel(iBatch);
	BatchList.SetFocus();
	
	auto pBatch = std::make_shared<CBatch>(this, Name);
	BatchList.SetDataPtr(iBatch, pBatch);
}



void CInstTab::AddTimbre(int iBatch, int iTimbre, CString Name)
{
	auto& ProgramList = *m_pProgramList;
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



void CInstTab::Load(nlohmann::json& j)
{
	auto& ProgramList = *m_pProgramList;
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
		
		ProgramList.ResetContent();
		BatchList.ResetContent();
		TimbreList.ResetContent();
		
		for (int iProgram = 0; iProgram < MidiPrograms; iProgram++){
			CString String;
			ProgramList.InsertString(iProgram, String);
		}
	}
	
	{	// 
		nlohmann::json jInst = j;
		
		{	// 
			int iBatch = 0;
			for (auto& jBatch : jInst.at("Batchs")){
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
			m_CEditBank.SetWindowText(_T("0"));
			
			for (int iBank = 0; iBank < MidiBanks; iBank++){
				for (int iProgram = 0; iProgram < MidiPrograms; iProgram++){
					CString String;
					ProgramList.SetText(iBank, iProgram, String);
					ProgramList.SetDataPtr(iBank, iProgram, std::make_shared<CBatch>());
				}
			}
			
			for (auto& jBank : jInst.at("Banks").items()){
				int iBank = std::stoi(jBank.key().c_str());
				if (iBank >= 0 && iBank < MidiBanks){
					int iProgram = 0;
					for (auto& jProgram : jBank.value()){
						auto iBatch = jProgram.get<int>();
						if (iBatch >= 0){
							auto pBatch = BatchList.GetDataPtr(iBatch);
							ProgramList.SetText(iBank, iProgram, pBatch->Name);
							ProgramList.SetDataPtr(iBank, iProgram, pBatch);
						}
						iProgram++;
					}
				}
			}
		}
		
		{	// 
			ProgramList.SetTopIndex(0);	ProgramList.SetCurSel(0);
			BatchList.SetTopIndex(0);	BatchList.SetCurSel(0);		UpdateTimbre(0);
			TimbreList.SetTopIndex(0);	TimbreList.SetCurSel(0);
		}
	}
}



nlohmann::json CInstTab::Save()
{
	auto& ProgramList = *m_pProgramList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	std::array<std::array<int, MidiPrograms>, MidiBanks> aaiBank;
	for (auto& aiBank: aaiBank) std::ranges::fill(aiBank, -1);
	
	for (int iBank = 0; iBank < MidiBanks; iBank++){
		for (int iProgram = 0; iProgram < MidiPrograms; iProgram++){
			auto pProgramBatch = ProgramList.GetDataPtr(iBank, iProgram);
			auto iBatch = BatchList.FindDataPtr(-1, pProgramBatch);
			aaiBank[iBank][iProgram] = iBatch;
		}
	}
	
	nlohmann::json jInst;
	
	int iBank = 0;
	auto jBank = nlohmann::json::object();
	for (auto& aiBank: aaiBank){
		auto bSave = false;
		for (auto& iBank: aiBank){
			bSave |= (iBank >= 0);
		}
		if (bSave){
			auto jPrograms = nlohmann::json::array();
			for (auto& iBank: aiBank){
				jPrograms.push_back(iBank);
			}
			jBank[std::to_string(iBank)] = jPrograms;
		}
		
		iBank++;
	}
	jInst["Banks"] = jBank;
	
	auto jBatchs = nlohmann::json::array();
	auto nBatch = BatchList.GetCount();
	for (int iBatch = 0; iBatch < nBatch; iBatch++){
		auto pBatch = BatchList.GetDataPtr(iBatch);
		jBatchs.push_back(pBatch->Save());
	}
	jInst["Batchs"] = jBatchs;
	
	return jInst;
}



void CInstTab::Reset()
{
	auto& ProgramList = *m_pProgramList;
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



void CInstTab::UpdateBatch(CBatch* pBatch)
{
	auto& ProgramList = *m_pProgramList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iBatch = BatchList.FindDataPtr(-1, pBatch);
	if (iBatch >= 0){
		BatchList.SetText(iBatch, pBatch->Name);
		BatchList.SetCurSel(iBatch);
		
		auto nProgram = ProgramList.GetCount();
		for (int iProgram = 0; iProgram < nProgram; iProgram++){
			auto pProgramBatch = ProgramList.GetDataPtr(iProgram);
			if (pProgramBatch.get() == pBatch){
				ProgramList.SetText(iProgram, pBatch->Name);
			}
		}
	}
}



void CInstTab::UpdateTimbre(CTimbre* pTimbre)
{
	auto& ProgramList = *m_pProgramList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iTimbre = TimbreList.FindDataPtr(-1, pTimbre);
	if (iTimbre >= 0){
		TimbreList.SetText(iTimbre, pTimbre->Name);
		TimbreList.SetCurSel(iTimbre);
	}
}



void CInstTab::OnBnClickedInstButtonProgramAdd()
{
	auto& ProgramList = *m_pProgramList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iBatch = BatchList.GetCurSel();
	if (iBatch >= 0){
		auto iProgram = ProgramList.GetCurSel();
		iProgram = (iProgram < 0)? 0: iProgram;
		
		AddProgram(iProgram, iBatch);
		ProgramList.SetTopIndex(iProgram);
		ProgramList.SetFocus();
		ProgramList.SetCurSel(iProgram);
	}
}



void CInstTab::OnBnClickedInstButtonProgramRemove()
{
	auto& ProgramList = *m_pProgramList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iProgram = ProgramList.GetCurSel();
	if (iProgram >= 0) RemoveProgram(iProgram);
	
	OnLbnSelchangeInstListProgram();
}



void CInstTab::OnBnClickedInstButtonBatchAdd()
{
	auto& ProgramList = *m_pProgramList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iBatch = BatchList.GetCurSel();
	iBatch = (iBatch < 0)? 0: (iBatch + 1);
	
	CString Name;
	Name.Format(_T("Batch : %d"), rand());
	AddBatch(iBatch, Name);
	{	// 
		UpdateTimbre(iBatch);
		OnBnClickedInstButtonTimbreAdd();
		UpdateTimbre(iBatch);
	}
	BatchList.SetTopIndex(iBatch);
	BatchList.SetCurSel(iBatch);
	BatchList.SetFocus();
}



void CInstTab::OnBnClickedInstButtonBatchDelete()
{
	auto& ProgramList = *m_pProgramList;
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
		
		ProgramList.Erase(pBatch);
		
		BatchList.DeleteString(iBatch);
		
		auto nBatch = BatchList.GetCount();
		iBatch = (iBatch < (nBatch - 1))? iBatch: (nBatch - 1);
		BatchList.SetCurSel(iBatch);
		BatchList.SetFocus();
		UpdateTimbre(iBatch);
	}
}



void CInstTab::OnBnClickedInstButtonTimbreAdd()
{
	auto& ProgramList = *m_pProgramList;
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



void CInstTab::OnBnClickedInstButtonTimbreDelete()
{
	auto& ProgramList = *m_pProgramList;
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



void CInstTab::OnLbnSelchangeInstListProgram()
{
	auto& ProgramList = *m_pProgramList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iProgram = ProgramList.GetCurSel();
	if (iProgram >= 0){
		auto pProgramBatch = ProgramList.GetDataPtr(iProgram);
		auto iBatch = BatchList.FindDataPtr(-1, pProgramBatch);
		if (iBatch != LB_ERR){
			BatchList.SetCurSel(iBatch);
			UpdateTimbre(iBatch);
		}
	}
}



void CInstTab::OnLbnSelchangeInstListBatch()
{
	auto& ProgramList = *m_pProgramList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iBatch = BatchList.GetCurSel();
	UpdateTimbre(iBatch);
}



void CInstTab::OnLbnSelchangeInstListTimbre()
{
	auto& ProgramList = *m_pProgramList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iTimbre = TimbreList.GetCurSel();
}



void CInstTab::OnDeltaposInstSpinBank(NMHDR* pNMHDR, LRESULT* pResult)
{
	auto& ProgramList = *m_pProgramList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	auto iBank = m_CSpinButtonCtrlBank.GetPos() + pNMUpDown->iDelta;
	ProgramList.SetPage(iBank);
	*pResult = 0;
}



void CInstTab::OnDeltaposInstSpinBatchOrder(NMHDR* pNMHDR, LRESULT* pResult)
{
	auto& ProgramList = *m_pProgramList;
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



void CInstTab::OnDeltaposInstSpinTimbreOrder(NMHDR* pNMHDR, LRESULT* pResult)
{
	auto& ProgramList = *m_pProgramList;
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



void CInstTab::OnLbnDblclkInstListProgram()
{
	auto& ProgramList = *m_pProgramList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iProgram = ProgramList.GetCurSel();
	if (iProgram >= 0){
		auto pProgramBatch = ProgramList.GetDataPtr(iProgram);
		auto& pBatchDlg = pProgramBatch->pBatchDlg;
		if (pBatchDlg){
			if (::IsWindow(pBatchDlg->m_hWnd)){
				if (pBatchDlg->IsIconic()) pBatchDlg->ShowWindow(SW_RESTORE);
				pBatchDlg->SetForegroundWindow();
			}
		} else {
			pBatchDlg = std::make_unique<CBatchDlg>();
			pBatchDlg->Create(IDD_BATCH_DIALOG);
			pBatchDlg->ShowWindow(SW_SHOW);
			pBatchDlg->SetBatch(pProgramBatch);
		}
	}
}



void CInstTab::OnLbnDblclkInstListBatch()
{
	auto& ProgramList = *m_pProgramList;
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



void CInstTab::OnLbnDblclkInstListTimbre()
{
	auto& ProgramList = *m_pProgramList;
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



void CInstTab::OnBnClickedInstButtonBatchDup()
{
	auto& ProgramList = *m_pProgramList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iBatchSrc = BatchList.GetCurSel();
	if (iBatchSrc >= 0){
		OnBnClickedInstButtonBatchAdd();
		OnBnClickedInstButtonTimbreDelete();
		
		int iTimbreDst = 0;
		auto& BatchSrc = *BatchList.GetDataPtr(iBatchSrc);
		for (auto& pTimbreSrc : BatchSrc.apTimbre){
			OnBnClickedInstButtonTimbreAdd();
			
			auto& TimbreSrc = *pTimbreSrc;
			auto& TimbreDst = *TimbreList.GetDataPtr(iTimbreDst);
			TimbreDst.SetIntermediate(TimbreSrc.GetIntermediate());
			
			iTimbreDst++;
		}
	}
}



void CInstTab::OnBnClickedInstButtonTimbreDup()
{
	auto& ProgramList = *m_pProgramList;
	auto& BatchList = *m_pBatchList;
	auto& TimbreList = *m_pTimbreList;
	
	auto iTimbreSrc = TimbreList.GetCurSel();
	if (iTimbreSrc >= 0){
		OnBnClickedInstButtonTimbreAdd();
		
		auto iTimbreDst = TimbreList.GetCurSel();
		auto& TimbreSrc = *TimbreList.GetDataPtr(iTimbreSrc);
		auto& TimbreDst = *TimbreList.GetDataPtr(iTimbreDst);
		TimbreDst.SetIntermediate(TimbreSrc.GetIntermediate());
	}
}
