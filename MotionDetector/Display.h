
//#ifndef _DISPLAY_H_
//#define _DISPLAY_H_

#pragma once
#include "afxwin.h"

class CDisplay
{
public:
	CDisplay();
	~CDisplay();

protected:
	HWND GetPreviewWindowHwnd() const;
	HWND GetBackgroundWindowHwnd() const;
	HWND GetMotionWindowHwnd() const;
	HWND GetForegroundWindowHwnd() const;
public:
	void SetPreviewWindowHwnd(const CStatic* pWnd);
	void SetBackgroundWindowHwnd(const CStatic* pWnd);
	void SetMotionWindowHwnd(const CStatic* pWnd);
	void SetForegroundWindowHwnd(const CStatic* pWnd);

	void GetForegroundWindowRect(LPRECT lpRect) const;
	void GetBackgroundWindowRect(LPRECT lpRect) const;
	void GetPreviewWindowRect(LPRECT lpRect) const;

	void AllDraw();
private:
//protected:
	CStatic* m_pcsWindow_00;
	CStatic* m_pcsWindow_01;
	CStatic* m_pcsWindow_02;
	CStatic* m_pcsWindow_03;
};


//#endif		//_DISPLAY_H_