
#include "stdafx.h"
#include <string>

#include "ImageSrc.h"

#include "Filter.h"


Image::Image(CDisplay& rImageDraw)
	:m_rImageDraw(rImageDraw)
{
}

BOOL Image::ConvertGreyscale32ToGreyscale8(const ImageInfo& rImgInfoSrc, const ImageInfo& rImgInfoDest)
{
	if(ImCT_GREYSCALE_32 != rImgInfoSrc.color_type)
		return FALSE;
	if(ImCT_GREYSCALE_8 != rImgInfoDest.color_type)
		return FALSE;
	PBYTE pDataDest = rImgInfoDest.pImage;
	DWORD* pDataSrc = reinterpret_cast<DWORD*>(rImgInfoSrc.pImage);
	for(int i=0; i<rImgInfoSrc.nHeigth; i++)
	{
		for(int i=0; i<rImgInfoSrc.nWidth; i++)
		{
			COLORREF Pixel = *pDataSrc;
			BYTE greyscale_8 = GetRValue(Pixel);
			*pDataDest = greyscale_8;
			++pDataSrc; ++pDataDest;
		}
	}
}
BOOL Image::ConvertGreyscale8ToGreyscale32(const ImageInfo& rImgInfoSrc, const ImageInfo& rImgInfoDest)
{
	if(ImCT_GREYSCALE_8 != rImgInfoSrc.color_type)
		return FALSE;
	if(ImCT_GREYSCALE_32 != rImgInfoDest.color_type)
		return FALSE;
	PBYTE pDataSrc = rImgInfoSrc.pImage;
	DWORD* pDataDest = reinterpret_cast<DWORD*>(rImgInfoDest.pImage);
	for(int i=0; i<rImgInfoSrc.nHeigth; i++)
	{
		for(int i=0; i<rImgInfoSrc.nWidth; i++)
		{
			BYTE bPixel = *pDataSrc;
			COLORREF Pixel_32 = RGB(bPixel,bPixel,bPixel);
			*pDataDest = Pixel_32;
			++pDataSrc; ++pDataDest;
		}
	}
}
/********************************************************************************

*********************************************************************************/
FileImage::FileImage(CDisplay& rImageDraw)
:Image(rImageDraw)
{
}

void FileImage::ImageRefresh()
{
	PreviewImage();
}

BOOL FileImage::CreateNextImage(const RECT& rObjPosition)
{
	return TRUE;
}

