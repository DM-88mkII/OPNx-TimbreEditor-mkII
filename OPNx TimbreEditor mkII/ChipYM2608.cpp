



#include "pch.h"
#include "framework.h"
#include "FmStation.h"
#include "FmStationDlg.h"
#include "ChipYM2608.h"
#include <numbers>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CChipYM2608::~CChipYM2608()
{
	m_CThread.Exit();
}



CChipYM2608::CChipYM2608(int SampleRate, size_t RenderSize)
:m_aChannelInfo{{0,0},{0,1},{0,2},{0,3},{0,4},{0,5},}
,output_rate(SampleRate)
,output_step(0x100000000ull / output_rate)
,output_pos(0)
,m_pFmChip(std::make_unique<FmChip<ymfm::ym2608>>(output_rate * 144, EChipType::YM2608))
,m_aRender(RenderSize)
,m_Tuning(440.0f)
{
	m_pFmChip->write(0x29, 0x80);// OPNA Mode
	
	for (int Channel = 0; Channel < ChipChannels; Channel++){
		auto& ChannelInfo = m_aChannelInfo[Channel];
		ChannelInfo.pTimbreReserve = std::make_shared<CTimbre>();
		ChannelInfo.pTimbrePlay = std::make_shared<CTimbre>();
	}
}



void CChipYM2608::Render()
{
	m_CThread.Proc(CThreadProc, this);
}



void CChipYM2608::Wait()
{
	m_CThread.Wait();
}



void CChipYM2608::SubmitSourceBuffer()
{
	auto a = m_aRender.data();
	auto n = m_aRender.size();
	for (int i = 0; i < n; i += 2){
		m_pFmChip->generate(output_pos, output_step, &a[i]);
		output_pos += output_step;
	}
	
	UpdateState();
}



float CChipYM2608::CalcFrequency(float BaseFreq, float Note)
{
	const float OctaveNotes = 12.0f;
	auto Octave = std::floor(Note / OctaveNotes) - 6.0f;
	Note = std::fmodf(Note, OctaveNotes);
	return BaseFreq * std::powf(2.0f, (Octave + (Note / OctaveNotes)));
}



float CChipYM2608::CalcFNumber(float Freq)
{
	return Freq * (150994944.0f / 8000000);
}



void CChipYM2608::BlockFNumber(ChannelInfo& ChannelInfo, i8 Note, float FineTune, int RegH, int RegL, int KT, int DT)
{
	auto& Control = ChannelInfo.pTimbrePlay->Control;
	auto& aOperator = ChannelInfo.pTimbrePlay->aOperator;
	
	Note += KT;
	Note += Control.KT.GetValue();
	Note = (Note > 0x00)? Note: 0x00;
	Note = (Note < 0x7f)? Note: 0x7f;
	
	auto BaseA = (Note + 3) + FineTune;
	auto Freq = CalcFrequency(GetTuning(), BaseA);
	auto FNum = CalcFNumber(Freq);
	
	auto BaseCs = (int)std::floor(BaseA + 8);
	auto OctaveCs = BaseCs / 12;
	auto Block = OctaveCs - 3;
	Block = (Block > 0)? Block: 0;
	Block = (Block < 7)? Block: 7;
	
	auto FNumber = ((int)std::round(FNum)) >> Block;
	FNumber += DT;
	FNumber += Control.FDT.GetValue();
	FNumber = (FNumber > 0x000)? FNumber: 0x000;
	FNumber = (FNumber < 0x7ff)? FNumber: 0x7ff;
	
	auto BlockFNumber = (Block<<11) | FNumber;
	m_pFmChip->write(RegH, BlockFNumber>>8);
	m_pFmChip->write(RegL, BlockFNumber&0xff);
}



void CChipYM2608::StateKeyOff(ChannelInfo& ChannelInfo)
{
	auto& Note = ChannelInfo.Note;
	auto Channel = ChannelInfo.Channel;
	auto ChannelL = Channel % 3;
	auto ChannelH = Channel / 3;
	
	m_pFmChip->write(0x28, ((ChannelH<<2) | ChannelL));
}



