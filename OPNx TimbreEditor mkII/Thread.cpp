



#include "pch.h"
#include "framework.h"
#include "Thread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_THREAD_EXIT (WM_USER + 1)
#define WM_THREAD_PROC (WM_USER + 2)



CThread::~CThread()
{
}



CThread::CThread()
:m_pThread(nullptr)
,m_Exit(CreateEvent(NULL, FALSE, FALSE, NULL))
,m_Proc(CreateEvent(NULL, FALSE, FALSE, NULL))
{
	m_pThread = AfxBeginThread(ThreadProc, (LPVOID)this, /*THREAD_PRIORITY_HIGHEST*/THREAD_PRIORITY_TIME_CRITICAL);
}



void CThread::Exit()
{
	PostThreadMessage(m_pThread->m_nThreadID, WM_THREAD_EXIT, 0, 0);
	WaitForSingleObject(m_Exit, INFINITE);
}



void CThread::Proc(AFX_THREADPROC pfnThreadProc, LPVOID pParam)
{
	PostThreadMessage(m_pThread->m_nThreadID, WM_THREAD_PROC, (WPARAM)pfnThreadProc, (LPARAM)pParam);
}



void CThread::Wait()
{
	WaitForSingleObject(m_Proc, INFINITE);
}



void CThread::ThreadProc()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)){
		switch (msg.message){
			case WM_THREAD_EXIT:{
				goto Exit;
			}
			case WM_THREAD_PROC:{
				AFX_THREADPROC pfnThreadProc = (AFX_THREADPROC)msg.wParam;
				LPVOID pParam = (LPVOID)msg.lParam;
				pfnThreadProc(pParam);
				SetEvent(m_Proc);
				break;
			}
		}
	}
	Exit:
	
	SetEvent(m_Exit);
}



UINT CThread::ThreadProc(LPVOID pParam)
{
	auto This = reinterpret_cast<CThread*>(pParam);
	This->ThreadProc();
	return 0;
}
