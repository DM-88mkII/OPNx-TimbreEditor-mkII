



#include "pch.h"
#include "afxdialogex.h"
#include "FmStation.h"
#include "FmStationDlg.h"
#include "TimbreDlg.h"
#include "Intermediate.h"



IMPLEMENT_DYNAMIC(CTimbreDlg, CDialogEx)



CTimbreDlg::~CTimbreDlg()
{
	TimbreCallback();
}



CTimbreDlg::CTimbreDlg(CWnd* pParent)
:CDialogEx(IDD_TIMBRE_DIALOG, pParent)
,m_IsActive(false)
,m_Edit{}
,m_Ctrl{
	{	// m_aaParam
		{	IDC_TIMBRE_EN_STATIC,
			IDC_TIMBRE_FDE_STATIC,
			IDC_TIMBRE_ALG_STATIC,
			IDC_TIMBRE_FB_STATIC,
			IDC_TIMBRE_NUM_STATIC,
			IDC_TIMBRE_KML_STATIC,
			IDC_TIMBRE_KMH_STATIC,
			IDC_TIMBRE_VOID1_STATIC,
			IDC_TIMBRE_VOID2_STATIC,
			IDC_TIMBRE_VOID3_STATIC,
			IDC_TIMBRE_VOID4_STATIC,
			IDC_TIMBRE_SE_STATIC,
			IDC_TIMBRE_FIX_STATIC,
			IDC_TIMBRE_KT_STATIC,
			IDC_TIMBRE_FDT_STATIC,
		},
		{	IDC_TIMBRE_EN1_STATIC,
			IDC_TIMBRE_FDE1_STATIC,
			IDC_TIMBRE_AR1_STATIC,
			IDC_TIMBRE_DR1_STATIC,
			IDC_TIMBRE_SR1_STATIC,
			IDC_TIMBRE_RR1_STATIC,
			IDC_TIMBRE_SL1_STATIC,
			IDC_TIMBRE_TL1_STATIC,
			IDC_TIMBRE_KS1_STATIC,
			IDC_TIMBRE_MT1_STATIC,
			IDC_TIMBRE_DT1_STATIC,
			IDC_TIMBRE_SSG1_STATIC,
			IDC_TIMBRE_SE_FIX1_STATIC,
			IDC_TIMBRE_SE_KT1_STATIC,
			IDC_TIMBRE_SE_FDT1_STATIC,
		},
		{	IDC_TIMBRE_EN2_STATIC,
			IDC_TIMBRE_FDE2_STATIC,
			IDC_TIMBRE_AR2_STATIC,
			IDC_TIMBRE_DR2_STATIC,
			IDC_TIMBRE_SR2_STATIC,
			IDC_TIMBRE_RR2_STATIC,
			IDC_TIMBRE_SL2_STATIC,
			IDC_TIMBRE_TL2_STATIC,
			IDC_TIMBRE_KS2_STATIC,
			IDC_TIMBRE_MT2_STATIC,
			IDC_TIMBRE_DT2_STATIC,
			IDC_TIMBRE_SSG2_STATIC,
			IDC_TIMBRE_SE_FIX2_STATIC,
			IDC_TIMBRE_SE_KT2_STATIC,
			IDC_TIMBRE_SE_FDT2_STATIC,
		},
		{	IDC_TIMBRE_EN3_STATIC,
			IDC_TIMBRE_FDE3_STATIC,
			IDC_TIMBRE_AR3_STATIC,
			IDC_TIMBRE_DR3_STATIC,
			IDC_TIMBRE_SR3_STATIC,
			IDC_TIMBRE_RR3_STATIC,
			IDC_TIMBRE_SL3_STATIC,
			IDC_TIMBRE_TL3_STATIC,
			IDC_TIMBRE_KS3_STATIC,
			IDC_TIMBRE_MT3_STATIC,
			IDC_TIMBRE_DT3_STATIC,
			IDC_TIMBRE_SSG3_STATIC,
			IDC_TIMBRE_SE_FIX3_STATIC,
			IDC_TIMBRE_SE_KT3_STATIC,
			IDC_TIMBRE_SE_FDT3_STATIC,
		},
		{	IDC_TIMBRE_EN4_STATIC,
			IDC_TIMBRE_FDE4_STATIC,
			IDC_TIMBRE_AR4_STATIC,
			IDC_TIMBRE_DR4_STATIC,
			IDC_TIMBRE_SR4_STATIC,
			IDC_TIMBRE_RR4_STATIC,
			IDC_TIMBRE_SL4_STATIC,
			IDC_TIMBRE_TL4_STATIC,
			IDC_TIMBRE_KS4_STATIC,
			IDC_TIMBRE_MT4_STATIC,
			IDC_TIMBRE_DT4_STATIC,
			IDC_TIMBRE_SSG4_STATIC,
			IDC_TIMBRE_SE_FIX4_STATIC,
			IDC_TIMBRE_SE_KT4_STATIC,
			IDC_TIMBRE_SE_FDT4_STATIC,
		},
	},
}
{
	m_Ctrl.mx = 2;
	m_Ctrl.my = 1;
	m_Ctrl.m_Octave = 5;
	std::ranges::fill(m_Ctrl.m_aNote, -1);
	
	SetCur(m_Ctrl.mx, m_Ctrl.my, false);
}



void CTimbreDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TIMBREDLG_EDIT_NAME, m_CEditName);

	DDX_Control(pDX, IDC_TIMBRE_ALG0_STATIC, m_Edit.m_aCStaticALG[0]);
	DDX_Control(pDX, IDC_TIMBRE_ALG1_STATIC, m_Edit.m_aCStaticALG[1]);
	DDX_Control(pDX, IDC_TIMBRE_ALG2_STATIC, m_Edit.m_aCStaticALG[2]);
	DDX_Control(pDX, IDC_TIMBRE_ALG3_STATIC, m_Edit.m_aCStaticALG[3]);
	DDX_Control(pDX, IDC_TIMBRE_ALG4_STATIC, m_Edit.m_aCStaticALG[4]);
	DDX_Control(pDX, IDC_TIMBRE_ALG5_STATIC, m_Edit.m_aCStaticALG[5]);
	DDX_Control(pDX, IDC_TIMBRE_ALG6_STATIC, m_Edit.m_aCStaticALG[6]);
	DDX_Control(pDX, IDC_TIMBRE_ALG7_STATIC, m_Edit.m_aCStaticALG[7]);
	DDX_Control(pDX, IDC_TIMBRE_LOG, m_CStaticLog);
}



BEGIN_MESSAGE_MAP(CTimbreDlg, CDialogEx)
	ON_COMMAND(IDOK, OnOK)
	ON_COMMAND(IDCANCEL, OnCancel)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_TIMBREDLG_OK, &CTimbreDlg::OnBnClickedTimbredlgOk)
END_MESSAGE_MAP()



