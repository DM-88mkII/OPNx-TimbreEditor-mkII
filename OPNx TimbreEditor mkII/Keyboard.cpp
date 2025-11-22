



#include "pch.h"
#include "FmStation.h"
#include "./Keyboard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CKeyboard::CKeyboard()
:m_bMouseTracking(false)
,m_abNoteState{}
,m_aNoteOn{}
,m_PrevNote(0)
,m_pCallback(NULL)
{
}



BEGIN_MESSAGE_MAP(CKeyboard, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()



void CKeyboard::Initialize(int Width, int Height)
{
	auto pWndDC = GetDC();
	m_OffScreenDC.CreateCompatibleDC(pWndDC);
	m_KeyboardDC.CreateCompatibleDC(pWndDC);
	m_CBitmap.CreateCompatibleBitmap(pWndDC, Width, Height);
	m_pOldBmp = m_OffScreenDC.SelectObject(&m_CBitmap);
	ReleaseDC(pWndDC);
}



void CKeyboard::SetCallback(IKeyboardCallback* pCallback)
{
	m_pCallback = pCallback;
	
	for (int MidiNote = 0; MidiNote < MidiNotes; MidiNote++){
		UpdateKeyboard(MidiNote, false);
	}
	
	Invalidate();
}



void CKeyboard::PreSubclassWindow()
{
	{	// 
		m_BmpWhiteOffRight.LoadBitmap(IDB_BIG_WHITE_OFF_RIGHT);
		m_BmpWhiteOffBoth.LoadBitmap(IDB_BIG_WHITE_OFF_BOTH);
		m_BmpWhiteOffLeft.LoadBitmap(IDB_BIG_WHITE_OFF_LEFT);
		m_BmpWhiteOnRight.LoadBitmap(IDB_BIG_WHITE_ON_RIGHT);
		m_BmpWhiteOnBoth.LoadBitmap(IDB_BIG_WHITE_ON_BOTH);
		m_BmpWhiteOnLeft.LoadBitmap(IDB_BIG_WHITE_ON_LEFT);
		m_BmpBlackOff.LoadBitmap(IDB_BIG_BLACK_OFF);
		m_BmpBlackOn.LoadBitmap(IDB_BIG_BLACK_ON);
		m_BmpWhiteOnRightInvalid.LoadBitmap(IDB_BIG_WHITE_ON_RIGHT_INVALID);
		m_BmpWhiteOnBothInvalid.LoadBitmap(IDB_BIG_WHITE_ON_BOTH_INVALID);
		m_BmpWhiteOnLeftInvalid.LoadBitmap(IDB_BIG_WHITE_ON_LEFT_INVALID);
		m_BmpBlackOnInvalid.LoadBitmap(IDB_BIG_BLACK_ON_INVALID);
	}
	
	CStatic::PreSubclassWindow();
}



BOOL CKeyboard::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;//CDialogEx::OnEraseBkgnd(pDC);
}



void CKeyboard::OnPaint()
{
	CPaintDC dc(this);
	
	CRect rect;
	GetClientRect(&rect);
	
	switch (GetStyle() & SS_TYPEMASK){
		case SS_LEFT:
		case SS_CENTER:
		case SS_RIGHT:
		{
			break;
		}
		case SS_ICON:{
			break;
		}
		case SS_BITMAP:{
			if (m_CBitmap.GetSafeHandle()){
				BITMAP bmp;
				m_CBitmap.GetBitmap(&bmp);
				
				dc.BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, &m_OffScreenDC, 0, 0, SRCCOPY);
			}
			break;
		}
		case SS_OWNERDRAW:{
			break;
		}
		default:{
			break;
		}
	}
}



void CKeyboard::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bMouseTracking){
		m_bMouseTracking = TRUE;
		
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = HOVER_DEFAULT;
		::TrackMouseEvent(&tme);
	}
	
	UpdateMouseState(nFlags, point);
	CStatic::OnMouseMove(nFlags, point);
}



void CKeyboard::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	
	UpdateMouseState(nFlags, point);
	CStatic::OnLButtonDown(nFlags, point);
}



void CKeyboard::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	
	UpdateMouseState(nFlags, point);
	CStatic::OnLButtonUp(nFlags, point);
}



void CKeyboard::OnMouseLeave()
{
	m_bMouseTracking = FALSE;
	CStatic::OnMouseLeave();
}



