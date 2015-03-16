

#ifndef _CONVERTOR_COLOR_SPACE_VIDEO_H_
#define _CONVERTOR_COLOR_SPACE_VIDEO_H_


#include <Windows.h>
#include <string>
#include "..\DLL_STUB\AvcodecStub.h"
#include "..\DLL_STUB\AvformatStub.h"
#include "..\DLL_STUB\AvutilStub.h"
#include "..\DLL_STUB\SwscaleStub.h"

#include "..\FFMPEG_WRAPPER\FfmpegWrapper.h"

#include "..\FFMPEG_LIB\include\libavcodec\avcodec.h"
//#include "..\MultimediaServer\EncoderDescriptor.h"

using namespace std;

enum eVideoColorType {vType_none=0,vType_Yuv420,vType_Yuv422,vType_grayscale,vType_rgb_3,vType_argb_4,vType_rgba_4,vType_abgr_4,vType_bgra_4,};

typedef struct sVideoFrameInfo
{
	int nImageWidth;
	int nImageHeight;
	int nFrameBufSize;
	eVideoColorType frame_color_type;
	//char* pData;
	BYTE* pData;
}VideoFrameInfo;


//class CConvertorColorSpace: public CAvutilDll,CAvformatlDll,CAcodecDll,CSwscaleDll
//class CConvertorColorSpace: public CAvutilDll,CSwscaleDll]
class CConvertorColorSpace
{
public:
	CConvertorColorSpace(std::string sLibraryPath);
	~CConvertorColorSpace();

public:
	//BOOL VConvertorColor(const VideoFrameInfo& rSrcFrameDescr
	//							,VideoFrameInfo& rDestFrameDescr
	//							,eVConvertColorType convert_type
	//							);
private:
	CFfmpegDll* m_pFfmpeg_obj;

	BOOL Yuv420P_To_Xrgb_4(const VideoFrameInfo& rSrcFrameDescr,VideoFrameInfo& rDestFrameDescr,PixelFormat pix_format);
	BOOL Xrgb_4_To_Yuv420P(const  VideoFrameInfo& rSrcFrameDescr,VideoFrameInfo& rDestFrameDescr,PixelFormat pix_format);
	//BOOL Yuv420p_FrameFileRecord(const AVPicture& rPicture, int width,int height);
private:
	std::string m_sLibraryPath;
	FILE* m_pTetstFile;
};





#endif		//_CONVERTOR_COLOR_SPACE_VIDEO_H_