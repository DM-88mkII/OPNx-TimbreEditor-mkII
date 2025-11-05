
#pragma once



#include "afxdialogex.h"
#include "Common/Common.h"
#include "SettingTab.h"
#include "Keyboard.h"



class CFmStationDlg;

class CBatch;
class CTimbre;



struct IUpdateBatchTab
{
	virtual ~IUpdateBatchTab(){}
	
	IUpdateBatchTab(){}
	
	virtual void UpdateBatch(CBatch* pBatch) = 0;
};



class CBatchDlg : public CDialogEx, public IKeyboardCallback
{
	DECLARE_DYNAMIC(CBatchDlg)
	
	public:
		virtual ~CBatchDlg();
		
		CBatchDlg(CWnd* pParent = nullptr);
		
		#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_BATCH_DIALOG };
		#endif
	
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);
		virtual BOOL OnInitDialog();
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		void PostNcDestroy() override { delete this; }
		
		DECLARE_MESSAGE_MAP()
	
	public:
		afx_msg void OnOK();
		afx_msg void OnCancel();
		afx_msg void OnClose();
		afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	
	public:
		void NoteOn(int MidiNote) override;
		void NoteOff(int MidiNote) override;
		
		void ResetKeyboard();
		void UpdateKeyboard(int MidiNote, bool bNoteState);
	
	protected:
		CEdit m_CEditName;
		CKeyboard m_CKeyboard;
		CStatic m_CStaticLog;
		
		sptr<CBatch> m_pBatch;
	
	protected:
		void BatchCallback(bool bResult = false);
		
		CFmStationDlg& GetFmStationDlg();
		CSettingTab& GetSettingTab();
	
	public:
		void SetBatch(sptr<CBatch> pBatch);
		void GetName(CString& Name);
		
		template<class... Args> void Log(std::wformat_string<Args...> fmt, Args&& ... args);
	
	
	protected:
		int m_Octave;
		int m_aNote[13];
	
	protected:
		void Play(int Key, CString KeyName);
		void Stop(int Key);
		
		void VolumeUp();
		void VolumeDown();
		
		void PreviewUp();
		void PreviewDown();
};



class CBatch
{
	public: IUpdateBatchTab* pUpdateBatchTab;
	public: CString Name;
	public: uptr<CBatchDlg> pBatchDlg;
	public: std::vector<sptr<CTimbre>> apTimbre;
	
	public: ~CBatch();
	
	public: CBatch():pUpdateBatchTab(NULL){}
	public: CBatch(IUpdateBatchTab* pUpdateBatchTab, CString Name):pUpdateBatchTab(pUpdateBatchTab),Name(Name){}
	
	public: void DlgOK();
	public: void DlgCancel();
	public: void DlgCloase();
	public: void DlgCallback(bool bResult = false);
	
	public: void MidiNoteOn(i8 MidiChannel, i8 MidiNote, int MidiVelocity);
	public: void MidiNoteOff(i8 MidiChannel, i8 MidiNote, int MidiVelocity);
	
	public: void Load(nlohmann::json& jBatch);
	public: nlohmann::json Save();
};
