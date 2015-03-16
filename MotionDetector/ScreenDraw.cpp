
#include "stdafx.h"
#include "ScreenDraw.h"
#include <stdio.h>

CScreenDraw::CScreenDraw()
{
}
CScreenDraw::~CScreenDraw()
{
}
void DrawPicture(PVOID pPicture,DWORD dwSize,int nWigth,int nHeight,CRect* pRect)
{
}
//		ATTENTION!!!  COLOR PICTURE DRAW (format of picture: RGB, 32-bits )
void CScreenDraw::DrawPicture(HDC hdc, PVOID pPicture,DWORD dwSize,int nWigthLine,int nHeight,CRect* pRect)
{
	int nOldMode;
	int nRet;
	CRect rcPicture;
	rcPicture=*pRect;

	BITMAPINFO bmpi;
	::ZeroMemory(&bmpi,sizeof(bmpi));				
	bmpi.bmiHeader.biSize=sizeof(bmpi);				
	bmpi.bmiHeader.biWidth=nWigthLine;				
	bmpi.bmiHeader.biHeight=-nHeight;				
	bmpi.bmiHeader.biPlanes=1;
	bmpi.bmiHeader.biBitCount=32;
	bmpi.bmiHeader.biCompression=BI_RGB;
	bmpi.bmiHeader.biSizeImage=dwSize;

	nOldMode=::SetStretchBltMode(hdc,COLORONCOLOR);
	nRet=::StretchDIBits(hdc
					,rcPicture.left
					,rcPicture.top
					,rcPicture.Width()
					,rcPicture.Height()
					,0
					,0				
					,bmpi.bmiHeader.biWidth			
					,bmpi.bmiHeader.biHeight*(-1)	
					,pPicture
					,&bmpi
					,DIB_RGB_COLORS
					,SRCCOPY);
	::SetStretchBltMode(hdc,nOldMode);
}

void CScreenDraw::DrawPicture(HWND hWnd, PVOID pPicture,DWORD dwSize,int nWigthLine,int nHeight,CRect* pRect)
{
	int nOldMode;
	int nRet;

	CRect rcPicture;
	rcPicture=*pRect;
	HDC hdc=::GetDC(hWnd);
	if(NULL==hdc) return;
	BITMAPINFO bmpi;
	::ZeroMemory(&bmpi,sizeof(bmpi));				
	bmpi.bmiHeader.biSize=sizeof(bmpi);				
	bmpi.bmiHeader.biWidth=nWigthLine;				
	bmpi.bmiHeader.biHeight=-nHeight;				
	bmpi.bmiHeader.biPlanes=1;
	bmpi.bmiHeader.biBitCount=32;
	bmpi.bmiHeader.biCompression=BI_RGB;
	bmpi.bmiHeader.biSizeImage=dwSize;

	nOldMode=::SetStretchBltMode(hdc,COLORONCOLOR);
	nRet=::StretchDIBits(hdc
					,rcPicture.left
					,rcPicture.top
					,rcPicture.Width()
					,rcPicture.Height()
					,0
					,0					
					,bmpi.bmiHeader.biWidth			
					,bmpi.bmiHeader.biHeight*(-1)	
					,pPicture
					,&bmpi
					,DIB_RGB_COLORS	
					,SRCCOPY);
	::SetStretchBltMode(hdc,nOldMode);
	::ReleaseDC(hWnd,hdc);

}
void CScreenDraw::DrawPicture(CDC* pDC,CRect& rcPicture,PVOID pPicture,DWORD dwSize,int nWigthLine,int nHighLine)
{
	int nOldMode;
	int nRet;
	if(NULL==pDC)
		return;

	BITMAPINFO bmpi;
	::ZeroMemory(&bmpi,sizeof(bmpi));				//
	bmpi.bmiHeader.biSize=sizeof(bmpi);				//
	bmpi.bmiHeader.biWidth=nWigthLine;				//
	bmpi.bmiHeader.biHeight=-nHighLine;				//
	bmpi.bmiHeader.biPlanes=1;
	bmpi.bmiHeader.biBitCount=32;
	bmpi.bmiHeader.biCompression=BI_RGB;
	bmpi.bmiHeader.biSizeImage=dwSize;

	nOldMode=::SetStretchBltMode(pDC->m_hDC,COLORONCOLOR);
	nRet=::StretchDIBits(pDC->m_hDC
					,rcPicture.left
					,rcPicture.top
					,rcPicture.Width()
					,rcPicture.Height()
					,0
					,0
					,bmpi.bmiHeader.biWidth			
					,bmpi.bmiHeader.biHeight*(-1)	
					,pPicture
					,&bmpi
					,DIB_RGB_COLORS	
					,SRCCOPY);
	::SetStretchBltMode(pDC->m_hDC,nOldMode);
}

