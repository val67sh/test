
#include "stdafx.h"
#include "ProcessCtrl.h"





CProcessCtrl::CProcessCtrl(ProcessCtrlInfo& process_info)
	:m_process_info(process_info)
	,m_vMBinfo(TOTAL_CELLS(process_info.nWidth,process_info.nHeigth,process_info.nCellSize.nX,process_info.nCellSize.nY))
	,m_pDataPrt(process_info.pDataPtr)
	,m_bImgObjectCtrl(FALSE)
	,m_nStepNumber(0)
	,m_pImgData(process_info.pData)
	,m_nWidth(process_info.nWidth)
	,m_nHeigth(process_info.nHeigth)
	,m_pAlgorithm(NULL)
{
	m_MeImgInfo.nCellsPerColumn = CELLS_PER_COLUMN(process_info.nHeigth,process_info.nCellSize.nY);
	m_MeImgInfo.nCellsPerRow = CELLS_PER_ROW(process_info.nWidth,process_info.nCellSize.nX);
	
	memcpy(&m_VibrCtrlInfo,&process_info.VibrationCtrlInfo,sizeof(m_VibrCtrlInfo));
	//m_pData.reset(process_info.pData);
	//memset(m_pDataPrt.get(),0,m_process_info.nDataSize);
	memcpy(m_pDataPrt.get(),m_pImgData,m_nWidth*m_nHeigth);
	switch(process_info.MotionEstimationType)
	{
	case ME_TSS:
		m_pAlgorithm = new CME_TSS;
		break;
	};
}

CProcessCtrl::~CProcessCtrl()
{
}

void CProcessCtrl::Perform()
{
	m_pAlgorithm->vFrame_parse((PBYTE)m_pImgData,(PBYTE)m_pDataPrt.get(),m_nWidth,m_nHeigth);
}

void CProcessCtrl::ObjectPositionSet(RECT& rObjrect) 
{ 
	m_ObjectPosition = rObjrect; m_bImgObjectCtrl=TRUE;
	CreateObjPositionList(&m_VibrCtrlInfo);
}
RECT CProcessCtrl::GetNextObjPosition()
{
	RECT rect;
	memset(&rect,-1,sizeof(rect));
	if(m_vObjPositionList.empty())
		return rect;
	if(m_nStepNumber >= m_vObjPositionList.size())
		return rect;
	rect = m_vObjPositionList[m_nStepNumber];
	++m_nStepNumber;
	return rect;
}

//
BOOL CProcessCtrl::CreateObjPositionList(const VibrationCtrlInfo* pVibrCtrlInfo)
{
	motion_vector vector;
	int nFrameCnt = pVibrCtrlInfo->nScenarioTotalSteps;
	if(!nFrameCnt) return FALSE;
	if(FALSE == m_bImgObjectCtrl)  return FALSE;
	memset(&vector,-1,sizeof(vector));
	vector.x = m_ObjectPosition.left;
	vector.y = m_ObjectPosition.top;

	for(int i=0; i<nFrameCnt; i++)
	{
		RECT rect = VectorToRect(vector);
		m_vObjPositionList.push_back(rect);
		vector = CreateNextPositionOfObj(vector,i/*m_nStepNumber*/,pVibrCtrlInfo);
		//++m_nStepNumber;
	}
	++m_nStepNumber;
	return TRUE;
}
inline RECT CProcessCtrl::VectorToRect(motion_vector vector)
{
	RECT rect;
	rect.top = vector.y;
	rect.left = vector.x;
	rect.bottom = rect.right = -1;
	return rect;
}
motion_vector CProcessCtrl::CreateNextPositionOfObj( motion_vector cur_v_pos, int nStepNumber, const VibrationCtrlInfo* pVibrCtrlInfo)
{
	motion_vector new_vect = cur_v_pos;
	//m_process_info.nHeigth;
	int nMotionStep;
	if(VT_CONSTANT_SHIFT == pVibrCtrlInfo->v_type)
	{
		nMotionStep=pVibrCtrlInfo->nMotionStep + pVibrCtrlInfo->nAcselarationStep *nStepNumber;
		switch(pVibrCtrlInfo->v_direction)
		{
		case VD_UP:
			new_vect.y = cur_v_pos.y-nMotionStep >=0? cur_v_pos.y-nMotionStep:0; 
			break;
		case VD_DOWN:
			new_vect.y = cur_v_pos.y+nMotionStep <=m_process_info.nHeigth? cur_v_pos.y+nMotionStep:m_process_info.nHeigth;
			break;
		case VD_LEFT:
			new_vect.x = cur_v_pos.x-nMotionStep >=0? cur_v_pos.x-nMotionStep:0; 
			break;
		case VD_RIGHT:
			new_vect.x = cur_v_pos.x+nMotionStep <=m_process_info.nWidth? cur_v_pos.x+nMotionStep:m_process_info.nWidth;
			break;
		case VD_UP_RIGHT:
			break;
		case VD_UP_LEFT:
			break;
		case VD_DOWN_RIGHT:
			break;
		case VD_DOWN_LEFT:
			break;
		case VD_SUBDIRECT:
			break;
		case VD_UNKN:
			break;
		};
	}
	else
		if ( VT_SHIFT_GO_BACK == pVibrCtrlInfo->v_type)
		{
		}
		return new_vect;
}