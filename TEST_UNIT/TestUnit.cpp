
#include "../MotionDetector/stdafx.h"
#include <IntSafe.h>
#include <windows.h>

#include "TestUnit.h"
#include "../MotionDetector/DataProcessing.h"

#define Wblock	16L
#define Hblock	Wblock

void MemoryBlocksCompare_test()
{
	DWORD dwResult;
	DWORD dwThreshold=15;
	BOOL bIsActive;
/*
	Attention!!! All memory blocks are must be aligned on 16-byte boundaries.
*/
	_declspec (align(16))BYTE SrcMemBlock[Wblock][Hblock];
	_declspec (align(16))BYTE DestMemBlock[Wblock][Hblock];

	// blocks init...
	for(int h=0; h<Hblock; h++)
	{
		for(int w=0; w<Wblock; w++)
		{
			SrcMemBlock[w][h]=50;//0xff;//2;
			DestMemBlock[w][h]=2;//0xff;
		}
	}

	SrcMemBlock[10][0]=0;
	SrcMemBlock[5][5]=2;
	SrcMemBlock[0][5]=8;
	SrcMemBlock[9][5]=8;
	SrcMemBlock[14][5]=8;
	// compare of blocks...
	Init();
	PBYTE pCurrBlock=&SrcMemBlock[0][0];
	PBYTE pPrevBlock=&DestMemBlock[0][0];

	//dwResult=block_1x16x16_get_active_pixels(pCurrBlock,pPrevBlock,GetThreshold_block16x16(),Wblock);
	//block_1x16x16_compare(pCurrBlock,pPrevBlock,Wblock);

	dwResult=block_8x8_compare(pCurrBlock,pPrevBlock,Wblock);
	bIsActive= dwThreshold>dwResult? false:true;
}