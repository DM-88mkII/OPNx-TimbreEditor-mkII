



#include "pch.h"
#include "FmStation.h"
#include "ModuleTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



IMPLEMENT_DYNAMIC(CModuleTab, CDialogEx)



CModuleTab::~CModuleTab()
{
	//if (m_timerID != 0) KillTimer(m_timerID);
}



CModuleTab::CModuleTab(CWnd* pParent /*=nullptr*/)
:CDialogEx(IDD_MODULE_TAB, pParent)
,m_timerID(0)
,m_aChipInfo{}
{
}



void CModuleTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MODULE_LIST, m_CListCtrl);
}



BEGIN_MESSAGE_MAP(CModuleTab, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()



BOOL CModuleTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	{	// Column
		struct Column
		{
			LPCWSTR Label;
			int Width;
		};
		Column aColumn[] = {
			{_T("Module"), 82},
			{_T("Ch.1"), 75},
			{_T("Ch.2"), 75},
			{_T("Ch.3"), 75},
			{_T("Ch.4"), 75},
			{_T("Ch.5"), 75},
			{_T("Ch.6"), 75},
		};
		for (int i = 0; i < std::size(aColumn); i++){
			LVCOLUMN Column;
			Column.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
			Column.fmt = LVCFMT_CENTER;
			Column.pszText = (LPWSTR)aColumn[i].Label;
			Column.cx = aColumn[i].Width;
			m_CListCtrl.InsertColumn(i, &Column);
		}
		
		{	// 
			auto pHeader = m_CListCtrl.GetHeaderCtrl();
			if (pHeader != nullptr){
				pHeader->EnableWindow(FALSE);
			}
		}
		
		{	// 
			m_CListCtrl.SetExtendedStyle(m_CListCtrl.GetExtendedStyle() | LVS_EX_DOUBLEBUFFER);
		}
	}
	
	{	// 
		std::array<StaticList::ColumnParam, StaticList::ColumnNum> aColumParam;
		aColumParam[0].Width = 82-1; aColumParam[0].dwStyle = WS_CHILD | WS_VISIBLE | SS_CENTER;
		aColumParam[1].Width = 75; aColumParam[1].dwStyle = WS_CHILD | WS_VISIBLE | SS_CENTER;
		aColumParam[2].Width = 75; aColumParam[2].dwStyle = WS_CHILD | WS_VISIBLE | SS_CENTER;
		aColumParam[3].Width = 75; aColumParam[3].dwStyle = WS_CHILD | WS_VISIBLE | SS_CENTER;
		aColumParam[4].Width = 75; aColumParam[4].dwStyle = WS_CHILD | WS_VISIBLE | SS_CENTER;
		aColumParam[5].Width = 75; aColumParam[5].dwStyle = WS_CHILD | WS_VISIBLE | SS_CENTER;
		aColumParam[6].Width = 75; aColumParam[6].dwStyle = WS_CHILD | WS_VISIBLE | SS_CENTER;
		m_StaticList.Create(2, 19, 13, aColumParam, this);
		
		{	// 
			int Ch = 0;
			for (auto& Row : m_StaticList.GetRows()){
				CString Text;
				Text.Format(_T("YM2608.%d"), ++Ch);
				
				auto& aColum = Row.GetColumns();
				aColum[0].Cell.SetText(Text);
			}
		}
		
		m_timerID = SetTimer(3, /*ms*/17, NULL);
	}
	
	return FALSE;
}