/*

*/
void CScreenDraw::DrawPreviewWindowPicture(PVOID pPicture,DWORD dwSize,int nWigth,int nHeight,CRect* pRect)
{
	DrawPicture(GetPreviewWindowHwnd(), pPicture,dwSize,nWigth,nHeight,pRect);
}
void CScreenDraw::DrawBackgroundWindowPicture(PVOID pPicture,DWORD dwSize,int nWigth,int nHeight,CRect* pRect)
{
	DrawPicture(GetBackgroundWindowHwnd(), pPicture,dwSize,nWigth,nHeight,pRect);
}
void CScreenDraw::DrawMotionWindowPicture(PVOID pPicture,DWORD dwSize,int nWigth,int nHeight,CRect* pRect)
{
	DrawPicture(GetMotionWindowHwnd(), pPicture,dwSize,nWigth,nHeight,pRect);
}
void CScreenDraw::DrawForegroundWindowPicture(PVOID pPicture,DWORD dwSize,int nWigth,int nHeight,CRect* pRect)
{
	DrawPicture(GetForegroundWindowHwnd(), pPicture,dwSize,nWigth,nHeight,pRect);
}

void CScreenDraw::ShowLine(CDC* pDC)
{
	RECT rect;
	int x,y;
	HDC hdc; 
	HWND hwnd = GetBackgroundWindowHwnd();
	GetBackgroundWindowRect(&rect);

	x=rect.left+10;
	y=rect.top+10;

    hdc = GetDC(hwnd);
	MoveToEx(hdc, (int) x, (int) y, (LPPOINT) NULL); 
    LineTo(hdc, (int) x+10, (int) y+10); 
    
    ReleaseDC(hwnd, hdc); 
}

void CScreenDraw::ShowRectangle(const RECT& rRect)
{

	int x,y;
	HDC hdc; 
	HWND hwnd = GetBackgroundWindowHwnd();
	//GetBackgroundWindowRect(&rect);
	hdc = GetDC(hwnd);
	MoveToEx(hdc, rRect.left,  rRect.top, (LPPOINT) NULL);
	LineTo(hdc,rRect.right, rRect.top);
	LineTo(hdc,rRect.right, rRect.bottom);
	LineTo(hdc,rRect.left, rRect.bottom);
	LineTo(hdc,rRect.left, rRect.top);
	ReleaseDC(hwnd, hdc); 
}

void CScreenDraw::ShowColorRectangle(const RECT& rRect)
{

		//RECT rect;
	HPEN hpen, hpenOld;
	HBRUSH hbrush, hbrushOld;
	int x,y;
	HDC hdc; 
	HWND hwnd = GetBackgroundWindowHwnd();
	hdc = GetDC(hwnd);

	// create a red pen...
	hpen = CreatePen(PS_SOLID, 2, RGB(255, 0/*255*/, 0));  
	//// Create a red brush.
	//hbrush = CreateSolidBrush(RGB(255, 0, 0));

	// Select the new pen and brush, and then draw.
	hpenOld = (HPEN)SelectObject(hdc, hpen);
	//hbrushOld = (HPEN)SelectObject(hdc, hbrush);
	//Rectangle(hdc, 100,100, 200,200);
	//GetBackgroundWindowRect(&rect);
	
	MoveToEx(hdc, rRect.left,  rRect.top, (LPPOINT) NULL);
	LineTo(hdc,rRect.right, rRect.top);
	LineTo(hdc,rRect.right, rRect.bottom);
	LineTo(hdc,rRect.left, rRect.bottom);
	LineTo(hdc,rRect.left, rRect.top);

	SelectObject(hdc, hpenOld); 
    DeleteObject(hpen);

	ReleaseDC(hwnd, hdc); 
}

// image draw
void CScreenDraw::PreviewDrawPicture(PVOID pPicture,DWORD dwSize,int nWigthLine,int nHighLine)
{
	int nOldMode;
	int nRet;
	BOOL bColor;

	BOOL bRet;
	CRect rcPicture;
	HDC hdc; 
	HWND hwnd = GetPreviewWindowHwnd();
	hdc = GetDC(hwnd);
	RECT rect;
	GetPreviewWindowRect(&rect);
	rcPicture.left = rect.left;
	rcPicture.top = rect.top;
	rcPicture.right = rect.right;
	rcPicture.bottom = rect.bottom;
	
	BITMAPINFO bmpi;
	::ZeroMemory(&bmpi,sizeof(bmpi));				
	bmpi.bmiHeader.biSize=sizeof(bmpi);				
	bmpi.bmiHeader.biWidth=nWigthLine;	
	bmpi.bmiHeader.biHeight=-nHighLine;	
	bmpi.bmiHeader.biPlanes=1;
	bmpi.bmiHeader.biBitCount=32;
	bmpi.bmiHeader.biCompression=BI_RGB;
	bmpi.bmiHeader.biSizeImage=dwSize;

	nOldMode=::SetStretchBltMode(hdc,COLORONCOLOR);
	nRet=::StretchDIBits(hdc
					,rcPicture.left
					,rcPicture.top
					,rcPicture.Width()
					,rcPicture.Height()
					,0//rcBitmap.left
					,0//rcBitmap.left					// =0
					,bmpi.bmiHeader.biWidth			//rcBitmap.Width()	// =0
					,bmpi.bmiHeader.biHeight*(-1)	//rcBitmap.Height()
					,pPicture
					,&bmpi
					,DIB_RGB_COLORS	//DIB_PAL_COLORS	//DIB_RGB_COLORS
					,SRCCOPY);
	::SetStretchBltMode(hdc,nOldMode);
	::ReleaseDC(hwnd,hdc);
}