void CKeyboard::UpdateMouseState(UINT nFlags, CPoint point)
{
	static const int kw = ImgWidth;
	static const int kh = ImgHeightB;
	static const int kk = 7;
	static const int ke = kw - 3;
	static const int kb = kw >> 1;
	
	auto wd = (point.x + 0) / kw;
	auto wm = (point.x + 0) % kw;
	auto wb = (wm < ke);
	auto bd = (point.x + kb) / kw;
	auto bm = (point.x + kb) % kw;
	auto bb = (bm < ke);
	
	auto o = wd / kk;
	auto w = wd % kk;
	auto b = bd % kk;
	
	static const int aaNote[2][kk]={
		{-1, 1, 3,-1, 6, 8,10},//Black
		{ 0, 2, 4, 5, 7, 9,11},//White
	};
	auto t = (point.y < kh && bb && b != 0 && b != 3);
	auto k = (t)? 0: 1;
	auto i = (t)? b: w;
	auto MidiNote = aaNote[k][i] + (o * 12);
	
	auto bNoteState = (bool)(nFlags & MK_LBUTTON);
	if (point.x >= 0 && MidiNote < MidiNotes){
		bool bInvalidate = false;
		
		if (m_PrevNote != MidiNote){
			if (m_abNoteState[m_PrevNote]){
				m_abNoteState[m_PrevNote] = false;
				//UpdateKeyboard(m_PrevNote, false);
				m_pCallback->NoteOff(m_PrevNote);
				bInvalidate = true;
			}
			m_PrevNote = MidiNote;
		}
		
		if (m_abNoteState[MidiNote] != bNoteState/* || bInvalidate*/){
			m_abNoteState[MidiNote] = bNoteState;
			//UpdateKeyboard(MidiNote, bNoteState);
			if (bNoteState){
				m_pCallback->NoteOn(MidiNote);
				SetFocus();
			} else {
				m_pCallback->NoteOff(MidiNote);
			}
			bInvalidate = true;
		}
		
		if (bInvalidate) Invalidate();
	} else {
		bool bInvalidate = false;
		
		if (m_abNoteState[m_PrevNote] && !bNoteState){
			m_abNoteState[m_PrevNote] = false;
			//UpdateKeyboard(m_PrevNote, false);
			m_pCallback->NoteOff(m_PrevNote);
			bInvalidate = true;
		}
		
		if (bInvalidate) Invalidate();
	}
}



void CKeyboard::Reset()
{
	std::ranges::fill(m_aNoteOn, 0);
	for (int MidiNote = 0; MidiNote < MidiNotes; MidiNote++){
		UpdateKeyboard(MidiNote, false);
	}
}



void CKeyboard::UpdateKeyboard(int MidiNote, bool bNoteState, bool bValid)
{
	//TRACE(_T("%d : %d\n"), MidiNote, bNoteState);
	int Oct = MidiNote / 12;
	int Key = MidiNote % 12;
	
	int x = (Oct * (ImgWidth * 7));
	int y = 0;
	
	auto& NoteOn = m_aNoteOn[MidiNote];
	NoteOn = (bNoteState)? (NoteOn+1): (NoteOn > 0)? (NoteOn-1): 0;
	bNoteState = (NoteOn > 0);
	
	{	// 
		CBitmap* pBmp = NULL;
		
		switch (Key){
			case 0: x +=  0; pBmp = (bNoteState)? (bValid)? &m_BmpWhiteOnRight: &m_BmpWhiteOnRightInvalid: &m_BmpWhiteOffRight;	break;//c
			case 1: x +=  4; pBmp = (bNoteState)? (bValid)? &m_BmpBlackOn: &m_BmpBlackOnInvalid: &m_BmpBlackOff;				break;//c+
			case 2: x +=  7; pBmp = (bNoteState)? (bValid)? &m_BmpWhiteOnBoth: &m_BmpWhiteOnBothInvalid: &m_BmpWhiteOffBoth;	break;//d
			case 3: x += 11; pBmp = (bNoteState)? (bValid)? &m_BmpBlackOn: &m_BmpBlackOnInvalid: &m_BmpBlackOff;				break;//d+
			case 4: x += 14; pBmp = (bNoteState)? (bValid)? &m_BmpWhiteOnLeft: &m_BmpWhiteOnLeftInvalid: &m_BmpWhiteOffLeft;	break;//e
			case 5: x += 21; pBmp = (bNoteState)? (bValid)? &m_BmpWhiteOnRight: &m_BmpWhiteOnRightInvalid: &m_BmpWhiteOffRight;	break;//f
			case 6: x += 25; pBmp = (bNoteState)? (bValid)? &m_BmpBlackOn: &m_BmpBlackOnInvalid: &m_BmpBlackOff;				break;//f+
			case 7: x += 28; pBmp = (bNoteState)? (bValid)? &m_BmpWhiteOnBoth: &m_BmpWhiteOnBothInvalid: &m_BmpWhiteOffBoth;	break;//g
			case 8: x += 32; pBmp = (bNoteState)? (bValid)? &m_BmpBlackOn: &m_BmpBlackOnInvalid: &m_BmpBlackOff;				break;//g+
			case 9: x += 35; pBmp = (bNoteState)? (bValid)? &m_BmpWhiteOnBoth: &m_BmpWhiteOnBothInvalid: &m_BmpWhiteOffBoth;	break;//a
			case 10: x += 39; pBmp = (bNoteState)? (bValid)? &m_BmpBlackOn: &m_BmpBlackOnInvalid: &m_BmpBlackOff;				break;//a+
			case 11: x += 42; pBmp = (bNoteState)? (bValid)? &m_BmpWhiteOnLeft: &m_BmpWhiteOnLeftInvalid: &m_BmpWhiteOffLeft;	break;//b
		}
		
		{	// 
			BITMAP bmp;
			pBmp->GetBitmap(&bmp);
			
			CBitmap* pOldBmp = m_KeyboardDC.SelectObject(pBmp);
			if (pOldBmp){
				while (!TransparentBlt(
					m_OffScreenDC.GetSafeHdc(),
					x, y, bmp.bmWidth, bmp.bmHeight,
					m_KeyboardDC.GetSafeHdc(),
					0, 0, bmp.bmWidth, bmp.bmHeight,
					RGB(255, 0, 0)
				));
				m_KeyboardDC.SelectObject(pOldBmp);
			}
		}
	}
}