BOOL CTimbreDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();//call DoDataExchange()
	
	{	// 
		int width = 525;
		int height = 32;
		
		CRect DlgRect;
		GetWindowRect(&DlgRect);
		SetWindowPos(NULL, DlgRect.left, DlgRect.top, width, DlgRect.Height(), (SWP_NOZORDER | SWP_NOMOVE));
		
		m_CEditName.SetLimitText(32);
		
		CRect KeyboardRect;
		m_CKeyboard.SubclassDlgItem(IDC_TIMBREDLG_STATIC_KEYBOARD, this);
		m_CKeyboard.GetWindowRect(&KeyboardRect); ScreenToClient(&KeyboardRect);
		m_CKeyboard.SetWindowPos(NULL, KeyboardRect.left, KeyboardRect.top, width, height, (SWP_NOZORDER | SWP_NOMOVE));
		m_CKeyboard.GetWindowRect(&KeyboardRect); ScreenToClient(&KeyboardRect);
		m_CKeyboard.ModifyStyle(0, SS_NOTIFY);
		m_CKeyboard.Initialize(width, height);
		m_CKeyboard.SetCallback(this);
		
		CRect Rect(0, 0, width, KeyboardRect.bottom);
		auto dwStyle = GetStyle();
		auto dwExStyle = GetExStyle();
		auto hasMenu = (GetMenu() != NULL);
		::AdjustWindowRectEx(&Rect, dwStyle, hasMenu, dwExStyle);
		
		GetWindowRect(&DlgRect);
		SetWindowPos(NULL, DlgRect.left, DlgRect.top, Rect.Width(), Rect.Height(), (SWP_NOZORDER | SWP_NOMOVE));
	}
	
	{	// 
		SetPicture(0);
	}
	
	return TRUE;
}