#if false//[
static const i8 aTL[]={
	//0   1   2   3   4   5   6   7    8   9   a   b   c   d   e   f
	  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  1,  1,  1,  1,  1,//00
	  1,  1,  1,  1,  2,  2,  2,  2,   2,  2,  2,  2,  2,  3,  3,  3,//10
	  3,  3,  3,  3,  3,  4,  4,  4,   4,  4,  4,  4,  4,  5,  5,  5,//20
	  5,  5,  5,  6,  6,  6,  6,  6,   6,  6,  7,  7,  7,  7,  7,  8,//30
	  8,  8,  8,  8,  8,  9,  9,  9,   9, 10, 10, 10, 10, 10, 11, 11,//40
	 11, 11, 12, 12, 12, 12, 13, 13,  13, 14, 14, 14, 15, 15, 15, 16,//50
	 16, 16, 17, 17, 18, 18, 19, 19,  20, 20, 21, 21, 22, 22, 23, 24,//60
	 25, 26, 27, 28, 29, 30, 31, 32,  34, 36, 38, 40, 43, 48, 56, 64,//70
};
#elif false//][
static const i8 aTL[]={
	//0   1   2   3   4   5   6   7    8   9   a   b   c   d   e   f
	  0,  0,  0,  0,  1,  1,  1,  1,   2,  2,  2,  2,  3,  3,  3,  3,//00
	  4,  4,  4,  4,  5,  5,  5,  5,   6,  6,  6,  6,  7,  7,  7,  7,//10
	  8,  8,  8,  8,  9,  9,  9,  9,  10, 10, 10, 10, 11, 11, 11, 11,//20
	 12, 12, 12, 12, 13, 13, 13, 13,  14, 14, 14, 14, 15, 15, 15, 15,//30
	 16, 16, 16, 16, 17, 17, 17, 17,  18, 18, 18, 18, 19, 19, 19, 19,//40
	 20, 20, 20, 20, 21, 21, 21, 21,  22, 22, 22, 22, 23, 23, 23, 23,//50
	 24, 24, 24, 24, 25, 25, 25, 25,  26, 26, 26, 26, 27, 27, 27, 27,//60
	 28, 28, 28, 28, 29, 30, 31, 32,  34, 36, 38, 40, 43, 48, 56, 64,//70
};
#elif true//][
static const i8 aTL[]={
	//0   1   2   3   4   5   6   7    8   9   a   b   c   d   e   f
	  0,  0,  0,  1,  1,  1,  2,  2,   2,  3,  3,  3,  4,  4,  4,  5,//00
	  5,  5,  6,  6,  6,  7,  7,  7,   8,  8,  8,  8,  9,  9,  9,  9,//10
	 10, 10, 10, 10, 11, 11, 11, 11,  12, 12, 12, 12, 13, 13, 13, 13,//20
	 14, 14, 14, 14, 15, 15, 15, 15,  16, 16, 16, 16, 17, 17, 17, 17,//30
	 18, 18, 18, 18, 19, 19, 19, 19,  20, 20, 20, 20, 21, 21, 21, 21,//40
	 22, 22, 22, 22, 23, 23, 23, 23,  24, 24, 24, 24, 25, 25, 25, 25,//50
	 26, 26, 26, 26, 27, 27, 27, 27,  28, 28, 28, 28, 29, 29, 29, 29,//60
	 30, 30, 30, 30, 31, 31, 31, 32,  34, 36, 38, 40, 43, 48, 56, 64,//70
};
#else//][
static const i8 aTL[]={
	//0   1   2   3   4   5   6   7    8   9   a   b   c   d   e   f
	  0,  0,  0,  1,  1,  1,  2,  2,   2,  3,  3,  3,  4,  4,  4,  5,//00
	  5,  5,  6,  6,  6,  7,  7,  7,   8,  8,  8,  9,  9,  9, 10, 10,//10
	 10, 11, 11, 11, 12, 12, 12, 13,  13, 13, 14, 14, 14, 15, 15, 15,//20
	 16, 16, 16, 16, 17, 17, 17, 17,  18, 18, 18, 18, 19, 19, 19, 19,//30
	 20, 20, 20, 20, 21, 21, 21, 21,  22, 22, 22, 22, 23, 23, 23, 23,//40
	 24, 24, 24, 24, 25, 25, 25, 25,  26, 26, 26, 26, 27, 27, 27, 27,//50
	 28, 28, 28, 28, 29, 29, 29, 29,  30, 30, 30, 30, 31, 31, 31, 31,//60
	 32, 32, 32, 32, 33, 33, 33, 33,  34, 36, 38, 40, 43, 48, 56, 64,//70
};
#endif//]
static const i8 aTLMask[]={
	0x8, 0x8, 0x8, 0x8, 0xa, 0xe, 0xe, 0xf,
};



