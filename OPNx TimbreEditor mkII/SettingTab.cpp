



#include "pch.h"
#include "FmStation.h"
#include "FmStationDlg.h"
#include "afxdialogex.h"
#include "SettingTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



IMPLEMENT_DYNAMIC(CSettingTab, CDialogEx)



CSettingTab::~CSettingTab()
{
}



CSettingTab::CSettingTab(CWnd* pParent /*=nullptr*/)
:CDialogEx(IDD_SETTING_TAB, pParent)
{
}



void CSettingTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETTING_COPY_FORMAT_COMBO, m_CComboBoxFormatType);
	DDX_Control(pDX, IDC_SETTING_SWAP_COPY_FORMAT_CHECK, m_CButtonSwapCopyFormat);
	DDX_Control(pDX, IDC_SETTING_AUTO_COPY_FORMAT_CHECK, m_CButtonAutoCopyFormat);
	DDX_Control(pDX, IDC_SETTING_SLTL_LINK, m_CButtonSLTLLink);
	DDX_Control(pDX, IDC_SETTING_VOLUME_SLIDER, m_CSliderCtrlVolume);
	DDX_Control(pDX, IDC_SETTING_PREVIEW_SLIDER, m_CSliderCtrlPreview);
	DDX_Control(pDX, IDC_SETTING_FILTER_COMBO, m_CComboBoxFilter);
	DDX_Control(pDX, IDC_SETTING_CUTOFF_SLIDER, m_CSliderCtrlCutoff);
	DDX_Control(pDX, IDC_SETTING_RESONANCE_SLIDER, m_CSliderCtrlResonance);
	DDX_Control(pDX, IDC_SETTING_DC_CUT_CHECK, m_CButtonDCCut);
	DDX_Control(pDX, IDC_SETTING_DC_CUT_RATE_SLIDER, m_CSliderCtrlDCCutRate);
	DDX_Control(pDX, IDC_SETTING_PORTAMENTO_COMBO, m_CComboBoxPortamento);
	DDX_Control(pDX, IDC_SETTING_TUNING_SLIDER, m_CSliderCtrlTuning);
	DDX_Control(pDX, IDC_SETTING_TUNING_STATIC, m_CStaticTuning);
}



BEGIN_MESSAGE_MAP(CSettingTab, CDialogEx)
	ON_CBN_SELCHANGE(IDC_SETTING_COPY_FORMAT_COMBO, &CSettingTab::OnCbnSelchangeSettingCopyFormatExtCombo)
	ON_BN_CLICKED(IDC_SETTING_SWAP_COPY_FORMAT_CHECK, &CSettingTab::OnBnClickedSettingSwapCopyFormatCheck)
	ON_BN_CLICKED(IDC_SETTING_AUTO_COPY_FORMAT_CHECK, &CSettingTab::OnBnClickedSrttingAutoCopyFormatCheck)
	ON_BN_CLICKED(IDC_SETTING_SLTL_LINK, &CSettingTab::OnBnClickedSettingSLTLLink)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SETTING_VOLUME_SLIDER, &CSettingTab::OnNMCustomdrawSettingVolumeSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SETTING_PREVIEW_SLIDER, &CSettingTab::OnNMCustomdrawSettingPreviewSlider)
	ON_CBN_SELCHANGE(IDC_SETTING_FILTER_COMBO, &CSettingTab::OnCbnSelchangeSettingFilterCombo)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SETTING_CUTOFF_SLIDER, &CSettingTab::OnNMCustomdrawSettingCutoffSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SETTING_RESONANCE_SLIDER, &CSettingTab::OnNMCustomdrawSettingResonanceSlider)
	ON_BN_CLICKED(IDC_SETTING_DC_CUT_CHECK, &CSettingTab::OnBnClickedSettingDcCutCheck)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SETTING_DC_CUT_RATE_SLIDER, &CSettingTab::OnNMCustomdrawSettingDcCutRateSlider)
	ON_CBN_SELCHANGE(IDC_SETTING_PORTAMENTO_COMBO, &CSettingTab::OnCbnSelchangeSettingPortamentoCombo)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SETTING_TUNING_SLIDER, &CSettingTab::OnNMCustomdrawSettingTuningSlider)
END_MESSAGE_MAP()



