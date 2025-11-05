
#pragma once



#include <xaudio2.h>
#include <queue>
#include <vector>
#include <chrono>
#include <mutex>
#include <atomic>
#include <algorithm>
#include "Common/Common.h"
#include "Chip.h"
#include "Thread.h"
#include "martin-finke/Filter.h"



class CVoice : public IXAudio2VoiceCallback
{
	public:
		~CVoice();
		
		CVoice(IXAudio2* m_pIXAudio2, int Channels, int SampleRate);
		
		size_t GetRenderSize(){ return m_aRender.size(); }
		
		void AddChip(uptr<CChip> pChip){ return m_apChip.push_back(std::move(pChip)); }
		const std::vector<uptr<CChip>>& GetChip(){ return m_apChip; }
		
		void Start();
		
		void SetVolume(double Volume){ m_Volume = (float)Volume; }
		
		void SetFilterMode(Filter::FilterMode FilterMode);
		void SetCutoff(double Cutoff);
		void SetResonance(double Resonance);
		void SetDCCut(bool bDCCut);
		void SetDCCutRate(double DCCutRate);
		
		void SetPortamentoMode(CChip::EPortamentoMode PortamentoMode);
		void SetTuning(float Tuning);
		
		bool WavRecord();
	
	protected:
		static constexpr int QueueNum = 32;
		
		CThread m_CThread;
		
		WAVEFORMATEX m_Format;
		IXAudio2SourceVoice* m_pIXAudio2SourceVoice;
		XAUDIO2_BUFFER m_Buffer;
		
		std::vector<int32_t> m_aRender;
		
		std::vector<float> m_aQueue[QueueNum];
		int m_iQueue;
		
		std::vector<uptr<CChip>> m_apChip;
		std::vector<HANDLE> m_aChipThreadHandle;
		
		float m_Volume;
		
		Filter m_FilterL;
		Filter m_FilterR;
		
		std::chrono::high_resolution_clock::time_point m_prev;
		
		std::mutex m_Mutex;
		bool m_bWavRecord;
		std::list<std::vector<float>> m_WavRecord;
		std::thread m_WavSave;
	
	protected:
		void STDMETHODCALLTYPE OnStreamEnd();
		void STDMETHODCALLTYPE OnVoiceProcessingPassEnd();
		void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 SamplesRequired);
		void STDMETHODCALLTYPE OnBufferEnd(void* pBufferContext);
		void STDMETHODCALLTYPE OnBufferStart(void* pBufferContext);
		void STDMETHODCALLTYPE OnLoopEnd(void* pBufferContext);
		void STDMETHODCALLTYPE OnVoiceError(void* pBufferContext, HRESULT Error);
		
		void SubmitSourceBuffer();
		
		void CThreadProc();
		static UINT CThreadProc(LPVOID pParam);
		
		void WavSave();
};
