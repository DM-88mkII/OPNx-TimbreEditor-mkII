
#pragma once



#include "FmChip.h"
#include "Common/Common.h"
#include "Chip.h"
#include "Thread.h"



class CChipYM2608 final : public CChip
{
	public:
		virtual ~CChipYM2608();
		
		CChipYM2608(int SampleRate, size_t RenderSize);
		
		void Render() override;
		void Wait() override;
		int32_t* Data() override { return m_aRender.data(); }
		HANDLE GetThreadHandle() override { return m_CThread.GetHandle(); }
		
		void SubmitSourceBuffer() override;
		
		void Play(i8 Channel, i8 Note, sptr<CTimbre> pTimbre) override;
		void Stop(i8 Channel) override;
		
		void Pan(i8 Channel, float Value, bool bUpdate) override;
		void Volume(i8 Channel, float Value, bool bUpdate) override;
		void Velocity(i8 Channel, float Value, bool bUpdate) override;
		void Expression(i8 Channel, float Value, bool bUpdate) override;
		void PitchBend(i8 Channel, float Value, bool bUpdate) override;
		void ModulationDepth(i8 Channel, float Value, bool bUpdate) override;
		void VibratoRate(i8 Channel, float Value, bool bUpdate) override;
		void VibatoDepth(i8 Channel, float Value, bool bUpdate) override;
		void PortamentoNote(i8 Channel, i8 Value, bool bUpdate) override;
		void PortamentoTime(i8 Channel, float Value, bool bUpdate) override;
		
		void SetPortamentoMode(EPortamentoMode PortamentoMode) override { m_PortamentoMode = PortamentoMode; }
		void SetTuning(float Tuning) override { m_Tuning = Tuning; }
	
	public:
		static constexpr int ChipChannels = 6;
	
	private:
		enum EState
		{
			EState_Stopped,
			EState_KeyOff,
			EState_KeyOffDelay = EState_KeyOff+1,
			EState_ForceKeyOn,
			EState_ForceDump = EState_ForceKeyOn+1,//ForceKeyOn+5,
			EState_Playing,
			EState_KeyOn,
			EState_Dump = EState_KeyOn+1,//KeyOn+5,
		};
		struct ChannelInfo
		{
			std::atomic<int> State;
			
			i8 Channel;
			i8 Note;
			
			float Pan;
			float Volume;
			float Velocity;
			float Expression;
			float PitchBend;
			float ModulationDepth;
			float VibratoRate;
			float VibatoDepth;
			i8 PortamentoNote;
			float PortamentoTime;
			
			float ModulationPhase;
			float Vibrato;
			float Portamento;
			
			sptr<CTimbre> pTimbreReserve;
			sptr<CTimbre> pTimbrePlay;
			
			bool bSe;
			
			i16 Duration;
		};
		ChannelInfo m_aChannelInfo[ChipChannels];
		
		uint32_t output_rate;
		emulated_time output_step;
		emulated_time output_pos;
		uptr<FmChipBase> m_pFmChip;
		
		std::vector<int32_t> m_aRender;
		
		EPortamentoMode m_PortamentoMode;
		float m_Tuning;
		
		CThread m_CThread;
	
	private:
		float CalcFrequency(float BaseFreq, float Note);
		float CalcFNumber(float Freq);
		void BlockFNumber(ChannelInfo& ChannelInfo, i8 Note, float PitchBend, int RegH, int RegL, int KT, int DT);
		
		void StateKeyOff(ChannelInfo& ChannelInfo);
		void StateKeyOn(ChannelInfo& ChannelInfo);
		void StateDump(ChannelInfo& ChannelInfo);
		void StateDefault(ChannelInfo& ChannelInfo);
		
		void UpdateState();
		
		void Pan(ChannelInfo& ChannelInfo);
		void Level(ChannelInfo& ChannelInfo);
		void BlockFNumber(ChannelInfo& ChannelInfo);
		
		EPortamentoMode GetPortamentoMode(){ return m_PortamentoMode; }
		float GetTuning(){ return m_Tuning; }
		
		void CThreadProc();
		static UINT CThreadProc(LPVOID pParam);
};
