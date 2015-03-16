// MotionDetectorDlg.h : header file
//

#pragma once
#include "afxwin.h"

//#include "Display.h"
//#include "ScreenDraw.h"
//#include "ImageSrc.h"
#include "WrapperCtrl.h"

// CMotionDetectorDlg dialog
class CMotionDetectorDlg : public CDialog
{
// Construction
public:
	CMotionDetectorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MOTIONDETECTOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_csWindow_00;
	CStatic m_csWindow_01;
	CStatic m_csWindow_02;
	CStatic m_csWindow_03;

private:
	CWControl m_control;
	CScreenDraw m_display;
	BOOL bPaintData;
public:
	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnBnClickedButtonStep();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonFileOpen();
	afx_msg void OnBnClickedButtonScenarioSel();
};
