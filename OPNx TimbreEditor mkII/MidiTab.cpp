



#include "pch.h"
#include "FmStation.h"
#include "MidiTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



IMPLEMENT_DYNAMIC(CMidiTab, CDialogEx)



CMidiTab::~CMidiTab()
{
	//if (m_timerID != 0) KillTimer(m_timerID);
}



CMidiTab::CMidiTab(CWnd* pParent)
:CDialogEx(IDD_MIDI_TAB, pParent)
,m_timerID(0)
,m_aChannelInfo{}
{
}



void CMidiTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MIDI_LIST, m_CListCtrl);
}



BEGIN_MESSAGE_MAP(CMidiTab, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()



BOOL CMidiTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	{	// 
		m_BmpWhiteOffRight.LoadBitmap(IDB_SMALL_WHITE_OFF_RIGHT);
		m_BmpWhiteOffBoth.LoadBitmap(IDB_SMALL_WHITE_OFF_BOTH);
		m_BmpWhiteOffLeft.LoadBitmap(IDB_SMALL_WHITE_OFF_LEFT);
		m_BmpWhiteOnRight.LoadBitmap(IDB_SMALL_WHITE_ON_RIGHT);
		m_BmpWhiteOnBoth.LoadBitmap(IDB_SMALL_WHITE_ON_BOTH);
		m_BmpWhiteOnLeft.LoadBitmap(IDB_SMALL_WHITE_ON_LEFT);
		m_BmpBlackOff.LoadBitmap(IDB_SMALL_BLACK_OFF);
		m_BmpBlackOn.LoadBitmap(IDB_SMALL_BLACK_ON);
	}
	
	{	// Column
		struct Column
		{
			LPCWSTR Label;
			int Width;
		};
		Column aColumn[] = {
			{_T("Ch"), 26},
			{_T("Prog"), 35},
			{_T("Bank"), 38},
			{_T("Pan"), 31},
			{_T("Vol"), 30},
			{_T("Exp"), 31},
			{_T("Hold"), 35},
			{_T("Mod"), 33},
			{_T("Bend"), 68},
			{_T("Porta"), 55},
			{_T("Keyboard"), 150},
		};
		for (int i = 0; i < std::size(aColumn); i++){
			LVCOLUMN Column;
			Column.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
			Column.fmt = LVCFMT_RIGHT;
			Column.pszText = (LPWSTR)aColumn[i].Label;
			Column.cx = aColumn[i].Width;
			m_CListCtrl.InsertColumn(i, &Column);
		}
		
		{	// 
			auto pHeader = m_CListCtrl.GetHeaderCtrl();
			if (pHeader != nullptr){
				pHeader->EnableWindow(FALSE);
			}
		}
		
		{	// 
			m_CListCtrl.SetExtendedStyle(m_CListCtrl.GetExtendedStyle() | LVS_EX_DOUBLEBUFFER);
		}
	}
	
	{	// 
		std::array<StaticList::ColumnParam, StaticList::ColumnNum> aColumnParam;
		aColumnParam[EColumn_Channel].Width = 26-1; aColumnParam[EColumn_Channel].dwStyle = WS_CHILD | WS_VISIBLE | SS_CENTER;
		aColumnParam[EColumn_Program].Width = 35; aColumnParam[EColumn_Program].dwStyle = WS_CHILD | WS_VISIBLE | SS_CENTER;
		aColumnParam[EColumn_Bank].Width = 38; aColumnParam[EColumn_Bank].dwStyle = WS_CHILD | WS_VISIBLE | SS_CENTER;
		aColumnParam[EColumn_Pan].Width = 31; aColumnParam[EColumn_Pan].dwStyle = WS_CHILD | WS_VISIBLE | SS_CENTER;
		aColumnParam[EColumn_Vol].Width = 30; aColumnParam[EColumn_Vol].dwStyle = WS_CHILD | WS_VISIBLE | SS_CENTER;
		aColumnParam[EColumn_Exp].Width = 31; aColumnParam[EColumn_Exp].dwStyle = WS_CHILD | WS_VISIBLE | SS_CENTER;
		aColumnParam[EColumn_Hold].Width = 35; aColumnParam[EColumn_Hold].dwStyle = WS_CHILD | WS_VISIBLE | SS_CENTER;
		aColumnParam[EColumn_Mod].Width = 33; aColumnParam[EColumn_Mod].dwStyle = WS_CHILD | WS_VISIBLE | SS_CENTER;
		aColumnParam[EColumn_Bend].Width = 68; aColumnParam[EColumn_Bend].dwStyle = WS_CHILD | WS_VISIBLE | SS_CENTER;
		aColumnParam[EColumn_Port].Width = 55; aColumnParam[EColumn_Port].dwStyle = WS_CHILD | WS_VISIBLE | SS_CENTER;
		aColumnParam[EColumn_Keyboard].Width = 150; aColumnParam[EColumn_Keyboard].dwStyle = WS_CHILD | WS_VISIBLE | SS_BITMAP;
		m_StaticList.Create(2, 19, 13, aColumnParam, this);
		
		{	// 
			int Ch = 0;
			for (auto& Row : m_StaticList.GetRows()){
				CString Text;
				Text.Format(_T("%d"), ++Ch);
				
				auto& aColumn = Row.GetColumns();
				aColumn[EColumn_Channel].Cell.SetText(Text);
				
				aColumn[EColumn_Channel].Cell.SetFormat(DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX | DT_VCENTER | DT_CENTER);
				aColumn[EColumn_Program].Cell.SetFormat(DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX | DT_VCENTER | DT_CENTER);
				aColumn[EColumn_Bank].Cell.SetFormat(DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX | DT_VCENTER | DT_CENTER);
				aColumn[EColumn_Pan].Cell.SetFormat(DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX | DT_VCENTER | DT_CENTER);
				aColumn[EColumn_Vol].Cell.SetFormat(DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX | DT_VCENTER | DT_CENTER);
				aColumn[EColumn_Mod].Cell.SetFormat(DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX | DT_VCENTER | DT_CENTER);
				aColumn[EColumn_Exp].Cell.SetFormat(DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX | DT_VCENTER | DT_CENTER);
				aColumn[EColumn_Hold].Cell.SetFormat(DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX | DT_VCENTER | DT_CENTER);
				aColumn[EColumn_Bend].Cell.SetFormat(DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX | DT_VCENTER | DT_CENTER);
				aColumn[EColumn_Port].Cell.SetFormat(DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX | DT_VCENTER | DT_CENTER);
				aColumn[EColumn_Keyboard].Cell.SetFormat(DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX | DT_VCENTER | DT_RIGHT);
				
				auto pDC = GetDC();
				auto& Bitmap = aColumn[EColumn_Keyboard].Cell.GetBitmap();
				Bitmap.CreateCompatibleBitmap(pDC, 150, 13);
				ReleaseDC(pDC);
			}
		}
		
		m_timerID = SetTimer(2, /*ms*/13, NULL);
	}
	
	for (int MidiChannel = 0; MidiChannel < MidiChannels; MidiChannel++){
		ResetAllControllers(MidiChannel);
	}
	
	return FALSE;
}



