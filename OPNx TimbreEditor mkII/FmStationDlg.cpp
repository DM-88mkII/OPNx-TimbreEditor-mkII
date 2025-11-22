



#include "pch.h"
#include "afxdialogex.h"
#include "framework.h"
#include "FmStation.h"
#include "FmStationDlg.h"
#include "ChipYM2608.h"
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CFmStationDlg::~CFmStationDlg()
{
	MidiInClose();
	
	m_pVoice.reset();
	m_pAudio.reset();
}



CFmStationDlg::CFmStationDlg(CWnd* pParent /*=nullptr*/)
:CDialogEx(IDD_FMSTATION_DIALOG, pParent)
,m_hMidiIn{}
,m_NoteOnId(0)
,m_aMidiChannelInfo{}
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	m_MidiHdr.lpData = m_aSysExBuffer;
	m_MidiHdr.dwBufferLength = std::size(m_aSysExBuffer);
	m_MidiHdr.dwFlags = 0;
}



void CFmStationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DEVICE, m_CComboBoxDevice);
	DDX_Control(pDX, IDC_MASTER_TAB, m_CTabCtrl);
	DDX_Control(pDX, IDC_MASTER_STATIC_LOG, m_CStaticLog);
}



BEGIN_MESSAGE_MAP(CFmStationDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICE, &CFmStationDlg::OnCbnSelchangeComboDevice)
	ON_NOTIFY(TCN_SELCHANGE, IDC_MASTER_TAB, &CFmStationDlg::OnSelchangeTabcontrol)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CFmStationDlg::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CFmStationDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CFmStationDlg::OnBnClickedButtonSave)
END_MESSAGE_MAP()