BOOL CTimbreDlg::PreTranslateMessage(MSG* pMsg)
{
	auto pFocusWnd = GetFocus();
	if (pFocusWnd && pFocusWnd->IsKindOf(RUNTIME_CLASS(CEdit))){
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB){
			m_CKeyboard.SetFocus();
			RedrawParam();
			return TRUE;
		}
		return CDialogEx::PreTranslateMessage(pMsg);
	}
	
	switch (pMsg->message){
		case WM_KEYDOWN:{
			auto bShift = (GetKeyState(VK_LSHIFT) < 0) | (GetKeyState(VK_RSHIFT) < 0);
			auto bControl = (GetKeyState(VK_LCONTROL) < 0) | (GetKeyState(VK_RCONTROL) < 0);
			if (bControl){
				switch (pMsg->wParam){
					case VK_UP:{		return TRUE;	}
					case VK_DOWN:{		return TRUE;	}
					
					case 'C':{			Copy(bShift);	return TRUE;	}
					case 'V':{			Paste();		return TRUE;	}
					case 'Z':{			Undo();			return TRUE;	}
					
					case 'R':{			Rotate();		return TRUE;	}
					
					case 'W':{			theApp.GetFmStationDlg().WavRecord();	return TRUE;	}
					
					case VK_INSERT:{	PreviewUp();	return TRUE;	}
					case VK_DELETE:{	PreviewDown();	return TRUE;	}
				}
			} else {
				switch (pMsg->wParam){
					case VK_TAB:{		m_CEditName.SetFocus();	RedrawParam();	return TRUE; }
					
					case VK_HOME:{		m_Ctrl.m_Octave += (m_Ctrl.m_Octave < 10)? 1: 0;	Log(_T("o{}"), m_Ctrl.m_Octave-1);	return TRUE;	}
					case VK_END:{		m_Ctrl.m_Octave -= (m_Ctrl.m_Octave >  0)? 1: 0;	Log(_T("o{}"), m_Ctrl.m_Octave-1);	return TRUE;	}
					
					case VK_INSERT:{	VolumeUp();		return TRUE;	}
					case VK_DELETE:{	VolumeDown();	return TRUE;	}
					
					case VK_UP:{		MoveCursor(0, -1);	return TRUE;	}
					case VK_DOWN:{		MoveCursor(0, 1);	return TRUE;	}
					case VK_LEFT:{		MoveCursor(-1, 0);	return TRUE;	}
					case VK_RIGHT:{		MoveCursor(1, 0);	return TRUE;	}
					
					case '0':
					case VK_NUMPAD0:{	if (!m_Ctrl.m_bRotate){	GetParamValue(m_Ctrl.mx, m_Ctrl.my).InputChar('0');	RedrawParam();	}	return TRUE;	}
					case '1':
					case VK_NUMPAD1:{	if (!m_Ctrl.m_bRotate){	GetParamValue(m_Ctrl.mx, m_Ctrl.my).InputChar('1');	RedrawParam();	}	return TRUE;	}
					case '2':
					case VK_NUMPAD2:{	if (!m_Ctrl.m_bRotate){	GetParamValue(m_Ctrl.mx, m_Ctrl.my).InputChar('2');	RedrawParam();	}	return TRUE;	}
					case '3':
					case VK_NUMPAD3:{	if (!m_Ctrl.m_bRotate){	GetParamValue(m_Ctrl.mx, m_Ctrl.my).InputChar('3');	RedrawParam();	}	return TRUE;	}
					case '4':
					case VK_NUMPAD4:{	if (!m_Ctrl.m_bRotate){	GetParamValue(m_Ctrl.mx, m_Ctrl.my).InputChar('4');	RedrawParam();	}	return TRUE;	}
					case '5':
					case VK_NUMPAD5:{	if (!m_Ctrl.m_bRotate){	GetParamValue(m_Ctrl.mx, m_Ctrl.my).InputChar('5');	RedrawParam();	}	return TRUE;	}
					case '6':
					case VK_NUMPAD6:{	if (!m_Ctrl.m_bRotate){	GetParamValue(m_Ctrl.mx, m_Ctrl.my).InputChar('6');	RedrawParam();	}	return TRUE;	}
					case '7':
					case VK_NUMPAD7:{	if (!m_Ctrl.m_bRotate){	GetParamValue(m_Ctrl.mx, m_Ctrl.my).InputChar('7');	RedrawParam();	}	return TRUE;	}
					case '8':
					case VK_NUMPAD8:{	if (!m_Ctrl.m_bRotate){	GetParamValue(m_Ctrl.mx, m_Ctrl.my).InputChar('8');	RedrawParam();	}	return TRUE;	}
					case '9':
					case VK_NUMPAD9:{	if (!m_Ctrl.m_bRotate){	GetParamValue(m_Ctrl.mx, m_Ctrl.my).InputChar('9');	RedrawParam();	}	return TRUE;	}
					case VK_OEM_MINUS:
					case VK_SUBTRACT:{	if (!m_Ctrl.m_bRotate){	GetParamValue(m_Ctrl.mx, m_Ctrl.my).InputChar('-');	RedrawParam();	}	return TRUE;	}
					case VK_SPACE:{		if (!m_Ctrl.m_bRotate){	GetParamValue(m_Ctrl.mx, m_Ctrl.my).InputChar(' ');	RedrawParam();	}	return TRUE;	}
					
					case VK_BACK:{		if (!m_Ctrl.m_bRotate){	GetParamValue(m_Ctrl.mx, m_Ctrl.my).InputBackSpace();	RedrawParam();	}	return TRUE;	}
					case VK_RETURN:{	if (!m_Ctrl.m_bRotate){	GetParamValue(m_Ctrl.mx, m_Ctrl.my).InputEnter();		RedrawParam();	}	return TRUE;	}
					case VK_ESCAPE:{	if (!m_Ctrl.m_bRotate){	GetParamValue(m_Ctrl.mx, m_Ctrl.my).InputCancel();		RedrawParam();	} else {	RotateCancel();	}	return TRUE;	}
					
					case VK_PRIOR:{		if (!m_Ctrl.m_bRotate){	AddParamValue(m_Ctrl.mx, m_Ctrl.my, (bShift)?  4:  1);	RedrawParam();	}	return TRUE;	}
					case VK_NEXT:{		if (!m_Ctrl.m_bRotate){	AddParamValue(m_Ctrl.mx, m_Ctrl.my, (bShift)? -4: -1);	RedrawParam();	}	return TRUE;	}
					
					case 'Z':{			Play(0, _T("c"));	return TRUE;	}
					case 'S':{			Play(1, _T("c+"));	return TRUE;	}
					case 'X':{			Play(2, _T("d"));	return TRUE;	}
					case 'D':{			Play(3, _T("d+"));	return TRUE;	}
					case 'C':{			Play(4, _T("e"));	return TRUE;	}
					case 'V':{			Play(5, _T("f"));	return TRUE;	}
					case 'G':{			Play(6, _T("f+"));	return TRUE;	}
					case 'B':{			Play(7, _T("g"));	return TRUE;	}
					case 'H':{			Play(8, _T("g+"));	return TRUE;	}
					case 'N':{			Play(9, _T("a"));	return TRUE;	}
					case 'J':{			Play(10, _T("a+"));	return TRUE;	}
					case 'M':{			Play(11, _T("b"));	return TRUE;	}
					case VK_OEM_COMMA:{	Play(12, _T("c"));	return TRUE;	}
					
					// nop
					case VK_MULTIPLY:
					case VK_ADD:
					case VK_SEPARATOR:
					case VK_DECIMAL:
					case VK_DIVIDE:
					case VK_OEM_PLUS:
					case VK_OEM_PERIOD:
					case VK_OEM_1:
					case VK_OEM_2:
					case VK_OEM_3:
					case VK_OEM_4:
					case VK_OEM_5:
					case VK_OEM_6:
					case VK_OEM_7:
					case VK_OEM_8:
					case VK_OEM_102:
					case 'Q':
					case 'W':
					case 'E':
					case 'R':
					case 'T':
					case 'Y':
					case 'U':
					case 'I':
					case 'O':
					case 'P':
					case 'A':
					case 'F':
					case 'K':
					case 'L':
					{
						return TRUE;
					}
				}
			}
			break;
		}
		case WM_KEYUP:{
			{	// 
				switch (pMsg->wParam){
					case 'Z':{			Stop(0);	return TRUE;	}
					case 'S':{			Stop(1);	return TRUE;	}
					case 'X':{			Stop(2);	return TRUE;	}
					case 'D':{			Stop(3);	return TRUE;	}
					case 'C':{			Stop(4);	return TRUE;	}
					case 'V':{			Stop(5);	return TRUE;	}
					case 'G':{			Stop(6);	return TRUE;	}
					case 'B':{			Stop(7);	return TRUE;	}
					case 'H':{			Stop(8);	return TRUE;	}
					case 'N':{			Stop(9);	return TRUE;	}
					case 'J':{			Stop(10);	return TRUE;	}
					case 'M':{			Stop(11);	return TRUE;	}
					case VK_OEM_COMMA:{	Stop(12);	return TRUE;	}
				}
			}
			break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}



HBRUSH CTimbreDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	auto hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	
	switch (pWnd->GetDlgCtrlID()){
		case IDC_TIMBRE_EN_STATIC:{		SetColor(pDC, 0, 0); break; }
		case IDC_TIMBRE_FDE_STATIC:{	SetColor(pDC, 1, 0); break; }
		case IDC_TIMBRE_ALG_STATIC:{	SetColor(pDC, 2, 0); break; }
		case IDC_TIMBRE_FB_STATIC:{		SetColor(pDC, 3, 0); break; }
		case IDC_TIMBRE_NUM_STATIC:{	SetColor(pDC, 4, 0); break; }
		case IDC_TIMBRE_KML_STATIC:{	SetColor(pDC, 5, 0); break; }
		case IDC_TIMBRE_KMH_STATIC:{	SetColor(pDC, 6, 0); break; }
		case IDC_TIMBRE_VOID1_STATIC:{	SetColor(pDC, 7, 0); break; }
		case IDC_TIMBRE_VOID2_STATIC:{	SetColor(pDC, 8, 0); break; }
		case IDC_TIMBRE_VOID3_STATIC:{	SetColor(pDC, 9, 0); break; }
		case IDC_TIMBRE_VOID4_STATIC:{	SetColor(pDC, 10, 0); break; }
		case IDC_TIMBRE_SE_STATIC:{		SetColor(pDC, 11, 0); break; }
		case IDC_TIMBRE_FIX_STATIC:{	SetColor(pDC, 12, 0); break; }
		case IDC_TIMBRE_KT_STATIC:{		SetColor(pDC, 13, 0); break; }
		case IDC_TIMBRE_FDT_STATIC:{	SetColor(pDC, 14, 0); break; }
		
		case IDC_TIMBRE_EN1_STATIC:{	SetColor(pDC, 0, 1); break; }
		case IDC_TIMBRE_FDE1_STATIC:{	SetColor(pDC, 1, 1); break; }
		case IDC_TIMBRE_AR1_STATIC:{	SetColor(pDC, 2, 1); break; }
		case IDC_TIMBRE_DR1_STATIC:{	SetColor(pDC, 3, 1); break; }
		case IDC_TIMBRE_SR1_STATIC:{	SetColor(pDC, 4, 1); break; }
		case IDC_TIMBRE_RR1_STATIC:{	SetColor(pDC, 5, 1); break; }
		case IDC_TIMBRE_SL1_STATIC:{	SetColor(pDC, 6, 1); break; }
		case IDC_TIMBRE_TL1_STATIC:{	SetColor(pDC, 7, 1); break; }
		case IDC_TIMBRE_KS1_STATIC:{	SetColor(pDC, 8, 1); break; }
		case IDC_TIMBRE_MT1_STATIC:{	SetColor(pDC, 9, 1); break; }
		case IDC_TIMBRE_DT1_STATIC:{	SetColor(pDC, 10, 1); break; }
		case IDC_TIMBRE_SSG1_STATIC:{	SetColor(pDC, 11, 1); break; }
		case IDC_TIMBRE_SE_FIX1_STATIC:{SetColor(pDC, 12, 1); break; }
		case IDC_TIMBRE_SE_KT1_STATIC:{	SetColor(pDC, 13, 1); break; }
		case IDC_TIMBRE_SE_FDT1_STATIC:{SetColor(pDC, 14, 1); break; }
		
		case IDC_TIMBRE_EN2_STATIC:{	SetColor(pDC, 0, 2); break; }
		case IDC_TIMBRE_FDE2_STATIC:{	SetColor(pDC, 1, 2); break; }
		case IDC_TIMBRE_AR2_STATIC:{	SetColor(pDC, 2, 2); break; }
		case IDC_TIMBRE_DR2_STATIC:{	SetColor(pDC, 3, 2); break; }
		case IDC_TIMBRE_SR2_STATIC:{	SetColor(pDC, 4, 2); break; }
		case IDC_TIMBRE_RR2_STATIC:{	SetColor(pDC, 5, 2); break; }
		case IDC_TIMBRE_SL2_STATIC:{	SetColor(pDC, 6, 2); break; }
		case IDC_TIMBRE_TL2_STATIC:{	SetColor(pDC, 7, 2); break; }
		case IDC_TIMBRE_KS2_STATIC:{	SetColor(pDC, 8, 2); break; }
		case IDC_TIMBRE_MT2_STATIC:{	SetColor(pDC, 9, 2); break; }
		case IDC_TIMBRE_DT2_STATIC:{	SetColor(pDC, 10, 2); break; }
		case IDC_TIMBRE_SSG2_STATIC:{	SetColor(pDC, 11, 2); break; }
		case IDC_TIMBRE_SE_FIX2_STATIC:{SetColor(pDC, 12, 2); break; }
		case IDC_TIMBRE_SE_KT2_STATIC:{	SetColor(pDC, 13, 2); break; }
		case IDC_TIMBRE_SE_FDT2_STATIC:{SetColor(pDC, 14, 2); break; }
		
		case IDC_TIMBRE_EN3_STATIC:{	SetColor(pDC, 0, 3); break; }
		case IDC_TIMBRE_FDE3_STATIC:{	SetColor(pDC, 1, 3); break; }
		case IDC_TIMBRE_AR3_STATIC:{	SetColor(pDC, 2, 3); break; }
		case IDC_TIMBRE_DR3_STATIC:{	SetColor(pDC, 3, 3); break; }
		case IDC_TIMBRE_SR3_STATIC:{	SetColor(pDC, 4, 3); break; }
		case IDC_TIMBRE_RR3_STATIC:{	SetColor(pDC, 5, 3); break; }
		case IDC_TIMBRE_SL3_STATIC:{	SetColor(pDC, 6, 3); break; }
		case IDC_TIMBRE_TL3_STATIC:{	SetColor(pDC, 7, 3); break; }
		case IDC_TIMBRE_KS3_STATIC:{	SetColor(pDC, 8, 3); break; }
		case IDC_TIMBRE_MT3_STATIC:{	SetColor(pDC, 9, 3); break; }
		case IDC_TIMBRE_DT3_STATIC:{	SetColor(pDC, 10, 3); break; }
		case IDC_TIMBRE_SSG3_STATIC:{	SetColor(pDC, 11, 3); break; }
		case IDC_TIMBRE_SE_FIX3_STATIC:{SetColor(pDC, 12, 3); break; }
		case IDC_TIMBRE_SE_KT3_STATIC:{	SetColor(pDC, 13, 3); break; }
		case IDC_TIMBRE_SE_FDT3_STATIC:{SetColor(pDC, 14, 3); break; }
		
		case IDC_TIMBRE_EN4_STATIC:{	SetColor(pDC, 0, 4); break; }
		case IDC_TIMBRE_FDE4_STATIC:{	SetColor(pDC, 1, 4); break; }
		case IDC_TIMBRE_AR4_STATIC:{	SetColor(pDC, 2, 4); break; }
		case IDC_TIMBRE_DR4_STATIC:{	SetColor(pDC, 3, 4); break; }
		case IDC_TIMBRE_SR4_STATIC:{	SetColor(pDC, 4, 4); break; }
		case IDC_TIMBRE_RR4_STATIC:{	SetColor(pDC, 5, 4); break; }
		case IDC_TIMBRE_SL4_STATIC:{	SetColor(pDC, 6, 4); break; }
		case IDC_TIMBRE_TL4_STATIC:{	SetColor(pDC, 7, 4); break; }
		case IDC_TIMBRE_KS4_STATIC:{	SetColor(pDC, 8, 4); break; }
		case IDC_TIMBRE_MT4_STATIC:{	SetColor(pDC, 9, 4); break; }
		case IDC_TIMBRE_DT4_STATIC:{	SetColor(pDC, 10, 4); break; }
		case IDC_TIMBRE_SSG4_STATIC:{	SetColor(pDC, 11, 4); break; }
		case IDC_TIMBRE_SE_FIX4_STATIC:{SetColor(pDC, 12, 4); break; }
		case IDC_TIMBRE_SE_KT4_STATIC:{	SetColor(pDC, 13, 4); break; }
		case IDC_TIMBRE_SE_FDT4_STATIC:{SetColor(pDC, 14, 4); break; }
		
		case IDC_TIMBRE_OP1_STATIC:{	SetColor(pDC, 0); break; }
		case IDC_TIMBRE_OP2_STATIC:{	SetColor(pDC, 1); break; }
		case IDC_TIMBRE_OP3_STATIC:{	SetColor(pDC, 2); break; }
		case IDC_TIMBRE_OP4_STATIC:{	SetColor(pDC, 3); break; }
	}
	
	return hbr;//(HBRUSH)GetStockObject(DKGRAY_BRUSH);
}



void CTimbreDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState, pWndOther, bMinimized);
	
	switch (nState){
		case WA_INACTIVE:{
			m_IsActive = false;
			
			FixParam();
			for (int Key = 0; Key <= 12; Key++) Stop(Key);
			break;
		}
		case WA_ACTIVE:
		case WA_CLICKACTIVE:
		{
			m_IsActive = true;
			break;
		}
	}
	DrawAllParam();
}



// 

void CTimbreDlg::SetColor(CDC* pDC, int x, int y)
{
	auto b = (m_Edit.mx == x && m_Edit.my == y);
	auto pFocusWnd = GetFocus();
	auto bFocusCEdit = (pFocusWnd && pFocusWnd->IsKindOf(RUNTIME_CLASS(CEdit)));
	pDC->SetTextColor((b)? (m_Edit.m_bEditing)? RGB(255,160,128): GetSysColor(COLOR_3DFACE): GetSysColor(COLOR_WINDOWTEXT));
	pDC->SetBkColor((b)? (m_IsActive && !bFocusCEdit)? (m_Edit.m_bEditing)? GetSysColor(COLOR_WINDOWTEXT): GetSysColor(COLOR_MENUHILIGHT): RGB(0x78,0x78,0x78): GetSysColor(COLOR_3DFACE));
}



void CTimbreDlg::SetColor(CDC* pDC, int iOperator)
{
	auto b = (m_Edit.m_iOperator == iOperator && m_Edit.m_bHighLight);
	pDC->SetTextColor((b)? RGB(255,160,128): GetSysColor(COLOR_WINDOWTEXT));
	pDC->SetBkColor((b)? GetSysColor(COLOR_WINDOWTEXT): GetSysColor(COLOR_3DFACE));
}



