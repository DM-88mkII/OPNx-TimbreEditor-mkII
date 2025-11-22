



#include "pch.h"
#include "framework.h"
#include "FmStation.h"
#include "FmStationDlg.h"
#include "Audio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CAudio::~CAudio()
{
	if (m_pIXAudio2MasteringVoice) m_pIXAudio2MasteringVoice->DestroyVoice();
	if (m_pIXAudio2) m_pIXAudio2->Release();
	CoUninitialize();
}



CAudio::CAudio()
{
	HRESULT ret;
	
	ret = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(ret)) OutputDebugStringA("CoInitializeEx.Error\n");
	
	ret = XAudio2Create(&m_pIXAudio2);
	if (FAILED(ret)) OutputDebugStringA("XAudio2Create.Error\n");
	
	ret = m_pIXAudio2->CreateMasteringVoice(&m_pIXAudio2MasteringVoice);
	if (FAILED(ret)) OutputDebugStringA("CreateMasteringVoice.Error\n");
}