BOOL CFmStationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();//call DoDataExchange()
	
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	
	{	// 
		m_pAudio = std::make_unique<CAudio>();
		m_pVoice = std::make_unique<CVoice>(m_pAudio->GetXAudio2(), 2, 55555);
		
		auto RenderSize = m_pVoice->GetRenderSize();
		for (i8 ChipId = 0; ChipId < ChipNum; ChipId++){
			m_pVoice->AddChip(std::make_unique<CChipYM2608>(55555, RenderSize));
		}
		
		m_pVoice->Start();
	}
	
	{	// 
		m_CTabCtrl.InsertItem(0, _T("Inst"));
		m_CTabCtrl.InsertItem(1, _T("Drum"));
		m_CTabCtrl.InsertItem(2, _T("MIDI"));
		m_CTabCtrl.InsertItem(3, _T("Module"));
		m_CTabCtrl.InsertItem(4, _T("Setting"));
		
		m_CInstTab.Create(IDD_INST_TAB, this);
		m_CDrumTab.Create(IDD_DRUM_TAB, this);
		m_CMidiTab.Create(IDD_MIDI_TAB, this);
		m_CModuleTab.Create(IDD_MODULE_TAB, this);
		m_CSettingTab.Create(IDD_SETTING_TAB, this);
		
		CRect Rect;
		m_CTabCtrl.GetItemRect(0, &Rect);
		m_CInstTab.SetWindowPos(NULL, Rect.left, Rect.bottom, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_CDrumTab.SetWindowPos(NULL, Rect.left, Rect.bottom, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_CMidiTab.SetWindowPos(NULL, Rect.left, Rect.bottom, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_CModuleTab.SetWindowPos(NULL, Rect.left, Rect.bottom, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_CSettingTab.SetWindowPos(NULL, Rect.left, Rect.bottom, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		
		m_CTabCtrl.SetCurFocus(0);
		OnSelchangeTabcontrol(NULL, NULL);
		
		m_CInstTab.SetFocus();
		//m_CDrumTab.SetFocus();
		//m_CMidiTab.SetFocus();
		//m_CModuleTab.SetFocus();
		//m_CSettingTab.SetFocus();
		
		{	// 
			CRect Rect;
			m_CComboBoxDevice.GetDroppedControlRect(&Rect);
			Rect.bottom += 1000;
			m_CComboBoxDevice.GetParent()->ScreenToClient(&Rect);
			m_CComboBoxDevice.MoveWindow(&Rect);
			
			auto nDevice = midiInGetNumDevs();
			for (UINT iDevice = 0; iDevice < nDevice; iDevice++){
				MIDIINCAPS Caps = {0};
				midiInGetDevCaps(iDevice, &Caps, sizeof(Caps));
				m_CComboBoxDevice.AddString(Caps.szPname);
			}
			m_CComboBoxDevice.SetCurSel(0);
			
			MidiInOpen();
		}
		
		SystemReset();
		
		m_pVoice->SetVolume(GetSettingTab().GetVolume());
	}
	
	{	// 
		auto Path = theApp.GetString(_T("LatestFile"), NULL);
		if (!Path.IsEmpty()) Load((LPCTSTR)Path);
	}
	
	{	// 
		CRect rect;
		GetWindowRect(&rect);
		MoveWindow(rect.left, rect.top, rect.Width(), rect.Height()+1, TRUE);
	}
	
	return TRUE;
}



BOOL CFmStationDlg::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message){
		case WM_KEYDOWN:{
			auto bControl = (GetKeyState(VK_LCONTROL) < 0) | (GetKeyState(VK_RCONTROL) < 0);
			if (bControl){
				switch (pMsg->wParam){
					case 'W':{			WavRecord();	return TRUE;	}
					
					case VK_INSERT:{	PreviewUp();	return TRUE;	}
					case VK_DELETE:{	PreviewDown();	return TRUE;	}
				}
			} else {
				switch (pMsg->wParam){
					case VK_RETURN:{	return TRUE;	}
					case VK_ESCAPE:{	return TRUE;	}
					
					case VK_INSERT:{	VolumeUp();		return TRUE;	}
					case VK_DELETE:{	VolumeDown();	return TRUE;	}
				}
			}
			break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}



void CFmStationDlg::OnPaint()
{
	if (IsIconic()){
		CPaintDC dc(this);
		
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		dc.DrawIcon(x, y, m_hIcon);
	} else {
		CDialogEx::OnPaint();
	}
}



HCURSOR CFmStationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFmStationDlg::OnCbnSelchangeComboDevice()
{
	MidiInClose();
	MidiInOpen();
}



void CFmStationDlg::OnSelchangeTabcontrol(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (pResult != NULL) *pResult = 0;
	
	auto iItem = m_CTabCtrl.GetCurSel();
	m_CInstTab.ShowWindow((iItem == 0)? SW_SHOW: SW_HIDE);
	m_CDrumTab.ShowWindow((iItem == 1)? SW_SHOW: SW_HIDE);
	m_CMidiTab.ShowWindow((iItem == 2)? SW_SHOW: SW_HIDE);
	m_CModuleTab.ShowWindow((iItem == 3)? SW_SHOW: SW_HIDE);
	m_CSettingTab.ShowWindow((iItem == 4)? SW_SHOW: SW_HIDE);
}



void CFmStationDlg::OnBnClickedButtonReset()
{
	SystemReset();
}



void CFmStationDlg::OnBnClickedButtonLoad()
{
	CFileDialog FileDialog(true, _T("opnx2"), _T("*.opnx2"), OFN_FILEMUSTEXIST | OFN_EXTENSIONDIFFERENT);
	if (FileDialog.DoModal() == IDOK){
		std::filesystem::path Path = (LPCTSTR)FileDialog.GetFileTitle();
		auto Extension = Path.extension();
		if (Extension.empty()) Path += ".opnx2";
		
		Load(Path);
	}
}



void CFmStationDlg::OnBnClickedButtonSave()
{
	CFileDialog FileDialog(false, _T("opnx2"), _T("*.opnx2"), OFN_OVERWRITEPROMPT | OFN_EXTENSIONDIFFERENT);
	if (FileDialog.DoModal() == IDOK){
		std::filesystem::path Path = (LPCTSTR)FileDialog.GetFileTitle();
		auto Extension = Path.extension();
		if (Extension.empty()) Path += ".opnx2";
		
		Save(Path);
	}
}



void CFmStationDlg::Load(std::filesystem::path Path)
{
	Path = std::filesystem::absolute(Path);
	
	std::ifstream File;
	try {
		nlohmann::json jLoad;
		File.open(Path.c_str(), std::ios::in);
		File >> jLoad;
		
		m_CInstTab.Load(jLoad.at("Inst"));
		m_CDrumTab.Load(jLoad.at("Drum"));
		
		theApp.SetString(_T("LatestFile"), Path.c_str());
		Log(_T("{}"), Path.filename().c_str());
	}
	catch (...){
		Log(_T("Load Error"));
		theApp.SetString(_T("LatestFile"), NULL);
	}
	File.close();
}



void CFmStationDlg::Save(std::filesystem::path Path)
{
	Path = std::filesystem::absolute(Path);
	
	std::ofstream File;
	try {
		nlohmann::json jSave;
		
		jSave["Inst"] = m_CInstTab.Save();
		jSave["Drum"] = m_CDrumTab.Save();
		
		File.open(Path.c_str(), std::ios::out);
		File << jSave << std::endl;
		
		theApp.SetString(_T("LatestFile"), Path.c_str());
		Log(_T("{}"), Path.filename().c_str());
	}
	catch (...){
		Log(_T("Save Error"));
	}
	File.close();
}



void CFmStationDlg::MidiInClose()
{
	midiInStop(m_hMidiIn);
	midiInClose(m_hMidiIn);
}



void CFmStationDlg::MidiInOpen()
{
	auto iDevice = m_CComboBoxDevice.GetCurSel();
	if (midiInOpen(&m_hMidiIn, iDevice, (DWORD_PTR)StaticMidiInProc, (DWORD_PTR)this, CALLBACK_FUNCTION) == MMSYSERR_NOERROR){
		midiInPrepareHeader(m_hMidiIn, &m_MidiHdr, sizeof(MIDIHDR));
		MidiInAddBuffer();
		
		midiInStart(m_hMidiIn);
	} else {
		TRACE(_T("midiInOpen : !!! Error !!!\n"));
	}
}



void CALLBACK CFmStationDlg::StaticMidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	auto& This = *(CFmStationDlg*)dwInstance;
	switch (wMsg){
		case MIM_DATA:{
			DWORD msg = (DWORD)dwParam1;
			
			BYTE status = msg & 0xff;
			BYTE data1 = (msg >> 8) & 0xff;
			BYTE data2 = (msg >> 16) & 0xff;
			
			This.MidiInProc(status, data1, data2);
			break;
		}
		case MIM_LONGDATA:{
			MIDIHDR* hdr = reinterpret_cast<MIDIHDR*>(dwParam1);
			if (hdr->dwBytesRecorded > 0){
				const unsigned char* data = reinterpret_cast<unsigned char*>(hdr->lpData);
				size_t len = hdr->dwBytesRecorded;
				
				if (len >= 11 &&
					data[0] == 0xf0 &&
					data[1] == 0x41 && data[2] == 0x10 && data[3] == 0x42 && data[4] == 0x12 &&// Roland GS ID
					data[5] == 0x40 && data[6] >= 0x10 && data[6] <= 0x1f && data[7] == 0x15 &&
					data[len-1] == 0xf7
					){
					int part = data[6] & 0x0f;
					int mode = data[8];
					TRACE(_T("GS : Part.%d : Mode.%d\n"), part, mode);
					
					const i8 aMidiChannel[]={9,0,1,2,3,4,5,6,7,8,10,11,12,13,14,15,};
					This.SetMidiChannelType(aMidiChannel[part], (mode == 0)? CMidiTab::EChannelType_Inst: CMidiTab::EChannelType_Drum);
				} else if (len >= 9 &&
					data[0] == 0xf0 &&
					data[1] == 0x43 && data[2] == 0x10 && data[3] == 0x4c &&// Yamaha XG ID
					data[4] == 0x08 && data[6] == 0x07 &&
					data[len-1] == 0xf7
					){
					int part = data[5];
					int mode = data[7];
					TRACE(_T("XG : Part.%d : Mode.%d\n"), part, mode);
					
					if (part <= 0x0f){
						const i8 aMidiChannel[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,};
						This.SetMidiChannelType(aMidiChannel[part], (mode == 0)? CMidiTab::EChannelType_Inst: CMidiTab::EChannelType_Drum);
					}
				} else {
					CString SysEx;
					for (int i = 0; i < len; i++){
						CString Data;
						Data.Format(_T("%02x "), data[i]);
						SysEx += Data;
					}
					TRACE(_T("SysEx : %s\n"), SysEx);
				}
				
				This.MidiInAddBuffer();
			}
			break;
		}
	}
}



void CFmStationDlg::MidiInProc(BYTE status, BYTE data1, BYTE data2)
{
	m_CMidiTab.MidiInProc(this, status, data1, data2);
}



void CFmStationDlg::SystemReset()
{
	TRACE(_T("SystemReset\n"));
	{	// 
		m_NoteOnId = 0;
		
		std::queue<ChipInfo>().swap(m_ChipOff);
		std::queue<ChipInfo>().swap(m_ChipOffSE);
		for (i8 ChipId = 0; ChipId < ChipNum; ChipId++){
			m_ChipOff.push(ChipInfo{ChipId, /*ChipChannel*/0, /*MidiChannel*/-1, /*MidiNote*/-1, 0});
			m_ChipOff.push(ChipInfo{ChipId, /*ChipChannel*/1, /*MidiChannel*/-1, /*MidiNote*/-1, 0});
			m_ChipOffSE.push(ChipInfo{ChipId, /*ChipChannel*/2, /*MidiChannel*/-1, /*MidiNote*/-1, 0});
			m_ChipOff.push(ChipInfo{ChipId, /*ChipChannel*/3, /*MidiChannel*/-1, /*MidiNote*/-1, 0});
			m_ChipOff.push(ChipInfo{ChipId, /*ChipChannel*/4, /*MidiChannel*/-1, /*MidiNote*/-1, 0});
			m_ChipOff.push(ChipInfo{ChipId, /*ChipChannel*/5, /*MidiChannel*/-1, /*MidiNote*/-1, 0});
		}
		
		m_ChipOn.clear();
		m_ChipOnSE.clear();
		
		i8 MidiChannel = 0;
		for (auto& MidiChannelInfo : m_aMidiChannelInfo){
			SetMidiChannelType(MidiChannel, (MidiChannel != 9)? CMidiTab::EChannelType_Inst: CMidiTab::EChannelType_Drum);
			
			MidiChannelInfo.Program = 0;
			MidiChannelInfo.PortamentoTime = 0.5f;
			MidiChannelInfo.LatestNote = 0;
			MidiChannelInfo.bSustainPedal = false;
			MidiChannelInfo.bPortamentoSwitch = false;
			
			for (int MidiNote = 0; MidiNote < MidiNotes; MidiNote++) std::queue<ChipInfo>().swap(MidiChannelInfo.aChipOn[MidiNote]);
			MidiChannelInfo.aChipInfo.clear();
			
			for (int MidiNote = 0; MidiNote < MidiNotes; MidiNote++) std::queue<VoiceInfo>().swap(MidiChannelInfo.aVoiceOn[MidiNote]);
			std::queue<i8>().swap(MidiChannelInfo.VoiceHoldNote);
			
			MidiChannel++;
		}
		
		auto& apChip = m_pVoice->GetChip();
		for (auto& pChip : apChip){
			pChip->Stop(0);
			pChip->Stop(1);
			pChip->Stop(2);
			pChip->Stop(3);
			pChip->Stop(4);
			pChip->Stop(5);
		}
		
		for (int ChipId = 0; ChipId < ChipNum; ChipId++){
			m_CModuleTab.SetState(ChipId, 0);
			m_CModuleTab.SetState(ChipId, 1);
			m_CModuleTab.SetState(ChipId, 2);
			m_CModuleTab.SetState(ChipId, 3);
			m_CModuleTab.SetState(ChipId, 4);
			m_CModuleTab.SetState(ChipId, 5);
		}
		m_CModuleTab.Invalidate();
	}
	
	for (i8 MidiChannel = 0; MidiChannel < MidiChannels; MidiChannel++){
		m_CMidiTab.ResetAllControllers(MidiChannel);
	}
	
	m_CInstTab.Reset();
	m_CDrumTab.Reset();
	
	{	// 
		SetVolume(m_CSettingTab.GetVolume());
		m_pVoice->SetFilterMode(m_CSettingTab.GetFilterMode());
		m_pVoice->SetCutoff(m_CSettingTab.GetCutoff());
		m_pVoice->SetResonance(m_CSettingTab.GetResonance());
		m_pVoice->SetDCCut(m_CSettingTab.IsDCCut());
		m_pVoice->SetDCCutRate(m_CSettingTab.GetDCCutRate());
		m_pVoice->SetPortamentoMode(m_CSettingTab.GetPortamentoMode());
		m_pVoice->SetTuning(m_CSettingTab.GetTuning());
	}
}



void CFmStationDlg::RemoveMidiChannelChipInfo(ChipInfo ChipInfo)
{
	if (ChipInfo.MidiChannel >= 0){
		auto& aChipInfo = m_aMidiChannelInfo[ChipInfo.MidiChannel].aChipInfo;
		auto Find = std::find_if(aChipInfo.begin(), aChipInfo.end(),
			[ChipInfo](auto& CmpChipInfo){ return (ChipInfo.ChipId == CmpChipInfo.ChipId) && (ChipInfo.ChipChannel == CmpChipInfo.ChipChannel); }
		);
		if (Find != aChipInfo.end()){
			aChipInfo.erase(Find.index());
		} else {
			TRACE(_T("!!! Error !!! : RemoveMidiChannelChipInfo : ChipInfo not found : MidiChannel.%d : ChipId.%d : ChipChannel.%d\n"), ChipInfo.MidiChannel, ChipInfo.ChipId, ChipInfo.ChipChannel);
		}
	}
}



bool CFmStationDlg::NoteOff(i8 MidiChannel, i8 MidiNote, int MidiVelocity, sptr<CTimbre> pTimbre)
{
	auto& MidiChannelInfo = m_aMidiChannelInfo[MidiChannel];
	auto& MidiChannelChipOn = MidiChannelInfo.aChipOn[MidiNote];
	if (!MidiChannelChipOn.empty()){
		auto ChipInfo = MidiChannelChipOn.front(); MidiChannelChipOn.pop();
		auto bChipCh3 = (ChipInfo.ChipChannel == 2);
		auto& ChipOff = (bChipCh3)? m_ChipOffSE: m_ChipOff;
		auto& ChipOn = (bChipCh3)? m_ChipOnSE: m_ChipOn;
		
		ChipOff.push(ChipInfo);
		
		auto Find = std::find_if(ChipOn.begin(), ChipOn.end(),
			[ChipInfo](auto& CmpChipInfo){ return (ChipInfo.NoteOnId == CmpChipInfo.NoteOnId); }
		);
		if (Find != ChipOn.end()){
			ChipOn.erase(Find);
		} else {
			if (!bChipCh3){
				auto FindSE = std::find_if(m_ChipOnSE.begin(), m_ChipOnSE.end(),
					[ChipInfo](auto& CmpChipInfo){ return (ChipInfo.NoteOnId == CmpChipInfo.NoteOnId); }
				);
				if (FindSE != m_ChipOnSE.end()){
					m_ChipOnSE.erase(FindSE);
				}
			} else {
				TRACE(_T("!!! Error !!! : NoteOff : NoteOnId not found : MidiChannel.%d : MidiNote.%d : ChipChannel.%d : NoteOnId.%d\n"), MidiChannel, MidiNote, ChipInfo.ChipChannel, ChipInfo.NoteOnId);
			}
		}
		
		auto ChipId = ChipInfo.ChipId;
		auto ChipChannel = ChipInfo.ChipChannel;
		
		auto& apChip = m_pVoice->GetChip();
		auto& Chip = *apChip[ChipId];
		Chip.Stop(ChipChannel);
		
		m_CModuleTab.SetState(ChipId, ChipChannel);
		m_CMidiTab.NoteOff(MidiChannel, MidiNote);
		
		return true;
	} else {
		//TRACE(_T("!!! Warning !!! : NoteOff : MidiChannelChipOn is empty : MidiChannel.%d : MidiNote.%d\n"), MidiChannel, MidiNote);
	}
	return false;
}



bool CFmStationDlg::NoteOn(i8 MidiChannel, i8 MidiNote, int MidiVelocity, sptr<CTimbre> pTimbre)
{
	auto bSe = (pTimbre->Control.SE.GetValue() != 0);
	auto& ChipOff = (bSe)? m_ChipOffSE: m_ChipOff;
	auto* pChipOn = (bSe)? &m_ChipOnSE: &m_ChipOn;
	
	i8 ChipId = -1;
	i8 ChipChannel = -1;
	
	Retry:
	if (!ChipOff.empty()){
		auto ChipInfo = ChipOff.front(); ChipOff.pop();
		RemoveMidiChannelChipInfo(ChipInfo);
		
		ChipId = ChipInfo.ChipId;
		ChipChannel = ChipInfo.ChipChannel;
		
		if (bSe){
			if (ChipChannel != 2) TRACE(_T("!!! Error !!! : NoteOn : bSe.%d : ChipChannel.%d\n"), bSe, ChipChannel);
		} else {
			if (ChipChannel == 2) TRACE(_T("!!! Error !!! : NoteOn : bSe.%d : ChipChannel.%d\n"), bSe, ChipChannel);
		}
	} else {
		if (!bSe && !m_ChipOffSE.empty()){
			auto ChipInfo = m_ChipOffSE.front(); m_ChipOffSE.pop();
			RemoveMidiChannelChipInfo(ChipInfo);
			
			ChipId = ChipInfo.ChipId;
			ChipChannel = ChipInfo.ChipChannel;
			
			if (ChipChannel != 2) TRACE(_T("!!! Error !!! : NoteOn : bSe.%d : ChipChannel.%d\n"), bSe, ChipChannel);
			
			pChipOn = &m_ChipOnSE;
		} else {
			auto& ChipOn = *pChipOn;
			if (!ChipOn.empty()){
				auto ChipInfo = ChipOn.front();
				NoteOff(ChipInfo.MidiChannel, ChipInfo.MidiNote, 0, NULL/*std::make_shared<CTimbre>()*/);
				goto Retry;
			} else {
				TRACE(_T("!!! Error !!! : NoteOn : MidiChannel.%d : MidiNote.%d\n"), MidiChannel, MidiNote);
				return false;
			}
		}
	}
	
	{	// 
		auto& MidiChannelInfo = m_aMidiChannelInfo[MidiChannel];
		auto& MidiChannelChipOn = MidiChannelInfo.aChipOn[MidiNote];
		
		auto& ChipOn = *pChipOn;
		ChipInfo ChipInfo{ChipId, ChipChannel, MidiChannel, MidiNote, ++m_NoteOnId};
		MidiChannelInfo.aChipInfo.push(ChipInfo);
		MidiChannelChipOn.push(ChipInfo);
		ChipOn.push_back(ChipInfo);
		
		auto Pan = m_CMidiTab.GetPan(MidiChannel);
		auto Volume = m_CMidiTab.GetVolume(MidiChannel);
		auto Expression = m_CMidiTab.GetExpression(MidiChannel);
		auto PitchBend = m_CMidiTab.GetPitchBend(MidiChannel);
		auto ModulationDepth = m_CMidiTab.GetModulationDepth(MidiChannel);
		auto VibratoRate = m_CMidiTab.GetVibratoRate(MidiChannel);
		auto VibatoDepth = m_CMidiTab.GetVibatoDepth(MidiChannel);
		
		auto& apChip = m_pVoice->GetChip();
		auto& Chip = *apChip[ChipId];
		Chip.Pan(ChipChannel, Pan);
		Chip.Volume(ChipChannel, Volume);
		Chip.Velocity(ChipChannel, MidiVelocity * (1.0f / 0x7f));
		Chip.Expression(ChipChannel, Expression);
		Chip.PitchBend(ChipChannel, PitchBend);
		Chip.ModulationDepth(ChipChannel, ModulationDepth);
		Chip.VibratoRate(ChipChannel, VibratoRate);
		Chip.VibatoDepth(ChipChannel, VibatoDepth);
		Chip.PortamentoNote(ChipChannel, (MidiChannelInfo.bPortamentoSwitch)? MidiChannelInfo.LatestNote: MidiNote);
		Chip.PortamentoTime(ChipChannel, MidiChannelInfo.PortamentoTime);
		Chip.Play(ChipChannel, MidiNote, pTimbre);
		
		m_CModuleTab.SetState(ChipId, ChipChannel, MidiChannel, MidiNote, MidiVelocity);
		m_CMidiTab.NoteOn(MidiChannel, MidiNote);
		
		MidiChannelInfo.LatestNote = MidiNote;
	}
	
	return true;
}



void CFmStationDlg::SetMidiChannelType(i8 MidiChannel, CMidiTab::EChannelType Type)
{
	auto& MidiChannelInfo = m_aMidiChannelInfo[MidiChannel];
	MidiChannelInfo.Type = Type;
	
	m_CMidiTab.SetMidiChannelType(MidiChannel, Type);
}



void CFmStationDlg::SetVolume(double Volume)
{
	auto& rCSettingTab = GetSettingTab();
	rCSettingTab.SetVolume(Volume);
	m_pVoice->SetVolume(rCSettingTab.GetVolume());
}



void CFmStationDlg::VolumeUp()
{
	SetVolume(GetSettingTab().GetVolume() + 1);
	
	auto& rCSettingTab = GetSettingTab();
	Log(_T("Volume x{:1.1f}"), rCSettingTab.GetVolume() * 0.1);
}



void CFmStationDlg::VolumeDown()
{
	SetVolume(GetSettingTab().GetVolume() - 1);
	
	auto& rCSettingTab = GetSettingTab();
	Log(_T("Volume x{:1.1f}"), rCSettingTab.GetVolume() * 0.1);
}



i8 CFmStationDlg::GetPreview()
{
	auto& rCSettingTab = GetSettingTab();
	return rCSettingTab.GetPreview();
}



void CFmStationDlg::SetPreview(i8 Preview)
{
	auto& rCSettingTab = GetSettingTab();
	rCSettingTab.SetPreview(Preview);
}



void CFmStationDlg::PreviewUp()
{
	SetPreview(GetSettingTab().GetPreview() + 1);
	
	auto& rCSettingTab = GetSettingTab();
	Log(_T("Preview ch.{}"), rCSettingTab.GetPreview() + 1);
}



void CFmStationDlg::PreviewDown()
{
	SetPreview(GetSettingTab().GetPreview() - 1);
	
	auto& rCSettingTab = GetSettingTab();
	Log(_T("Preview ch.{}"), rCSettingTab.GetPreview() + 1);
}



void CFmStationDlg::WavRecord()
{
	if (m_pVoice->WavRecord()){
		Log(_T("Wav Record Start"));
	} else {
		Log(_T("Wav Record End"));
	}
}



template<class... Args> void CFmStationDlg::Log(std::wformat_string<Args...> fmt, Args&& ... args)
{
	m_CStaticLog.SetWindowText((LPCTSTR)std::format(fmt, std::forward<Args>(args)...).c_str());
}



bool CFmStationDlg::MidiNoteOff(i8 MidiChannel, i8 MidiNote, int MidiVelocity)
{
	auto& MidiChannelInfo = m_aMidiChannelInfo[MidiChannel];
	if (MidiChannelInfo.bSustainPedal){
		MidiChannelInfo.VoiceHoldNote.push(MidiNote);
		
		auto& MidiChannelInfo = m_aMidiChannelInfo[MidiChannel];
		auto& MidiChannelChipOn = MidiChannelInfo.aChipOn[MidiNote];
		if (!MidiChannelChipOn.empty()){
			auto ChipInfo = MidiChannelChipOn.front();
			m_CModuleTab.SetHold(ChipInfo.ChipId, ChipInfo.ChipChannel);
		}
	} else {
		auto& VoiceOn = MidiChannelInfo.aVoiceOn[MidiNote];
		if (!VoiceOn.empty()){
			auto VoiceInfo = VoiceOn.front(); VoiceOn.pop();
			auto& pBatch = VoiceInfo.pBatch;
			if (pBatch) pBatch->MidiNoteOff(MidiChannel, MidiNote, MidiVelocity);
			return true;
		}
	}
	return false;
}



bool CFmStationDlg::MidiNoteOff(i8 MidiChannel, i8 MidiNote, int MidiVelocity, sptr<CTimbre> pTimbre)
{
	return NoteOff(MidiChannel, MidiNote, MidiVelocity, pTimbre);
}



bool CFmStationDlg::MidiNoteOn(i8 MidiChannel, i8 MidiNote, int MidiVelocity)
{
	auto& MidiChannelInfo = m_aMidiChannelInfo[MidiChannel];
	auto& VoiceOn = MidiChannelInfo.aVoiceOn[MidiNote];
	auto pBatch = (MidiChannelInfo.Type == CMidiTab::EChannelType_Inst)? m_CInstTab.GetProgram(MidiChannelInfo.Bank, MidiChannelInfo.Program): m_CDrumTab.GetProgram(MidiChannelInfo.Program, MidiNote);
	if (pBatch){
		VoiceOn.push(VoiceInfo{MidiChannel, MidiNote, pBatch});
		pBatch->MidiNoteOn(MidiChannel, MidiNote, MidiVelocity);
		return true;
	}
	return false;
}



bool CFmStationDlg::MidiNoteOn(i8 MidiChannel, i8 MidiNote, int MidiVelocity, sptr<CTimbre> pTimbre)
{
	return NoteOn(MidiChannel, MidiNote, MidiVelocity, pTimbre);
}



bool CFmStationDlg::MidiPan(i8 MidiChannel, float MidiValue)
{
	auto& MidiChannelInfo = m_aMidiChannelInfo[MidiChannel];
	auto& apChip = m_pVoice->GetChip();
	for (auto& ChipInfo : MidiChannelInfo.aChipInfo){
		apChip[ChipInfo.ChipId]->Pan(ChipInfo.ChipChannel, MidiValue, true);
	}
	return true;
}



bool CFmStationDlg::MidiVolume(i8 MidiChannel, float MidiValue)
{
	auto& MidiChannelInfo = m_aMidiChannelInfo[MidiChannel];
	auto& apChip = m_pVoice->GetChip();
	for (auto& ChipInfo : MidiChannelInfo.aChipInfo){
		apChip[ChipInfo.ChipId]->Volume(ChipInfo.ChipChannel, MidiValue, true);
	}
	return true;
}



bool CFmStationDlg::MidiExpression(i8 MidiChannel, float MidiValue)
{
	auto& MidiChannelInfo = m_aMidiChannelInfo[MidiChannel];
	auto& apChip = m_pVoice->GetChip();
	for (auto& ChipInfo : MidiChannelInfo.aChipInfo){
		apChip[ChipInfo.ChipId]->Expression(ChipInfo.ChipChannel, MidiValue, true);
	}
	return true;
}



bool CFmStationDlg::MidiModulationDepth(i8 MidiChannel, float MidiValue)
{
	auto& MidiChannelInfo = m_aMidiChannelInfo[MidiChannel];
	auto& apChip = m_pVoice->GetChip();
	for (auto& ChipInfo : MidiChannelInfo.aChipInfo){
		apChip[ChipInfo.ChipId]->ModulationDepth(ChipInfo.ChipChannel, MidiValue, true);
	}
	return true;
}



bool CFmStationDlg::MidiHoldPedal1(i8 MidiChannel, i8 MidiValue)
{
	auto& MidiChannelInfo = m_aMidiChannelInfo[MidiChannel];
	MidiChannelInfo.bSustainPedal = (MidiValue >= 64);
	if (!MidiChannelInfo.bSustainPedal){
		auto& VoiceHoldNote = MidiChannelInfo.VoiceHoldNote;
		while (!VoiceHoldNote.empty()){
			auto MidiNote = VoiceHoldNote.front(); VoiceHoldNote.pop();
			MidiNoteOff(MidiChannel, MidiNote, 0);
		}
	}
	return true;
}



bool CFmStationDlg::MidiPitchBend(i8 MidiChannel, float MidiValue)
{
	auto& MidiChannelInfo = m_aMidiChannelInfo[MidiChannel];
	auto& apChip = m_pVoice->GetChip();
	for (auto& ChipInfo : MidiChannelInfo.aChipInfo){
		apChip[ChipInfo.ChipId]->PitchBend(ChipInfo.ChipChannel, MidiValue, true);
	}
	return true;
}



bool CFmStationDlg::MidiVibratoRate(i8 MidiChannel, float MidiValue)
{
	auto& MidiChannelInfo = m_aMidiChannelInfo[MidiChannel];
	auto& apChip = m_pVoice->GetChip();
	for (auto& ChipInfo : MidiChannelInfo.aChipInfo){
		apChip[ChipInfo.ChipId]->VibratoRate(ChipInfo.ChipChannel, MidiValue, true);
	}
	return true;
}



bool CFmStationDlg::MidiVibatoDepth(i8 MidiChannel, float MidiValue)
{
	auto& MidiChannelInfo = m_aMidiChannelInfo[MidiChannel];
	auto& apChip = m_pVoice->GetChip();
	for (auto& ChipInfo : MidiChannelInfo.aChipInfo){
		apChip[ChipInfo.ChipId]->VibatoDepth(ChipInfo.ChipChannel, MidiValue, true);
	}
	return true;
}



bool CFmStationDlg::MidiProgramChange(i8 MidiChannel, int MidiValue)
{
	auto& MidiChannelInfo = m_aMidiChannelInfo[MidiChannel];
	MidiChannelInfo.Program = MidiValue;
	return true;
}



bool CFmStationDlg::MidiPortamentoTime(i8 MidiChannel, float MidiValue)
{
	auto& MidiChannelInfo = m_aMidiChannelInfo[MidiChannel];
	MidiChannelInfo.PortamentoTime = MidiValue;
	return true;
}



bool CFmStationDlg::MidiPortamentoSwitch(i8 MidiChannel, i8 MidiValue)
{
	auto& MidiChannelInfo = m_aMidiChannelInfo[MidiChannel];
	MidiChannelInfo.bPortamentoSwitch = (MidiValue >= 64);
	return true;
}



bool CFmStationDlg::MidiBankSelect(i8 MidiChannel, int MidiValue)
{
	auto& MidiChannelInfo = m_aMidiChannelInfo[MidiChannel];
	MidiChannelInfo.Bank = MidiValue;
	return true;
}



bool CFmStationDlg::MidiSystemReset()
{
	SystemReset();
	return true;
}