void CChipYM2608::StateKeyOn(ChannelInfo& ChannelInfo)
{
	auto Channel = ChannelInfo.Channel;
	auto ChannelL = Channel % 3;
	auto ChannelH = Channel / 3;
	auto Offset = (ChannelH<<8) | ChannelL;
	
	ChannelInfo.pTimbrePlay = ChannelInfo.pTimbreReserve;
	
	auto& Control = ChannelInfo.pTimbrePlay->Control;
	auto& aOperator = ChannelInfo.pTimbrePlay->aOperator;
	
	ChannelInfo.bSe = (Channel == 2 && Control.SE.GetValue() != 0);
	if (Channel == 2) m_pFmChip->write(0x27, (ChannelInfo.bSe<<7));
	
	ChannelInfo.Duration = Control.DUR.GetValue();
	
	m_pFmChip->write(0x30 + Offset, ((aOperator[0].DT.GetValue()<<4) | aOperator[0].MT.GetValue()));
	m_pFmChip->write(0x38 + Offset, ((aOperator[1].DT.GetValue()<<4) | aOperator[1].MT.GetValue()));
	m_pFmChip->write(0x34 + Offset, ((aOperator[2].DT.GetValue()<<4) | aOperator[2].MT.GetValue()));
	m_pFmChip->write(0x3c + Offset, ((aOperator[3].DT.GetValue()<<4) | aOperator[3].MT.GetValue()));
	m_pFmChip->write(0x50 + Offset, ((aOperator[0].KS.GetValue()<<6) | aOperator[0].AR.GetValue()));
	m_pFmChip->write(0x58 + Offset, ((aOperator[1].KS.GetValue()<<6) | aOperator[1].AR.GetValue()));
	m_pFmChip->write(0x54 + Offset, ((aOperator[2].KS.GetValue()<<6) | aOperator[2].AR.GetValue()));
	m_pFmChip->write(0x5c + Offset, ((aOperator[3].KS.GetValue()<<6) | aOperator[3].AR.GetValue()));
	m_pFmChip->write(0x60 + Offset, aOperator[0].DR.GetValue());
	m_pFmChip->write(0x68 + Offset, aOperator[1].DR.GetValue());
	m_pFmChip->write(0x64 + Offset, aOperator[2].DR.GetValue());
	m_pFmChip->write(0x6c + Offset, aOperator[3].DR.GetValue());
	m_pFmChip->write(0x70 + Offset, aOperator[0].SR.GetValue());
	m_pFmChip->write(0x78 + Offset, aOperator[1].SR.GetValue());
	m_pFmChip->write(0x74 + Offset, aOperator[2].SR.GetValue());
	m_pFmChip->write(0x7c + Offset, aOperator[3].SR.GetValue());
	m_pFmChip->write(0x80 + Offset, ((aOperator[0].SL.GetValue()<<4) | aOperator[0].RR.GetValue()));
	m_pFmChip->write(0x88 + Offset, ((aOperator[1].SL.GetValue()<<4) | aOperator[1].RR.GetValue()));
	m_pFmChip->write(0x84 + Offset, ((aOperator[2].SL.GetValue()<<4) | aOperator[2].RR.GetValue()));
	m_pFmChip->write(0x8c + Offset, ((aOperator[3].SL.GetValue()<<4) | aOperator[3].RR.GetValue()));
	m_pFmChip->write(0x90 + Offset, aOperator[0].SSG.GetValue());
	m_pFmChip->write(0x98 + Offset, aOperator[1].SSG.GetValue());
	m_pFmChip->write(0x94 + Offset, aOperator[2].SSG.GetValue());
	m_pFmChip->write(0x9c + Offset, aOperator[3].SSG.GetValue());
	m_pFmChip->write(0xb0 + Offset, ((Control.FB.GetValue()<<3) | Control.ALG.GetValue()));
	
	Pan(ChannelInfo);
	Level(ChannelInfo);
	BlockFNumber(ChannelInfo);
	
	auto Note = ChannelInfo.Note;
	if (Note >= Control.KML.GetValue() && Note <= Control.KMH.GetValue()){
		uint8_t KeyOn = 0;
		KeyOn |= (aOperator[0].EN.GetValue()<<4);
		KeyOn |= (aOperator[1].EN.GetValue()<<5);
		KeyOn |= (aOperator[2].EN.GetValue()<<6);
		KeyOn |= (aOperator[3].EN.GetValue()<<7);
		KeyOn = (Control.EN.GetValue() == 0)? 0: KeyOn;
		m_pFmChip->write(0x28, (KeyOn | ((ChannelH<<2) | ChannelL)));
	}
	
	ChannelInfo.ModulationPhase = 0.0f;
	ChannelInfo.Portamento = 1.0f;
}



