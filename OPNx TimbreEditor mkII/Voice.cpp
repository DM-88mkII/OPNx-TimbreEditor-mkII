



#include "pch.h"
#include "framework.h"
#include "FmStation.h"
#include "FmStationDlg.h"
#include "Voice.h"
#include <ppl.h>
#include <immintrin.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_THREAD_EXIT (WM_USER + 1)
#define WM_THREAD_PROC (WM_USER + 2)

using namespace concurrency;
using namespace Concurrency;



CVoice::~CVoice()
{
	m_CThread.Exit();
	
	if (m_pIXAudio2SourceVoice){
		m_pIXAudio2SourceVoice->Stop();
		m_pIXAudio2SourceVoice->DestroyVoice();
	}
	
	if (m_WavSave.joinable()) m_WavSave.join();
}



CVoice::CVoice(IXAudio2* m_pIXAudio2, int Channels, int SampleRate)
:m_Format{0}
,m_pIXAudio2SourceVoice(nullptr)
,m_Buffer{0}
,m_aRender(((SampleRate * /*Latency(ms)*/5) / /*ms*/1000) * /*ch*/Channels)
,m_iQueue(0)
,m_Volume(0)
,m_bWavRecord(false)
{
	m_Format.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
	m_Format.nChannels = Channels;
	m_Format.nSamplesPerSec = SampleRate;
	m_Format.wBitsPerSample = sizeof(float) * 8;
	m_Format.cbSize = 0;
	m_Format.nBlockAlign = (m_Format.wBitsPerSample * m_Format.nChannels) / 8;
	m_Format.nAvgBytesPerSec = m_Format.nSamplesPerSec * m_Format.nBlockAlign;
	
	HRESULT ret = m_pIXAudio2->CreateSourceVoice(&m_pIXAudio2SourceVoice, &m_Format, 0, XAUDIO2_DEFAULT_FREQ_RATIO, this);
	if (FAILED(ret)){
		m_pIXAudio2SourceVoice = nullptr;
		return;
	}
}



void CVoice::Start()
{
	auto nChip = m_apChip.size();
	m_aChipThreadHandle.resize(nChip);
	
	int i = 0;
	for (auto& pChip: m_apChip) m_aChipThreadHandle[i++] = pChip->GetThreadHandle();
	
	for (auto& Queue: m_aQueue) Queue.resize(m_aRender.size());
	for (auto n = QueueNum - 1; n > 0; n--) SubmitSourceBuffer();
	
	HRESULT ret = m_pIXAudio2SourceVoice->Start();
	if (FAILED(ret)) OutputDebugStringA("Start.Error\n");
}



void STDMETHODCALLTYPE CVoice::OnStreamEnd(){}
void STDMETHODCALLTYPE CVoice::OnVoiceProcessingPassEnd(){}
void STDMETHODCALLTYPE CVoice::OnVoiceProcessingPassStart(UINT32 SamplesRequired){}
void STDMETHODCALLTYPE CVoice::OnBufferEnd(void* pBufferContext){}
void STDMETHODCALLTYPE CVoice::OnBufferStart(void* pBufferContext)
{
	m_CThread.Proc(CThreadProc, this);
}
void STDMETHODCALLTYPE CVoice::OnLoopEnd(void* pBufferContext){}
void STDMETHODCALLTYPE CVoice::OnVoiceError(void* pBufferContext, HRESULT Error){ OutputDebugStringA("OnVoiceError\n"); }



void CVoice::SubmitSourceBuffer()
{
	#if false//[
	{	// 
		auto now = std::chrono::high_resolution_clock::now();
		TRACE(_T("%lf\n"), std::chrono::duration_cast<std::chrono::nanoseconds>(now - m_prev).count() / 1000000000.0);
		m_prev = now;
	}
	#endif//]
	
	//auto old = std::chrono::high_resolution_clock::now();
	{	// 
		auto& Queue = m_aQueue[m_iQueue];
		
		{	// 
			for (auto& pChip: m_apChip) pChip->Render();
			
			WaitForMultipleObjects((DWORD)m_aChipThreadHandle.size(), m_aChipThreadHandle.data(), TRUE, INFINITE);
			
			{	// 
				auto dst = m_aRender.data();
				
				size_t n = m_aRender.size();
				size_t s = 256 / 32;
				
				std::fill(m_aRender.begin(), m_aRender.end(), 0);
				for (auto& pChip: m_apChip){
					auto src = pChip->Data();
					
					size_t i = 0;
					for (; (i + s) <= n; i += s){
						__m256i v_dst = _mm256_loadu_si256((__m256i const*)(dst + i));
						__m256i v_src = _mm256_loadu_si256((__m256i const*)(src + i));
						__m256i v_add = _mm256_add_epi32(v_dst, v_src);
						_mm256_storeu_si256((__m256i*)(dst + i), v_add);
					}
					for (; i < n; i++){
						dst[i] += src[i];
					}
				}
			}
		}
		
		{	// 
			float p = (1.0f / (/*int16_t.MaxValue*/0x8000 * 2)) * m_Volume;
			
			int32_t* src = m_aRender.data();
			float* dst = Queue.data();
			
			size_t n = m_aRender.size();
			size_t i = 0;
			size_t s = 256 / 32;
			
			__m256  v_p = _mm256_set1_ps(p);
			for (; (i + s) <= n; i += s){
				__m256i v_src_i32 = _mm256_loadu_si256((const __m256i*)(src + i));
				__m256  v_src_f32 = _mm256_cvtepi32_ps(v_src_i32);
				__m256  v_mul_f32 = _mm256_mul_ps(v_src_f32, v_p);
				_mm256_store_ps((dst + i), v_mul_f32);
			}
			for (; i < n; i++){
				dst[i] = src[i] * p;
			}
		}
		
		{	// 
			float* data = Queue.data();
			size_t size = Queue.size();
			size_t step = m_Format.nChannels;//2
			for (size_t i = 0; (i + step) <= size; i += step){
				data[i+0] = (float)m_FilterL.process(data[i+0]);
				data[i+1] = (float)m_FilterR.process(data[i+1]);
			}
		}
		
		if (m_bWavRecord){
			m_Mutex.lock();
			m_WavRecord.emplace_back(Queue);
			m_Mutex.unlock();
		}
		
		{	// 
			HRESULT ret;
			
			m_Buffer.AudioBytes = (UINT)(Queue.size() * sizeof(Queue[0]));
			m_Buffer.pAudioData = (const BYTE*)Queue.data();
			
			ret = m_pIXAudio2SourceVoice->SubmitSourceBuffer(&m_Buffer);
			if (FAILED(ret)) OutputDebugStringA("SubmitSourceBuffer.Error\n");
		}
		
		m_iQueue = (m_iQueue + 1) % QueueNum;
	}
	//auto now = std::chrono::high_resolution_clock::now();
	//TRACE(_T("%lf\n"), std::chrono::duration_cast<std::chrono::nanoseconds>(now - old).count() / 1000000000.0);
}