void CScreenDraw::PreviewDrawPicture2(PVOID pPicture,DWORD dwSize,int nWigthLine,int nHighLine)
{
	int nOldMode;
	int nRet;
	BOOL bColor;

	BOOL bRet;
	CRect rcPicture;
	HDC hdc; 
	HWND hwnd = GetBackgroundWindowHwnd();	//GetPreviewWindowHwnd();
	hdc = GetDC(hwnd);
	RECT rect;
	GetPreviewWindowRect(&rect);
	rcPicture.left = rect.left;
	rcPicture.top = rect.top;
	rcPicture.right = rect.right;
	rcPicture.bottom = rect.bottom;
	
	BITMAPINFO bmpi;
	::ZeroMemory(&bmpi,sizeof(bmpi));				
	bmpi.bmiHeader.biSize=sizeof(bmpi);				
	bmpi.bmiHeader.biWidth=nWigthLine;	
	bmpi.bmiHeader.biHeight=-nHighLine;	
	bmpi.bmiHeader.biPlanes=1;
	bmpi.bmiHeader.biBitCount=32;
	bmpi.bmiHeader.biCompression=BI_RGB;
	bmpi.bmiHeader.biSizeImage=dwSize;

	nOldMode=::SetStretchBltMode(hdc,COLORONCOLOR);
	nRet=::StretchDIBits(hdc
					,rcPicture.left
					,rcPicture.top
					,rcPicture.Width()
					,rcPicture.Height()
					,0//rcBitmap.left
					,0//rcBitmap.left					// =0
					,bmpi.bmiHeader.biWidth			//rcBitmap.Width()	// =0
					,bmpi.bmiHeader.biHeight*(-1)	//rcBitmap.Height()
					,pPicture
					,&bmpi
					,DIB_RGB_COLORS	//DIB_PAL_COLORS	//DIB_RGB_COLORS
					,SRCCOPY);
	::SetStretchBltMode(hdc,nOldMode);
	::ReleaseDC(hwnd,hdc);
}


//void CScreenDraw::DrawPicture(CDC* pDC,CRect& rcPicture,PVOID pPicture,DWORD dwSize,int nWigthLine,int nHighLine)
//{
//	int nOldMode;
//	int nRet;
//	BOOL bColor;
//
//	BOOL bRet;
//	//CRect rcPicture;
//	//m_csImage.GetClientRect(&rcPicture);
//	//HDC hdc=::GetDC(m_csImage.m_hWnd);
//
//	BITMAPINFO bmpi;
//	::ZeroMemory(&bmpi,sizeof(bmpi));				//sizeof(BITMAPINFO));
//	bmpi.bmiHeader.biSize=sizeof(bmpi);				//sizeof(BITMAPINFOHEADER);
//	bmpi.bmiHeader.biWidth=nWigthLine;	//rcBitmap.Width();
//	bmpi.bmiHeader.biHeight=-nHighLine;	//-rcBitmap.Height();
//	bmpi.bmiHeader.biPlanes=1;
//	bmpi.bmiHeader.biBitCount=32;
//	bmpi.bmiHeader.biCompression=BI_RGB;
//	bmpi.bmiHeader.biSizeImage=dwSize;
//
//	nOldMode=::SetStretchBltMode(/*hdc*/ pDC->m_hDC,COLORONCOLOR);
//	nRet=::StretchDIBits(/*hdc*/ pDC->m_hDC
//					,rcPicture.left
//					,rcPicture.top
//					,rcPicture.Width()
//					,rcPicture.Height()
//					,0//rcBitmap.left
//					,0//rcBitmap.left					// =0
//					,bmpi.bmiHeader.biWidth			//rcBitmap.Width()	// =0
//					,bmpi.bmiHeader.biHeight*(-1)	//rcBitmap.Height()
//					,pPicture
//					,&bmpi
//					,DIB_RGB_COLORS	//DIB_PAL_COLORS	//DIB_RGB_COLORS
//					,SRCCOPY);
//	::SetStretchBltMode(/*hdc*/ pDC->m_hDC,nOldMode);
//}


//================

void CScreenDraw::AllDraw()
{
	AllDraw();
}