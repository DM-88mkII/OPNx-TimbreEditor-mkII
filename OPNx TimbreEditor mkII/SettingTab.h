
#pragma once



#include "afxdialogex.h"
#include "martin-finke/Filter.h"
#include "Common/Common.h"
#include "Chip.h"



class CFmStationDlg;



class CSettingTab : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingTab)
	
	public:
		virtual ~CSettingTab();
		
		CSettingTab(CWnd* pParent = nullptr);
		
		#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_SETTING_TAB };
		#endif
	
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);
		virtual BOOL OnInitDialog();
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		
		afx_msg void OnCbnSelchangeSettingCopyFormatExtCombo();
		afx_msg void OnBnClickedSettingSwapCopyFormatCheck();
		afx_msg void OnBnClickedSrttingAutoCopyFormatCheck();
		afx_msg void OnBnClickedSettingSLTLLink();
		afx_msg void OnNMCustomdrawSettingVolumeSlider(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnNMCustomdrawSettingPreviewSlider(NMHDR* pNMHDR, LRESULT* pResult);
		
		afx_msg void OnCbnSelchangeSettingFilterCombo();
		afx_msg void OnNMCustomdrawSettingCutoffSlider(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnNMCustomdrawSettingResonanceSlider(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnBnClickedSettingDcCutCheck();
		afx_msg void OnNMCustomdrawSettingDcCutRateSlider(NMHDR* pNMHDR, LRESULT* pResult);
		
		afx_msg void OnCbnSelchangeSettingPortamentoCombo();
		afx_msg void OnNMCustomdrawSettingTuningSlider(NMHDR* pNMHDR, LRESULT* pResult);
		
		DECLARE_MESSAGE_MAP()
	
	protected:
		CFmStationDlg& GetFmStationDlg();
	
	protected:
		CComboBox m_CComboBoxFormatType;
		CButton m_CButtonSwapCopyFormat;
		CButton m_CButtonAutoCopyFormat;
		CButton m_CButtonSLTLLink;
		CSliderCtrl m_CSliderCtrlVolume;
		CSliderCtrl m_CSliderCtrlPreview;
		
		CComboBox m_CComboBoxFilter;
		CSliderCtrl m_CSliderCtrlCutoff;
		CSliderCtrl m_CSliderCtrlResonance;
		CButton m_CButtonDCCut;
		CSliderCtrl m_CSliderCtrlDCCutRate;
		
		CComboBox m_CComboBoxPortamento;
		CSliderCtrl m_CSliderCtrlTuning;
		CStatic m_CStaticTuning;
	
	public:
		enum EFormatType
		{
			Mucom,
			PmdOPN,
			PmdOPM,
			Fmp,
			FmpA,
			Fmp7F,
			Fmp7FA,
			Fmp7FC,
			Mml2VgmF,
			Mml2VgmN,
			Mml2VgmM,
			mucomDotNET,
			MAmidiMemoMOPN,
			MAmidiMemoMOPM,
			ZMusicV,
			ZMusicAt,
			NagDrv,
			NrtDrv,
			MmlDrv,
			MmlGui,
			Muap98,
			V3MmlOPN,
			V3MmlOPNA,
			V3MmlOPM,
			N88Basic,
			Num,
		};
	
	public:
		EFormatType GetFormatType();
		void SetFormatType(EFormatType EFormatType);
		bool IsSwapCopyFormat();
		bool IsAutoCopyFormat();
		bool IsSLTLLink();
		double GetVolume();
		void SetVolume(double Volume);
		i8 GetPreview();
		void SetPreview(i8 Preview);
		
		Filter::FilterMode GetFilterMode();
		double GetCutoff();
		double GetResonance();
		bool IsDCCut();
		double GetDCCutRate();
		
		CChip::EPortamentoMode GetPortamentoMode();
		float GetTuning();
};
