
#pragma once



class CThread
{
	public:
		~CThread();
		
		CThread();
		
		void Exit();
		void Wait();
		void Proc(AFX_THREADPROC pfnThreadProc, LPVOID pParam);
		HANDLE GetHandle(){ return m_Proc; }
	
	private:
		CWinThread* m_pThread;
		HANDLE m_Exit;
		HANDLE m_Proc;
	
	private:
		void ThreadProc();
		static UINT ThreadProc(LPVOID pParam);
};
