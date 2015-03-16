
#include "stdafx.h"
#include "VideoFrameCreator.h"


CVideoFrameCreator::CVideoFrameCreator(CDisplay& rImageDraw,DWORD nWidth,DWORD nHeigth)
	:Image(rImageDraw)
	//,m_pFrame(new BYTE [nWidth*nHeigth])
	,m_nHeigth (nHeigth)
	,m_nWidth (nWidth)
	,m_nImgSize(nWidth*nHeigth)
	,m_pFramePtr(new _declspec (align(16)) char [nWidth*nHeigth],std::default_delete<char[]>())
	,m_pFrame(reinterpret_cast<PBYTE>(m_pFramePtr.get()))
	,m_pRect_obj_pos(NULL)
	,m_bScenarioError(FALSE)
{
	//memset(m_pFrame,0,nWidth*nHeigth);
	memset(m_pFramePtr.get(),0,nWidth*nHeigth);
}
CVideoFrameCreator::~CVideoFrameCreator()
{
}

// functions of interface (virtual functions)
BOOL CVideoFrameCreator::IsImageReady()
{
	return IsImageReadyAsync();
}

BOOL CVideoFrameCreator::CreateNextImage(const RECT& rObjPosition)
{
	if(rObjPosition.top == -1 || rObjPosition.left == -1)
	{
		m_bScenarioError=TRUE;
		return FALSE;
	}
	memcpy(m_pRect_obj_pos,&rObjPosition,sizeof(RECT));
	//return CreateObject(m_ImgCreatorInfo.objType,m_ImgCreatorInfo.pObjDescriptor);
	CreateImageRunAsync(m_ImInfo);
	return TRUE;
}

BOOL CVideoFrameCreator::CreateFullImage(const ImageInfo& rIInfo, BOOL bStart)
{
	const ImageCreatorInfo* pRectDescr =  static_cast<ImageCreatorInfo*>(rIInfo.pPrivateInfo);
	CreateBackground(rIInfo.pPrivateInfo);
	return CreateObject(pRectDescr->objType,pRectDescr->pObjDescriptor);
	//return TRUE;
}

void CVideoFrameCreator::CreateBackground(const PVOID pImageDescriptor)
{
	const ImageCreatorInfo* pRectDescr =  static_cast<ImageCreatorInfo*>(pImageDescriptor);
	if(ImCT_GREYSCALE_8 == pRectDescr->ImgColorType)
	{
		ImageBackgroundDescr* pBackGrounddescr = static_cast<ImageBackgroundDescr*>(pRectDescr->pBackgroundDescriptor);
		FillGreyScaleBackground(pBackGrounddescr);
	}
}

void CVideoFrameCreator::FillGreyScaleBackground(const ImageBackgroundDescr* pBackGrounddescr)
{
	switch(pBackGrounddescr->fillType)
	{
	case FILL_CONST:
		memset(m_pFramePtr.get(),pBackGrounddescr->nPixelVal,m_nImgSize);
		break;
	};
}

/*
Function CreateObject():
	RECT pos			- position of object in frame (x,y)
	PVOID pObjDescrp	- object descriptor
*/
BOOL CVideoFrameCreator::CreateObject(const OBJECT_TYPE objType, const PVOID pObjDescrp)
{
	switch(objType)
	{
	case OBJ_RECT:
		CreateRect(pObjDescrp);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
/*
	The PVOID pObjDescrp parameter is a RectDescriptor
*/
void CVideoFrameCreator::CreateRect(const PVOID pObjDescrp)
{
	const RectDescriptor* pRectDescr =  static_cast<RectDescriptor*>(pObjDescrp);
	char* pData = m_pFramePtr.get();	//m_pFrame;
	switch(pRectDescr->fill_type)
	{
	case FILL_CONST:
		int nTop = pRectDescr->rect_pos.top;
		int nBottom = pRectDescr->rect_pos.top + pRectDescr->rect_param.bottom;
		int n;
		for(n=0; n<m_nHeigth;/*pRectDescr->rect_param.bottom;*/ n++)
		{
			if( n>=nTop && n<nBottom)
			{
				int nRectLen = LEN_OF_OBJECT(pData+pRectDescr->rect_pos.left,pRectDescr->rect_param.right,pData+m_nWidth);
				//memset(pData+pRectDescr->rect_pos.left,pRectDescr->nPixelVal,pRectDescr->rect_param.right);
				memset(pData+pRectDescr->rect_pos.left,pRectDescr->nPixelVal,nRectLen);
			}
				pData+=m_nWidth;  
			//}
		}
		break;
	}
}

//		control functions...
void CVideoFrameCreator::CreateImageRunAsync(const ImageInfo& rIInfo)
{
	m_img_creator = std::async(std::launch::async,&CVideoFrameCreator::CreateFullImage,this,rIInfo,true);
	//m_img_creator = std::async(&CVideoFrameCreator::CreateFullImage,this,rIInfo,true);
}
BOOL CVideoFrameCreator::IsImageReadyAsync() 
{
	if(m_bScenarioError)
		return FALSE;
	return m_img_creator.get();
}

// preview...
void CVideoFrameCreator::ImageRefresh()
{
	PreviewImage();
}

BOOL CVideoFrameCreator::PreviewImage()
{
	try
	{
	shared_ptr<BYTE> pGrayscaleWiew(new BYTE[m_nImgSize*4],std::default_delete<BYTE[]>());
	ImageInfo info_src_8;
	ImageInfo info_dest_32;

	info_src_8.color_type = ImCT_GREYSCALE_8;
	info_src_8.pImage = reinterpret_cast<PBYTE>(m_pFramePtr.get());
	info_src_8.nWidth = m_nWidth;
	info_src_8.nHeigth = m_nHeigth;

	info_dest_32.color_type = ImCT_GREYSCALE_32;
	info_dest_32.pImage = pGrayscaleWiew.get();
	info_dest_32.nWidth = m_nWidth;
	info_dest_32.nHeigth = m_nHeigth;
	// RGBA format...
	CScreenDraw& draw = static_cast<CScreenDraw&>(m_rImageDraw);
	ConvertGreyscale8ToGreyscale32(info_src_8,info_dest_32);
	//draw.PreviewDrawPicture(pDataBufSrc,nBufSize,bitmap.bmWidth,bitmap.bmHeight);
	draw.PreviewDrawPicture(pGrayscaleWiew.get(),m_nImgSize*4,m_nWidth,m_nHeigth);
	return TRUE;
	}
	catch(...)
	{
		return FALSE;
	}
}