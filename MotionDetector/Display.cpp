
#include "stdafx.h"
#include "Display.h"

CDisplay::CDisplay()
:m_pcsWindow_00(NULL)
,m_pcsWindow_01(NULL)
,m_pcsWindow_02(NULL)
,m_pcsWindow_03(NULL)
{
}

CDisplay::~CDisplay()
{
}

//------------  get params...
HWND CDisplay::GetPreviewWindowHwnd() const
{
	if(!m_pcsWindow_00)
		return NULL;
	return m_pcsWindow_00->GetSafeHwnd();
}
HWND CDisplay::GetBackgroundWindowHwnd() const
{
	if(!m_pcsWindow_01)
		return NULL;
	return m_pcsWindow_01->GetSafeHwnd();
}
HWND CDisplay::GetMotionWindowHwnd() const
{
	if(!m_pcsWindow_02)
		return NULL;
	return m_pcsWindow_02->GetSafeHwnd();
}
HWND CDisplay::GetForegroundWindowHwnd() const
{
	if(!m_pcsWindow_03)
		return NULL;
	return m_pcsWindow_03->GetSafeHwnd();  
}

void CDisplay::GetForegroundWindowRect(LPRECT lpRect) const
{
	m_pcsWindow_03->GetClientRect(lpRect);
}
void CDisplay::GetBackgroundWindowRect(LPRECT lpRect) const
{
	m_pcsWindow_01->GetClientRect(lpRect);  
}

void CDisplay::GetPreviewWindowRect(LPRECT lpRect) const
{
	m_pcsWindow_00->GetClientRect(lpRect);
}
//-------------  params setting...
void CDisplay::SetPreviewWindowHwnd(const CStatic* pWnd)
{
	m_pcsWindow_00=const_cast<CStatic*>(pWnd);  //pWnd->GetClientRect
}
void CDisplay::SetBackgroundWindowHwnd(const CStatic* pWnd)
{
	m_pcsWindow_01=const_cast<CStatic*>(pWnd);
}
void CDisplay::SetMotionWindowHwnd(const CStatic* pWnd)
{
	m_pcsWindow_02=const_cast<CStatic*>(pWnd);
}
void CDisplay::SetForegroundWindowHwnd(const CStatic* pWnd)
{
	m_pcsWindow_03=const_cast<CStatic*>(pWnd);
}

void CDisplay::AllDraw()
{
	m_pcsWindow_00->Invalidate(0);
	m_pcsWindow_01->Invalidate(0);
	m_pcsWindow_02->Invalidate(0);
	m_pcsWindow_03->Invalidate(0);
}