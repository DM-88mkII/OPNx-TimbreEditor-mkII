
#pragma once



#include "afxdialogex.h"
#include "Common/Common.h"
#include "SettingTab.h"
#include "Keyboard.h"
#include "ChipYM2608.h"
#include "Intermediate.h"
#include "Value.h"



class CFmStationDlg;

class CBatch;
class CTimbre;



struct IUpdateTimbreTab
{
	virtual ~IUpdateTimbreTab(){}
	
	IUpdateTimbreTab(){}
	
	virtual void UpdateTimbre(CTimbre* pTimbre) = 0;
};



class CTimbreDlg : public CDialogEx, public IKeyboardCallback
{
	DECLARE_DYNAMIC(CTimbreDlg)
	
	public:
		virtual ~CTimbreDlg();
		
		CTimbreDlg(CWnd* pParent = nullptr);
		
		#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_TIMBRE_DIALOG };
		#endif
	
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);
		virtual BOOL OnInitDialog();
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		virtual HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		void PostNcDestroy() override { delete this; }
		
		DECLARE_MESSAGE_MAP()
	
	public:
		afx_msg void OnOK();
		afx_msg void OnCancel();
		afx_msg void OnClose();
		afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
		afx_msg void OnBnClickedTimbredlgOk();
	
	protected:
		static constexpr int MidiChannels = 16;
		static constexpr int MidiNotes = 128;
	
	
	// 
	public:
		void NoteOn(int MidiNote) override;
		void NoteOff(int MidiNote) override;
		
		void ResetKeyboard();
		void UpdateKeyboard(int MidiNote, bool bNoteState, bool bValid = true);
	
	
	// 
	protected:
		bool m_IsActive;
		
		CEdit m_CEditName;
		CKeyboard m_CKeyboard;
		CStatic m_CStaticLog;
		
		sptr<CTimbre> m_pTimbre;
	
	protected:
		void TimbreCallback(bool bResult = false);
	
	public:
		void SetTimbre(sptr<CTimbre> pTimbre, CString BatchName);
		void GetName(CString& Name);
		
		template<class... Args> void Log(std::wformat_string<Args...> fmt, Args&& ... args);
	
	
	// 
	protected:
		struct Edit
		{
			CStatic m_aCStaticALG[8];
			int mx;
			int my;
			int m_iOperator;
			bool m_bEditing;
			bool m_bHighLight;
		};
		Edit m_Edit;
	
	protected:
		void SetColor(CDC* pDC, int x, int y);
		void SetColor(CDC* pDC, int iOperator);
	
	public:
		void SetCur(int x, int y, bool bEditing);
		void SetPicture(int ALG);
		void SetOperator(int iOperator, bool bHighLight);
	
	
	// 
	protected:
		struct Ctrl
		{
			int m_aaParam[5][15];
			int mx;
			int my;
			int m_Octave;
			bool m_bRotate;
			int m_iRotate;
			int m_aNote[13];
		};
		Ctrl m_Ctrl;
	
	protected:
		CFmStationDlg& GetFmStationDlg();
		CSettingTab& GetSettingTab();
		
		CWnd* GetParamWnd(int x, int y);
		IValue& GetParamValue(int x, int y);
		void AddParamValue(int x, int y, int Add);
		void DrawAllParam();
		
		void MoveCursor(int ax, int ay);
		void RedrawParam(int x, int y);
		void RedrawParam();
		void FixParam();
		
		bool ClipboardCopy(CString Text);
		CString ClipboardPaste();
		
		void Copy(bool bShift);
		void Paste();
		
		void Undo();
		
		void VolumeUp();
		void VolumeDown();
		
		void PreviewUp();
		void PreviewDown();
		
		void Rotate();
		void RotateCancel();
		
		void Play(int Key, CString KeyName);
		void Stop(int Key);
	
	
	// 
	public:
		CDummy Dummy;
		
		struct Control
		{
			i16 EN;
			i16 FDE;
			
			i16 ALG;
			i16 FB;
			
			i16 NUM;
			i16 KML;
			i16 KMH;
			i16 DUR;
			
			i16 SE;
			
			i16 FIX;
			i16 KT;
			i16 FDT;
		};
		Control BackupControl;
		
		struct Operator
		{
			i16 EN;
			i16 FDE;
			
			i16 AR;
			i16 DR;
			i16 SR;
			i16 RR;
			i16 SL;
			i16 TL;
			i16 KS;
			i16 MT;
			i16 DT;
			i16 SSG;
			
			i16 SE_FIX;
			i16 SE_KT;
			i16 SE_FDT;
		};
		Operator aBackupOperator[4];
};



class CTimbre
{
	protected:
		static constexpr int MidiChannels = 16;
		static constexpr int MidiNotes = 128;
	
	public: IUpdateTimbreTab* pUpdateTimbreTab;
	public: CString Name;
	public: uptr<CTimbreDlg> pTimbreDlg;
	public: int aabNoteState[MidiChannels][MidiNotes];
	
	public:
		CDummy Dummy;
		
		struct Control
		{
			CValue<1, 0, 1> EN;
			CValue<1, 0, 1> FDE;
			
			CValue<1, 0, 7> ALG;
			CValue<1, 0, 7> FB;
			
			CValue<3, 0, 999> NUM;
			CValue<3, 0, 127> KML;
			CValue<3, 0, 127> KMH;
			CValue<4, 0, 9999> DUR;
			
			CValue<1, 0, 1> SE;
			
			CValue<1, 0, 1> FIX;
			CValue<4, -127, 127> KT;
			CValue<4, -999, 999> FDT;
		};
		Control Control;
		
		struct Operator
		{
			CValue<1, 0, 1> EN;
			CValue<1, 0, 1> FDE;
			
			CValue<2, 0, 31> AR;
			CValue<2, 0, 31> DR;
			CValue<2, 0, 31> SR;
			CValue<2, 0, 15> RR;
			CValue<2, 0, 15> SL;
			CValue<3, 0, 127> TL;
			CValue<1, 0, 3> KS;
			CValue<2, 0, 15> MT;
			CValue<1, 0, 7> DT;
			CValue<2, 0, 15> SSG;
			
			CValue<1, 0, 1> SE_FIX;
			CValue<4, -127, 127> SE_KT;
			CValue<4, -999, 999> SE_FDT;
		};
		Operator aOperator[4];
	
	public: ~CTimbre();
	
	public: CTimbre(): CTimbre(NULL, _T("")) {}
	public: CTimbre(IUpdateTimbreTab* pUpdateTimbreTab, CString Name);
	
	public: void DlgOK();
	public: void DlgCancel();
	public: void DlgCloase();
	public: void DlgCallback(bool bResult = false);
	
	public: IValue& GetValue(int x, int y);
	
	public: void SetIntermediate(CIntermediate v);
	public: CIntermediate GetIntermediate();
	
	public: bool IsKeyMapValid(i8 MidiNote){ return (MidiNote >= Control.KML.GetValue() && MidiNote <= Control.KMH.GetValue()); }
	
	public: bool MidiNoteOn(i8 MidiChannel, i8 MidiNote, int MidiVelocity);
	public: bool MidiNoteOff(i8 MidiChannel, i8 MidiNote, int MidiVelocity);
};