void CChipYM2608::StateDump(ChannelInfo& ChannelInfo)
{
	auto Channel = ChannelInfo.Channel;
	auto ChannelL = Channel % 3;
	auto ChannelH = Channel / 3;
	auto Offset = (ChannelH<<8) | ChannelL;
	
	auto& Control = ChannelInfo.pTimbrePlay->Control;
	auto& aOperator = ChannelInfo.pTimbrePlay->aOperator;
	
	bool bFDE = Control.FDE.GetValue();
	bool bFDE1 = (bool)aOperator[0].FDE.GetValue() && bFDE;
	bool bFDE2 = (bool)aOperator[1].FDE.GetValue() && bFDE;
	bool bFDE3 = (bool)aOperator[2].FDE.GetValue() && bFDE;
	bool bFDE4 = (bool)aOperator[3].FDE.GetValue() && bFDE;
	
	if (bFDE1) m_pFmChip->write(0x80 + Offset, ((aOperator[0].SL.GetValue()<<4) | /*RR*/15));
	if (bFDE2) m_pFmChip->write(0x88 + Offset, ((aOperator[1].SL.GetValue()<<4) | /*RR*/15));
	if (bFDE3) m_pFmChip->write(0x84 + Offset, ((aOperator[2].SL.GetValue()<<4) | /*RR*/15));
	if (bFDE4) m_pFmChip->write(0x8c + Offset, ((aOperator[3].SL.GetValue()<<4) | /*RR*/15));
	
	StateKeyOff(ChannelInfo);
}