BOOL CMidiTab::PreTranslateMessage(MSG* pMsg)
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
					case VK_RETURN:
					case VK_ESCAPE:
					{
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



CFmStationDlg& CMidiTab::GetFmStationDlg()
{
	return theApp.GetFmStationDlg();
}



void CMidiTab::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_timerID){
		auto& aRow = m_StaticList.GetRows();
		for (int MidiChannel = 0; MidiChannel < MidiChannels; MidiChannel++){
			auto& ChannelInfo = m_aChannelInfo[MidiChannel];
			
			for (int Column = 0; Column < EColumn_Keyboard; Column++){
				auto& Cell = aRow[MidiChannel].aColumnInfo[Column].Cell;
				bool bInvalidate = ChannelInfo.bInvalidate;
				
				{	// Text
					auto c0 = GetSysColor(COLOR_MENUTEXT);
					auto c1 = GetSysColor(COLOR_WINDOW);
					
					auto r0 = (float)GetRValue(c0);
					auto g0 = (float)GetGValue(c0);
					auto b0 = (float)GetBValue(c0);
					
					auto r1 = (float)GetRValue(c1);
					auto g1 = (float)GetGValue(c1);
					auto b1 = (float)GetBValue(c1);
					
					auto p1 = ChannelInfo.aText[Column];
					auto p0 = 1.0f - p1;
					auto r = std::round((r0 * p0) + (r1 * p1));
					auto g = std::round((g0 * p0) + (g1 * p1));
					auto b = std::round((b0 * p0) + (b1 * p1));
					auto c = RGB(r, g, b);
					
					Cell.SetTextColor(c);
					
					bInvalidate |= (p1 > (1.0f / 0xff));
				}
				
				{	// Back
					auto c0 = (ChannelInfo.Type == CMidiTab::EChannelType_Inst)? (MidiChannel & 0x4)? GetSysColor(COLOR_MENUBAR): GetSysColor(COLOR_WINDOW): RGB(0xf4, 0xec, 0xe4);
					auto c1 = GetSysColor(COLOR_MENUHILIGHT);
					
					auto r0 = (float)GetRValue(c0);
					auto g0 = (float)GetGValue(c0);
					auto b0 = (float)GetBValue(c0);
					
					auto r1 = (float)GetRValue(c1);
					auto g1 = (float)GetGValue(c1);
					auto b1 = (float)GetBValue(c1);
					
					auto p1 = ChannelInfo.aBack[Column];
					auto p0 = 1.0f - p1;
					auto r = std::round((r0 * p0) + (r1 * p1));
					auto g = std::round((g0 * p0) + (g1 * p1));
					auto b = std::round((b0 * p0) + (b1 * p1));
					auto c = RGB(r, g, b);
					
					Cell.SetBackColor(c);
					
					bInvalidate |= (p1 > (1.0f / 0xff));
				}
				
				if (bInvalidate) Cell.Invalidate();
				
				ChannelInfo.aText[Column] *= 1.0f - 0.030f;
				ChannelInfo.aBack[Column] *= 1.0f - 0.100f;
			}
			
			{	// Keyboard
				auto& Cell = aRow[MidiChannel].aColumnInfo[EColumn_Keyboard].Cell;
				bool bInvalidate = false;
				
				{	// 
					auto pDC = GetDC();
					
					auto& Row = aRow[MidiChannel];
					auto& aColumn = Row.GetColumns();
					auto& Bitmap = Cell.GetBitmap();
					
					CDC srcDC;
					srcDC.CreateCompatibleDC(pDC);
					
					CDC bmpDC;
					bmpDC.CreateCompatibleDC(&srcDC);
					
					int x = 0;
					int y = 0;
					
					CBitmap* pOldSrc = srcDC.SelectObject(&Bitmap);
					for (int MidiNote = 0; MidiNote < MidiNotes; MidiNote++){
						CBitmap* pBmp = NULL;
						int o = 0;
						
						auto& NoteOn = ChannelInfo.aNoteOn[MidiNote];
						auto bNoteState = (NoteOn > 0);
						switch (MidiNote % 12){
							case 0: o = 1; pBmp = (bNoteState)? &m_BmpWhiteOnRight: &m_BmpWhiteOffRight;break;//c
							case 1: o = 1; pBmp = (bNoteState)? &m_BmpBlackOn: &m_BmpBlackOff;			break;//c+
							case 2: o = 1; pBmp = (bNoteState)? &m_BmpWhiteOnBoth: &m_BmpWhiteOffBoth;	break;//d
							case 3: o = 1; pBmp = (bNoteState)? &m_BmpBlackOn: &m_BmpBlackOff;			break;//d+
							case 4: o = 2; pBmp = (bNoteState)? &m_BmpWhiteOnLeft: &m_BmpWhiteOffLeft;	break;//e
							case 5: o = 1; pBmp = (bNoteState)? &m_BmpWhiteOnRight: &m_BmpWhiteOffRight;break;//f
							case 6: o = 1; pBmp = (bNoteState)? &m_BmpBlackOn: &m_BmpBlackOff;			break;//f+
							case 7: o = 1; pBmp = (bNoteState)? &m_BmpWhiteOnBoth: &m_BmpWhiteOffBoth;	break;//g
							case 8: o = 1; pBmp = (bNoteState)? &m_BmpBlackOn: &m_BmpBlackOff;			break;//g+
							case 9: o = 1; pBmp = (bNoteState)? &m_BmpWhiteOnBoth: &m_BmpWhiteOffBoth;	break;//a
							case 10: o = 1; pBmp = (bNoteState)? &m_BmpBlackOn: &m_BmpBlackOff;			break;//a+
							case 11: o = 2; pBmp = (bNoteState)? &m_BmpWhiteOnLeft: &m_BmpWhiteOffLeft;	break;//b
						}
						
						if (ChannelInfo.abNoteState[MidiNote] != bNoteState){
							ChannelInfo.abNoteState[MidiNote] = bNoteState;
							
							bInvalidate |= true;
							
							BITMAP bmp;
							pBmp->GetBitmap(&bmp);
							
							CBitmap* pOldBmp = bmpDC.SelectObject(pBmp);
							TransparentBlt(
								srcDC.GetSafeHdc(),
								x, y, bmp.bmWidth, bmp.bmHeight,
								bmpDC.GetSafeHdc(),
								0, 0, bmp.bmWidth, bmp.bmHeight,
								RGB(255, 0, 0)
							);
							bmpDC.SelectObject(pOldBmp);
						}
						
						x += o;
					}
					srcDC.SelectObject(pOldSrc);
					
					bmpDC.DeleteDC();
					srcDC.DeleteDC();
					
					ReleaseDC(pDC);
				}
				
				if (bInvalidate) Cell.Invalidate();
			}
			
			ChannelInfo.bInvalidate = false;
		}
	}
	
	CDialogEx::OnTimer(nIDEvent);
}