BOOL CSettingTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();//call DoDataExchange()
	
	auto SetDropdownSize = [this](CComboBox& rCComboBox)
	{
		//rCComboBox.SetMinVisibleItems(rCComboBox.GetCount());//doesn't work
		
		CRect Rect;
		rCComboBox.GetDroppedControlRect(&Rect);
		Rect.bottom += 1000;
		rCComboBox.GetParent()->ScreenToClient(&Rect);
		rCComboBox.MoveWindow(&Rect);
	};
	
	m_CComboBoxFormatType.SetCurSel(theApp.GetValue(_T("FormatType"), (int)EFormatType::Mucom));
	SetDropdownSize(m_CComboBoxFormatType);
	
	m_CButtonSwapCopyFormat.SetCheck(theApp.GetValue(_T("SwapCopyFormat"), BST_UNCHECKED));
	
	m_CButtonAutoCopyFormat.SetCheck(theApp.GetValue(_T("AutoCopyFormat"), BST_UNCHECKED));
	
	m_CButtonSLTLLink.SetCheck(theApp.GetValue(_T("SLTLLink"), BST_UNCHECKED));
	
	m_CSliderCtrlVolume.SetRange(0, 40);
	m_CSliderCtrlVolume.SetPos(theApp.GetValue(_T("Volume"), 10));
	
	m_CSliderCtrlPreview.SetRange(0, 15);
	m_CSliderCtrlPreview.SetPos(theApp.GetValue(_T("Preview"), 0));
	
	m_CComboBoxFilter.SetCurSel(theApp.GetValue(_T("FilterMode"), (int)Filter::FilterMode::FILTER_MODE_LOWPASS));
	SetDropdownSize(m_CComboBoxFilter);
	
	m_CSliderCtrlCutoff.SetRange(1, 99);
	m_CSliderCtrlCutoff.SetPos(theApp.GetValue(_T("Cutoff"), 99));
	
	m_CSliderCtrlResonance.SetRange(1, 100);
	m_CSliderCtrlResonance.SetPos(theApp.GetValue(_T("Resonance"), 1));
	
	m_CButtonDCCut.SetCheck(theApp.GetValue(_T("DCCut"), BST_UNCHECKED));
	
	m_CSliderCtrlDCCutRate.SetRange(0, 9);
	m_CSliderCtrlDCCutRate.SetPos(theApp.GetValue(_T("DCCutRate"), 5));
	
	m_CComboBoxPortamento.SetCurSel(theApp.GetValue(_T("PortamentoMode"), (int)CChip::EPortamentoMode_TimeMode));
	SetDropdownSize(m_CComboBoxPortamento);
	
	m_CSliderCtrlTuning.SetRange(390, 490);
	m_CSliderCtrlTuning.SetPos(theApp.GetValue(_T("Tuning"), 440));
	
	CString TuningText;
	TuningText.Format(_T("%dHz"), (int)GetTuning());
	m_CStaticTuning.SetWindowText(TuningText);
	
	return FALSE;
}