void CChipYM2608::StateDefault(ChannelInfo& ChannelInfo)
{
	auto Channel = ChannelInfo.Channel;
	
	ChannelInfo.ModulationPhase += (ChannelInfo.VibratoRate * /*Hz*/10.75f) * /*Latency(sec)*/0.005f;
	
	auto pi = std::numbers::pi_v<float>;
	auto sin = std::sin(ChannelInfo.ModulationPhase * pi * 2.0f);
	ChannelInfo.Vibrato = sin * ChannelInfo.ModulationDepth * ChannelInfo.VibatoDepth * /*cent*/0.7f;
	
	switch (GetPortamentoMode()){
		case CChip::EPortamentoMode_TimeMode:{
			const float Tick = 0.005f;
			const float tMin = 0.005f;
			const float tMax = 5.000f;
			const float alpha = 0.7f;
			const float tSec = tMin * std::powf(tMax / tMin, std::powf(ChannelInfo.PortamentoTime, alpha));
			ChannelInfo.Portamento -= Tick / tSec;
			break;
		}
		case CChip::EPortamentoMode_RateMode:{
			const float Tick = 0.005f;
			const float tMin = 0.005f;
			const float tMax = 0.500f;
			const float tSec = tMin * std::powf(tMax / tMin, ChannelInfo.PortamentoTime);
			ChannelInfo.Portamento -= Tick / (tSec * std::abs(ChannelInfo.PortamentoNote - ChannelInfo.Note));
			break;
		}
	}
	ChannelInfo.Portamento = std::max(ChannelInfo.Portamento, 0.0f);
	
	BlockFNumber(ChannelInfo);
	
	ChannelInfo.Duration = (ChannelInfo.Duration > /*ms*/5)? ChannelInfo.Duration-5: 0;
}



void CChipYM2608::UpdateState()
{
	for (auto& ChannelInfo : m_aChannelInfo){
		auto& State = ChannelInfo.State;
		
		int expected = State.load();
		switch (expected){
			case EState_Stopped:{
				StateDefault(ChannelInfo);
				break;
			}
			case EState_KeyOff:{
				if (State.compare_exchange_strong(expected, EState_Stopped)) StateKeyOff(ChannelInfo);
				break;
			}
			case EState_KeyOffDelay:{
				if (ChannelInfo.Duration == 0) State.compare_exchange_strong(expected, expected-1);
				StateDefault(ChannelInfo);
				break;
			}
			case EState_ForceKeyOn:{
				if (State.compare_exchange_strong(expected, EState_KeyOffDelay)) StateKeyOn(ChannelInfo);
				break;
			}
			case EState_ForceDump:{
				if (State.compare_exchange_strong(expected, expected-1)) StateDump(ChannelInfo);
				break;
			}
			case EState_Playing:{
				StateDefault(ChannelInfo);
				break;
			}
			case EState_KeyOn:{
				if (State.compare_exchange_strong(expected, EState_Playing)) StateKeyOn(ChannelInfo);
				break;
			}
			default:{
				State.compare_exchange_strong(expected, expected-1);
				StateDefault(ChannelInfo);
				break;
			}
			case EState_Dump:{
				if (State.compare_exchange_strong(expected, expected-1)) StateDump(ChannelInfo);
				break;
			}
		}
	}
}



void CChipYM2608::Play(i8 Channel, i8 Note, sptr<CTimbre> pTimbre)
{
	auto& ChannelInfo = m_aChannelInfo[Channel];
	auto& State = ChannelInfo.State;
	
	int expected = EState_Stopped;
	if (State.compare_exchange_strong(expected, EState_Dump)){
		ChannelInfo.Note = Note;
		ChannelInfo.pTimbreReserve = pTimbre;
	} else {
		expected = State.load();
		if (State.compare_exchange_strong(expected, EState_Dump)){
			ChannelInfo.Note = Note;
			ChannelInfo.pTimbreReserve = pTimbre;
		}
	}
}



void CChipYM2608::Stop(i8 Channel)
{
	auto& ChannelInfo = m_aChannelInfo[Channel];
	auto& State = ChannelInfo.State;
	
	int expected = State.load();
	if (expected < EState_KeyOn){
		State.compare_exchange_strong(expected, EState_KeyOffDelay);
	} else {
		State.compare_exchange_strong(expected, EState_ForceDump);
	}
}