using namespace std;
void FileImage::PreviewImage()
{
	//return;
	int nImgCnt=0;
	const char *fns[] = {"1.bmp","2.bmp","3.bmp","4.bmp","5.bmp","6.bmp","7.bmp","8.bmp","9.bmp","10.bmp","11.bmp"};
		//const char *fns[] = {"1_1.bmp","1_1.bmp","1_1.bmp","1_1.bmp","1_1.bmp","1_1.bmp","1_1.bmp","1_1.bmp","1_1.bmp","1_1.bmp","1_1.bmp"};
	BYTE* pMemAllockArray[] ={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
	std::string strImageFileNameBmp=fns[0];
		//std::string strImagePathBmp="";//"e:\\PROJECTS\\VIDEOEDITOR_PRG_TEST\\FFMPEG_VIDEO_TEST\\FFMPEG_VIDEO_TEST\\";
	std::string strImagePathBmp	= "i:\\PROJECTS\\VIDEOEDITOR_PRG_TEST\\FFMPEG_VIDEO_TEST\\FFMPEG_VIDEO_TEST\\";
	
	//std::string strFullPathBmp=strImagePathBmp+strImageFileNameBmp;
	//std::string strFullPathBmp = "E:\\TMP\\XXX.bmp";
	//std::string strFullPathBmp = "E:\\TMP\\room21.bmp";
	//std::string strFullPathBmp = "E:\\TMP\\street13.bmp";
	//std::string strFullPathBmp = "E:\\TMP\\Trole.bmp";	//Trole
	std::string strFullPathBmp = "E:\\TMP\\LenaImg.bmp";

	HBITMAP hBitmap;
	HDC hdcMem;
	HGDIOBJ oldBitmap;
	BITMAP 	bitmap;
	PBYTE pDataBufSrc, pGrayscaleWiew;
	int nBufSize;

//-------------------------------------------------------------------------------------------------------------------------
	hBitmap=(HBITMAP)LoadImage(NULL,strFullPathBmp.c_str(),IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	hdcMem=CreateCompatibleDC(GetDC(NULL));
	oldBitmap=SelectObject(hdcMem, hBitmap);
	GetObject(hBitmap, sizeof(bitmap), &bitmap);
	//bitmap.bmWidth=720;
	//bitmap.bmHeight=408;
	nBufSize=bitmap.bmWidth*bitmap.bmHeight*4;
	pDataBufSrc=(PBYTE)::malloc(nBufSize);//new BYTE(nBufSize);	// SOURCE BUF
	pGrayscaleWiew = (PBYTE)::malloc(nBufSize);


	DWORD	dwRet=GetBitmapBits(hBitmap,nBufSize/*bitmap.bmWidth*bitmap.bmHeight*/,pDataBufSrc);
	if(32 == bitmap.bmBitsPixel || 24 == bitmap.bmBitsPixel)
	{
		if(32 == bitmap.bmBitsPixel)
		{
		for(int y = 0; y < bitmap.bmHeight; y++)
		{
			for(int x = 0; x < bitmap.bmWidth; x++)
			{
				COLORREF rgb = ::GetPixel(hdcMem,x, y);
				BYTE r = GetRValue(rgb);
				BYTE g = GetGValue(rgb);
				BYTE b = GetBValue(rgb);
				BYTE bw = (BYTE)(0.3 * r + 0.59 * g + 0.11 * b + 0.5);
				::SetPixel(hdcMem,x, y, RGB(bw, bw, bw));
			}
		}
		}
		dwRet=GetBitmapBits(hBitmap,nBufSize,pGrayscaleWiew);
// test...  ----------------------=======================================================
		PBYTE pGreyscale_8 = (PBYTE)::malloc(nBufSize/4);
		ImageInfo info_src_32;
		ImageInfo info_dest_8;

		info_dest_8.color_type = ImCT_GREYSCALE_8;
		info_dest_8.pImage = pGreyscale_8;
		info_dest_8.nWidth = bitmap.bmWidth;
		info_dest_8.nHeigth = bitmap.bmHeight;

		info_src_32.color_type = ImCT_GREYSCALE_32;
		info_src_32.pImage = pGrayscaleWiew;
		info_src_32.nWidth = bitmap.bmWidth;
		info_src_32.nHeigth = bitmap.bmHeight;

		ConvertGreyscale32ToGreyscale8(info_src_32,info_dest_8);

		ImageInfo info_src_8;
		ImageInfo info_dest_32;

		info_src_8 = info_dest_8;
		info_dest_32 = info_src_32;

		// FILTER TEST ====================================================
		char* pFilter = (char*)::malloc(nBufSize/4);
		char* pFilter0 = (char*)::malloc(nBufSize/4);
		memset(pFilter,0,nBufSize/4);
		
		//CreateGradientArray((char*)pGreyscale_8,pFilter,bitmap.bmWidth,bitmap.bmHeight);
		CreateLaplasianArray((unsigned char*)pGreyscale_8,(unsigned char*)pFilter,bitmap.bmWidth,bitmap.bmHeight);

		//CreateGradientArray((char*)pGreyscale_8,pFilter0,bitmap.bmWidth,bitmap.bmHeight);
		//CreateGradientArray((char*)pFilter0,pFilter,bitmap.bmWidth,bitmap.bmHeight);


		//ArrayParser((char*)pGreyscale_8,pFilter,bitmap.bmWidth,bitmap.bmHeight);

		info_src_8.color_type = ImCT_GREYSCALE_8;
		info_src_8.pImage = (PBYTE)pFilter;
		info_src_8.nWidth = bitmap.bmWidth;
		info_src_8.nHeigth = bitmap.bmHeight;
		//=================================================================
		memset(pGrayscaleWiew,0,nBufSize);

		ConvertGreyscale8ToGreyscale32(info_src_8,info_dest_32);
		::free (pGreyscale_8);
		::free (pFilter);
		::free (pFilter0);
//--------------------------------======================================================
		// RGBA format...
		CScreenDraw& draw = static_cast<CScreenDraw&>(m_rImageDraw);

		draw.PreviewDrawPicture(pDataBufSrc,nBufSize,bitmap.bmWidth,bitmap.bmHeight);
		// filter preview...
		draw.PreviewDrawPicture2(pGrayscaleWiew,nBufSize,bitmap.bmWidth,bitmap.bmHeight);
		//draw.PreviewDrawPicture(pGrayscaleWiew,nBufSize,bitmap.bmWidth,bitmap.bmHeight);
	}
	else if(8 == bitmap.bmBitsPixel)
	{
		// gray scale...
	}

	if(pDataBufSrc)
		::free (pDataBufSrc);
	if(pGrayscaleWiew)
		::free (pGrayscaleWiew);

	

	
}