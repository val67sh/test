#include "stdafx.h"
#include <windows.h>
#include <process.h>
#include "MEAlgorithms.h"


CMEAlgorithms::CMEAlgorithms()
{
}
CMEAlgorithms::~CMEAlgorithms()
{
}

/*
	PBYTE pStartPos - position of etalon MB (in current frame)
	PBYTE pEndPos	- position of found MB (in previous frame)
*/
motion_vector CMEAlgorithms::CreateMotionVctor(const PBYTE pStartPos, const PBYTE pEndPos,  int nRowLen)
{
	motion_vector vector;
	vector.y = (pEndPos - pStartPos)/nRowLen;
	vector.x = pEndPos - (pStartPos + nRowLen*vector.y);
	return vector;
}
#pragma intrinsic (memcpy)
//inline DWORD CMEAlgorithms::block_8x8_compare(const PBYTE pCurrentBlock, const PBYTE pPrevBlock, DWORD nWidth)const
//{
//	DWORD dwResult=-1;
//	__m64 v_compare_result_buf[8];
//	__m64* pDCurBlock;
//	__m64* pDPrevBlock;
//	u_row_m64 n_sum;
//	n_sum.n_m64_data=_mm_setzero_si64();
//
//	for(size_t n=0; n<CELL_SIZE_8x8;n++)
//	{
//		pDCurBlock=reinterpret_cast<__m64*>(&pCurrentBlock[nWidth*n]);
//		pDPrevBlock=reinterpret_cast<__m64*>(&pPrevBlock[nWidth*n]);
//		v_compare_result_buf[n]=_mm_sad_pu8((__m64)*pDCurBlock,(__m64)*pDPrevBlock);
//		n_sum.n_m64_data=_mm_add_pi8(n_sum.n_m64_data,v_compare_result_buf[n]);
//	}
//	memcpy(&dwResult,&n_sum.n_bytes_data[0],sizeof(dwResult));
//	return dwResult>>6;
//}



