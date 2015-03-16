
#ifndef _WRAPPER_CTRL_H_
#define _WRAPPER_CTRL_H_

#include "ImageSrc.h"
#include "VideoFrameCreator.h"
#include <memory>
#include <functional>

#include "ProcessCtrl.h"

using namespace std;
template <class T> bool IS_POINTER()
{
	bool bRet = std::is_pointer<T>::value == true? true:false;
	return bRet;
}

class CWControl
{
public:
	CWControl()
		:m_pImageSrc(nullptr)
		,m_pImage(nullptr)
		,m_pDisplay(nullptr)
	{
	}
	~CWControl()
	{}

	//template <typename T> void 

	void ImageInit(FileImage* pImage);
	void ImageInit(CVideoFrameCreator* pImage);
	void DisplayInit(CScreenDraw* pDisplay);

	void SetParams(PVOID pParams);
	void Start();
	void Run();
	void NextFrameRun();
	void AllImagesRefresh();
private:
	shared_ptr<FileImage> m_pImageSrc;
	shared_ptr<CVideoFrameCreator> m_pImage;
	shared_ptr<CScreenDraw> m_pDisplay;

	shared_ptr<Image> m_pImageObgPtr;
	vector<shared_ptr<CProcessCtrl>> m_pTasksList;

	//image interface...
	//void CreateImage(const ImageInfo& rIInfo)
	std::function<void(const ImageInfo&)> m_pfnCreateImage;
	//ImageInfo GetImage() const 
	std::function<ImageInfo()> m_pfnGetImage;
	//void ImageRefresh()
	std::function<void()> m_pfnImageRefresh;
	// BOOL IsImageReady()
	std::function<BOOL()> m_pfnIsImageReady;
	// BOOL CreateNextImage(const RECT& rObjPosition)
	std::function<BOOL(const RECT&)> m_pfnCreateNextImage;
};

#endif	//_WRAPPER_CTRL_H_