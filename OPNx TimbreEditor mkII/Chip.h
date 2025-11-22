
#pragma once



#include <vector>
#include <atomic>
#include "Common/Common.h"



class CTimbre;



class CChip
{
	public:
		enum EPortamentoMode
		{
			EPortamentoMode_TimeMode,
			EPortamentoMode_RateMode,
		};
	
	public:
		virtual ~CChip() = default;
		
		CChip() = default;
		
		virtual void Render() = 0;
		virtual void Wait() = 0;
		virtual int32_t* Data() = 0;
		virtual HANDLE GetThreadHandle() = 0;
		
		virtual void SubmitSourceBuffer() = 0;
		
		virtual void Play(i8 Channel, i8 Note, sptr<CTimbre> pTimbre) = 0;
		virtual void Stop(i8 Channel) = 0;
		
		virtual void Pan(i8 Channel, float Value, bool bUpdate = false) = 0;
		virtual void Volume(i8 Channel, float Value, bool bUpdate = false) = 0;
		virtual void Velocity(i8 Channel, float Value, bool bUpdate = false) = 0;
		virtual void Expression(i8 Channel, float Value, bool bUpdate = false) = 0;
		virtual void PitchBend(i8 Channel, float Value, bool bUpdate = false) = 0;
		virtual void ModulationDepth(i8 Channel, float Value, bool bUpdate = false) = 0;
		virtual void VibratoRate(i8 Channel, float Value, bool bUpdate = false) = 0;
		virtual void VibatoDepth(i8 Channel, float Value, bool bUpdate = false) = 0;
		virtual void PortamentoNote(i8 Channel, i8 Value, bool bUpdate = false) = 0;
		virtual void PortamentoTime(i8 Channel, float Value, bool bUpdate = false) = 0;
		
		virtual void SetPortamentoMode(EPortamentoMode PortamentoMode) = 0;
		virtual void SetTuning(float Tuning) = 0;
};
