
#include "..\MotionDetector\stdafx.h"
#include "ConvertorColorSpace.h"




CConvertorColorSpace::CConvertorColorSpace(std::string sLibraryPath)
	:m_sLibraryPath(sLibraryPath)
	,m_pTetstFile(NULL)
{
	try
	{
	m_pFfmpeg_obj=CFfmpegDll::Instance();
	//if(NULL==m_pFfmpeg_obj)
	//	throw CC_attach_dll_error;
	}
	catch(...)
	{
		//throw CC_attach_dll_error;
	}
}

CConvertorColorSpace::~CConvertorColorSpace()
{
	m_pFfmpeg_obj->FreeInst();
}

//BOOL CConvertorColorSpace::LoadLibrary(void* pLibraryPath)
//{
//	string str, str_path; 
//	LPCTSTR lpLibraryPath;
//
//	str_path.insert(0,static_cast<const char*>(pLibraryPath));
//
//	str=str_path;
//	str+="avutil-52.dll";
//	lpLibraryPath=str.c_str();
//	if(FALSE==CAvutilDll::LoadLibrary((void*)lpLibraryPath))
//		return FALSE;
//	//swscale-2.dll
//	str=str_path;
//	str+="swscale-2.dll";
//	lpLibraryPath=str.c_str();
//	if(FALSE==CSwscaleDll::LoadLibrary((void*)lpLibraryPath))
//		return FALSE;
//
//	str=str_path;
//	str+="avcodec-55.dll";
//	lpLibraryPath=str.c_str();
//	if(FALSE==CAcodecDll::LoadLibrary((void*)lpLibraryPath))
//		return FALSE;
//	str=str_path;
//	str+="avformat-55.dll";
//	lpLibraryPath=str.c_str();
//	if(FALSE==CAvformatlDll::LoadLibrary((void*)lpLibraryPath))
//		return FALSE;
//
//	av_register_all();
//	const char* pCConfig;
//	pCConfig=avcodec_configuration();
//	return TRUE;
//}
//
//void CConvertorColorSpace::av_register_all(void)
//{
//	CAvformatlDll::av_register_all();
//}
//const char *CConvertorColorSpace::avcodec_configuration()
//{
//	return CAcodecDll::avcodec_configuration();
//}
//struct SwsContext *CConvertorColorSpace::sws_getContext(int srcW, int srcH, enum AVPixelFormat srcFormat,
//                                  int dstW, int dstH, enum AVPixelFormat dstFormat,
//                                  int flags, SwsFilter *srcFilter,
//                                  SwsFilter *dstFilter, const double *param)
// {
//	 return CSwscaleDll::sws_getContext(srcW, srcH, srcFormat,dstW,dstH, dstFormat,flags,srcFilter,dstFilter, param);
// }
// int CConvertorColorSpace::sws_scale(struct SwsContext *c, const uint8_t *const srcSlice[],
//              const int srcStride[], int srcSliceY, int srcSliceH,
//              uint8_t *const dst[], const int dstStride[])
// {
//	 return CSwscaleDll::sws_scale(c, srcSlice, srcStride, srcSliceY, srcSliceH, dst,dstStride);
// }
// void CConvertorColorSpace::sws_freeContext(struct SwsContext *swsContext)
// {
//	 CSwscaleDll::sws_freeContext(swsContext);
// }

 //BOOL CConvertorColorSpace::VConvertorColor(const VideoFrameInfo& rSrcFrameDescr
	//							,VideoFrameInfo& rDestFrameDescr 
	//							,eVConvertColorType convert_type)
 //{
	// PixelFormat pixel_format;
	//switch(convert_type)
	//{
	//case CC_argb_4_to_Yuv420:
	//case CC_Yuv420_to_argb_4:
	//	pixel_format=PIX_FMT_ARGB;
	//	break;
	//case CC_rgba_4_to_Yuv420:
	//case CC_Yuv420_to_rgba_4:
	//	pixel_format=PIX_FMT_RGBA;
	//	break;
	////case CC_abgr_4_to_Yuv420:
	////case CC_Yuv420_to_abgr_4:
	////	pixel_format=PIX_FMT_ABRG;
	////	break;
	//case CC_bgra_4_to_Yuv420:
	//case CC_Yuv420_to_bgra_4:
	//	pixel_format=PIX_FMT_BGRA;
	//	break;
	//default:
	//	return FALSE;
	//};

	//switch(convert_type)
	//{
	//case CC_argb_4_to_Yuv420:
	//case CC_rgba_4_to_Yuv420:
	////case CC_abgr_4_to_Yuv420:
	//case CC_bgra_4_to_Yuv420:
	//	Xrgb_4_To_Yuv420P(rSrcFrameDescr,rDestFrameDescr,pixel_format);
	//	break;
	//case CC_Yuv420_to_argb_4:
	//case CC_Yuv420_to_rgba_4:
	////case CC_Yuv420_to_abgr_4:
	//case CC_Yuv420_to_bgra_4:
	//	Yuv420P_To_Xrgb_4(rSrcFrameDescr,rDestFrameDescr,pixel_format);
	//	break;
	//default:
	//	return FALSE;
	//};
	// return TRUE;
 //}
