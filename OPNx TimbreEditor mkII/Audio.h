
#pragma once



#include <memory>
#include <xaudio2.h>

#pragma comment(lib, "xaudio2.lib")
#pragma comment(lib, "winmm.lib")



class CAudio
{
	public:
		~CAudio();
		
		CAudio();
		
		IXAudio2* GetXAudio2(){ return m_pIXAudio2; }
		IXAudio2MasteringVoice* GetMasteringVoice(){ return m_pIXAudio2MasteringVoice; }
	
	private:
		IXAudio2* m_pIXAudio2;
		IXAudio2MasteringVoice* m_pIXAudio2MasteringVoice;
};