BOOL CModuleTab::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message){
		case WM_KEYDOWN:{
			auto bControl = (GetKeyState(VK_LCONTROL) < 0) | (GetKeyState(VK_RCONTROL) < 0);
			if (bControl){
				switch (pMsg->wParam){
					case 'W':{			theApp.GetFmStationDlg().WavRecord();	return TRUE;	}
					
					case VK_INSERT:{	theApp.GetFmStationDlg().PreviewUp();	return TRUE;	}
					case VK_DELETE:{	theApp.GetFmStationDlg().PreviewDown();	return TRUE;	}
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



CFmStationDlg& CModuleTab::GetFmStationDlg()
{
	return theApp.GetFmStationDlg();
}



void CModuleTab::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_timerID){
		auto& aRow = m_StaticList.GetRows();
		for (int ChipId = 0; ChipId < ChipNum; ChipId++){
			for (int ChipChannel = 0; ChipChannel < ChipChannels; ChipChannel++){
				auto& ChannelInfo = m_aChipInfo[ChipId].aChannelInfo[ChipChannel];
				auto& Cell = aRow[ChipId].aColumnInfo[ChipChannel+1].Cell;
				
				auto bInvalidate = ChannelInfo.bInvalidate;
				ChannelInfo.bInvalidate = false;
				
				{	// Text
					auto c0 = (ChannelInfo.bNoteOn)? GetSysColor(COLOR_MENUTEXT): GetSysColor(COLOR_WINDOW);
					auto c1 = GetSysColor(COLOR_WINDOW);
					
					auto r0 = (float)GetRValue(c0);
					auto g0 = (float)GetGValue(c0);
					auto b0 = (float)GetBValue(c0);
					
					auto r1 = (float)GetRValue(c1);
					auto g1 = (float)GetGValue(c1);
					auto b1 = (float)GetBValue(c1);
					
					auto p1 = ChannelInfo.Text;
					auto p0 = 1.0f - p1;
					auto r = std::round((r0 * p0) + (r1 * p1));
					auto g = std::round((g0 * p0) + (g1 * p1));
					auto b = std::round((b0 * p0) + (b1 * p1));
					auto c = RGB(r, g, b);
					
					Cell.SetTextColor(c);
					
					bInvalidate |= (p1 > (1.0f / 255));
				}
				
				{	// Back
					auto c0 = (ChannelInfo.bNoteOn)? (ChannelInfo.bHold)? GetSysColor(COLOR_WINDOW): GetSysColor(COLOR_MENUBAR): GetSysColor(COLOR_WINDOW);
					auto c1 = GetSysColor(COLOR_MENUHILIGHT);
					
					auto r0 = (float)GetRValue(c0);
					auto g0 = (float)GetGValue(c0);
					auto b0 = (float)GetBValue(c0);
					
					auto r1 = (float)GetRValue(c1);
					auto g1 = (float)GetGValue(c1);
					auto b1 = (float)GetBValue(c1);
					
					auto p1 = ChannelInfo.Back;
					auto p0 = 1.0f - p1;
					auto r = std::round((r0 * p0) + (r1 * p1));
					auto g = std::round((g0 * p0) + (g1 * p1));
					auto b = std::round((b0 * p0) + (b1 * p1));
					auto c = RGB(r, g, b);
					
					Cell.SetBackColor(c);
					
					bInvalidate |= (p1 > (1.0f / 255));
				}
				
				if (bInvalidate) Cell.Invalidate();
				
				ChannelInfo.Text *= 1.0f - 0.01500f;
				ChannelInfo.Back *= 1.0f - 0.09375f;
			}
		}
	}
	
	CDialogEx::OnTimer(nIDEvent);
}



BOOL CModuleTab::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;//CDialogEx::OnEraseBkgnd(pDC);
}



void CModuleTab::SetState(int ChipId, int ChipChannel, int MidiChannel, int MidiNote, int MidiVelocity)
{
	if (m_CListCtrl.m_hWnd == 0) return;
	
	auto& ChannelInfo = m_aChipInfo[ChipId].aChannelInfo[ChipChannel];
	ChannelInfo.bNoteOn = (MidiChannel >= 0 && MidiNote >= 0);
	if (ChannelInfo.bNoteOn){
		ChannelInfo.Text = 1.0f;
		ChannelInfo.Back = 1.0f;
		
		CString Text;
		Text.Format(_T("%d.%d.%d"), MidiChannel+1, MidiNote, MidiVelocity);
		
		auto& aRow = m_StaticList.GetRows();
		aRow[ChipId].aColumnInfo[ChipChannel+1].Cell.SetText(Text);
	} else {
		ChannelInfo.bInvalidate = true;
		ChannelInfo.bHold = false;
	}
}



void CModuleTab::SetHold(int ChipId, int ChipChannel)
{
	if (m_CListCtrl.m_hWnd == 0) return;
	
	auto& ChannelInfo = m_aChipInfo[ChipId].aChannelInfo[ChipChannel];
	if (ChannelInfo.bNoteOn){
		ChannelInfo.bInvalidate = true;
		ChannelInfo.bHold = true;
	}
}
