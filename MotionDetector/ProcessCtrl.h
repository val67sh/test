
#ifndef _PROCESS_CTRL_H_
#define _PROCESS_CTRL_H_

#include <memory>
#include <vector>
#include "MEAlgorithms.h"
using namespace std;

enum eVibrationType { VT_UNKN, VT_CONSTANT_SHIFT, VT_SHIFT_GO_BACK, };
enum eVibrationDirection {VD_UNKN, VD_UP, VD_DOWN, VD_LEFT, VD_RIGHT, VD_UP_RIGHT, VD_UP_LEFT, VD_DOWN_RIGHT, VD_DOWN_LEFT, VD_SUBDIRECT};
// the subdirection vector can use if nMotionStep like: 2,4,6,8 !!!
enum eVibrationSubDirect {VSD_UNKN, VD_UP_RIGHT_HALF,VD_UP_LEFT_HALF};
enum eVibrationAcselaration {VA_UNKN, VA_CONSTANT, VA_LINEAR, VA_WAVE};

typedef struct VibrationCtrlInfo_
{
	int nMotionStep;			// pos=pos+nMotionStep
	int nAcselarationStep;		// nMotionStep=nMotionStep+nAcselarationStep*nStepNumber
	int nDepthStep;				// for VT_SHIFT_GO_BACK mode
	int nScenarioTotalSteps;	/* The total frames of scenario 
	(we need to create or we need get from a file, it depend on the images source: CVideoFrameCreator or FileImage) */ 
	eVibrationType v_type;
	eVibrationDirection v_direction;
	eVibrationSubDirect v_sub_direction;
	eVibrationAcselaration v_acselaration;
}VibrationCtrlInfo;

/*
	eProcessType - describes type of process, like: 
		- motion estimation if a background is stable and not active: PR_NO_ACTIVE_STABLE
		- motion estimation if a background is unstable and not active: PR_NO_ACTIVE_UNSTABLE

  * if background is not active it means that it is a constant.
  ** if background is active it means that this background is a dynamic background (not constatnt).
*/
enum eProcessType {PR_UNKN, PR_NO_ACTIVE_STABLE, PR_NO_ACTIVE_UNSTABLE};

/*
two Dimensional Logarithmic search(2DLS), three step search(TSS), diamond search
algorithm (DSA), Cross Search(CS),Binary Search(BS), Hexagonal Search Pattern(HXSP), Adaptive Rood Pattern
Search(ARPS) etc. 
*/
enum eMEAlgorithmType {ME_UNKN, ME_TSS, ME_DSA, ME_CS, ME_2DLS, ME_BS, ME_HXSP};

/*
Matching Criteria:
In order to find out the co-relation between consecutive frames k and k+1 and to find out the best
MV(motion vector) in reference frames, the following criteria are considered. 
	Mean Absolute Difference (MAD); Sum of absolute difference (SAD)
*/
enum eMatchingCriteria {MC_UNKN, MC_SAD, MC_MAD};

typedef struct CellType_
{
	int nX;
	int nY;
}CellType;

typedef struct ME_ImageInfo_
{
	int nCellsPerRow;
	int nCellsPerColumn;
}ME_ImageInfo;

typedef struct ProcessCtrlInfo_
{
	ProcessCtrlInfo_(int size)
		:pDataPtr(new _declspec (align(16))char[size],std::default_delete<char[]>())
		,nDataSize(size)
		,pData(NULL)
	{
	}
	char* pData;								// grayscale always (1 byte: 8 bits); the pointer to a Image (Image class)
	int nWidth;
	int nHeigth;
	int nDataSize;
	shared_ptr <char> pDataPtr;					// this array need to keep previous image

	CellType nCellSize;							// major cell size
	eProcessType ProcType;
	eMEAlgorithmType MotionEstimationType;
	eMatchingCriteria MatchingCriteria;			// Matching Criteria Set

	VibrationCtrlInfo VibrationCtrlInfo;
}ProcessCtrlInfo;

#define TOTAL_CELLS(w,h,x,y) ((int)(((w)*(h))/((x)*(y))))
#define CELLS_PER_ROW(w,x) ((int)((w)/(x)))
#define CELLS_PER_COLUMN(h,y) ((int)((h)/(y)))

//			see a Start()function
/*
	this class needs to save two frames: previous and current frames
*/
class CProcessCtrl
{
public:
	CProcessCtrl(ProcessCtrlInfo& process_info);
	~CProcessCtrl();
	void ObjectPositionSet(RECT& rObjrect);
//private:
	RECT GetNextObjPosition();
	void Perform();

private:
	inline RECT VectorToRect(motion_vector vector);
	BOOL CreateObjPositionList(const VibrationCtrlInfo* pVibrCtrlInfo);
	motion_vector CreateNextPositionOfObj( motion_vector cur_v_pos, int nStepNumber, const VibrationCtrlInfo* pVibrCtrlInfo);
private:
	char* m_pImgData;
	DWORD m_nWidth;
	DWORD m_nHeigth;
	ME_ImageInfo m_MeImgInfo;
	shared_ptr<char> m_pDataPrt;
	ProcessCtrlInfo& m_process_info;
	vector <MackroBlockInfo> m_vMBinfo;
	VibrationCtrlInfo m_VibrCtrlInfo;

	RECT m_ObjectPosition;
	vector <RECT> m_vObjPositionList;
	// Marker m_bImgObjectCtrl: object of image control. It means that class must create position list of jbject
	BOOL m_bImgObjectCtrl;	
	int m_nStepNumber;

	CMEAlgorithms* m_pAlgorithm;
};

#endif	// _PROCESS_CTRL_H_