BOOL CMidiTab::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;//CDialogEx::OnEraseBkgnd(pDC);
}



void CMidiTab::MidiInProc(IMidiCallback* pCallback, BYTE status, BYTE data1, BYTE data2)
{
	BYTE Type = status >> 4;
	
	i8 MidiChannel = status & 0xf;
	auto& ChannelInfo = m_aChannelInfo[MidiChannel];
	
	//TRACE(_T("status.%02x : %3d : %3d\n"), status, data1, data2);
	switch (status){
		default:{
			switch (Type){
				case 0x8:{//NoteOff
					if (pCallback) pCallback->MidiNoteOff(MidiChannel, data1, data2);
					break;
				}
				case 0x9:{//NoteOn
					if (pCallback){
						if (data2 == 0){
							pCallback->MidiNoteOff(MidiChannel, data1, data2);
						} else {
							pCallback->MidiNoteOn(MidiChannel, data1, data2);
						}
					}
					break;
				}
				case 0xa:{//KeyAfterTouch
					TRACE(_T("KeyAfterTouch : %d : %02x : %02x\n"), MidiChannel, data1, data2);
					break;
				}
				case 0xb:{//ControlChange
					ControlChange(pCallback, MidiChannel, data1, data2);
					break;
				}
				case 0xc:{//ProgramChange
					ChannelInfo.aParam[EParam_Program] = data1;
					SetItem(MidiChannel, EColumn_Program, ChannelInfo.aParam[EParam_Program]+1, _T("%3d"));
					
					if (pCallback) pCallback->MidiProgramChange(MidiChannel, GetProgram(MidiChannel));
					break;
				}
				case 0xd:{//ChannelAfterTouch
					TRACE(_T("ChannelAfterTouch : %d : %02x : %02x\n"), MidiChannel, data1, data2);
					break;
				}
				case 0xe:{//PitchBend
					ChannelInfo.aParam[EParam_PitchBend] = ((data2<<7) | data1);
					SetItem(MidiChannel, EColumn_Bend, ChannelInfo.aParam[EParam_RPN_PitchBendSensitivity]>>7, ChannelInfo.aParam[EParam_PitchBend], _T("%3d.%04x"));
					
					if (pCallback) pCallback->MidiPitchBend(MidiChannel, GetPitchBend(MidiChannel));
					break;
				}
				case 0xf:{//SysEx
					break;
				}
			}
			break;
		}
		case 0xff:{//SystemReset
			if (pCallback) pCallback->MidiSystemReset();
			break;
		}
	}
}