BOOL CConvertorColorSpace::Yuv420P_To_Xrgb_4(const  VideoFrameInfo& rSrcFrameDescr,VideoFrameInfo& rDestFrameDescr,PixelFormat pix_format)
 {
	// конвертация из YUV420P в BGRA/ARGB/RGBA/ABGR. 
	// to aligned buf...
	AVPicture InPict;
	int nBufSize=rSrcFrameDescr.nImageWidth*rSrcFrameDescr.nImageHeight;
	InPict.data[0]=reinterpret_cast<uint8_t*>(rSrcFrameDescr.pData);
	InPict.data[1]=reinterpret_cast<uint8_t*>(rSrcFrameDescr.pData)+nBufSize;
	InPict.data[2]=reinterpret_cast<uint8_t*>(rSrcFrameDescr.pData)+(nBufSize+nBufSize/4);

	InPict.linesize[0]=rSrcFrameDescr.nImageWidth;
	InPict.linesize[1]=rSrcFrameDescr.nImageWidth/2;
	InPict.linesize[2]=rSrcFrameDescr.nImageWidth/2;

	int nRez;
	AVPicture pict;
	pict.data[0]=rDestFrameDescr.pData;	
	pict.data[1]=NULL;
	pict.data[2]=NULL;

	pict.linesize[0]=rDestFrameDescr.nImageWidth*4;
	pict.linesize[1]=0;
	pict.linesize[2]=0;

	//=================================================================================
	SwsContext *convert_ctx = m_pFfmpeg_obj->m_ffmpeg_obj.sws_getContext(
					  rSrcFrameDescr.nImageWidth					// srctW
					, rSrcFrameDescr.nImageHeight					// srctH
					, PIX_FMT_YUV420P
					, rDestFrameDescr.nImageWidth
					, rDestFrameDescr.nImageHeight
					, pix_format		//PIX_FMT_BGRA
					, SWS_BICUBIC
					, NULL
					, NULL
					, NULL);

	if(convert_ctx)
	{
		nRez=m_pFfmpeg_obj->m_ffmpeg_obj.sws_scale(convert_ctx, InPict.data, InPict.linesize, 0, rSrcFrameDescr.nImageHeight, pict.data, pict.linesize);
		m_pFfmpeg_obj->m_ffmpeg_obj.sws_freeContext(convert_ctx);
		return TRUE;
	}
	return FALSE;
 }

BOOL CConvertorColorSpace::Xrgb_4_To_Yuv420P(const  VideoFrameInfo& rSrcFrameDescr,VideoFrameInfo& rDestFrameDescr,PixelFormat pix_format)
 {
	// конвертация из YUV420P в BGRA/ARGB/RGBA/ABGR. 
	// to aligned buf...
	AVPicture pict;
	int nBufSize=rDestFrameDescr.nImageWidth*rDestFrameDescr.nImageHeight;
	pict.data[0]=reinterpret_cast<uint8_t*>(rDestFrameDescr.pData);
	pict.data[1]=reinterpret_cast<uint8_t*>(rDestFrameDescr.pData)+nBufSize;
	pict.data[2]=reinterpret_cast<uint8_t*>(rDestFrameDescr.pData)+(nBufSize+nBufSize/4);

	pict.linesize[0]=rDestFrameDescr.nImageWidth;
	pict.linesize[1]=rDestFrameDescr.nImageWidth/2;
	pict.linesize[2]=rDestFrameDescr.nImageWidth/2;

	int nRez;
	AVPicture InPict;	//pict;
	InPict.data[0]=reinterpret_cast<uint8_t*>(rSrcFrameDescr.pData);		//pRgbBufTmp;
	InPict.data[1]=NULL;
	InPict.data[2]=NULL;

	InPict.linesize[0]=rSrcFrameDescr.nImageWidth*4;
	InPict.linesize[1]=0;
	InPict.linesize[2]=0;

	//=================================================================================
	SwsContext *convert_ctx = m_pFfmpeg_obj->m_ffmpeg_obj.sws_getContext(
					  rSrcFrameDescr.nImageWidth					// srctW
					, rSrcFrameDescr.nImageHeight					// srctH
					, pix_format									// source format
					, rDestFrameDescr.nImageWidth					// destW
					, rDestFrameDescr.nImageHeight					// destH
					, PIX_FMT_YUV420P								// dest format
					, SWS_BICUBIC
					, NULL
					, NULL
					, NULL);

	if(convert_ctx)
	{
		nRez=m_pFfmpeg_obj->m_ffmpeg_obj.sws_scale(convert_ctx, InPict.data, InPict.linesize, 0, rSrcFrameDescr.nImageHeight, pict.data, pict.linesize);
		m_pFfmpeg_obj->m_ffmpeg_obj.sws_freeContext(convert_ctx);
		//-------  test  -----------------------------------------------------------------------
		//Yuv420p_FrameFileRecord(pict,rDestFrameDescr.nImageWidth,rDestFrameDescr.nImageHeight);
		//--------------------------------------------------------------------------------------
		return TRUE;
	}
	return FALSE;
 }

/************************************************************************************************************

************************************************************************************************************/

//BOOL CConvertorColorSpace::Yuv420p_FrameFileRecord(const AVPicture& rPicture, int width,int height)
//{
//	const char* pFileName="c:\\ffmpeg\\bin\\convertor_color_test.yuv";
//	int i;
//	__try
//	{
//	if(!m_pTetstFile)
//	{
//		m_pTetstFile=::fopen(pFileName,"wb");
//	}
//	if(m_pTetstFile)
//	{
//		/* записываем в фал тремя массивами: Y U V , помним, что U и V меньше в 2 раза по отношению к Y */
//         for(i=0; i<height; i++)
//            fwrite(rPicture.data[0] + i * rPicture.linesize[0], 1, width,m_pTetstFile);
//         for(i=0; i<height/2; i++)
//            fwrite(rPicture.data[1] + i * rPicture.linesize[1], 1,width/2, m_pTetstFile);
//         for(i=0; i<height/2; i++)
//            fwrite(rPicture.data[2] + i * rPicture.linesize[2], 1,width/2, m_pTetstFile);
//		 return TRUE;
//	}
//	return FALSE;
//	}
//	__except(EXCEPTION_EXECUTE_HANDLER)
//	{
//		return FALSE;
//	}
//}
