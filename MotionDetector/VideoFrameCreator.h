
#ifndef _VIDEO_FRAME_CREATOR_
#define _VIDEO_FRAME_CREATOR_

#include "ImageSrc.h"
#include <memory>
#include <future>

using namespace std;

#define WIDTH	8*6
#define HEIGTH	8*4

#define LEN_OF_OBJECT(start_pos,len,limit) ((int)((((start_pos)+(len))>(limit))? ((limit)-(start_pos)):len))

class CVideoFrameCreator: protected Image
{
public:
	CVideoFrameCreator(CDisplay& rImageDraw,DWORD nWidth,DWORD nHeigth);
	~CVideoFrameCreator();
	//
	void CreateBackground(const PVOID pImageDescriptor);
	BOOL CreateGreyScaleBackground(FILL_TYPE FillType,DWORD nGreyScaleVal);
	BOOL CreateObject(const OBJECT_TYPE objType, const PVOID pObjDescrp);

	// virtual functions....
	void CreateImage(const ImageInfo& rIInfo) { 
		m_ImInfo = rIInfo;
		m_ImInfo.pPrivateInfo = &m_ImgCreatorInfo;
		memcpy(&m_ImgCreatorInfo,rIInfo.pPrivateInfo,sizeof(m_ImgCreatorInfo));
		memcpy(&m_ImgBackgrDescr,m_ImgCreatorInfo.pBackgroundDescriptor,sizeof(m_ImgBackgrDescr));
		//RectDescriptor* pObjDescr = static_cast<RectDescriptor*>(m_ImgCreatorInfo.pObjDescriptor);
		m_ImgCreatorInfo.pBackgroundDescriptor=&m_ImgBackgrDescr;
		switch(m_ImgCreatorInfo.objType)
		{
		case OBJ_RECT:
			memcpy(&m_RectDescr,m_ImgCreatorInfo.pObjDescriptor,sizeof(m_RectDescr));
			m_ImgCreatorInfo.pObjDescriptor=&m_RectDescr;
			m_pRect_obj_pos = &m_RectDescr.rect_pos;
			break;
		default:
			break;
		};
		m_ImInfo.pImage = reinterpret_cast<PBYTE>(m_pFramePtr.get());
		CreateImageRunAsync(m_ImInfo);
	}
	ImageInfo GetImage() const { 
		return m_ImInfo;
	}
	BOOL CreateNextImage(const RECT& rObjPosition);
	void ImageRefresh();
	BOOL IsImageReady();
	//------------------------  virtuals functions end...
	BOOL IsImageReadyAsync();
private:
	BOOL PreviewImage();
	void FillGreyScaleBackground(const ImageBackgroundDescr* pBackGrounddescr);
	void CreateRect(const PVOID pObjDescrp);
	void CreateImageRunAsync(const ImageInfo& rIInfo);
	BOOL CreateFullImage(const ImageInfo& rIInfo, BOOL bStart=false);
private:
	DWORD m_nWidth;
	DWORD m_nHeigth;
	DWORD m_nImgSize;
	PBYTE m_pFrame;
	BOOL m_bScenarioError;
	shared_ptr<char> m_pFramePtr;

	std::future<BOOL> m_img_creator;

	// parameters
	ImageCreatorInfo m_ImgCreatorInfo;
	ImageBackgroundDescr m_ImgBackgrDescr;
	RECT* m_pRect_obj_pos;
	RectDescriptor m_RectDescr;
};

#endif