CME_TSS::CME_TSS()
//:m_pFrameAllocator(NULL)
{
	
}
CME_TSS::~CME_TSS()
{
	//printf( "CME_TSS: destructor\n");
}
//void CME_TSS::SetTaskDescriptor(const TaskDescriptor& rtask_descr)
//{
//	m_task_descr=rtask_descr;
//	m_task_descr.sVideoFrameInfo.nHight+=6;
//}
//BOOL CME_TSS::SetFrameAllocator(const CFrameAllocator* pFrameAllocator)
//{
//	if(NULL==pFrameAllocator) return FALSE;
//	m_pFrameAllocator=const_cast<CFrameAllocator*>(pFrameAllocator);
//	return TRUE;
//}
/*
create_start_pointer_of_block()-
	create start pointer of the searche block in the searche array
	pCurrentFrame:			pointer to start of frame (left up corner)
	pStartBlockPosition:	start position (center position) of searche array (position number zerro)
*/
inline PBYTE CME_TSS::create_start_pointer_of_block(const PBYTE pCurrentFrame
													   ,const PBYTE pStartBlockPosition
													   ,DWORD nWidth
													   ,DWORD nNewPositionNum
													   ,DWORD nStep)const
{
	PBYTE pNewPosition;
	if(FALSE==IsPositionValid(nNewPositionNum,nStep))
		return NULL;
	switch(nNewPositionNum)
	{
	case 0:
		pNewPosition=pStartBlockPosition;
		break;
	case 1:
		pNewPosition=pStartBlockPosition-nWidth*nStep;
		break;
	case 2:
		pNewPosition=pStartBlockPosition-nWidth*nStep+nStep;
		break;
	case 3:
		pNewPosition=pStartBlockPosition+nStep;
		break;
	case 4:
		pNewPosition=pStartBlockPosition+nWidth+nStep;
		break;
	case 5:
		pNewPosition=pStartBlockPosition+nWidth;
		break;
	case 6:
		pNewPosition=pStartBlockPosition+nWidth-nStep;
		break;
	case 7:
		pNewPosition=pStartBlockPosition-nStep;
		break;
	case 8:
		pNewPosition=pStartBlockPosition-nWidth-nStep;
		break;
	}
	if(pCurrentFrame-pNewPosition<=0)// check frame boundary overflow
		return pNewPosition;
	return NULL;
}
BOOL CME_TSS::IsPositionValid(DWORD nPos,DWORD nStep) const
{
	//if(m_FramePosInfo.nRowCnt && m_FramePosInfo.nRowCnt!=m_FramePosInfo.nTatalRows)
	//	if(m_FramePosInfo.nColumnCnt && m_FramePosInfo.nColumnCnt!=m_FramePosInfo.nTotalColumns)
	//		return TRUE;
	switch(nPos)
	{
		case 2:
			if(m_FramePosInfo.nColumnCnt == m_FramePosInfo.nTotalColumns-1)
				return FALSE;
			if(!m_FramePosInfo.nColumnCnt && !m_FramePosInfo.nRowCnt)
				return FALSE;
			break;
		case 8:
			if(!m_FramePosInfo.nColumnCnt)
				return FALSE;
			if(!m_FramePosInfo.nRowCnt)
				return FALSE;
			break;
		case 1:
			if(!m_FramePosInfo.nRowCnt)
				return FALSE;
			break;
		case 6:
			if(!m_FramePosInfo.nColumnCnt)
				return FALSE;
			if(m_FramePosInfo.nRowCnt == m_FramePosInfo.nTatalRows-1)
				return FALSE;
			break;
		case 4:
			if(m_FramePosInfo.nColumnCnt == m_FramePosInfo.nTotalColumns-1)
				return FALSE;
			if(m_FramePosInfo.nRowCnt == m_FramePosInfo.nTatalRows-1)
				return FALSE;
			break;
		case 5:
			if(m_FramePosInfo.nRowCnt == m_FramePosInfo.nTatalRows-1)
				return FALSE;
			break;
		case 7:
			if(!m_FramePosInfo.nColumnCnt)
				return FALSE;
			break;
		case 3:
			if(m_FramePosInfo.nColumnCnt == m_FramePosInfo.nTotalColumns-1)
				return FALSE;
			break;
	};
	return TRUE;
}
/*
block_searche()-
...try find the etalon block (8x8) of current frame in the searche array of previous frame
	pRequiredBlock: pointer to the etalon block (MB) of current frame
	pSearchBlock:	pointer to the start point of searche block of prev. frame
		realy this pointer is a position of etalon block in prev. frame...
	nBlockType:		reserved
*/
DWORD CME_TSS::block_searche(const PBYTE pRequiredBlock,const PBYTE pSearchBlock,DWORD nBlockType)
{
	DWORD nStartDispl=0;
	DWORD dwCompare[9];
	PBYTE pSearchBlockPos;
	static int nMbCnt = 0;
	/*
		
	*/
	for(int nRingl0Pos=0; nRingl0Pos<9; nRingl0Pos++)
	{
		pSearchBlockPos=create_start_pointer_of_block(m_pPrevFrame,pSearchBlock,m_nWidth,nRingl0Pos);
		if(pSearchBlockPos)
			dwCompare[nRingl0Pos]=block_8x8_compare(pRequiredBlock,pSearchBlockPos,m_nWidth);
		else
			dwCompare[nRingl0Pos]=-1;
	}
	// find minimum diference of compare operation (nThreshold)
	//for(int n
	++nMbCnt;
	return -1;
}
/*

*/
DWORD CME_TSS::vFrame_parse(const PBYTE pCurrentFrame, const PBYTE pPrevFrame, DWORD nWidth, DWORD nHeigth)
{
	DWORD nStartDispl;
	PBYTE pSearchBuf;
	if(NULL==pPrevFrame) return -1;
	if(NULL==pCurrentFrame) return -1;
	pSearchBuf=new (nothrow) _declspec (align(16))BYTE [nWidth*nHeigth];
	//pSearchBuf=new (nothrow) BYTE [nWidth*(nHeigth+6)];
	if(NULL==pSearchBuf)
		return -1;
	///*
	//	------------------------ video frame for search begin ---
	//	row0													|
	//	row0													|	<- +3 rows (top level)
	//	row0													|
	//	------------------------------- real video frame begin --
	//	row0													|
	//		...													|
	//	row N (N==nHeigth)										|
	//	-------------------------------- real video frame end  --
	//	row N													|
	//	row N													|	<- +3 rows (bottom level)
	//	row N													|
	//	------------------------ video frame for search end  ----
	//*/
	//for(int i=0;i<3;i++)
	//{
	//	memcpy(&pSearchBuf[i*nWidth],pCurrentFrame,nWidth);
	//	memcpy(&pSearchBuf[nHeigth*nWidth+3*nWidth+i*nWidth],&pCurrentFrame[(nHeigth-1)*nWidth],nWidth);
	//}

	//memcpy(&pSearchBuf[3*nWidth],pCurrentFrame,nWidth*nHeigth);
	//nStartDispl=nWidth*3;	// 
	memcpy(pSearchBuf,pCurrentFrame,nWidth*nHeigth);
	nStartDispl=0;
	
	m_nWidth=nWidth;
	//m_nWidth=m_task_descr.sVideoFrameInfo.nWidth;
	PBYTE pFrame;
	PBYTE pPrevFrameTmp;
	m_pPrevFrame=pPrevFrame;
	//------------------------------------------------------------------------------------------------
	m_FramePosInfo.nColumnCnt = m_FramePosInfo.nRowCnt = 0;
	m_FramePosInfo.nTatalRows=nHeigth/CELL_SIZE_8x8;
	m_FramePosInfo.nTotalColumns=nWidth/CELL_SIZE_8x8;
	for(unsigned int heigth=0; heigth<nHeigth; heigth+=CELL_SIZE_8x8)
	{
		pFrame=pCurrentFrame+nWidth*heigth;
		//pPrevFrameTmp=pPrevFrameBuf.get();
		//pPrevFrameTmp+=nStartDispl+nWidth*heigth;
		//pPrevFrameTmp=&m_pPrevFrame[nStartDispl+nWidth*heigth];
		pPrevFrameTmp=m_pPrevFrame + nWidth*heigth;
		m_FramePosInfo.nColumnCnt=0;
		for(unsigned int width=0; width<nWidth; width+=CELL_SIZE_8x8)
		{
			//block_searche(&pFrame[width],&pPrevFrame[width]);
			block_searche(&pFrame[width],&pPrevFrameTmp[width]);
			m_FramePosInfo.nColumnCnt++;
		}
		m_FramePosInfo.nRowCnt++;
		
	}
	return 0;
}