
#pragma once



#include "afxdialogex.h"
#include "StaticList.h"



class IMidiCallback
{
	public:
		virtual ~IMidiCallback() = default;
		
		IMidiCallback() = default;
		
		virtual bool MidiNoteOff(i8 MidiChannel, i8 MidiNote, int MidiVelocity) = 0;
		virtual bool MidiNoteOff(i8 MidiChannel, i8 MidiNote, int MidiVelocity, sptr<CTimbre> pTimbre) = 0;
		virtual bool MidiNoteOn(i8 MidiChannel, i8 MidiNote, int MidiVelocity) = 0;
		virtual bool MidiNoteOn(i8 MidiChannel, i8 MidiNote, int MidiVelocity, sptr<CTimbre> pTimbre) = 0;
		
		virtual bool MidiPan(i8 MidiChannel, float MidiValue) = 0;
		virtual bool MidiVolume(i8 MidiChannel, float MidiValue) = 0;
		virtual bool MidiExpression(i8 MidiChannel, float MidiValue) = 0;
		virtual bool MidiModulationDepth(i8 MidiChannel, float MidiValue) = 0;
		virtual bool MidiHoldPedal1(i8 MidiChannel, i8 MidiValue) = 0;
		virtual bool MidiPitchBend(i8 MidiChannel, float MidiValue) = 0;
		virtual bool MidiVibratoRate(i8 MidiChannel, float MidiValue) = 0;
		virtual bool MidiVibatoDepth(i8 MidiChannel, float MidiValue) = 0;
		virtual bool MidiProgramChange(i8 MidiChannel, int MidiValue) = 0;
		virtual bool MidiPortamentoTime(i8 MidiChannel, float MidiValue) = 0;
		virtual bool MidiPortamentoSwitch(i8 MidiChannel, i8 MidiValue) = 0;
		virtual bool MidiBankSelect(i8 MidiChannel, int MidiValue) = 0;
		
		virtual bool MidiSystemReset() = 0;
};



class CMidiTab : public CDialogEx
{
	DECLARE_DYNAMIC(CMidiTab)
	
	public:
		virtual ~CMidiTab();
		
		CMidiTab(CWnd* pParent = nullptr);
		
		#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_MIDI_TAB };
		#endif
	
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);
		virtual BOOL OnInitDialog();
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		
		DECLARE_MESSAGE_MAP()
	
	public:
		afx_msg void OnTimer(UINT_PTR nIDEvent);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		
		void MidiInProc(IMidiCallback* pCallback, BYTE status, BYTE data1, BYTE data2);
		
		void ResetAllControllers(i8 MidiChannel);
		void ControlChange(IMidiCallback* pCallback, i8 MidiChannel, BYTE CC, BYTE Value);
		void NoteOn(i8 MidiChannel, i8 MidiNote);
		void NoteOff(i8 MidiChannel, i8 MidiNote);
		
		float GetPan(i8 MidiChannel);
		float GetVolume(i8 MidiChannel);
		float GetExpression(i8 MidiChannel);
		float GetModulationDepth(i8 MidiChannel);
		float GetPitchBend(i8 MidiChannel);
		float GetVibratoRate(i8 MidiChannel);
		float GetVibatoDepth(i8 MidiChannel);
		int GetProgram(i8 MidiChannel);
		int GetBankSelect(i8 MidiChannel);
		float GetPortamentoTime(i8 MidiChannel);
	
	protected:
		CFmStationDlg& GetFmStationDlg();
	
	public:
		enum EChannelType
		{
			EChannelType_Inst,
			EChannelType_Drum,
		};
	
	protected:
		static constexpr int MidiChannels = 16;
		static constexpr int MidiNotes = 128;
		
		enum EParam
		{
			EParam_Program,
			
			EParam_PitchBend,
			
			EParam_BankSelect,
			EParam_ModulationDepth,
			EParam_FootPedal,
			EParam_PortamentoTime,
			EParam_Volume,
			EParam_Balance,
			EParam_Pan,
			EParam_Expression,
			
			EParam_HoldPedal1,
			EParam_PortamentoSwitch,
			EParam_SostenutoPedal,
			EParam_SoftPedal,
			EParam_LegatoFootSwitch,
			EParam_HoldPedal2,
			
			EParam_SoundVariation,
			EParam_HarmonicContents,
			EParam_ReleaseTime,
			EParam_AttackTime,
			EParam_Brightness,
			EParam_DecayTime,
			EParam_VibratoRate,
			EParam_VibatoDepth,
			EParam_VibratoDelay,
			EParam_SoundController10,
			
			EParam_PortamentControl,
			
			// 
			EParam_RPN,
			EParam_NRPN,
			
			// RPN
			EParam_RPN_PitchBendSensitivity,
			EParam_RPN_MasterFineTuning,
			EParam_RPN_MasterCourseTuning,
			EParam_RPN_TuningProgramSelect,
			EParam_RPN_TuningBankSelect,
			EParam_RPN_ModulationDepthRange,
			
			// NPRN
			EParam_NRPN_VibratoRate,
			EParam_NRPN_VibratoDepth,
			EParam_NRPN_VibratoDelay,
			
			// 
			EParam_LocalSwitch,
			
			// 
			EParam_size,
		};
		enum EDataEntry
		{
			EDataEntry_None,
			EDataEntry_ModeRPN,
			EDataEntry_ModeNRPN,
		};
		enum EColumn
		{
			EColumn_Channel,
			EColumn_Program,
			EColumn_Bank,
			EColumn_Pan,
			EColumn_Vol,
			EColumn_Exp,
			EColumn_Hold,
			EColumn_Mod,
			EColumn_Bend,
			EColumn_Port,
			
			EColumn_Keyboard,
			
			EColumn_size,
		};
		
		CListCtrl m_CListCtrl;
		
		using StaticList = CStaticList<MidiChannels, EColumn_size>;
		StaticList m_StaticList;
		UINT_PTR m_timerID;
		
		CBitmap m_BmpWhiteOffRight;
		CBitmap m_BmpWhiteOffBoth;
		CBitmap m_BmpWhiteOffLeft;
		CBitmap m_BmpWhiteOnRight;
		CBitmap m_BmpWhiteOnBoth;
		CBitmap m_BmpWhiteOnLeft;
		CBitmap m_BmpBlackOff;
		CBitmap m_BmpBlackOn;
		
		struct ChannelInfo
		{
			EChannelType Type;
			
			int16_t aParam[EParam_size];
			EDataEntry DataEntry;
			
			int32_t aNoteOn[MidiNotes];
			bool abNoteState[MidiNotes];
			
			float aText[EColumn_size];
			float aBack[EColumn_size];
			
			bool bInvalidate;
		};
		ChannelInfo m_aChannelInfo[MidiChannels];
		
		void SetParamMSB(int16_t& Param, BYTE Value);
		void SetParamLSB(int16_t& Param, BYTE Value);
		
		void SetItem(i8 MidiChannel, EColumn Column, CString Text = _T(""));
		void SetItem(i8 MidiChannel, EColumn Column, int16_t Value, LPCWSTR Format = _T("%d"));
		void SetItem(i8 MidiChannel, EColumn Column, int16_t Value1, int16_t Value2, LPCWSTR Format = _T("%d.%d"));
	
	public:
		void SetMidiChannelType(i8 MidiChannel, CMidiTab::EChannelType Type);
};
