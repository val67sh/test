
#ifndef __ME_ALGORITHMS_H__
#define __ME_ALGORITHMS_H__

#include <memory>
#include <intrin.h>

#include "DataProcessing.h"

//#include "VideoFrameCtrl.h"

using namespace std;
#define CELL_SIZE_8x8	8L

//typedef union _u_row_m64
//{
//	__m64 n_m64_data;
//	BYTE n_bytes_data[8];
//}u_row_m64;
typedef struct _frame_positions_descr
{
	DWORD nRowCnt;
	DWORD nColumnCnt;
	DWORD nTatalRows;
	DWORD nTotalColumns;
}frame_positions_descr;

typedef struct _motion_vector
{
	int x;
	int y;
}motion_vector;

typedef struct MackroBlockInfo_
{
	int nSliseNum;
	int mMackroBlockNum;
	motion_vector nMVector;	// top of vector: 
}MackroBlockInfo;
/*
class motion estimation algorithms...
*/
class CMEAlgorithms
{
public:
	CMEAlgorithms();
	virtual ~CMEAlgorithms();
	
//protected:

public:
	virtual DWORD vFrame_parse(const PBYTE pCurrentFrame, const PBYTE pPrevFrame, DWORD nWidth, DWORD nHeigth) = 0;
protected:
	//inline DWORD block_8x8_compare(const PBYTE pCurrentBlock, const PBYTE pPrevBlock, DWORD nWidth)const;
	virtual DWORD block_searche(const PBYTE pRequiredBlock,const PBYTE pSearchBlock,DWORD nBlockType=0)=0;
	//virtual BOOL SetFrameAllocator(const CFrameAllocator* pFrameAllocator)=0;
	//virtual void SetTaskDescriptor(const TaskDescriptor& rtask_descr)=0;
	motion_vector CreateMotionVctor(const PBYTE pStartPos, const PBYTE pEndPos, int nRowLen);

	frame_positions_descr m_FramePosInfo;
};


class CME_TSS: public CMEAlgorithms
{
public:
	CME_TSS();
	~CME_TSS();
	inline PBYTE create_start_pointer_of_block(const PBYTE pCurrentFrame,const PBYTE pStartBlockPosition,DWORD nWidth, DWORD nNewPositionNum,DWORD nStep=2)const;
	
	
public:
	virtual DWORD vFrame_parse(const PBYTE pCurrentFrame, const PBYTE pPrevFrame, DWORD nWidth, DWORD nHeigth);
	virtual DWORD block_searche(const PBYTE pRequiredBlock,const PBYTE pSearchBlock,DWORD nBlockType=0);
	//virtual BOOL SetFrameAllocator(const CFrameAllocator* pFrameAllocator);
	//virtual void SetTaskDescriptor(const TaskDescriptor& rtask_descr);
private:
	BOOL IsPositionValid(DWORD nPos,DWORD nStep) const;
protected:
	PBYTE m_pCurrentFrame;
	PBYTE m_pPrevFrame;
	DWORD m_nWidth;
	DWORD m_nHeigth;
	//CFrameAllocator* m_pFrameAllocator;
	//TaskDescriptor m_task_descr;
};

#endif