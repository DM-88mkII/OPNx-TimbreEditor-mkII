
#pragma once



#include "afxdialogex.h"
#include "StaticList.h"



class CModuleTab : public CDialogEx
{
	DECLARE_DYNAMIC(CModuleTab)
	
	public:
		virtual ~CModuleTab();
		
		CModuleTab(CWnd* pParent = nullptr);
		
		#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_MODULE_TAB };
		#endif
	
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);
		virtual BOOL OnInitDialog();
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		
		DECLARE_MESSAGE_MAP()
	
	protected:
		CFmStationDlg& GetFmStationDlg();
	
	protected:
		static constexpr int ChipNum = 8;
		static constexpr int ChipChannels = 6;
	
	protected:
		CListCtrl m_CListCtrl;
		CRect m_CListCtrlRect;
		
		using StaticList = CStaticList<ChipNum, (1 + ChipChannels)>;
		StaticList m_StaticList;
		UINT_PTR m_timerID;
		
		struct ChannelInfo
		{
			bool bInvalidate;
			bool bNoteOn;
			bool bHold;
			float Text;
			float Back;
		};
		struct ChipInfo
		{
			ChannelInfo aChannelInfo[ChipChannels];
		};
		ChipInfo m_aChipInfo[ChipNum];
	
	
	public:
		afx_msg void OnTimer(UINT_PTR nIDEvent);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		
		void SetState(int ChipId, int ChipChannel, int MidiChannel = -1, int MidiNote = -1, int MidiVelocity = 0);
		void SetHold(int ChipId, int ChipChannel);
};
