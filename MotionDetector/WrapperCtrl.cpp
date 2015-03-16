
#include "stdafx.h"
#include "WrapperCtrl.h"

// source of image...

//template <class T> void CWControl::ImageInitT(T* pImage)
//{
//	IMAGE<T> ImgT(pImage);
//	m_pImageObgPtr=dynamic_cast<>(ImgT.GetImage());
//}



void CWControl::ImageInit(FileImage* pImage)
{
	//bool bRet;
	//bRet = IS_POINTER<FileImage>();
	//bRet = IS_POINTER<FileImage*>();
	//bRet = IS_POINTER<FileImage&>();

	m_pImageSrc.reset(pImage);
	m_pfnCreateImage = std::bind(&FileImage::CreateImage, pImage, std::placeholders::_1);
	m_pfnGetImage = std::bind(&FileImage::GetImage, pImage);
	m_pfnImageRefresh = std::bind(&FileImage::ImageRefresh, pImage);
	m_pfnIsImageReady = std::bind(&FileImage::IsImageReady, pImage);
	m_pfnCreateNextImage = std::bind(&FileImage::CreateNextImage, pImage,std::placeholders::_1);
}

void CWControl::ImageInit(CVideoFrameCreator* pImage)
{
	m_pImage.reset(pImage);
	m_pfnCreateImage = std::bind(&CVideoFrameCreator::CreateImage, pImage, std::placeholders::_1);
	m_pfnGetImage = std::bind(&CVideoFrameCreator::GetImage, pImage);
	m_pfnImageRefresh = std::bind(&CVideoFrameCreator::ImageRefresh, pImage);
	m_pfnIsImageReady = std::bind(&CVideoFrameCreator::IsImageReady, pImage);
	m_pfnCreateNextImage = std::bind(&CVideoFrameCreator::CreateNextImage, pImage,std::placeholders::_1);
}

void CWControl::DisplayInit(CScreenDraw* pDisplay)
{
	m_pDisplay.reset(pDisplay);
}

//================================== control...

void CWControl::Run()						// run test...  automatically play mode
{
}

void CWControl::NextFrameRun()				// step by step play mode
{
	if(m_pTasksList.empty())
		return;
	BOOL bRet;
	vector<shared_ptr<CProcessCtrl>>::iterator vIter = m_pTasksList.begin();
	shared_ptr<CProcessCtrl> pProcCtrl(*vIter);
	//if(m_pfnIsImageReady())
	//{
		int nRet;
		nRet=100;
		RECT rect = pProcCtrl->GetNextObjPosition();
		bRet = m_pfnCreateNextImage(rect);
		if(bRet)
			if(m_pfnIsImageReady())
			{
				//ImageInfo im_info =	m_pfnGetImage();
				m_pfnImageRefresh();
				pProcCtrl->Perform();
			}
	//}
}

#define TOTAL_FRAMES	8

void CWControl::Start()					// start... sets first frame of frames chain (or play list)
{
	RECT rect;
	BOOL bIsFrameCreator=FALSE;
	RectDescriptor* pObjDescr=NULL;
	ImageCreatorInfo* pImgCreatorInfo=NULL;
	if(FALSE==m_pfnIsImageReady())
		return;
	ImageInfo im_info =	m_pfnGetImage();	//m_pImageSrc->GetImage();
	int nImgSize;
	switch(im_info.color_type)
	{
	case ImCT_GREYSCALE_8:
		nImgSize=im_info.nWidth*im_info.nHeigth;
		break;
	};
	if(im_info.pPrivateInfo)
	{
		// this's a video frame creator...
		pImgCreatorInfo=static_cast<ImageCreatorInfo*>(im_info.pPrivateInfo);
		switch(pImgCreatorInfo->objType)
		{
		case OBJ_RECT:
			pObjDescr=static_cast<RectDescriptor*>(pImgCreatorInfo->pObjDescriptor);
			rect = pObjDescr->rect_pos;
			bIsFrameCreator=TRUE;
			break;
		};
	}
	//
	VibrationCtrlInfo VibrCtrlInfo;
	memset(&VibrCtrlInfo,0,sizeof(VibrCtrlInfo));
	VibrCtrlInfo.v_type =			VT_CONSTANT_SHIFT;
	VibrCtrlInfo.v_direction =		VD_RIGHT;
	VibrCtrlInfo.v_acselaration =	VA_CONSTANT;
	VibrCtrlInfo.v_sub_direction =  VSD_UNKN;
	VibrCtrlInfo.nAcselarationStep =	0;
	VibrCtrlInfo.nMotionStep =			2;
	VibrCtrlInfo.nScenarioTotalSteps =	TOTAL_FRAMES;
	VibrCtrlInfo.nDepthStep =0;
	// process info create
	ProcessCtrlInfo info(nImgSize);
	info.nWidth = im_info.nWidth;
	info.nHeigth = im_info.nHeigth;
	info.pData = reinterpret_cast<char*>(im_info.pImage);
	info.nCellSize.nY=16;
	info.nCellSize.nX=16;
	info.ProcType = PR_NO_ACTIVE_STABLE;
	info.VibrationCtrlInfo = VibrCtrlInfo;
	info.MotionEstimationType = ME_TSS;
	shared_ptr<CProcessCtrl> pProcCtrl(new CProcessCtrl(info));
	m_pTasksList.push_back(pProcCtrl);
	m_pfnImageRefresh();
	if(bIsFrameCreator)
	{
		pProcCtrl->ObjectPositionSet(rect);
	}
}

void CWControl::AllImagesRefresh()
{
	//m_pImageSrc->ImageRefresh();
	m_pfnImageRefresh();
}

void CWControl::SetParams(PVOID pParams)
{
	ImageInfo* pImgInfo = static_cast<ImageInfo*>(pParams);
	ImageInfo param;
	memcpy(&param,pParams,sizeof(ImageInfo));
	m_pfnCreateImage(param);
	//ImageInfo param1;
	//param1 = m_pfnGetImage();
}