void CTimbreDlg::SetCur(int x, int y, bool bEditing)
{
	m_Edit.mx = x;
	m_Edit.my = y;
	m_Edit.m_bEditing = bEditing;
}



void CTimbreDlg::SetPicture(int ALG)
{
	for (int i = 0; i <= 7; ++i){
		m_Edit.m_aCStaticALG[i].ShowWindow((ALG == i)? SW_SHOW: SW_HIDE);
	}
}



void CTimbreDlg::SetOperator(int iOperator, bool bHighLight)
{
	m_Edit.m_iOperator = iOperator;
	m_Edit.m_bHighLight = bHighLight;
	
	switch (m_Edit.m_iOperator){
		case 0:{	GetDlgItem(IDC_TIMBRE_OP1_STATIC)->RedrawWindow();	break;	}
		case 1:{	GetDlgItem(IDC_TIMBRE_OP2_STATIC)->RedrawWindow();	break;	}
		case 2:{	GetDlgItem(IDC_TIMBRE_OP3_STATIC)->RedrawWindow();	break;	}
		case 3:{	GetDlgItem(IDC_TIMBRE_OP4_STATIC)->RedrawWindow();	break;	}
	}
}



// 

CFmStationDlg& CTimbreDlg::GetFmStationDlg()
{
	return theApp.GetFmStationDlg();
}



CSettingTab& CTimbreDlg::GetSettingTab()
{
	return GetFmStationDlg().GetSettingTab();
}



bool CTimbreDlg::ClipboardCopy(CString Text)
{
	if (OpenClipboard() == FALSE) return false;
	if (EmptyClipboard() == FALSE) return false;
	
	{	// 
		auto sText = (Text.GetLength() + 1) * sizeof(TCHAR);
		auto hText = GlobalAlloc(GMEM_MOVEABLE, sText);
		auto pText = (hText != NULL)? GlobalLock(hText): NULL;
		if (pText != NULL) memcpy_s(pText, sText, Text.LockBuffer(), sText);
		if (hText != NULL) GlobalUnlock(hText);
		Text.UnlockBuffer();
		
		auto Format = (sizeof(TCHAR) == sizeof(WCHAR))? CF_UNICODETEXT: CF_TEXT;
		auto bResult = (SetClipboardData(Format, hText) != NULL);
		CloseClipboard();
		return bResult;
	}
}



CString CTimbreDlg::ClipboardPaste()
{
	COleDataObject DataObject;
	auto Format = (sizeof(TCHAR) == sizeof(WCHAR))? CF_UNICODETEXT: CF_TEXT;
	if (DataObject.AttachClipboard() && DataObject.IsDataAvailable(Format)){
		auto hText = DataObject.GetGlobalData(Format);
		auto pText = (LPCTSTR)GlobalLock(hText);
		CString Text(pText);
		GlobalUnlock(hText);
		
		return std::move(Text);
	}
	return std::move(CString());
}



void CTimbreDlg::Copy(bool bShift)
{
	FixParam();
	
	auto v = m_pTimbre->GetIntermediate();
	CString Text;
	
	auto& rCSettingTab = GetSettingTab();
	bShift ^= rCSettingTab.IsSwapCopyFormat();
	if (bShift){
		v.ToFormat(rCSettingTab.GetFormatType(), Text);
	} else {
		nlohmann::json j = v;
		Text = CString(j.dump().c_str());
	}
	if (ClipboardCopy(Text)){
		Log(_T("Copy"));
	} else {
		Log(_T("Copy Error"));
	}
}



void CTimbreDlg::Paste()
{
	FixParam();
	
	CIntermediate v;
	auto Text = ClipboardPaste();
	auto Result = false;
	
	for (int EFormatType = CSettingTab::EFormatType::Num; --EFormatType >= 0 && !Result;){
		try {
			v.FromFormat((CSettingTab::EFormatType)EFormatType, Text);
			Result = true;
			
			auto& rCSettingTab = GetSettingTab();
			if (rCSettingTab.IsAutoCopyFormat()){
				rCSettingTab.SetFormatType((CSettingTab::EFormatType)EFormatType);
			}
		}
		catch (...){}
	}
	if (!Result){
		try {
			auto j = nlohmann::json::parse(CStringA(Text).GetBuffer());
			v = j.get<CIntermediate>();
			Result = true;
		}
		catch (...){}
	}
	if (Result){
		if (m_pTimbre) m_pTimbre->SetIntermediate(v);
		DrawAllParam();
		
		TRACE(_T("Paste"));
	} else {
		TRACE(_T("Paste Error"));
	}
}



void CTimbreDlg::OnOK()
{
	TimbreCallback(true);
	DestroyWindow();
}



void CTimbreDlg::OnCancel()
{
	TimbreCallback(false);
	DestroyWindow();
}



void CTimbreDlg::OnClose()
{
	OnCancel();
}



void CTimbreDlg::OnBnClickedTimbredlgOk()
{
	OnOK();
}



void CTimbreDlg::NoteOn(int MidiNote)
{
	if (m_pTimbre){
		auto MidiChannel = theApp.GetFmStationDlg().GetPreview();
		if (m_pTimbre->MidiNoteOn(MidiChannel, MidiNote, 127)){
			auto& FmStationDlg = GetFmStationDlg();
			FmStationDlg.MidiNoteOn(MidiChannel, MidiNote, 127, m_pTimbre);
		}
	}
}



void CTimbreDlg::NoteOff(int MidiNote)
{
	if (m_pTimbre){
		auto MidiChannel = theApp.GetFmStationDlg().GetPreview();
		if (m_pTimbre->MidiNoteOff(MidiChannel, MidiNote, 0)){
			auto& FmStationDlg = GetFmStationDlg();
			FmStationDlg.MidiNoteOff(MidiChannel, MidiNote, 0, m_pTimbre);
		}
	}
}



void CTimbreDlg::ResetKeyboard()
{
	if (!::IsWindow(m_CKeyboard.GetSafeHwnd())) return;
	m_CKeyboard.Reset();
	m_CKeyboard.Invalidate();
}



void CTimbreDlg::UpdateKeyboard(int MidiNote, bool bNoteState, bool bValid)
{
	if (!::IsWindow(m_CKeyboard.GetSafeHwnd())) return;
	m_CKeyboard.UpdateKeyboard(MidiNote, bNoteState, bValid);
	m_CKeyboard.Invalidate();
}