void CMidiTab::ResetAllControllers(i8 MidiChannel)
{
	//TRACE(_T("CMidiTab::ResetAllControllers : %d\n"), MidiChannel);
	auto& ChannelInfo = m_aChannelInfo[MidiChannel];
	
	{	// 
		ChannelInfo.aParam[EParam_Program] = 0;
		
		ChannelInfo.aParam[EParam_PitchBend] = 0x2000;
		
		ChannelInfo.aParam[EParam_BankSelect] = 0;
		ChannelInfo.aParam[EParam_ModulationDepth] = 0<<7;
		ChannelInfo.aParam[EParam_FootPedal] = 0<<7;
		ChannelInfo.aParam[EParam_PortamentoTime] = 0x0000;
		ChannelInfo.aParam[EParam_Volume] = 100<<7;
		ChannelInfo.aParam[EParam_Balance] = 64<<7;
		ChannelInfo.aParam[EParam_Pan] = 64<<7;
		ChannelInfo.aParam[EParam_Expression] = 127<<7;
		
		ChannelInfo.aParam[EParam_HoldPedal1] = 0;
		ChannelInfo.aParam[EParam_PortamentoSwitch] = 0;
		ChannelInfo.aParam[EParam_SostenutoPedal] = 0;
		ChannelInfo.aParam[EParam_SoftPedal] = 0;
		ChannelInfo.aParam[EParam_LegatoFootSwitch] = 0;
		ChannelInfo.aParam[EParam_HoldPedal2] = 0;
		
		ChannelInfo.aParam[EParam_SoundVariation] = 64;
		ChannelInfo.aParam[EParam_HarmonicContents] = 64;
		ChannelInfo.aParam[EParam_ReleaseTime] = 64;
		ChannelInfo.aParam[EParam_AttackTime] = 64;
		ChannelInfo.aParam[EParam_Brightness] = 64;
		ChannelInfo.aParam[EParam_DecayTime] = 64;
		ChannelInfo.aParam[EParam_VibratoRate] = 64;
		ChannelInfo.aParam[EParam_VibatoDepth] = 64;
		ChannelInfo.aParam[EParam_VibratoDelay] = 64;
		ChannelInfo.aParam[EParam_SoundController10] = 64;
		
		ChannelInfo.aParam[EParam_PortamentControl] = 0;
		
		ChannelInfo.aParam[EParam_RPN] = 0x3fff;
		ChannelInfo.aParam[EParam_NRPN] = 0x3fff;
		
		ChannelInfo.aParam[EParam_RPN_PitchBendSensitivity] = 2<<7;
		
		ChannelInfo.aParam[EParam_LocalSwitch] = 127;
		
		ChannelInfo.DataEntry = EDataEntry_None;
		
		// 
		std::ranges::fill(ChannelInfo.aNoteOn, 0);
		std::ranges::fill(ChannelInfo.abNoteState, true);
	}
	
	{	// 
		ChannelInfo.aText[EColumn_Channel] = 1.0f;
		ChannelInfo.aBack[EColumn_Channel] = 1.0f;
		
		SetItem(MidiChannel, EColumn_Program);
		SetItem(MidiChannel, EColumn_Bank, ChannelInfo.aParam[EParam_BankSelect], _T("%04x"));
		SetItem(MidiChannel, EColumn_Pan, ChannelInfo.aParam[EParam_Pan]>>7, _T("%3d"));
		SetItem(MidiChannel, EColumn_Vol, ChannelInfo.aParam[EParam_Volume]>>7, _T("%3d"));
		SetItem(MidiChannel, EColumn_Mod, ChannelInfo.aParam[EParam_ModulationDepth]>>7, _T("%3d"));
		SetItem(MidiChannel, EColumn_Exp, ChannelInfo.aParam[EParam_Expression]>>7, _T("%3d"));
		SetItem(MidiChannel, EColumn_Hold, ChannelInfo.aParam[EParam_HoldPedal1]>>6, _T("%3d"));
		SetItem(MidiChannel, EColumn_Bend, ChannelInfo.aParam[EParam_RPN_PitchBendSensitivity]>>7, ChannelInfo.aParam[EParam_PitchBend], _T("%3d.%04x"));
		SetItem(MidiChannel, EColumn_Port, ChannelInfo.aParam[EParam_PortamentoSwitch]>>6, ChannelInfo.aParam[EParam_PortamentoTime], _T("%d.%04x"));
	}
}



