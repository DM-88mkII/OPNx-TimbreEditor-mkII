
#pragma once



#include "afxdialogex.h"
#include <iterator>
#include <algorithm>



class IKeyboardCallback
{
	public:
		virtual ~IKeyboardCallback() = default;
		
		IKeyboardCallback() = default;
		
		virtual void NoteOn(int MidiNote) = 0;
		virtual void NoteOff(int MidiNote) = 0;
};



class CKeyboard : public CStatic
{
	public:
		virtual ~CKeyboard(){}
		
		CKeyboard();
		
		CBitmap& GetBitmap(){ return m_CBitmap; }
	
	public:
		void Initialize(int Width, int Height);
		void SetCallback(IKeyboardCallback* pCallback);
	
	protected:
		virtual void PreSubclassWindow();
		
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnPaint();
		
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnMouseLeave();
		
		DECLARE_MESSAGE_MAP()
	
	protected:
		static constexpr int MidiChannels = 16;
		static constexpr int MidiNotes = 128;
		
		static constexpr int ImgWidth = 7;
		static constexpr int ImgHeightB = 20;
		static constexpr int ImgHeightW = 32;
	
	protected:
		CDC m_OffScreenDC;
		CDC m_KeyboardDC;
		CBitmap m_CBitmap;
		CBitmap* m_pOldBmp;
		
		CBitmap m_BmpWhiteOffRight;
		CBitmap m_BmpWhiteOffBoth;
		CBitmap m_BmpWhiteOffLeft;
		CBitmap m_BmpWhiteOnRight;
		CBitmap m_BmpWhiteOnBoth;
		CBitmap m_BmpWhiteOnLeft;
		CBitmap m_BmpBlackOff;
		CBitmap m_BmpBlackOn;
		CBitmap m_BmpWhiteOnRightInvalid;
		CBitmap m_BmpWhiteOnBothInvalid;
		CBitmap m_BmpWhiteOnLeftInvalid;
		CBitmap m_BmpBlackOnInvalid;
		
		BOOL m_bMouseTracking;
		
		bool m_abNoteState[MidiNotes];
		int m_aNoteOn[MidiNotes];
		int m_PrevNote;
		
		IKeyboardCallback* m_pCallback;
	
	protected:
		void UpdateMouseState(UINT nFlags, CPoint point);
	
	public:
		void Reset();
		void UpdateKeyboard(int MidiNote, bool bNoteState, bool bValid = true);
};