void CTimbreDlg::TimbreCallback(bool bResult)
{
	if (m_pTimbre){
		if (!bResult){
			auto& Control = m_pTimbre->Control;
			auto& aOperator = m_pTimbre->aOperator;
			
			{	// 
				Control.EN.SetValue(BackupControl.EN);
				Control.FDE.SetValue(BackupControl.FDE);
				Control.ALG.SetValue(BackupControl.ALG);
				Control.FB.SetValue(BackupControl.FB);
				Control.NUM.SetValue(BackupControl.NUM);
				Control.KML.SetValue(BackupControl.KML);
				Control.KMH.SetValue(BackupControl.KMH);
				Control.DUR.SetValue(BackupControl.DUR);
				Control.SE.SetValue(BackupControl.SE);
				Control.FIX.SetValue(BackupControl.FIX);
				Control.KT.SetValue(BackupControl.KT);
				Control.FDT.SetValue(BackupControl.FDT);
			}
			
			for (int i = 0; i < std::size(aOperator); ++i){
				aOperator[i].EN.SetValue(aBackupOperator[i].EN);
				aOperator[i].FDE.SetValue(aBackupOperator[i].FDE);
				aOperator[i].AR.SetValue(aBackupOperator[i].AR);
				aOperator[i].DR.SetValue(aBackupOperator[i].DR);
				aOperator[i].SR.SetValue(aBackupOperator[i].SR);
				aOperator[i].RR.SetValue(aBackupOperator[i].RR);
				aOperator[i].SL.SetValue(aBackupOperator[i].SL);
				aOperator[i].TL.SetValue(aBackupOperator[i].TL);
				aOperator[i].KS.SetValue(aBackupOperator[i].KS);
				aOperator[i].MT.SetValue(aBackupOperator[i].MT);
				aOperator[i].DT.SetValue(aBackupOperator[i].DT);
				aOperator[i].SSG.SetValue(aBackupOperator[i].SSG);
				aOperator[i].SE_FIX.SetValue(aBackupOperator[i].SE_FIX);
				aOperator[i].SE_KT.SetValue(aBackupOperator[i].SE_KT);
				aOperator[i].SE_FDT.SetValue(aBackupOperator[i].SE_FDT);
			}
		}
		m_pTimbre->DlgCallback(bResult);
		m_pTimbre.reset();
	}
}



void CTimbreDlg::SetTimbre(sptr<CTimbre> pTimbre, CString BatchName)
{
	m_pTimbre = pTimbre;
	if (::IsWindow(m_CEditName.GetSafeHwnd())){
		m_CEditName.SetWindowText(m_pTimbre->Name);
		m_CKeyboard.SetFocus();//m_CEditName.SetSel(0, -1);
		
		{	// 
			auto& Control = m_pTimbre->Control;
			auto& aOperator = m_pTimbre->aOperator;
			
			{	// 
				BackupControl.EN = Control.EN.GetValue();
				BackupControl.FDE = Control.FDE.GetValue();
				BackupControl.ALG = Control.ALG.GetValue();
				BackupControl.FB = Control.FB.GetValue();
				BackupControl.NUM = Control.NUM.GetValue();
				BackupControl.KML = Control.KML.GetValue();
				BackupControl.KMH = Control.KMH.GetValue();
				BackupControl.DUR = Control.DUR.GetValue();
				BackupControl.SE = Control.SE.GetValue();
				BackupControl.FIX = Control.FIX.GetValue();
				BackupControl.KT = Control.KT.GetValue();
				BackupControl.FDT = Control.FDT.GetValue();
			}
			
			for (int i = 0; i < std::size(aOperator); ++i){
				aBackupOperator[i].EN = aOperator[i].EN.GetValue();
				aBackupOperator[i].FDE = aOperator[i].FDE.GetValue();
				aBackupOperator[i].AR = aOperator[i].AR.GetValue();
				aBackupOperator[i].DR = aOperator[i].DR.GetValue();
				aBackupOperator[i].SR = aOperator[i].SR.GetValue();
				aBackupOperator[i].RR = aOperator[i].RR.GetValue();
				aBackupOperator[i].SL = aOperator[i].SL.GetValue();
				aBackupOperator[i].TL = aOperator[i].TL.GetValue();
				aBackupOperator[i].KS = aOperator[i].KS.GetValue();
				aBackupOperator[i].MT = aOperator[i].MT.GetValue();
				aBackupOperator[i].DT = aOperator[i].DT.GetValue();
				aBackupOperator[i].SSG = aOperator[i].SSG.GetValue();
				aBackupOperator[i].SE_FIX = aOperator[i].SE_FIX.GetValue();
				aBackupOperator[i].SE_KT = aOperator[i].SE_KT.GetValue();
				aBackupOperator[i].SE_FDT = aOperator[i].SE_FDT.GetValue();
			}
		}
		
		CString WindowText;
		WindowText.Format(_T("%s : %s\n"), BatchName, m_pTimbre->Name);
		SetWindowText(WindowText);
	}
	DrawAllParam();
}



void CTimbreDlg::GetName(CString& Name)
{
	if (::IsWindow(m_CEditName.GetSafeHwnd())){
		m_CEditName.GetWindowText(Name);
	}
}



template<class... Args> void CTimbreDlg::Log(std::wformat_string<Args...> fmt, Args&& ... args)
{
	m_CStaticLog.SetWindowText((LPCTSTR)std::format(fmt, std::forward<Args>(args)...).c_str());
}



CWnd* CTimbreDlg::GetParamWnd(int x, int y)
{
	return GetDlgItem(m_Ctrl.m_aaParam[y][x]);
}



IValue& CTimbreDlg::GetParamValue(int x, int y)
{
	return (m_pTimbre)? m_pTimbre->GetValue(x, y): Dummy;
}



void CTimbreDlg::AddParamValue(int x, int y, int Add)
{
	if (GetParamValue(m_Ctrl.mx, m_Ctrl.my).AddValue(Add)){
		auto& rCSettingTab = GetSettingTab();
		if (rCSettingTab.IsSLTLLink() && m_Ctrl.mx == /*SL*/6 && m_Ctrl.my >= 1 && m_Ctrl.my <= 4){
			GetParamValue(/*TL*/7, m_Ctrl.my).AddValue(-Add * 4);
			RedrawParam(/*TL*/7, m_Ctrl.my);
		}
	}
}



void CTimbreDlg::DrawAllParam()
{
	for (int y = 0; y < _countof(m_Ctrl.m_aaParam); ++y){
		for (int x = 0; x < _countof(m_Ctrl.m_aaParam[0]); ++x){
			auto pCWnd = GetParamWnd(x, y);
			if (pCWnd != nullptr){
				pCWnd->SetWindowText((LPCTSTR)GetParamValue(x, y).GetText());
			}
		}
	}
	
	{	// 
		auto& rValueNew = GetParamValue(2, 0);
		auto ALG = rValueNew.GetValue();
		SetPicture(ALG);
	}
}



void CTimbreDlg::MoveCursor(int ax, int ay)
{
	auto ox = m_Ctrl.mx;
	auto oy = m_Ctrl.my;
	
	{	// 
		auto& rValueOld = GetParamValue(m_Ctrl.mx, m_Ctrl.my);
		
		if ((ax != 0 || ay != 0) && rValueOld.IsEditing()) rValueOld.InputEnter();
		
		m_Ctrl.mx += ax;
		m_Ctrl.mx = (m_Ctrl.mx < 0)? 0: m_Ctrl.mx;
		m_Ctrl.mx = (m_Ctrl.mx < _countof(m_Ctrl.m_aaParam[0]))? m_Ctrl.mx: _countof(m_Ctrl.m_aaParam[0])-1;
		
		m_Ctrl.my += ay;
		m_Ctrl.my = (m_Ctrl.my < 0)? 0: m_Ctrl.my;
		m_Ctrl.my = (m_Ctrl.my < _countof(m_Ctrl.m_aaParam))? m_Ctrl.my: _countof(m_Ctrl.m_aaParam)-1;
	}
	
	RedrawParam(ox, oy);
	RedrawParam();
}



