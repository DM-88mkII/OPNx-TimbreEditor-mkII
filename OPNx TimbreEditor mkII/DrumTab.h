
#pragma once



#include "afxdialogex.h"
#include "Common/Common.h"
#include "ListBoxCustom.h"
#include "ListBoxT.h"
#include "BatchDlg.h"
#include "TimbreDlg.h"



class CBatch;
class CTimbre;



class CDrumTab : public CDialogEx, public IUpdateBatchTab, public IUpdateTimbreTab
{
	DECLARE_DYNAMIC(CDrumTab)
	
	public:
		virtual ~CDrumTab();
		
		CDrumTab(CWnd* pParent = nullptr);
		
		#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_DRUM_TAB };
		#endif
	
	protected:
		CListBoxCustom m_CListBoxNote;
		CListBoxCustom m_CListBoxBatch;
		CListBoxCustom m_CListBoxTimbre;
		
		CStatic m_CStaticTimbre;
		
		CSpinButtonCtrl m_CSpinButtonCtrlProgram;
		CSpinButtonCtrl m_CSpinButtonCtrlBatch;
		CSpinButtonCtrl m_CSpinButtonCtrlTimbre;
		
		CEdit m_CEditProgram;
	
	public:
		static constexpr int MidiBanks = 128;
		static constexpr int MidiPrograms = 128;
		static constexpr int MidiChannels = 16;
		static constexpr int MidiNotes = 128;
	
	protected:
		using CListBoxNote = CListBoxT</*sptr*/CBatch>;
		using CListBoxBatch = CListBoxT</*sptr*/CBatch>;
		using CListBoxTimbre = CListBoxT</*sptr*/CTimbre>;
		
		uptr<CListBoxNote> m_pNoteList;
		uptr<CListBoxBatch> m_pBatchList;
		uptr<CListBoxTimbre> m_pTimbreList;
		
		sptr<CBatch> m_pDefaultBatch;
	
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);
		virtual BOOL OnInitDialog();
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		
		DECLARE_MESSAGE_MAP()
	
	protected:
		CFmStationDlg& GetFmStationDlg();
		
		void UpdateTimbre(int iBatch);
	
	public:
		void Reset();
		
		void UpdateBatch(CBatch* pBatch) override;
		void UpdateTimbre(CTimbre* pTimbre) override;
		
		sptr<CBatch> GetProgram(int iProgram, int iNote);
		void RemoveNote(int iNote);
		
		void AddNote(int iNote, int iBatch);
		void AddBatch(int iBatch, CString Name);
		void AddTimbre(int iBatch, int iTimbre, CString Name);
		
		void Load(nlohmann::json& j);
		nlohmann::json Save();
	
	public:
		afx_msg void OnBnClickedDrumButtonNoteAdd();
		afx_msg void OnBnClickedDrumButtonNoteRemove();
		afx_msg void OnBnClickedDrumButtonBatchAdd();
		afx_msg void OnBnClickedDrumButtonBatchDelete();
		afx_msg void OnBnClickedDrumButtonTimbreAdd();
		afx_msg void OnBnClickedDrumButtonTimbreDelete();
		afx_msg void OnLbnSelchangeDrumListNote();
		afx_msg void OnLbnSelchangeDrumListBatch();
		afx_msg void OnLbnSelchangeDrumListTimbre();
		afx_msg void OnDeltaposDrumSpinProgram(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnDeltaposDrumSpinBatchOrder(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnDeltaposDrumSpinTimbreOrder(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnLbnDblclkDrumListNote();
		afx_msg void OnLbnDblclkDrumListBatch();
		afx_msg void OnLbnDblclkDrumListTimbre();
		afx_msg void OnBnClickedDrumButtonBatchDup();
		afx_msg void OnBnClickedDrumButtonTimbreDup();
};