BOOL CSettingTab::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message){
		case WM_KEYDOWN:{
			auto bControl = (GetKeyState(VK_LCONTROL) < 0) | (GetKeyState(VK_RCONTROL) < 0);
			if (bControl){
				switch (pMsg->wParam){
					case 'W':{			theApp.GetFmStationDlg().WavRecord();	return TRUE;	}
					
					case VK_INSERT:{	GetFmStationDlg().PreviewUp();		return TRUE;	}
					case VK_DELETE:{	GetFmStationDlg().PreviewDown();	return TRUE;	}
				}
			} else {
				switch (pMsg->wParam){
					case VK_RETURN:
					case VK_ESCAPE:
					{
						return TRUE;
					}
					case VK_INSERT:{	GetFmStationDlg().VolumeUp();	return TRUE;	}
					case VK_DELETE:{	GetFmStationDlg().VolumeDown();	return TRUE;	}
				}
			}
			break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}



CFmStationDlg& CSettingTab::GetFmStationDlg()
{
	return theApp.GetFmStationDlg();
}



void CSettingTab::OnCbnSelchangeSettingCopyFormatExtCombo()
{
	theApp.SetValue(_T("FormatType"), (int)GetFormatType());
}



void CSettingTab::OnBnClickedSettingSwapCopyFormatCheck()
{
	theApp.SetValue(_T("SwapCopyFormat"), (IsSwapCopyFormat())? BST_CHECKED: BST_UNCHECKED);
}



void CSettingTab::OnBnClickedSrttingAutoCopyFormatCheck()
{
	theApp.SetValue(_T("AutoCopyFormat"), (IsAutoCopyFormat())? BST_CHECKED: BST_UNCHECKED);
}



void CSettingTab::OnBnClickedSettingSLTLLink()
{
	theApp.SetValue(_T("SLTLLink"), (IsSLTLLink())? BST_CHECKED: BST_UNCHECKED);
}



void CSettingTab::OnNMCustomdrawSettingVolumeSlider(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	auto Value = GetVolume();
	theApp.SetValue(_T("Volume"), (int)Value);
	GetFmStationDlg().SetVolume(Value);
	*pResult = 0;
}



void CSettingTab::OnNMCustomdrawSettingPreviewSlider(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	auto Value = GetPreview();
	theApp.SetValue(_T("Preview"), (int)Value);
	GetFmStationDlg().SetPreview(Value);
	*pResult = 0;
}



void CSettingTab::OnCbnSelchangeSettingFilterCombo()
{
	theApp.SetValue(_T("FilterMode"), (int)GetFilterMode());
	GetFmStationDlg().GetVoice()->SetFilterMode(GetFilterMode());
}



void CSettingTab::OnNMCustomdrawSettingCutoffSlider(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	auto Value = GetCutoff();
	//Log(_T("Cutoff {}"), Value);
	theApp.SetValue(_T("Cutoff"), (int)(Value * 100.0));
	GetFmStationDlg().GetVoice()->SetCutoff(Value);
	*pResult = 0;
}



void CSettingTab::OnNMCustomdrawSettingResonanceSlider(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	auto Value = GetResonance();
	//Log(_T("Resonance {}"), Value);
	theApp.SetValue(_T("Resonance"), (int)(Value * 100.0));
	GetFmStationDlg().GetVoice()->SetResonance(Value);
	*pResult = 0;
}



void CSettingTab::OnBnClickedSettingDcCutCheck()
{
	theApp.SetValue(_T("DCCut"), (IsDCCut())? BST_CHECKED: BST_UNCHECKED);
	GetFmStationDlg().GetVoice()->SetDCCut(IsDCCut());
}



void CSettingTab::OnNMCustomdrawSettingDcCutRateSlider(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	auto Value = GetDCCutRate();
	//Log(_T("DCCutRate {}"), Value);
	theApp.SetValue(_T("DCCutRate"), (int)((Value - 0.99) * 1000.0));
	GetFmStationDlg().GetVoice()->SetDCCutRate(Value);
	*pResult = 0;
}



void CSettingTab::OnCbnSelchangeSettingPortamentoCombo()
{
	theApp.SetValue(_T("PortamentoMode"), (int)GetPortamentoMode());
	GetFmStationDlg().GetVoice()->SetPortamentoMode(GetPortamentoMode());
}



void CSettingTab::OnNMCustomdrawSettingTuningSlider(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	auto Value = GetTuning();
	//Log(_T("Tuning {}"), Value);
	theApp.SetValue(_T("Tuning"), (int)Value);
	GetFmStationDlg().GetVoice()->SetTuning(Value);
	
	CString TuningText;
	TuningText.Format(_T("%dHz"), (int)Value);
	m_CStaticTuning.SetWindowText(TuningText);
	*pResult = 0;
}



CSettingTab::EFormatType CSettingTab::GetFormatType()
{
	return (EFormatType)m_CComboBoxFormatType.GetCurSel();
}



void CSettingTab::SetFormatType(EFormatType EFormatType)
{
	m_CComboBoxFormatType.SetCurSel((int)EFormatType);
}



bool CSettingTab::IsSwapCopyFormat()
{
	return (m_CButtonSwapCopyFormat.GetCheck() == BST_CHECKED);
}



bool CSettingTab::IsAutoCopyFormat()
{
	return (m_CButtonAutoCopyFormat.GetCheck() == BST_CHECKED);
}



bool CSettingTab::IsSLTLLink()
{
	return (m_CButtonSLTLLink.GetCheck() == BST_CHECKED);
}



double CSettingTab::GetVolume()
{
	return m_CSliderCtrlVolume.GetPos();
}



void CSettingTab::SetVolume(double Volume)
{
	m_CSliderCtrlVolume.SetPos((int)Volume);
	
	auto Value = GetVolume();
	theApp.SetValue(_T("Volume"), (int)Value);
}



i8 CSettingTab::GetPreview()
{
	return m_CSliderCtrlPreview.GetPos();
}



void CSettingTab::SetPreview(i8 Preview)
{
	m_CSliderCtrlPreview.SetPos(Preview);
	
	auto Value = GetPreview();
	theApp.SetValue(_T("Preview"), Value);
}



Filter::FilterMode CSettingTab::GetFilterMode()
{
	return (Filter::FilterMode)m_CComboBoxFilter.GetCurSel();
}



double CSettingTab::GetCutoff()
{
	return m_CSliderCtrlCutoff.GetPos() / 100.0;
}



double CSettingTab::GetResonance()
{
	return m_CSliderCtrlResonance.GetPos() / 100.0;
}



bool CSettingTab::IsDCCut()
{
	return (m_CButtonDCCut.GetCheck() == BST_CHECKED);
}



double CSettingTab::GetDCCutRate()
{
	return (m_CSliderCtrlDCCutRate.GetPos() / 1000.0) + 0.99;
}



CChip::EPortamentoMode CSettingTab::GetPortamentoMode()
{
	return (CChip::EPortamentoMode)m_CComboBoxPortamento.GetCurSel();
}



float CSettingTab::GetTuning()
{
	return (float)m_CSliderCtrlTuning.GetPos();
}
