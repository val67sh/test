
#include "stdafx.h"
#include "DataProcessing.h"

/*
	Attention!!! All memory blocks are must be aligned on 16-byte boundaries.
	Because variables of type _m128i are automatically aligned on 16-byte boundaries.


*/

__m128i ThresholdBlock16x16[CELL_SIZE_16x16];

_declspec (align(16)) u_row_m128 u_zerro_const_m128;
const __m128i zerro_const_m128=_mm_setr_epi16(0,0,0,0,0,0,0,0);

#pragma intrinsic (memcpy)
// TMP!!!!! inline DWORD block_8x8_compare(const PBYTE pCurrentBlock, const PBYTE pPrevBlock, DWORD nWidth)
DWORD block_8x8_compare(const PBYTE pCurrentBlock, const PBYTE pPrevBlock, DWORD nWidth)
{
	DWORD dwResult=-1;
	__m64 v_compare_result_buf[8];
	__m64* pDCurBlock;
	__m64* pDPrevBlock;
	u_row_m64 n_sum;
	n_sum.n_m64_data=_mm_setzero_si64();

	for(size_t n=0; n<CELL_SIZE_8x8;n++)
	{
		pDCurBlock=reinterpret_cast<__m64*>(&pCurrentBlock[nWidth*n]);
		pDPrevBlock=reinterpret_cast<__m64*>(&pPrevBlock[nWidth*n]);
		v_compare_result_buf[n]=_mm_sad_pu8((__m64)*pDCurBlock,(__m64)*pDPrevBlock);// return result in the word
		
		//n_sum.n_m64_data=_mm_add_pi8(n_sum.n_m64_data,v_compare_result_buf[n]);
		n_sum.n_m64_data=_mm_add_pi16(n_sum.n_m64_data,v_compare_result_buf[n]); 
	}
	//memcpy(&dwResult,&n_sum.n_bytes_data[0],sizeof(dwResult));
	dwResult=n_sum.n_word_data[0];
	//get average value of difference for block 8x8
	return dwResult>>6;		// :64 (8x8)
	//return dwResult;
}
/*
http://stackoverflow.com/questions/3380785/compute-the-absolute-difference-between-unsigned-integers-using-sse

int diff = abs( a - b );

Easily vectorisable (Using SSE3 as):

__m128i sseDiff = _mm_abs_epi32( _mm_sub_epi32( a, b ) );

a and b are unsigned integers. Consider a=0 and b=0xffffffff. The correct absolute difference is 0xffffffff, but your solution will give 1.
*/
inline DWORD block_16x16_compare_sum(const PBYTE pCurrentBlock, const PBYTE pPrevBlock, DWORD nWidth)
{
	DWORD dwResult=-1;
	__m128i v_compare_result_buf[8];
	__m128i* pDCurBlock;
	__m128i* pDPrevBlock;
	//u_row_m64 n_sum;
	//n_sum.n_m64_data=_mm_setzero_si64();

	for(size_t n=0; n<CELL_SIZE_8x8;n++)
	{
		pDCurBlock=reinterpret_cast<__m128i*>(&pCurrentBlock[nWidth*n]);
		pDPrevBlock=reinterpret_cast<__m128i*>(&pPrevBlock[nWidth*n]);
		v_compare_result_buf[n]=_mm_sad_epu8((__m128i)*pDCurBlock,(__m128i)*pDPrevBlock);
		//n_sum.n_m64_data=_mm_add_pi8(n_sum.n_m64_data,v_compare_result_buf[n]);
		_mm_abs_epi8((__m128i)*pDPrevBlock);
	}
	//memcpy(&dwResult,&n_sum.n_bytes_data[0],sizeof(dwResult));
	//return dwResult>>6;
	return 0;
}
/*

*/
void Init()
{
	SetThreshold_block16x16(&ThresholdBlock16x16[0],10);
	u_zerro_const_m128.n_m128_data=_mm_setr_epi16(0,0,0,0,0,0,0,0);
}
void SetThreshold_block16x16(__m128i* pBlock, BYTE nThreshold)
{
	__m128i* p_mBlock;
	//p_mBlock=reinterpret_cast<__m128i*>(&pBlock);
	p_mBlock=pBlock;
	for(size_t h=0; h<CELL_SIZE_16x16; h++)
	{
		*(p_mBlock+h)=_mm_setr_epi8(nThreshold,nThreshold,nThreshold,nThreshold,nThreshold,nThreshold,nThreshold,nThreshold
								,nThreshold,nThreshold,nThreshold,nThreshold,nThreshold,nThreshold,nThreshold,nThreshold);
	}
}

__m128i* GetThreshold_block16x16()
{
	return &ThresholdBlock16x16[0];
}
/*
	block size: 256 pixels
*/
DWORD block_1x16x16_get_active_pixels(const PBYTE pCurrentBlock, const PBYTE pPrevBlock, __m128i* pThresholdBlock, DWORD nWidth)
{
	__m128i* pDCurBlock;
	__m128i* pDPrevBlock;
	__m128i* pThreshold;
	__m128i v_compare_result_buf[CELL_SIZE_16x16];
	u_row_m128 sseSum=u_zerro_const_m128;
	int nCellCnt=0;
	DWORD dwCellTmp;
	for(size_t h=0; h<CELL_SIZE_16x16; h++)
	{
		pDCurBlock=reinterpret_cast<__m128i*>(&pCurrentBlock[nWidth*h]);
		pDPrevBlock=reinterpret_cast<__m128i*>(&pPrevBlock[nWidth*h]);
		
		// SSE2 TEST_ONLY: _mm_sub_epi8((__m128i)*pDCurBlock,(__m128i)*pDPrevBlock);

		v_compare_result_buf[h]=_mm_abs_epi8(_mm_sub_epi8((__m128i)*pDCurBlock,(__m128i)*pDPrevBlock));
		//pThreshold=&ThresholdBlock16x16[h];
		pThreshold=pThresholdBlock+h;
		v_compare_result_buf[h]=_mm_cmpgt_epi8(v_compare_result_buf[h],*pThreshold);
		sseSum.n_m128_data=_mm_sad_epu8(v_compare_result_buf[h],zerro_const_m128);
		dwCellTmp=sseSum.n_word_data[0]+sseSum.n_word_data[4];
		if(dwCellTmp)
			nCellCnt+=dwCellTmp/0xff;
	}
	return nCellCnt;
}
