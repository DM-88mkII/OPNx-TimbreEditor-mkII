
#pragma once



#include <list>
#include <queue>
#include <mmsystem.h>
#include "Audio.h"
#include "Voice.h"
#include "InstTab.h"
#include "DrumTab.h"
#include "MidiTab.h"
#include "ModuleTab.h"
#include "SettingTab.h"
#include "RingQueue.h"
#include "BatchDlg.h"

#pragma comment(lib, "winmm.lib")



class CFmStationDlg : public CDialogEx, public IMidiCallback
{
	public:
		~CFmStationDlg();
		
		CFmStationDlg(CWnd* pParent = nullptr);
		
		#ifdef AFX_DESIGN_TIME//[
		enum { IDD = IDD_FMSTATION_DIALOG };
		#endif//]
	
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);
	
	protected:
		HICON m_hIcon;
		
		CComboBox m_CComboBoxDevice;
		
		CTabCtrl m_CTabCtrl;
		CInstTab m_CInstTab;
		CDrumTab m_CDrumTab;
		CMidiTab m_CMidiTab;
		CModuleTab m_CModuleTab;
		CSettingTab m_CSettingTab;
		CStatic m_CStaticLog;
	
	
	public:
		static constexpr int MidiChannels = 16;
		static constexpr int MidiNotes = 128;
		static constexpr int ChipNum = 8;
		static constexpr int ChipChannels = 6;
	
	protected:
		HMIDIIN m_hMidiIn;
		MIDIHDR m_MidiHdr;
		
		char m_aSysExBuffer[1024];
		
		uptr<CAudio> m_pAudio;
		uptr<CVoice> m_pVoice;
		
		u32 m_NoteOnId;
		
		struct ChipInfo
		{
			i8 ChipId;
			i8 ChipChannel;
			i8 MidiChannel;
			i8 MidiNote;
			u32 NoteOnId;
		};
		std::queue<ChipInfo> m_ChipOff;
		std::queue<ChipInfo> m_ChipOffSE;
		std::vector<ChipInfo> m_ChipOn;
		std::vector<ChipInfo> m_ChipOnSE;
		
		struct VoiceInfo
		{
			i8 MidiChannel;
			i8 MidiNote;
			sptr<CBatch> pBatch;
		};
		struct MidiChannelInfo
		{
			CMidiTab::EChannelType Type;
			
			int Bank;
			int Program;
			float PortamentoTime;
			i8 LatestNote;
			bool bSustainPedal;
			bool bPortamentoSwitch;
			
			std::queue<ChipInfo> aChipOn[MidiNotes];
			RingQueue<ChipInfo, (ChipNum * ChipChannels)> aChipInfo;
			
			std::queue<VoiceInfo> aVoiceOn[MidiNotes];
			std::queue<i8> VoiceHoldNote;
		};
		MidiChannelInfo m_aMidiChannelInfo[MidiChannels];
	
	
	protected:
		virtual BOOL OnInitDialog();
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		
		afx_msg void OnPaint();
		afx_msg HCURSOR OnQueryDragIcon();
		afx_msg void OnCbnSelchangeComboDevice();
		afx_msg void OnSelchangeTabcontrol(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnBnClickedButtonReset();
		afx_msg void OnBnClickedButtonLoad();
		afx_msg void OnBnClickedButtonSave();
		
		DECLARE_MESSAGE_MAP()
		
		void Load(std::filesystem::path Path);
		void Save(std::filesystem::path Path);
		
		void MidiInClose();
		void MidiInOpen();
		
		static void CALLBACK StaticMidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
		void MidiInProc(BYTE status, BYTE data1, BYTE data2);
		
		void MidiInAddBuffer(){ midiInAddBuffer(m_hMidiIn, &m_MidiHdr, sizeof(MIDIHDR)); }
		
		void SystemReset();
		void RemoveMidiChannelChipInfo(ChipInfo ChipInfo);
		
		bool NoteOff(i8 MidiChannel, i8 MidiNote, int MidiVelocity, sptr<CTimbre> pTimbre);
		bool NoteOn(i8 MidiChannel, i8 MidiNote, int MidiVelocity, sptr<CTimbre> pTimbre);
		
		void SetMidiChannelType(i8 MidiChannel, CMidiTab::EChannelType Type);
	
	public:
		CSettingTab& GetSettingTab(){ return m_CSettingTab; }
		
		void SetVolume(double Volume);
		void VolumeUp();
		void VolumeDown();
		
		i8 GetPreview();
		void SetPreview(i8 Preview);
		void PreviewUp();
		void PreviewDown();
		
		const uptr<CVoice>& GetVoice(){ return m_pVoice; }
		
		void WavRecord();
		
		template<class... Args> void Log(std::wformat_string<Args...> fmt, Args&& ... args);
	
	public:
		bool MidiNoteOff(i8 MidiChannel, i8 MidiNote, int MidiVelocity) override;
		bool MidiNoteOff(i8 MidiChannel, i8 MidiNote, int MidiVelocity, sptr<CTimbre> pTimbre) override;
		bool MidiNoteOn(i8 MidiChannel, i8 MidiNote, int MidiVelocity) override;
		bool MidiNoteOn(i8 MidiChannel, i8 MidiNote, int MidiVelocity, sptr<CTimbre> pTimbre) override;
		
		bool MidiPan(i8 MidiChannel, float MidiValue) override;
		bool MidiVolume(i8 MidiChannel, float MidiValue) override;
		bool MidiExpression(i8 MidiChannel, float MidiValue) override;
		bool MidiModulationDepth(i8 MidiChannel, float MidiValue) override;
		bool MidiHoldPedal1(i8 MidiChannel, i8 MidiValue) override;
		bool MidiPitchBend(i8 MidiChannel, float MidiValue) override;
		bool MidiVibratoRate(i8 MidiChannel, float MidiValue) override;
		bool MidiVibatoDepth(i8 MidiChannel, float MidiValue) override;
		bool MidiProgramChange(i8 MidiChannel, int MidiValue) override;
		bool MidiPortamentoTime(i8 MidiChannel, float MidiValue) override;
		bool MidiPortamentoSwitch(i8 MidiChannel, i8 MidiValue) override;
		bool MidiBankSelect(i8 MidiChannel, int MidiValue) override;
		
		bool MidiSystemReset() override;
};