void CChipYM2608::Pan(ChannelInfo& ChannelInfo)
{
	auto Channel = ChannelInfo.Channel;
	auto Pan = ChannelInfo.Pan;
	
	auto ChannelL = Channel % 3;
	auto ChannelH = Channel / 3;
	auto Offset = (ChannelH<<8) | ChannelL;
	
	auto& Control = ChannelInfo.pTimbrePlay->Control;
	auto& aOperator = ChannelInfo.pTimbrePlay->aOperator;
	
	bool L = (Pan <=  0.5f);//Control.L.GetValue();
	bool R = (Pan >= -0.5f);//Control.R.GetValue();
	
	int LR_AMS_PMS = (L<<7) | (R<<6);// | (Control.AMS.GetValue()<<4) | Control.PMS.GetValue();
	
	m_pFmChip->write(0xb4 + Offset, LR_AMS_PMS);
}



void CChipYM2608::Level(ChannelInfo& ChannelInfo)
{
	auto Channel = ChannelInfo.Channel;
	auto Level = ((ChannelInfo.Volume * ChannelInfo.Velocity * ChannelInfo.Expression) * 127.0f);
	
	auto ChannelL = Channel % 3;
	auto ChannelH = Channel / 3;
	auto Offset = (ChannelH<<8) | ChannelL;
	
	auto& Control = ChannelInfo.pTimbrePlay->Control;
	auto& aOperator = ChannelInfo.pTimbrePlay->aOperator;
	
	auto TLMask = aTLMask[Control.ALG.GetValue()];
	auto iTL = 127 - (int)Level;
	auto TL = aTL[iTL];
	auto TL0 = aOperator[0].TL.GetValue();
	auto TL1 = aOperator[1].TL.GetValue();
	auto TL2 = aOperator[2].TL.GetValue();
	auto TL3 = aOperator[3].TL.GetValue();
	TL0 += (TLMask & (1<<0))? TL: 0;
	TL1 += (TLMask & (1<<1))? TL: 0;
	TL2 += (TLMask & (1<<2))? TL: 0;
	TL3 += (TLMask & (1<<3))? TL: 0;
	TL0 = (TL0 > 0x00)? TL0: 0x00;
	TL1 = (TL1 > 0x00)? TL1: 0x00;
	TL2 = (TL2 > 0x00)? TL2: 0x00;
	TL3 = (TL3 > 0x00)? TL3: 0x00;
	TL0 = (TL0 < 0x7f)? TL0: 0x7f;
	TL1 = (TL1 < 0x7f)? TL1: 0x7f;
	TL2 = (TL2 < 0x7f)? TL2: 0x7f;
	TL3 = (TL3 < 0x7f)? TL3: 0x7f;
	
	m_pFmChip->write(0x40 + Offset, TL0);
	m_pFmChip->write(0x48 + Offset, TL1);
	m_pFmChip->write(0x44 + Offset, TL2);
	m_pFmChip->write(0x4c + Offset, TL3);
}