void CTimbreDlg::RedrawParam(int x, int y)
{
	auto& rValue = GetParamValue(x, y);
	SetCur(m_Ctrl.mx, m_Ctrl.my, rValue.IsEditing());
	
	auto pCWnd = GetParamWnd(x, y);
	if (pCWnd != nullptr) pCWnd->SetWindowText((LPCTSTR)rValue.GetText());
	
	if (x == 2 && y == 0){
		auto ALG = rValue.GetValue();
		SetPicture(ALG);
	}
}



void CTimbreDlg::RedrawParam()
{
	RedrawParam(m_Ctrl.mx, m_Ctrl.my);
}



void CTimbreDlg::FixParam()
{
	auto& rValue = GetParamValue(m_Ctrl.mx, m_Ctrl.my);
	if (rValue.IsEditing()){
		rValue.InputEnter();
		RedrawParam();
	}
}



void CTimbreDlg::Undo()
{
	FixParam();
	
	auto& rValueOld = GetParamValue(m_Ctrl.mx, m_Ctrl.my);
	rValueOld.Undo();
	
	RedrawParam();
}



void CTimbreDlg::VolumeUp()
{
	GetFmStationDlg().VolumeUp();
	
	auto& rCSettingTab = GetSettingTab();
	Log(_T("Volume x{:1.1f}"), rCSettingTab.GetVolume() * 0.1);
}



void CTimbreDlg::VolumeDown()
{
	GetFmStationDlg().VolumeDown();
	
	auto& rCSettingTab = GetSettingTab();
	Log(_T("Volume x{:1.1f}"), rCSettingTab.GetVolume() * 0.1);
}



void CTimbreDlg::PreviewUp()
{
	GetFmStationDlg().PreviewUp();
	
	auto& rCSettingTab = GetSettingTab();
	Log(_T("Preview ch.{}"), rCSettingTab.GetPreview() + 1);
}



void CTimbreDlg::PreviewDown()
{
	GetFmStationDlg().PreviewDown();
	
	auto& rCSettingTab = GetSettingTab();
	Log(_T("Preview ch.{}"), rCSettingTab.GetPreview() + 1);
}



void CTimbreDlg::Rotate()
{
	auto iRotate = (m_Ctrl.my-1);
	if (iRotate >= 0){
		if (!m_Ctrl.m_bRotate){
			FixParam();
			
			m_Ctrl.m_bRotate = true;
			m_Ctrl.m_iRotate = iRotate;
		} else {
			if (m_Ctrl.m_iRotate != iRotate){
				auto v = m_pTimbre->GetIntermediate();
				
				auto t = v.aOperator[m_Ctrl.m_iRotate];
				if (m_Ctrl.m_iRotate < iRotate){
					for (int i = m_Ctrl.m_iRotate; i < iRotate; ++i) v.aOperator[i] = v.aOperator[i+1];
				} else {
					for (int i = m_Ctrl.m_iRotate; i > iRotate; --i) v.aOperator[i] = v.aOperator[i-1];
				}
				v.aOperator[iRotate] = t;
				
				m_pTimbre->SetIntermediate(v);
			}
			
			m_Ctrl.m_bRotate = false;
		}
	}
	
	SetOperator(m_Ctrl.m_iRotate, m_Ctrl.m_bRotate);
	DrawAllParam();
}



void CTimbreDlg::RotateCancel()
{
	if (m_Ctrl.m_bRotate){
		m_Ctrl.m_bRotate = false;
		
		SetOperator(m_Ctrl.m_iRotate, m_Ctrl.m_bRotate);
		DrawAllParam();
	}
}



void CTimbreDlg::Play(int Key, CString KeyName)
{
	auto NewNote = Key + (m_Ctrl.m_Octave * 12);
	Log(_T("o{}{}({})"), ((NewNote / 12) - 1), (LPCTSTR)KeyName, NewNote);
	if (m_Ctrl.m_aNote[Key] != NewNote){
		FixParam();
		Stop(Key);
		
		m_Ctrl.m_aNote[Key] = NewNote;
		
		NoteOn(NewNote);
		//UpdateKeyboard(NewNote, true);
	}
}



void CTimbreDlg::Stop(int Key)
{
	auto OldNote = m_Ctrl.m_aNote[Key];
	if (OldNote >= 0){
		m_Ctrl.m_aNote[Key] = -1;
		
		NoteOff(OldNote);
		//UpdateKeyboard(OldNote, false);
	}
}



// CTimbre

CTimbre::~CTimbre()
{
	DlgCancel();
}



CTimbre::CTimbre(IUpdateTimbreTab* pUpdateTimbreTab, CString Name)
:pUpdateTimbreTab(pUpdateTimbreTab)
,Name(Name)
,aabNoteState{}
{
	Control.EN.SetValue(1);
	Control.FDE.SetValue(1);
	Control.ALG.SetValue(0);
	Control.FB.SetValue(0);
	Control.NUM.SetValue(0);
	Control.KML.SetValue(0);
	Control.KMH.SetValue(127);
	Control.DUR.SetValue(0);
	Control.SE.SetValue(0);
	Control.FIX.SetValue(0);
	Control.KT.SetValue(12);
	Control.FDT.SetValue(0);
	
	for (int i = 0; i < std::size(aOperator); ++i){
		aOperator[i].EN.SetValue(1);
		aOperator[i].FDE.SetValue(1);
		aOperator[i].AR.SetValue(31);
		aOperator[i].DR.SetValue(0);
		aOperator[i].SR.SetValue(0);
		aOperator[i].RR.SetValue(0);
		aOperator[i].SL.SetValue(0);
		aOperator[i].TL.SetValue(0);
		aOperator[i].KS.SetValue(0);
		aOperator[i].MT.SetValue(1);
		aOperator[i].DT.SetValue(0);
		aOperator[i].SSG.SetValue(0);
		aOperator[i].SE_FIX.SetValue(0);
		aOperator[i].SE_KT.SetValue(0);
		aOperator[i].SE_FDT.SetValue(0);
	}
	
	aOperator[0].TL.SetValue(36);
	aOperator[1].TL.SetValue(36);
	aOperator[2].TL.SetValue(36);
	aOperator[3].TL.SetValue(16);
	aOperator[3].SR.SetValue(8);
	aOperator[3].RR.SetValue(6);
}



void CTimbre::DlgOK()
{
	if (pTimbreDlg && ::IsWindow(pTimbreDlg->GetSafeHwnd())){
		pTimbreDlg->OnOK();
	}
}



