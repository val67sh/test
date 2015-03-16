
#ifndef _IMAGE_SRC_H_
#define _IMAGE_SRC_H_

//#include "Display.h"
#include "ScreenDraw.h"
#include <string>
#include <memory>
using namespace std;

enum eImageColorType {ImCT_UNKN, ImCT_RGB, ImCT_RGBA, ImCT_GREYSCALE_8, ImCT_GREYSCALE_32, ImCT_BW};
enum OBJECT_TYPE {OBJ_UNKN, OBJ_RECT, OBJ_TRIANGLE};

// gradient: i:\PROJECTS\2D3DVideoEditor\2D3DVideoEditor\Gradient.cpp
enum FILL_TYPE {FILL_UNKN, FILL_CONST, FILL_HORIZONTAL, FILL_VERTICAL};

typedef struct ImageCreatorInfo_
{
	eImageColorType ImgColorType;
	OBJECT_TYPE objType;
	PVOID pObjDescriptor;
	PVOID pBackgroundDescriptor;

}ImageCreatorInfo;

typedef struct ImageInfoX_
{
	DWORD nWidth;
	DWORD nHeigth;
	PBYTE pImage;
	//shared_ptr<char> pImagePtr;
	PVOID pPrivateInfo;			// additional information, like: ImageCreatorInfo
	eImageColorType color_type;
}ImageInfo;

typedef struct ImageBackgroundDescr_
{
	FILL_TYPE fillType;
	unsigned int nPixelVal;
}ImageBackgroundDescr;

typedef struct RectDescriptor_
{
	RECT rect_pos;			// rectangle's position in the frame (top left corner: x,y)
	RECT rect_param;		// size of rectangle
	FILL_TYPE fill_type;
	unsigned int nPixelVal;
}RectDescriptor;

//void xx();

class Image
{
protected:
	Image(CDisplay& rImageDraw);
public:
	virtual ~Image()
	{}
	//
	virtual void CreateImage(const ImageInfo& rIInfo) = 0;
	virtual ImageInfo GetImage() const = 0;
	virtual BOOL CreateNextImage(const RECT& rObjPosition) = 0;
	virtual void ImageRefresh() = 0;
	virtual BOOL IsImageReady() = 0;

protected:
	BOOL ConvertGreyscale32ToGreyscale8(const ImageInfo& rImgInfoSrc, const ImageInfo& rImgInfoDest);
	BOOL ConvertGreyscale8ToGreyscale32(const ImageInfo& rImgInfoSrc, const ImageInfo& rImgInfoDest);
protected:
	CDisplay& m_rImageDraw;
	ImageInfo m_ImInfo;
};
/*
	class FileImage - a source of image is a file/files...
	The file format: 
		.bmp	(path: i:\\PROJECTS\\VIDEOEDITOR_PRG_TEST\\FFMPEG_VIDEO_TEST\\FFMPEG_VIDEO_TEST\\)
		.jpg	(path: i:\\PROJECTS\\LIB_JPEG_TURBO\\LibJpegTurboTest\\)
*/
class FileImage	: protected Image
{
public:
	FileImage(CDisplay& rImageDraw);
	~FileImage()
	{}

	// virtual functions....
	void CreateImage(const ImageInfo& rIInfo) 
	{ 
		m_ImInfo = rIInfo;
		//GetFilePath(reinterpret_cast<char*>(rIInfo.pImage));
		GetFilePath(reinterpret_cast<char*>(rIInfo.pImage));
	}
	ImageInfo GetImage() const {
		//ImageInfo info = m_ImInfo;
		//char* pPath = const_cast<char*>(m_sFilePath.c_str());
		//info.pImage=reinterpret_cast<PBYTE>(pPath);
		//return info;
		return m_ImInfo;
	}
	BOOL IsImageReady()
	{
		return TRUE;
	}
	void ImageRefresh();
	BOOL CreateNextImage(const RECT& rObjPosition);
	//------------------------  virtuals functions end...

	//void 
	void PreviewImage();
	//int AS() { return 10;}
private:
	//ImageInfo m_ImInfo;
	void GetFilePath(char* pPath)
	{
		m_sFilePath = pPath;
	}
	std::string m_sFilePath;
};

#endif	// _IMAGE_SRC_H_
