
#pragma once



#include "afxdialogex.h"
#include "Common/Common.h"
#include "ListBoxCustom.h"
#include "ListBoxT.h"
#include "BatchDlg.h"
#include "TimbreDlg.h"



class CBatch;
class CTimbre;



class CInstTab : public CDialogEx, public IUpdateBatchTab, public IUpdateTimbreTab
{
	DECLARE_DYNAMIC(CInstTab)
	
	public:
		virtual ~CInstTab();
		
		CInstTab(CWnd* pParent = nullptr);
		
		#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_INST_TAB };
		#endif
	
	protected:
		CListBoxCustom m_CListBoxProgram;
		CListBoxCustom m_CListBoxBatch;
		CListBoxCustom m_CListBoxTimbre;
		
		CStatic m_CStaticTimbre;
		
		CSpinButtonCtrl m_CSpinButtonCtrlBank;
		CSpinButtonCtrl m_CSpinButtonCtrlBatch;
		CSpinButtonCtrl m_CSpinButtonCtrlTimbre;
		
		CEdit m_CEditBank;
	
	public:
		static constexpr int MidiBanks = 128;
		static constexpr int MidiPrograms = 128;
		static constexpr int MidiChannels = 16;
		static constexpr int MidiNotes = 128;
	
	protected:
		using CListBoxProgram = CListBoxT</*sptr*/CBatch>;
		using CListBoxBatch = CListBoxT</*sptr*/CBatch>;
		using CListBoxTimbre = CListBoxT</*sptr*/CTimbre>;
		
		uptr<CListBoxProgram> m_pProgramList;
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
		
		sptr<CBatch> GetProgram(int iBank, int iProgram);
		void RemoveProgram(int iProgram);
		
		void AddProgram(int iProgram, int iBatch);
		void AddBatch(int iBatch, CString Name);
		void AddTimbre(int iBatch, int iTimbre, CString Name);
		
		void Load(nlohmann::json& j);
		nlohmann::json Save();
	
	public:
		afx_msg void OnBnClickedInstButtonProgramAdd();
		afx_msg void OnBnClickedInstButtonProgramRemove();
		afx_msg void OnBnClickedInstButtonBatchAdd();
		afx_msg void OnBnClickedInstButtonBatchDelete();
		afx_msg void OnBnClickedInstButtonTimbreAdd();
		afx_msg void OnBnClickedInstButtonTimbreDelete();
		afx_msg void OnLbnSelchangeInstListProgram();
		afx_msg void OnLbnSelchangeInstListBatch();
		afx_msg void OnLbnSelchangeInstListTimbre();
		afx_msg void OnDeltaposInstSpinBank(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnDeltaposInstSpinBatchOrder(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnDeltaposInstSpinTimbreOrder(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnLbnDblclkInstListProgram();
		afx_msg void OnLbnDblclkInstListBatch();
		afx_msg void OnLbnDblclkInstListTimbre();
		afx_msg void OnBnClickedInstButtonBatchDup();
		afx_msg void OnBnClickedInstButtonTimbreDup();
};