void CChipYM2608::BlockFNumber(ChannelInfo& ChannelInfo)
{
	auto Channel = ChannelInfo.Channel;
	auto Note = ChannelInfo.Note;
	auto PitchBend = ChannelInfo.PitchBend;
	auto Vibrato = ChannelInfo.Vibrato;
	auto Portamento = ChannelInfo.Portamento;
	auto PortamentoNote = ChannelInfo.PortamentoNote;
	
	auto ChannelL = Channel % 3;
	auto ChannelH = Channel / 3;
	auto Offset = (ChannelH<<8) | ChannelL;
	
	auto& Control = ChannelInfo.pTimbrePlay->Control;
	auto& aOperator = ChannelInfo.pTimbrePlay->aOperator;
	
	auto FineTune = PitchBend + Vibrato + ((PortamentoNote - Note) * Portamento);
	
	if (ChannelInfo.bSe){
		BlockFNumber(ChannelInfo, ((aOperator[0].SE_FIX.GetValue() == 0)? Note: 0), FineTune, 0xad, 0xa9, aOperator[0].SE_KT.GetValue(), aOperator[0].SE_FDT.GetValue());
		BlockFNumber(ChannelInfo, ((aOperator[1].SE_FIX.GetValue() == 0)? Note: 0), FineTune, 0xae, 0xaa, aOperator[1].SE_KT.GetValue(), aOperator[1].SE_FDT.GetValue());
		BlockFNumber(ChannelInfo, ((aOperator[2].SE_FIX.GetValue() == 0)? Note: 0), FineTune, 0xac, 0xa8, aOperator[2].SE_KT.GetValue(), aOperator[2].SE_FDT.GetValue());
		BlockFNumber(ChannelInfo, ((aOperator[3].SE_FIX.GetValue() == 0)? Note: 0), FineTune, 0xa6, 0xa2, aOperator[3].SE_KT.GetValue(), aOperator[3].SE_FDT.GetValue());
	} else {
		BlockFNumber(ChannelInfo, ((Control.FIX.GetValue() == 0)? Note: 0), FineTune, 0xa4 + Offset, 0xa0 + Offset, 0, 0);
	}
}



void CChipYM2608::Pan(i8 Channel, float Value, bool bUpdate)
{
	auto& ChannelInfo = m_aChannelInfo[Channel];
	ChannelInfo.Pan = Value;
	if (bUpdate) Pan(ChannelInfo);
}



void CChipYM2608::Volume(i8 Channel, float Value, bool bUpdate)
{
	auto& ChannelInfo = m_aChannelInfo[Channel];
	ChannelInfo.Volume = Value;
	if (bUpdate) Level(ChannelInfo);
}



void CChipYM2608::Velocity(i8 Channel, float Value, bool bUpdate)
{
	auto& ChannelInfo = m_aChannelInfo[Channel];
	ChannelInfo.Velocity = Value;
	if (bUpdate) Level(ChannelInfo);
}



void CChipYM2608::Expression(i8 Channel, float Value, bool bUpdate)
{
	auto& ChannelInfo = m_aChannelInfo[Channel];
	ChannelInfo.Expression = Value;
	if (bUpdate) Level(ChannelInfo);
}



void CChipYM2608::PitchBend(i8 Channel, float Value, bool bUpdate)
{
	auto& ChannelInfo = m_aChannelInfo[Channel];
	ChannelInfo.PitchBend = Value;
}



void CChipYM2608::ModulationDepth(i8 Channel, float Value, bool bUpdate)
{
	auto& ChannelInfo = m_aChannelInfo[Channel];
	ChannelInfo.ModulationDepth = Value;
}



void CChipYM2608::VibratoRate(i8 Channel, float Value, bool bUpdate = false)
{
	auto& ChannelInfo = m_aChannelInfo[Channel];
	ChannelInfo.VibratoRate = Value;
}



void CChipYM2608::VibatoDepth(i8 Channel, float Value, bool bUpdate = false)
{
	auto& ChannelInfo = m_aChannelInfo[Channel];
	ChannelInfo.VibatoDepth = Value;
}



void CChipYM2608::PortamentoNote(i8 Channel, i8 Value, bool bUpdate)
{
	auto& ChannelInfo = m_aChannelInfo[Channel];
	ChannelInfo.PortamentoNote = Value;
}



void CChipYM2608::PortamentoTime(i8 Channel, float Value, bool bUpdate)
{
	auto& ChannelInfo = m_aChannelInfo[Channel];
	ChannelInfo.PortamentoTime = Value;
}



void CChipYM2608::CThreadProc()
{
	SubmitSourceBuffer();
}



UINT CChipYM2608::CThreadProc(LPVOID pParam)
{
	auto& This = *reinterpret_cast<CChipYM2608*>(pParam);
	This.CThreadProc();
	return 0;
}