void CTimbre::DlgCancel()
{
	if (pTimbreDlg && ::IsWindow(pTimbreDlg->GetSafeHwnd())){
		pTimbreDlg->OnCancel();
	}
}



void CTimbre::DlgCloase()
{
	if (pTimbreDlg && ::IsWindow(pTimbreDlg->GetSafeHwnd())){
		pTimbreDlg->OnClose();
	}
}



void CTimbre::DlgCallback(bool bResult)
{
	if (pTimbreDlg){
		if (bResult){
			pTimbreDlg->GetName(Name);
			pUpdateTimbreTab->UpdateTimbre(this);
		}
		pTimbreDlg.release();
	}
}



IValue& CTimbre::GetValue(int x, int y)
{
	switch (y){
		case 0:{
			switch (x){
				case 0: return Control.EN;
				case 1: return Control.FDE;
				case 2: return Control.ALG;
				case 3: return Control.FB;
				case 4: return Control.NUM;
				case 5: return Control.KML;
				case 6: return Control.KMH;
				case 7: return Control.DUR;
				case 8: break;
				case 9: break;
				case 10: break;
				case 11: return Control.SE;
				case 12: return Control.FIX;
				case 13: return Control.KT;
				case 14: return Control.FDT;
			}
			break;
		}
		default:{
			switch (x){
				case 0: return aOperator[y-1].EN;
				case 1: return aOperator[y-1].FDE;
				case 2: return aOperator[y-1].AR;
				case 3: return aOperator[y-1].DR;
				case 4: return aOperator[y-1].SR;
				case 5: return aOperator[y-1].RR;
				case 6: return aOperator[y-1].SL;
				case 7: return aOperator[y-1].TL;
				case 8: return aOperator[y-1].KS;
				case 9: return aOperator[y-1].MT;
				case 10: return aOperator[y-1].DT;
				case 11: return aOperator[y-1].SSG;
				case 12: return aOperator[y-1].SE_FIX;
				case 13: return aOperator[y-1].SE_KT;
				case 14: return aOperator[y-1].SE_FDT;
			}
			break;
		}
	}
	return Dummy;
}



void CTimbre::SetIntermediate(CIntermediate v)
{
	Control.EN.SetValue(v.Control.EN);
	Control.FDE.SetValue(v.Control.FDE);
	Control.ALG.SetValue(v.Control.ALG);
	Control.FB.SetValue(v.Control.FB);
	Control.NUM.SetValue(v.Control.NUM);
	Control.KML.SetValue(v.Control.KML);
	Control.KMH.SetValue(v.Control.KMH);
	Control.DUR.SetValue(v.Control.DUR);
	Control.SE.SetValue(v.Control.SE);
	Control.FIX.SetValue(v.Control.FIX);
	Control.KT.SetValue(v.Control.KT);
	Control.FDT.SetValue(v.Control.FDT);
	
	for (int i = 0; i < std::size(aOperator); ++i){
		aOperator[i].EN.SetValue(v.aOperator[i].EN);
		aOperator[i].FDE.SetValue(v.aOperator[i].FDE);
		aOperator[i].AR.SetValue(v.aOperator[i].AR);
		aOperator[i].DR.SetValue(v.aOperator[i].DR);
		aOperator[i].SR.SetValue(v.aOperator[i].SR);
		aOperator[i].RR.SetValue(v.aOperator[i].RR);
		aOperator[i].SL.SetValue(v.aOperator[i].SL);
		aOperator[i].TL.SetValue(v.aOperator[i].TL);
		aOperator[i].KS.SetValue(v.aOperator[i].KS);
		aOperator[i].MT.SetValue(v.aOperator[i].MT);
		aOperator[i].DT.SetValue(v.aOperator[i].DT);
		aOperator[i].SSG.SetValue(v.aOperator[i].SSG);
		aOperator[i].SE_FIX.SetValue(v.aOperator[i].SE_FIX);
		aOperator[i].SE_KT.SetValue(v.aOperator[i].SE_KT);
		aOperator[i].SE_FDT.SetValue(v.aOperator[i].SE_FDT);
	}
}



CIntermediate CTimbre::GetIntermediate()
{
	CIntermediate v;
	
	v.Control.EN = Control.EN.GetValue();
	v.Control.FDE = Control.FDE.GetValue();
	v.Control.ALG = Control.ALG.GetValue();
	v.Control.FB = Control.FB.GetValue();
	v.Control.NUM = Control.NUM.GetValue();
	v.Control.KML = Control.KML.GetValue();
	v.Control.KMH = Control.KMH.GetValue();
	v.Control.DUR = Control.DUR.GetValue();
	v.Control.SE = Control.SE.GetValue();
	v.Control.FIX = Control.FIX.GetValue();
	v.Control.KT = Control.KT.GetValue();
	v.Control.FDT = Control.FDT.GetValue();
	
	for (int i = 0; i < std::size(aOperator); ++i){
		v.aOperator[i].EN = aOperator[i].EN.GetValue();
		v.aOperator[i].FDE = aOperator[i].FDE.GetValue();
		v.aOperator[i].AR = aOperator[i].AR.GetValue();
		v.aOperator[i].DR = aOperator[i].DR.GetValue();
		v.aOperator[i].SR = aOperator[i].SR.GetValue();
		v.aOperator[i].RR = aOperator[i].RR.GetValue();
		v.aOperator[i].SL = aOperator[i].SL.GetValue();
		v.aOperator[i].TL = aOperator[i].TL.GetValue();
		v.aOperator[i].KS = aOperator[i].KS.GetValue();
		v.aOperator[i].MT = aOperator[i].MT.GetValue();
		v.aOperator[i].DT = aOperator[i].DT.GetValue();
		v.aOperator[i].SSG = aOperator[i].SSG.GetValue();
		v.aOperator[i].SE_FIX = aOperator[i].SE_FIX.GetValue();
		v.aOperator[i].SE_KT = aOperator[i].SE_KT.GetValue();
		v.aOperator[i].SE_FDT = aOperator[i].SE_FDT.GetValue();
	}
	
	return std::move(v);
}



bool CTimbre::MidiNoteOn(i8 MidiChannel, i8 MidiNote, int MidiVelocity)
{
	//TRACE(_T("o : %d\n"), MidiNote);
	if (pTimbreDlg) pTimbreDlg->UpdateKeyboard(MidiNote, true, IsKeyMapValid(MidiNote));
	if (IsKeyMapValid(MidiNote)){
		aabNoteState[MidiChannel][MidiNote]++;
		return true;
	}
	return false;
}



bool CTimbre::MidiNoteOff(i8 MidiChannel, i8 MidiNote, int MidiVelocity)
{
	//TRACE(_T("x : %d\n"), MidiNote);
	if (pTimbreDlg) pTimbreDlg->UpdateKeyboard(MidiNote, false, IsKeyMapValid(MidiNote));
	if (aabNoteState[MidiChannel][MidiNote] > 0){
		aabNoteState[MidiChannel][MidiNote]--;
		return true;
	}
	return false;
}
