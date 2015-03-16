
#pragma once
#include "afxwin.h"
#include "Display.h"

#include "ImageSrc.h"

//class CScreenDrawXXX {};

class CScreenDraw: public CDisplay
{
public:
	CScreenDraw();
	~CScreenDraw();

	void DrawPicture(HDC hdc, PVOID pPicture,DWORD dwSize,int nWigthLine,int nHighLine,CRect* pRect);
	void DrawPicture(HWND hWnd, PVOID pPicture,DWORD dwSize,int nWigthLine,int nHeight,CRect* pRect);
	void DrawPicture(CDC* pDC,CRect& rcPicture,PVOID pPicture,DWORD dwSize,int nWigthLine,int nHighLine);
public:
	void DrawPreviewWindowPicture(PVOID pPicture,DWORD dwSize,int nWigth,int nHeight,CRect* pRect);
	void DrawBackgroundWindowPicture(PVOID pPicture,DWORD dwSize,int nWigth,int nHeight,CRect* pRect);
	void DrawMotionWindowPicture(PVOID pPicture,DWORD dwSize,int nWigth,int nHeight,CRect* pRect);
	void DrawForegroundWindowPicture(PVOID pPicture,DWORD dwSize,int nWigth,int nHeight,CRect* pRect);

	void ShowLine(CDC* pDC);	//,LONG lX, LONG lY /*CRect* pRectZone*/);
	void ShowRectangle(const RECT& rRect);
	void ShowColorRectangle(const RECT& rRect);
	void PreviewDrawPicture(PVOID pPicture,DWORD dwSize,int nWigthLine,int nHighLine);
	void CScreenDraw::PreviewDrawPicture2(PVOID pPicture,DWORD dwSize,int nWigthLine,int nHighLine);

	void AllDraw();

private:
	//void DrawPicture(HWND hWnd, PVOID pPicture,DWORD dwSize,int nWigthLine,int nHeight,CRect* pRect);

};