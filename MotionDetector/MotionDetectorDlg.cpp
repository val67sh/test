// MotionDetectorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MotionDetector.h"
#include "MotionDetectorDlg.h"

#include <vld.h>

//#include "ImageSrc.h"

//================================================================    test...
#include "Filter.h"

/*
	test units...
*/
//#include "../TEST_UNIT/TestUnit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WIDTH_CREATOR	8*6
#define HEIGTH_CREATOR	8*4


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMotionDetectorDlg dialog




CMotionDetectorDlg::CMotionDetectorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMotionDetectorDlg::IDD, pParent)
	,bPaintData(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMotionDetectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_WINDOW_0, m_csWindow_00);
	DDX_Control(pDX, IDC_STATICWINDOW_1, m_csWindow_01);
	DDX_Control(pDX, IDC_STATICWINDOW_2, m_csWindow_02);
	DDX_Control(pDX, IDC_STATICWINDOW_3, m_csWindow_03);
}

BEGIN_MESSAGE_MAP(CMotionDetectorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CMotionDetectorDlg::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_BUTTON_STEP, &CMotionDetectorDlg::OnBnClickedButtonStep)
	ON_BN_CLICKED(IDC_BUTTON_START, &CMotionDetectorDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_FILE_OPEN, &CMotionDetectorDlg::OnBnClickedButtonFileOpen)
	ON_BN_CLICKED(IDC_BUTTON_SCENARIO_SEL, &CMotionDetectorDlg::OnBnClickedButtonScenarioSel)
END_MESSAGE_MAP()


// CMotionDetectorDlg message handlers

BOOL CMotionDetectorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	bPaintData=false;
	m_display.SetPreviewWindowHwnd(&m_csWindow_00);
	m_display.SetBackgroundWindowHwnd(&m_csWindow_01);
	m_display.SetMotionWindowHwnd(&m_csWindow_02);
	m_display.SetForegroundWindowHwnd(&m_csWindow_03);
	//======================================
						test_gradinet();
	//======================================

	m_control.ImageInit(new FileImage(m_display));
	//m_control.ImageInit(new CVideoFrameCreator(m_display,WIDTH_CREATOR,HEIGTH_CREATOR));

	//===============================================================   TEST
	//static FileImage f_image(m_display);
	bPaintData=true;
	//f_image.PreviewImage();
	//MemoryBlocksCompare_test();
	//======================================================================
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMotionDetectorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMotionDetectorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

	if(bPaintData)
	{
		CPaintDC dc(this);
		m_display.ShowLine(&dc);
		RECT rect;
		rect.top=5;
		rect.left=5;
		rect.bottom=30;
		rect.right=30;
		//m_display.ShowRectangle(rect);
		m_display.ShowColorRectangle(rect);

		//FileImage f_image(m_display);
		//f_image.PreviewImage();

		m_control.AllImagesRefresh();  //!!!  First-chance exception at 0x00342B77 in MotionDetector.exe: 0xC0000005: Access violation reading location 0x00000000.
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMotionDetectorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMotionDetectorDlg::OnBnClickedButtonRun()
{
	// TODO: Add your control notification handler code here
	m_control.Run();
}


void CMotionDetectorDlg::OnBnClickedButtonStep()
{
	// TODO: Add your control notification handler code here
	m_control.NextFrameRun();
}


void CMotionDetectorDlg::OnBnClickedButtonStart()
{
	// TODO: 2 - create a process, based on a CProcessCtrl class
	// this process uses Image from FileImage/CVideoFrameCreator object. If Image was created by separate thread - we need to
	// wait for 
	m_control.Start();
}


void CMotionDetectorDlg::OnBnClickedButtonFileOpen()
{
	// TODO: 1 - create a Image, based on a FileImage class

	// to do: create a file open procedure
	std::string strImagePathBmp	= "i:\\PROJECTS\\VIDEOEDITOR_PRG_TEST\\FFMPEG_VIDEO_TEST\\FFMPEG_VIDEO_TEST\\1.bmp";
	
	ImageInfo img_info;
	img_info.nHeigth = -1;
	img_info.nWidth = -1;
	img_info.pImage = (PBYTE)strImagePathBmp.c_str();
	img_info.color_type = ImCT_UNKN;
	img_info.pPrivateInfo = NULL;
	m_control.SetParams(&img_info);
}

//************************************************
#define BACKGR_OBJECT_THRESHOLD_LEVEL	20	//****
//************************************************
void CMotionDetectorDlg::OnBnClickedButtonScenarioSel()
{
	// TODO: 1 - create a Image, based on a CVideoFrameCreator class
	// this image must be created by separate thread (!???)
	ImageInfo ImgInfo;
	ImageCreatorInfo ImgCreatorInfo;
	RectDescriptor ObjDescr;
	ImageBackgroundDescr BackGrounddescr;

	ImgInfo.nWidth = WIDTH_CREATOR;
	ImgInfo.nHeigth = HEIGTH_CREATOR;
	ImgInfo.pPrivateInfo = &ImgCreatorInfo;
	ImgInfo.pImage = NULL;
	ImgInfo.color_type = ImCT_GREYSCALE_8;

	ImgCreatorInfo.ImgColorType = ImgInfo.color_type;	//ImCT_GREYSCALE_8;
	ImgCreatorInfo.objType = OBJ_RECT;
	ImgCreatorInfo.pObjDescriptor = &ObjDescr;
	ImgCreatorInfo.pBackgroundDescriptor = &BackGrounddescr;

	BackGrounddescr.fillType = FILL_CONST;
	BackGrounddescr.nPixelVal = 125;

	ObjDescr.fill_type = FILL_CONST;
	ObjDescr.nPixelVal = BackGrounddescr.nPixelVal - (BACKGR_OBJECT_THRESHOLD_LEVEL + 10);
	ObjDescr.rect_param.right = (WIDTH_CREATOR/4)*2;
	ObjDescr.rect_param.bottom = (HEIGTH_CREATOR/4)*2;
	ObjDescr.rect_pos.top = HEIGTH_CREATOR/4;
	ObjDescr.rect_pos.left = WIDTH_CREATOR/4;
	ObjDescr.rect_pos.bottom = -1;//(HEIGTH_CREATOR/4)*2;
	ObjDescr.rect_pos.right = -1;//(WIDTH_CREATOR/4)*2;

	m_control.SetParams(&ImgInfo);


}
