
#ifndef _DATA_PROCESSING_H_
#define _DATA_PROCESSING_H_

#include <memory>
#include <intrin.h>
#include <tmmintrin.h>
#include <emmintrin.h>
#include <mmintrin.h>
#include <immintrin.h>
#include <ammintrin.h>

#define CELL_SIZE_8x8		8L
#define CELL_SIZE_16x16		16L

typedef union _u_row_m64
{
	__m64 n_m64_data;
	DWORD n_dword_data[2];
	WORD n_word_data[4];
	BYTE n_bytes_data[8];
}u_row_m64;

typedef union _u_row_m128
{
	__m128i n_m128_data;
	__m64 n_m64_data[2];
	DWORD n_dword_data[4];
	WORD n_word_data[8];
	BYTE n_bytes_data[16];
}u_row_m128;

void Init();
void SetThreshold_block16x16(__m128i* pBlock, BYTE nThreshold);
__m128i* GetThreshold_block16x16();
DWORD block_8x8_compare(const PBYTE pCurrentBlock, const PBYTE pPrevBlock, DWORD nWidth);

DWORD block_1x16x16_compare(const PBYTE pCurrentBlock, const PBYTE pPrevBlock, DWORD nWidth);

DWORD block_1x16x16_get_active_pixels(const PBYTE pCurrentBlock, const PBYTE pPrevBlock, __m128i* pThresholdBlock, DWORD nWidth);


#endif	//_DATA_PROCESSING_H_