void CMidiTab::ControlChange(IMidiCallback* pCallback, i8 MidiChannel, BYTE CC, BYTE Value)
{
	auto& ChannelInfo = m_aChannelInfo[MidiChannel];
	
	switch (CC){
		//MSB
		case 0:{//Bank Select
			SetParamMSB(ChannelInfo.aParam[EParam_BankSelect], Value);
			SetItem(MidiChannel, EColumn_Bank, ChannelInfo.aParam[EParam_BankSelect], _T("%04x"));
			
			if (pCallback) pCallback->MidiBankSelect(MidiChannel, GetBankSelect(MidiChannel));
			break;
		}
		case 1:{//Modulation Depth
			SetParamMSB(ChannelInfo.aParam[EParam_ModulationDepth], Value);
			SetItem(MidiChannel, EColumn_Mod, ChannelInfo.aParam[EParam_ModulationDepth]>>7, _T("%3d"));
			
			if (pCallback) pCallback->MidiModulationDepth(MidiChannel, GetModulationDepth(MidiChannel));
			break;
		}
		case 4:{//Foot Pedal
			SetParamMSB(ChannelInfo.aParam[EParam_FootPedal], Value);
			break;
		}
		case 5:{//Portamento Time
			SetParamMSB(ChannelInfo.aParam[EParam_PortamentoTime], Value);
			SetItem(MidiChannel, EColumn_Port, ChannelInfo.aParam[EParam_PortamentoSwitch]>>6, ChannelInfo.aParam[EParam_PortamentoTime], _T("%d.%04x"));
			
			if (pCallback) pCallback->MidiPortamentoTime(MidiChannel, GetPortamentoTime(MidiChannel));
			break;
		}
		case 6:{//Data Entry
			//TRACE(_T("Data Entry MSB : %d : %d\n"), Value, ChannelInfo.DataEntry);
			switch (ChannelInfo.DataEntry){
				default:
				case EDataEntry_None:
				{
					break;
				}
				case EDataEntry_ModeRPN:
				{
					switch (ChannelInfo.aParam[EParam_RPN]){
						case 0:{//Pitch Bend Sensitivity
							SetParamMSB(ChannelInfo.aParam[EParam_RPN_PitchBendSensitivity], Value);
							SetItem(MidiChannel, EColumn_Bend, ChannelInfo.aParam[EParam_RPN_PitchBendSensitivity]>>7, ChannelInfo.aParam[EParam_PitchBend], _T("%3d.%04x"));
							break;
						}
						case 1:{//Master Fine Tuning
							SetParamMSB(ChannelInfo.aParam[EParam_RPN_MasterFineTuning], Value);
							break;
						}
						case 2:{//Master Course Tuning
							SetParamMSB(ChannelInfo.aParam[EParam_RPN_MasterCourseTuning], Value);
							break;
						}
						case 3:{//Tuning Program Select
							SetParamMSB(ChannelInfo.aParam[EParam_RPN_TuningProgramSelect], Value);
							break;
						}
						case 4:{//Tuning Bank Select
							SetParamMSB(ChannelInfo.aParam[EParam_RPN_TuningBankSelect], Value);
							break;
						}
						case 5:{//Modulation Depth Range
							SetParamMSB(ChannelInfo.aParam[EParam_RPN_ModulationDepthRange], Value);
							break;
						}
					}
					break;
				}
				case EDataEntry_ModeNRPN:
				{
					switch (ChannelInfo.aParam[EParam_NRPN]){
						case 8:{//Vibrato Rate
							SetParamMSB(ChannelInfo.aParam[EParam_NRPN_VibratoRate], Value);
							break;
						}
						case 9:{//Vibrato Depth
							SetParamMSB(ChannelInfo.aParam[EParam_NRPN_VibratoDepth], Value);
							break;
						}
						case 10:{//Vibrato Delay
							SetParamMSB(ChannelInfo.aParam[EParam_NRPN_VibratoDelay], Value);
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case 7:{//Volume
			SetParamMSB(ChannelInfo.aParam[EParam_Volume], Value);
			SetItem(MidiChannel, EColumn_Vol, ChannelInfo.aParam[EParam_Volume]>>7, _T("%3d"));
			
			if (pCallback) pCallback->MidiVolume(MidiChannel, GetVolume(MidiChannel));
			break;
		}
		case 8:{//Balance
			SetParamMSB(ChannelInfo.aParam[EParam_Balance], Value);
			break;
		}
		case 10:{//Pan
			SetParamMSB(ChannelInfo.aParam[EParam_Pan], Value);
			SetItem(MidiChannel, EColumn_Pan, ChannelInfo.aParam[EParam_Pan]>>7, _T("%3d"));
			
			if (pCallback) pCallback->MidiPan(MidiChannel, GetPan(MidiChannel));
			break;
		}
		case 11:{//Expression
			SetParamMSB(ChannelInfo.aParam[EParam_Expression], Value);
			SetItem(MidiChannel, EColumn_Exp, ChannelInfo.aParam[EParam_Expression]>>7, _T("%3d"));
			
			if (pCallback) pCallback->MidiExpression(MidiChannel, GetExpression(MidiChannel));
			break;
		}
		
		//LSB
		case 32:{//Bank Select
			SetParamLSB(ChannelInfo.aParam[EParam_BankSelect], Value);
			SetItem(MidiChannel, EColumn_Bank, ChannelInfo.aParam[EParam_BankSelect], _T("%04x"));
			
			if (pCallback) pCallback->MidiBankSelect(MidiChannel, GetBankSelect(MidiChannel));
			break;
		}
		case 33:{//Modulation Depth
			SetParamLSB(ChannelInfo.aParam[EParam_ModulationDepth], Value);
			SetItem(MidiChannel, EColumn_Mod, ChannelInfo.aParam[EParam_ModulationDepth]>>7, _T("%3d"));
			break;
		}
		case 36:{//Foot Pedal
			SetParamLSB(ChannelInfo.aParam[EParam_FootPedal], Value);
			break;
		}
		case 37:{//Portamento Time
			SetParamLSB(ChannelInfo.aParam[EParam_PortamentoTime], Value);
			SetItem(MidiChannel, EColumn_Port, ChannelInfo.aParam[EParam_PortamentoSwitch]>>6, ChannelInfo.aParam[EParam_PortamentoTime], _T("%d.%04x"));
			
			if (pCallback) pCallback->MidiPortamentoTime(MidiChannel, GetPortamentoTime(MidiChannel));
			break;
		}
		case 38:{//Data Entry
			//TRACE(_T("Data Entry LSB : %d : %d\n"), Value, ChannelInfo.DataEntry);
			switch (ChannelInfo.DataEntry){
				default:
				case EDataEntry_None:
				{
					break;
				}
				case EDataEntry_ModeRPN:
				{
					switch (ChannelInfo.aParam[EParam_RPN]){
						case 0:{//Pitch Bend Sensitivity
							SetParamLSB(ChannelInfo.aParam[EParam_RPN_PitchBendSensitivity], Value);
							SetItem(MidiChannel, EColumn_Bend, ChannelInfo.aParam[EParam_RPN_PitchBendSensitivity]>>7, ChannelInfo.aParam[EParam_PitchBend], _T("%3d.%04x"));
							break;
						}
						case 1:{//Master Fine Tuning
							SetParamLSB(ChannelInfo.aParam[EParam_RPN_MasterFineTuning], Value);
							break;
						}
						case 2:{//Master Course Tuning
							SetParamLSB(ChannelInfo.aParam[EParam_RPN_MasterCourseTuning], Value);
							break;
						}
						case 3:{//Tuning Program Select
							SetParamLSB(ChannelInfo.aParam[EParam_RPN_TuningProgramSelect], Value);
							break;
						}
						case 4:{//Tuning Bank Select
							SetParamLSB(ChannelInfo.aParam[EParam_RPN_TuningBankSelect], Value);
							break;
						}
						case 5:{//Modulation Depth Range
							SetParamLSB(ChannelInfo.aParam[EParam_RPN_ModulationDepthRange], Value);
							break;
						}
					}
					break;
				}
				case EDataEntry_ModeNRPN:
				{
					switch (ChannelInfo.aParam[EParam_NRPN]){
						case 8:{//Vibrato Rate
							SetParamLSB(ChannelInfo.aParam[EParam_NRPN_VibratoRate], Value);
							break;
						}
						case 9:{//Vibrato Depth
							SetParamLSB(ChannelInfo.aParam[EParam_NRPN_VibratoDepth], Value);
							break;
						}
						case 10:{//Vibrato Delay
							SetParamLSB(ChannelInfo.aParam[EParam_NRPN_VibratoDelay], Value);
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case 39:{//Volume
			SetParamLSB(ChannelInfo.aParam[EParam_Volume], Value);
			SetItem(MidiChannel, EColumn_Vol, ChannelInfo.aParam[EParam_Volume]>>7, _T("%3d"));
			break;
		}
		case 40:{//Balance
			SetParamLSB(ChannelInfo.aParam[EParam_Balance], Value);
			break;
		}
		case 42:{//Pan
			SetParamLSB(ChannelInfo.aParam[EParam_Pan], Value);
			SetItem(MidiChannel, EColumn_Pan, ChannelInfo.aParam[EParam_Pan]>>7, _T("%3d"));
			break;
		}
		case 43:{//Expression
			SetParamLSB(ChannelInfo.aParam[EParam_Expression], Value);
			SetItem(MidiChannel, EColumn_Exp, ChannelInfo.aParam[EParam_Expression]>>7, _T("%3d"));
			break;
		}
		
		// 
		case 64:{//Hold Pedal 1
			ChannelInfo.aParam[EParam_HoldPedal1] = Value;
			SetItem(MidiChannel, EColumn_Hold, ChannelInfo.aParam[EParam_HoldPedal1]>>6, _T("%3d"));
			
			if (pCallback) pCallback->MidiHoldPedal1(MidiChannel, Value);
			break;
		}
		case 65:{//Portamento Switch
			ChannelInfo.aParam[EParam_PortamentoSwitch] = Value;
			SetItem(MidiChannel, EColumn_Port, ChannelInfo.aParam[EParam_PortamentoSwitch]>>6, ChannelInfo.aParam[EParam_PortamentoTime], _T("%d.%04x"));
			
			if (pCallback) pCallback->MidiPortamentoSwitch(MidiChannel, Value);
			break;
		}
		case 66:{//Sostenuto Pedal
			ChannelInfo.aParam[EParam_SostenutoPedal] = Value;
			TRACE(_T("Sostenuto Pedal : %d : %d\n"), MidiChannel, Value);
			break;
		}
		case 67:{//Soft Pedal
			ChannelInfo.aParam[EParam_SoftPedal] = Value;
			TRACE(_T("Soft Pedal : %d : %d\n"), MidiChannel, Value);
			break;
		}
		case 68:{//Legato FootSwitch
			ChannelInfo.aParam[EParam_LegatoFootSwitch] = Value;
			TRACE(_T("Legato FootSwitch : %d : %d\n"), MidiChannel, Value);
			break;
		}
		case 69:{//Hold Pedal 2
			ChannelInfo.aParam[EParam_HoldPedal2] = Value;
			TRACE(_T("Hold Pedal 2 : %d : %d\n"), MidiChannel, Value);
			break;
		}
		case 70:{//Sound Controller 1 (Sound Variation)
			ChannelInfo.aParam[EParam_SoundVariation] = Value;
			TRACE(_T("Sound Variation : %d : %d\n"), MidiChannel, Value);
			break;
		}
		case 71:{//Sound Controller 2 (Harmonic Contents)
			ChannelInfo.aParam[EParam_HarmonicContents] = Value;
			TRACE(_T("Harmonic Contents : %d : %d\n"), MidiChannel, Value);
			break;
		}
		case 72:{//Sound Controller 3 (Release Time)
			ChannelInfo.aParam[EParam_ReleaseTime] = Value;
			TRACE(_T("Release Time : %d : %d\n"), MidiChannel, Value);
			break;
		}
		case 73:{//Sound Controller 4 (Attack Time)
			ChannelInfo.aParam[EParam_AttackTime] = Value;
			TRACE(_T("Attack Time : %d : %d\n"), MidiChannel, Value);
			break;
		}
		case 74:{//Sound Controller 5 (Brightness)
			ChannelInfo.aParam[EParam_Brightness] = Value;
			TRACE(_T("Brightness : %d : %d\n"), MidiChannel, Value);
			break;
		}
		case 75:{//Sound Controller 6 (Decay Time)
			ChannelInfo.aParam[EParam_DecayTime] = Value;
			TRACE(_T("Decay Time : %d : %d\n"), MidiChannel, Value);
			break;
		}
		case 76:{//Sound Controller 7 (Vibrato Rate)
			ChannelInfo.aParam[EParam_VibratoRate] = Value;
			TRACE(_T("Vibrato Rate : %d : %d\n"), MidiChannel, Value);
			break;
		}
		case 77:{//Sound Controller 8 (Vibato Depth)
			ChannelInfo.aParam[EParam_VibatoDepth] = Value;
			TRACE(_T("Vibato Depth : %d : %d\n"), MidiChannel, Value);
			break;
		}
		case 78:{//Sound Controller 9 (Vibrato Delay)
			ChannelInfo.aParam[EParam_VibratoDelay] = Value;
			TRACE(_T("Vibrato Delay : %d : %d\n"), MidiChannel, Value);
			break;
		}
		case 79:{//Sound Controller 10
			ChannelInfo.aParam[EParam_SoundController10] = Value;
			TRACE(_T("Sound Controller 10 : %d : %d\n"), MidiChannel, Value);
			break;
		}
		case 84:{//Portament Control
			ChannelInfo.aParam[EParam_PortamentControl] = Value;
			TRACE(_T("Portament Control : %d : %d\n"), MidiChannel, Value);
			break;
		}
		
		// 
		case 96:{//Data Increment
			TRACE(_T("Data Increment : %d : %d\n"), Value, ChannelInfo.DataEntry);
			switch (ChannelInfo.DataEntry){
				default:
				case EDataEntry_None:
				{
					break;
				}
				case EDataEntry_ModeRPN:
				{
					switch (ChannelInfo.aParam[EParam_RPN]){
						case 0:{//Pitch Bend Sensitivity
							ChannelInfo.aParam[EParam_RPN_PitchBendSensitivity]++;
							SetItem(MidiChannel, EColumn_Bend, ChannelInfo.aParam[EParam_RPN_PitchBendSensitivity]>>7, ChannelInfo.aParam[EParam_PitchBend], _T("%3d.%04x"));
							break;
						}
						case 1:{//Master Fine Tuning
							ChannelInfo.aParam[EParam_RPN_MasterFineTuning]++;
							break;
						}
						case 2:{//Master Course Tuning
							ChannelInfo.aParam[EParam_RPN_MasterCourseTuning]++;
							break;
						}
						case 3:{//Tuning Program Select
							ChannelInfo.aParam[EParam_RPN_TuningProgramSelect]++;
							break;
						}
						case 4:{//Tuning Bank Select
							ChannelInfo.aParam[EParam_RPN_TuningBankSelect]++;
							break;
						}
						case 5:{//Modulation Depth Range
							ChannelInfo.aParam[EParam_RPN_ModulationDepthRange]++;
							break;
						}
					}
					break;
				}
				case EDataEntry_ModeNRPN:
				{
					switch (ChannelInfo.aParam[EParam_NRPN]){
						case 8:{//Vibrato Rate
							ChannelInfo.aParam[EParam_NRPN_VibratoRate]++;
							break;
						}
						case 9:{//Vibrato Depth
							ChannelInfo.aParam[EParam_NRPN_VibratoDepth]++;
							break;
						}
						case 10:{//Vibrato Delay
							ChannelInfo.aParam[EParam_NRPN_VibratoDelay]++;
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case 97:{//Data Decrement
			TRACE(_T("Data Decrement : %d : %d\n"), Value, ChannelInfo.DataEntry);
			switch (ChannelInfo.DataEntry){
				default:
				case EDataEntry_None:
				{
					break;
				}
				case EDataEntry_ModeRPN:
				{
					switch (ChannelInfo.aParam[EParam_RPN]){
						case 0:{//Pitch Bend Sensitivity
							ChannelInfo.aParam[EParam_RPN_PitchBendSensitivity]--;
							SetItem(MidiChannel, EColumn_Bend, ChannelInfo.aParam[EParam_RPN_PitchBendSensitivity]>>7, ChannelInfo.aParam[EParam_PitchBend], _T("%3d.%04x"));
							break;
						}
						case 1:{//Master Fine Tuning
							ChannelInfo.aParam[EParam_RPN_MasterFineTuning]--;
							break;
						}
						case 2:{//Master Course Tuning
							ChannelInfo.aParam[EParam_RPN_MasterCourseTuning]--;
							break;
						}
						case 3:{//Tuning Program Select
							ChannelInfo.aParam[EParam_RPN_TuningProgramSelect]--;
							break;
						}
						case 4:{//Tuning Bank Select
							ChannelInfo.aParam[EParam_RPN_TuningBankSelect]--;
							break;
						}
						case 5:{//Modulation Depth Range
							ChannelInfo.aParam[EParam_RPN_ModulationDepthRange]--;
							break;
						}
					}
					break;
				}
				case EDataEntry_ModeNRPN:
				{
					switch (ChannelInfo.aParam[EParam_NRPN]){
						case 8:{//Vibrato Rate
							ChannelInfo.aParam[EParam_NRPN_VibratoRate]--;
							break;
						}
						case 9:{//Vibrato Depth
							ChannelInfo.aParam[EParam_NRPN_VibratoDepth]--;
							break;
						}
						case 10:{//Vibrato Delay
							ChannelInfo.aParam[EParam_NRPN_VibratoDelay]--;
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case 98:{//NRPN LSB
			SetParamLSB(ChannelInfo.aParam[EParam_NRPN], Value);
			ChannelInfo.DataEntry = (ChannelInfo.aParam[EParam_NRPN] != 0x3fff)? EDataEntry_ModeNRPN: EDataEntry_None;
			//TRACE(_T("NRPN LSB : %d : %d\n"), Value, ChannelInfo.DataEntry);
			break;
		}
		case 99:{//NRPN MSB
			SetParamMSB(ChannelInfo.aParam[EParam_NRPN], Value);
			ChannelInfo.DataEntry = EDataEntry_None;
			//TRACE(_T("NRPN MSB : %d : %d\n"), Value, ChannelInfo.DataEntry);
			break;
		}
		case 100:{//RPN LSB
			SetParamLSB(ChannelInfo.aParam[EParam_RPN], Value);
			ChannelInfo.DataEntry = (ChannelInfo.aParam[EParam_RPN] != 0x3fff)? EDataEntry_ModeRPN: EDataEntry_None;
			//TRACE(_T("RPN LSB : %d : %d\n"), Value, ChannelInfo.DataEntry);
			break;
		}
		case 101:{//RPN MSB
			SetParamMSB(ChannelInfo.aParam[EParam_RPN], Value);
			ChannelInfo.DataEntry = EDataEntry_None;
			//TRACE(_T("RPN MSB : %d : %d\n"), Value, ChannelInfo.DataEntry);
			break;
		}
		
		// 
		case 120:{//All Sound Off
			break;
		}
		case 121:{//Reset All Controllers
			break;
		}
		case 122:{//Local Switch on/off
			ChannelInfo.aParam[EParam_LocalSwitch] = Value;
			TRACE(_T("Local Switch : %d : %d\n"), MidiChannel, Value);
			break;
		}
		case 123:{//All Notes Off
			break;
		}
		case 124:{//Omni Mode Off
			TRACE(_T("Omni Mode Off : %d\n"), MidiChannel);
			break;
		}
		case 125:{//Omni Mode On
			TRACE(_T("Omni Mode On : %d\n"), MidiChannel);
			break;
		}
		case 126:{//Mono Mode
			TRACE(_T("Mono Mode : %d\n"), MidiChannel);
			break;
		}
		case 127:{//Poly Mode
			TRACE(_T("Poly Mode : %d\n"), MidiChannel);
			break;
		}
	}
}



void CMidiTab::NoteOn(i8 MidiChannel, i8 MidiNote)
{
	auto& ChannelInfo = m_aChannelInfo[MidiChannel];
	auto& NoteOn = ChannelInfo.aNoteOn[MidiNote];
	NoteOn++;
	
	ChannelInfo.aText[EColumn_Channel] = 1.0f;
	ChannelInfo.aBack[EColumn_Channel] = 1.0f;
}



void CMidiTab::NoteOff(i8 MidiChannel, i8 MidiNote)
{
	auto& ChannelInfo = m_aChannelInfo[MidiChannel];
	auto& NoteOn = ChannelInfo.aNoteOn[MidiNote];
	NoteOn = (NoteOn > 0)? NoteOn-1: 0;
}



float CMidiTab::GetPan(i8 MidiChannel)
{
	auto& ChannelInfo = m_aChannelInfo[MidiChannel];
	return ((ChannelInfo.aParam[EParam_Pan]>>7) - 0x40) * (1.0f / 0x3f);
}



float CMidiTab::GetVolume(i8 MidiChannel)
{
	auto& ChannelInfo = m_aChannelInfo[MidiChannel];
	return (ChannelInfo.aParam[EParam_Volume]>>7) * (1.0f / 0x7f);
}



float CMidiTab::GetExpression(i8 MidiChannel)
{
	auto& ChannelInfo = m_aChannelInfo[MidiChannel];
	return (ChannelInfo.aParam[EParam_Expression]>>7) * (1.0f / 0x7f);
}



float CMidiTab::GetModulationDepth(i8 MidiChannel)
{
	auto& ChannelInfo = m_aChannelInfo[MidiChannel];
	return (ChannelInfo.aParam[EParam_ModulationDepth]>>7) * (1.0f / 0x7f);
}



float CMidiTab::GetPitchBend(i8 MidiChannel)
{
	auto& ChannelInfo = m_aChannelInfo[MidiChannel];
	
	auto PitchBend = (ChannelInfo.aParam[EParam_PitchBend] - 0x2000) * (1.0f / 0x1fff);
	auto PitchBendSensitivity = ChannelInfo.aParam[EParam_RPN_PitchBendSensitivity]>>7;
	return PitchBend * PitchBendSensitivity;
}



float CMidiTab::GetVibratoRate(i8 MidiChannel)
{
	auto& ChannelInfo = m_aChannelInfo[MidiChannel];
	return ChannelInfo.aParam[EParam_VibratoRate] * (1.0f / 0x7f);
}



float CMidiTab::GetVibatoDepth(i8 MidiChannel)
{
	auto& ChannelInfo = m_aChannelInfo[MidiChannel];
	return ChannelInfo.aParam[EParam_VibatoDepth] * (1.0f / 0x7f);
}



int CMidiTab::GetProgram(i8 MidiChannel)
{
	auto& ChannelInfo = m_aChannelInfo[MidiChannel];
	return ChannelInfo.aParam[EParam_Program];
}



int CMidiTab::GetBankSelect(i8 MidiChannel)
{
	auto& ChannelInfo = m_aChannelInfo[MidiChannel];
	return ChannelInfo.aParam[EParam_BankSelect];
}



float CMidiTab::GetPortamentoTime(i8 MidiChannel)
{
	auto& ChannelInfo = m_aChannelInfo[MidiChannel];
	return ChannelInfo.aParam[EParam_PortamentoTime] * (1.0f / 0x3fff);
}



void CMidiTab::SetParamMSB(int16_t& Param, BYTE Value)
{
	Param &= ~(0x7f<<7);
	Param |= (Value<<7);
}



void CMidiTab::SetParamLSB(int16_t& Param, BYTE Value)
{
	Param &= ~(0x7f<<0);
	Param |= (Value<<0);
}



void CMidiTab::SetItem(i8 MidiChannel, EColumn Column, CString Text)
{
	auto& aRow = m_StaticList.GetRows();
	aRow[MidiChannel].aColumnInfo[Column].Cell.SetText(Text);
	
	auto& ChannelInfo = m_aChannelInfo[MidiChannel];
	ChannelInfo.aText[Column] = 1.0f;
	ChannelInfo.aBack[Column] = 1.0f;
}



void CMidiTab::SetItem(i8 MidiChannel, EColumn Column, int16_t Value, LPCWSTR Format)
{
	CString Text;
	Text.Format(Format, Value);
	SetItem(MidiChannel, Column, Text);
}



void CMidiTab::SetItem(i8 MidiChannel, EColumn Column, int16_t Value1, int16_t Value2, LPCWSTR Format)
{
	CString Text;
	Text.Format(Format, Value1, Value2);
	SetItem(MidiChannel, Column, Text);
}



void CMidiTab::SetMidiChannelType(i8 MidiChannel, CMidiTab::EChannelType Type)
{
	auto& ChannelInfo = m_aChannelInfo[MidiChannel];
	ChannelInfo.Type = Type;
	ChannelInfo.bInvalidate = true;
}