void CVoice::CThreadProc()
{
	SubmitSourceBuffer();
}



UINT CVoice::CThreadProc(LPVOID pParam)
{
	auto& This = *reinterpret_cast<CVoice*>(pParam);
	This.CThreadProc();
	return 0;
}



void CVoice::SetFilterMode(Filter::FilterMode FilterMode)
{
	m_FilterL.setFilterMode(FilterMode);
	m_FilterR.setFilterMode(FilterMode);
}



void CVoice::SetCutoff(double Cutoff)
{
	m_FilterL.setCutoff(Cutoff);
	m_FilterR.setCutoff(Cutoff);
}



void CVoice::SetResonance(double Resonance)
{
	m_FilterL.setResonance(Resonance);
	m_FilterR.setResonance(Resonance);
}



void CVoice::SetDCCut(bool bDCCut)
{
	m_FilterL.setDCCut(bDCCut);
	m_FilterR.setDCCut(bDCCut);
}



void CVoice::SetDCCutRate(double DCCutRate)
{
	m_FilterL.setDCCutRate(DCCutRate);
	m_FilterR.setDCCutRate(DCCutRate);
}



void CVoice::SetPortamentoMode(CChip::EPortamentoMode PortamentoMode)
{
	for (auto& pChip: m_apChip) pChip->SetPortamentoMode(PortamentoMode);
}



void CVoice::SetTuning(float Tuning)
{
	for (auto& pChip: m_apChip) pChip->SetTuning(Tuning);
}



bool CVoice::WavRecord()
{
	if (m_bWavRecord){
		m_bWavRecord = false;
		
		if (m_WavSave.joinable()) m_WavSave.join();
		m_WavSave = std::thread(&CVoice::WavSave, this);
	} else {
		m_bWavRecord = true;
	}
	return m_bWavRecord;
}



void CVoice::WavSave()
{
	auto Data = std::vector<float>();
	
	{	// 
		m_Mutex.lock();
		auto WavRecord = std::move(m_WavRecord);
		m_WavRecord.clear();
		m_Mutex.unlock();
		
		size_t Size = std::accumulate(
			WavRecord.begin(), WavRecord.end(), 0ULL,
			[](size_t sum, const auto& v){ return sum + v.size(); }
		);
		Data.reserve(Size);
		for (auto& v: WavRecord) Data.insert(Data.end(), v.begin(), v.end());
	}
	
	{	// 
		FILE* fp;
		CTime cTime = CTime::GetCurrentTime();
		CStringA time = (CStringA)cTime.Format("%Y%m%d_%H%M%S.wav");
		if (fopen_s(&fp, (const char*)time, "wb") == 0){
			auto dataSize = (Data.size() * sizeof(float));
			
			fwrite("RIFF", 1, 4, fp);
			uint32_t riffSize = 36 + (uint32_t)dataSize;
			fwrite(&riffSize, 4, 1, fp);
			fwrite("WAVE", 1, 4, fp);
			
			fwrite("fmt ", 1, 4, fp);
			uint32_t fmtSize = sizeof(WAVEFORMATEX);
			fwrite(&fmtSize, 4, 1, fp);
			fwrite(&m_Format, 1, fmtSize, fp);
			
			fwrite("data", 1, 4, fp);
			uint32_t dataChunkSize = (uint32_t)dataSize;
			fwrite(&dataChunkSize, 4, 1, fp);
			fwrite(Data.data(), 1, dataChunkSize, fp);
			
			fclose(fp);
		}
	}
}
