
#include "stdafx.h"
#include "Filter.h"

#include <math.h>
#include <set>
#include <vector>
#include <queue>
/****************************************************************************************
http://stackoverflow.com/questions/23830097/sobel-filter-algorithm-thresholding-no-external-libs-used

****************************************************************************************/

using namespace std;

#define DATA_SIZE_TEST 4*CELL_ROW_3x3*3*CELL_ROW_3x3

#define SUM_ARRAY_W	32
#define SUM_ARRAY_H	16

void sse_optim_test()
{
	queue<__m128i*> row_map;
	_declspec (align(16))DWORD IntegralSumArray[SUM_ARRAY_H][SUM_ARRAY_W]; 
	const _declspec (align(16))WORD WordMask[] = {0xffff,0,0,0,0,0,0,0};
	_declspec (align(16)) BYTE string00[] = //{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}; //24
	{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,
	 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32};
	const __m64 zerro_byte_string = _mm_setzero_si64();
	__m64* pDataSrc0 = reinterpret_cast<__m64*>(&string00[0]);
	__m64 byte_string;
	__m64 word_string_lo,word_string_hi;	// = (__m64)*pDataSrc0;
	//zerro_byte_string = _mm_setzero_si64();
	//zerro_byte_string = _mm_xor_si64(zerro_byte_string,zerro_byte_string);

	//word_string = _mm_unpacklo_pi8(zerro_byte_string,byte_string);
	word_string_lo = _mm_unpacklo_pi8((__m64)*pDataSrc0/*byte_string*/,zerro_byte_string);
	word_string_hi = _mm_unpackhi_pi8((__m64)*pDataSrc0/*byte_string*/,zerro_byte_string);

	__m128i word_row_lo, word_row_hi, word_row;
	__m128i dword_row_lo_0,dword_row_lo_1,dword_row_hi_0,dword_row_hi_1;
	const __m128i zerro_byte_string_128 = _mm_setzero_si128();

	__m128i* pDataSrc128;

	__m128i integer_sun_tmp = _mm_setzero_si128();

	_declspec (align(16))unsigned int A_prev, D_prev;
	int h,w;
	int nLen = sizeof(__m128i)/sizeof(DWORD);
	for(h=0; h<SUM_ARRAY_H; h++)
	{    //IntegralSumArray
		__m128i *pPrevRow, *pCurRow;
		A_prev = D_prev = 0;
		pCurRow = reinterpret_cast<__m128i*>(&IntegralSumArray[h][0]);
		row_map.push(pCurRow);
		for(w=0; w<SUM_ARRAY_W/sizeof(__m128i); w++)
		{
			__m128i int_sum_prev_row;	// = _mm_setzero_si128();
			__m128i int_sum_row = _mm_setzero_si128();
			__m128i mask_word_row = _mm_setzero_si128();

			if(h && 1 < row_map.size())
			{
				pPrevRow = row_map.front();
				row_map.pop();
			}

			pDataSrc128= reinterpret_cast<__m128i*>(&string00[w*sizeof(__m128i)]);
			word_row_lo = _mm_unpacklo_epi8((__m128i)*pDataSrc128,zerro_byte_string_128);
			word_row_hi = _mm_unpackhi_epi8((__m128i)*pDataSrc128,zerro_byte_string_128);

			dword_row_lo_0 = _mm_unpacklo_epi16(word_row_lo,zerro_byte_string_128);
			dword_row_lo_1 = _mm_unpackhi_epi16(word_row_lo,zerro_byte_string_128);

			dword_row_hi_0 = _mm_unpacklo_epi16(word_row_hi,zerro_byte_string_128);
			dword_row_hi_1 = _mm_unpackhi_epi16(word_row_hi,zerro_byte_string_128);

			
			for(int dw=0; dw<4; dw++)
			{
				switch(dw)
				{
					case 0: word_row = dword_row_lo_0; break;
					case 1: word_row = dword_row_lo_1; break;
					case 2: word_row = dword_row_hi_0; break;
					case 3: word_row = dword_row_hi_1; break;
				};
				if(!h)
					int_sum_prev_row = _mm_setzero_si128();
				else
					int_sum_prev_row = *(pPrevRow++);

				for(int i=0; i<nLen; i++)
				{
					__m128i A_tmp, B_tmp, D_tmp, Sum_tmp;
					switch(i)
					{
					case 0:
						int_sum_row = _mm_add_epi32(int_sum_prev_row,word_row);		// Sum_B + x
						break;
					case 1:
						mask_word_row = _mm_setr_epi32(0xffffffff, 0, 0, 0);
						break;
					case 2:
						mask_word_row = _mm_setr_epi32(0, 0xffffffff, 0, 0);
						break;
					case 3:
						mask_word_row = _mm_setr_epi32(0, 0, 0xffffffff, 0);
						break;
					case 4:
						mask_word_row = _mm_setr_epi32(0, 0, 0, 0xffffffff);
						break;
					};
					if(i)
					{
						A_tmp = _mm_and_si128(int_sum_prev_row,mask_word_row);
						D_tmp = _mm_and_si128(int_sum_row,mask_word_row);
						Sum_tmp = _mm_sub_epi32(D_tmp,A_tmp);							// Sum_D - Sum_A
						Sum_tmp = _mm_slli_si128 (Sum_tmp,(int)4);
						int_sum_row = _mm_add_epi32(int_sum_row,Sum_tmp);
						if(nLen-1 == i)
						{
							A_prev = int_sum_prev_row.m128i_u32[3];
							D_prev = int_sum_row.m128i_u32[3];
						}
					}
					else 
					{
						Sum_tmp = _mm_setzero_si128();
						Sum_tmp.m128i_u32[0]=D_prev-A_prev;
						int_sum_row = _mm_add_epi32(int_sum_row,Sum_tmp);
					}
				}
				*pCurRow++ = int_sum_row;
			}
		}
	}
}

void test_gradinet()
{
	//============================================  test...
												return;
	//=====================================================
	char DataSrc[DATA_SIZE_TEST];
	char DataDst[DATA_SIZE_TEST];
	memset(&DataDst,0,DATA_SIZE_TEST);
	int i=0;
	for(int h=0; h<3*CELL_ROW_3x3; h++)
		for(int w=0; w<4*CELL_ROW_3x3; w++)
			DataSrc[i++]=w+h;
			//DataSrc[i]=i++;
	//CreateGradientArray(DataSrc,DataDst,4*CELL_ROW_3x3,3*CELL_ROW_3x3);
	ArrayParser(DataSrc,DataDst,4*CELL_ROW_3x3,3*CELL_ROW_3x3);
}

void GetArctgValue(/*unsigned int nDxMod, unsigned int nDyMod*/)
{
	float nDxMod = 0.0;
	float nDyMod = 0.0;
	vector<unsigned int> arctg_list;

	nDyMod=0.0;
	nDxMod=2.0;
	for(int i=0; i<256; i++)
	{
		//arctg_list.push_back(atan(nDyMod/nDxMod));
		//arctg_list.push_back(nDyMod+(50-(unsigned int)(log(nDyMod+9.0)*10)));  8-9 the best
		//arctg_list.push_back(nDyMod+(40-(unsigned int)(log(nDyMod)*10)));  
		arctg_list.push_back(nDyMod+(50-(unsigned int)(log10(nDyMod)*10)));
		nDyMod+=1.0;	//0.006;
		//nDxMod+=1.0;
	}
}
#define FULL_FILTER 1
//#define FULL_FILTER 0

#define IMAGE_IMPROVE	1
//#define IMAGE_IMPROVE	0

void CreateLaplasianArray(unsigned char* pDataSrc,unsigned char* pDataDst,int nW, int nH)
{
	//---------------
	sse_optim_test();
	//---------------
	unsigned int nGrad;
	unsigned char* pDataDstTmp;
	for(int h=0; h<nH-2; h++)
	{
		sDxCernel_3x3 DxKernel;
		pDataDstTmp =  pDataDst + (h*nW);
		
		for(int w=0; w<nW-2; w++)
		{
			int nDxRow[2];
			int nDxColume[2];
			int nDx;
			int nDy;
			//---------------------------------------------------------------
			memcpy(&DxKernel.Row3_0.dRow,pDataSrc+(h*nW+w),sizeof(DWORD));
			memcpy(&DxKernel.Row3_1.dRow,pDataSrc+((h+1)*nW+w),sizeof(DWORD));
			memcpy(&DxKernel.Row3_2.dRow,pDataSrc+((h+2)*nW+w),sizeof(DWORD));

			// -1 -1 -1 -1 8 -1 -1 -1 -1 
			nDx = 8*DxKernel.Row3_1.row[1] - (DxKernel.Row3_0.row[0]+DxKernel.Row3_0.row[1]+
				DxKernel.Row3_0.row[2]+DxKernel.Row3_2.row[0]+DxKernel.Row3_2.row[1]+
				DxKernel.Row3_2.row[2]+DxKernel.Row3_1.row[0]+DxKernel.Row3_1.row[2]);

			// 1 -2 1 -2 4 -2 1 -2 1 
			//nDx = 4*DxKernel.Row3_1.row[1]+DxKernel.Row3_0.row[0]+DxKernel.Row3_0.row[2]+
			//	DxKernel.Row3_2.row[0]+	DxKernel.Row3_2.row[2] - (2*DxKernel.Row3_0.row[1]+
			//	2*DxKernel.Row3_2.row[1]+2*DxKernel.Row3_1.row[0]+2*DxKernel.Row3_1.row[2]);

			// 0 -1 0 -1 4 -1 0 -1 0
			//nDx = 4*DxKernel.Row3_1.row[1] - (DxKernel.Row3_0.row[1]+
			//	DxKernel.Row3_2.row[1]+DxKernel.Row3_1.row[0]+DxKernel.Row3_1.row[2]);
	
			//unsigned int nDxMod, nDyMod;
			////nDxMod = abs(nDx);
			//if(nDx<0)
			//	nDxMod = DxKernel.Row3_1.row[1];
			//else
			//	nDxMod = nDx;
			//if(255<nDxMod)
			//	nDxMod=255;
			//nDxMod = abs((int)(nDxMod-128));
			//pDataDstTmp[w+1]=(unsigned char)(nDxMod);

			//if (nDx < min) min = pixel_value;
			//if (nDx > max) max = pixel_value;

			if(255<nDx)
			{
				nDx = 255;
			}else if(-255 > nDx)
				nDx = -255;
			pDataDstTmp[w+1]=(unsigned char)(abs(nDx));
			//pDataDstTmp[w+1]=(unsigned char)(nDx);
		}
	}
}

void CreateGradientArray(char* pDataSrc,char* pDataDst,int nW, int nH)
{
	unsigned int nGrad;
	char gradient;
	char cell[KERNEL_SIZE][KERNEL_SIZE+1];
	bool bNegative=false;
	int i=0;
	char* pDataDstTmp;
	short val;
	//---------------
	//GetArctgValue();
	//---------------
	for(int h=0; h<nH-2; h++)
	{
		sDxCernel_3x3 DxKernel;
		pDataDstTmp =  pDataDst + (h*nW);
		
		for(int w=0; w<nW-2; w++)
		{
			int nDxRow[2];
			int nDxColume[2];
			int nDx;
			int nDy;
			//---------------------------------------------------------------
			memcpy(&DxKernel.Row3_0.dRow,pDataSrc+(h*nW+w),sizeof(DWORD));
			memcpy(&DxKernel.Row3_1.dRow,pDataSrc+((h+1)*nW+w),sizeof(DWORD));
			memcpy(&DxKernel.Row3_2.dRow,pDataSrc+((h+2)*nW+w),sizeof(DWORD));

			memcpy(&cell[0][0],pDataSrc+(h*nW+w),sizeof(DWORD));
			memcpy(&cell[1][0],pDataSrc+((h+1)*nW+w),sizeof(DWORD));
			memcpy(&cell[2][0],pDataSrc+((h+2)*nW+w),sizeof(DWORD));

			if(FULL_FILTER)
			{

			nDx = DxKernel.Row3_0.row[2] + 2*DxKernel.Row3_1.row[2] + DxKernel.Row3_2.row[2] - 
				(DxKernel.Row3_0.row[0] + 2*DxKernel.Row3_1.row[0] + DxKernel.Row3_2.row[0]);
			nDy = DxKernel.Row3_2.row[0] + 2*DxKernel.Row3_2.row[1] + DxKernel.Row3_2.row[2] - 
				(DxKernel.Row3_0.row[0] + 2*DxKernel.Row3_0.row[1] + DxKernel.Row3_0.row[2]);
				//-----------------------------------------------------
				//short x_weight=0;
				//short y_weight=0;
				//nDx=0;
				//nDy=0;
				//char ans;
				//for (short i=0; i<KERNEL_SIZE; i++)
				//{
				//	for(short j=0; j<KERNEL_SIZE; j++)
				//	{
				//		//nDx+=cell[i][j]*dx_mask[i][j];
				//		//nDy+=cell[i][j]*dy_mask[i][j];

				//		x_weight+=cell[i][j]*dx_mask[i][j];
				//		y_weight+=cell[i][j]*dy_mask[i][j];
				//	}
				//}
				//val=abs(x_weight)+abs(y_weight);
				//nDx = x_weight;
				//nDy = y_weight;
			}
			else
			{
			// filter 2
			//// dx
			//nDxRow[0] = DxKernel.Row3_1.row[0] *(-1) /*+ DxKernel.Row3_1.row[1] *(-2)*/ + DxKernel.Row3_1.row[2] *(1);
			//nDx = nDxRow[0];//+nDxRow[1];	//abs(nDxRow[0]+nDxRow[1]);
			//// dy
			//
			//nDxColume[0] = DxKernel.Row3_0.row[1]*(1) /*+ DxKernel.Row3_1.row[1]*(-2)*/ + DxKernel.Row3_2.row[1]*(-1);
			//nDy = nDxColume[0];	//+nDxColume[1];

			nDx = DxKernel.Row3_1.row[2] - DxKernel.Row3_1.row[0];
			nDy = DxKernel.Row3_0.row[1] - DxKernel.Row3_2.row[1];
			}

			//=== corection...

			// save...
			//if(0>nDx)
			//	bNegative=true;
			//if(0>nDy)
			//	bNegative=true;

			//nGrad = (abs(nDx) + abs(nDy))/4;
			//nGrad = abs (nDx + nDy);

			unsigned int nDxMod, nDyMod;
			nDxMod = abs(nDx);
			nDyMod = abs(nDy);
			nGrad = nDxMod+nDyMod;
			//nGrad = abs(nDx) + abs(nDy);

			//if( 30 < nGrad) nGrad = 255;
			//if (100 > nGrad) nGrad = 0;
			//float fArctn ;
			//if(nDyMod && nDxMod)
			//{
			//	fArctn = atan((float)(nDyMod/nDxMod));
			//	if(0.0 == fArctn)
			//	{
			//		static int nZerro = 0;
			//		bNegative=true;
			//		++nZerro;
			//	}
			//	else
			//	{
			//		static int nNoZerro = 0;
			//		++nNoZerro;
			//	}
			//}
			//-------------------------------------------------------------------
			if(IMAGE_IMPROVE)
			{
			if(200 > nGrad && nGrad > 10)
			//if(200 > nGrad)
			{
			//int Nn = 50-(unsigned int)(log(nGrad)*5);
			//int Nn = nGrad+(50-(unsigned int)(log(nGrad+10.0)*10));
			//int Nn = nGrad+(60-(unsigned int)(log(nGrad)*10));
			//int Nn = nGrad+(50-(unsigned int)(log10(nGrad)*10));
			int Nn = nGrad+(60-(unsigned int)(log10(nGrad)*10));
				nGrad=nGrad+Nn;
			}
			}


			if( 255 < nGrad) nGrad = 255;

			//gradient = 255 - (unsigned char)(nGrad);	
			gradient =(char)(nGrad);	

			pDataDstTmp[w+1] = gradient;	//abs(nDx) + abs(nDy);	//abs(nDx+nDy);	//nDx;
		}
	}
}



void ArrayParser(char* pDataSrc,char* pDataDst,int nW, int nH)
{
	//sobel( pDataSrc, nW, nH, pDataDst);
	//return;

	char* pData;
	char* pDataDstTmp;
	char cTmp;
	char window[KERNEL_SIZE][KERNEL_SIZE];
	for(int h=0; h<nH-2; h++)
	{
		pDataDstTmp =  pDataDst + h*nW;
		pData = pDataSrc + h*nW;
		for(int w=0; w<nW-2; w++)
		{
			//pData = pDataSrc+w;
			cTmp = *pData;
			window[0][0] = *pData;
			window[0][1] = *(pData+1);
			window[0][2] = *(pData+2);

			window[1][0] = *(pData+nW);
			window[1][1] = *(pData+1+nW);
			window[1][2] = *(pData+2+nW);

			window[2][0] = *(pData+2*nW);
			window[2][1] = *(pData+1+2*nW);
			window[2][2] = *(pData+2+2*nW);
			char grad = sobel_op(window);
			*pDataDstTmp = grad;
			pDataDstTmp++;
			pData++;
		}
	}
}
char sobel_op(char window[KERNEL_SIZE][KERNEL_SIZE])
{
    const char x_op[KERNEL_SIZE][KERNEL_SIZE] = { {-1,0,1},
                {-2,0,2},
                {-1,0,1}};

    const char y_op[KERNEL_SIZE][KERNEL_SIZE] = { {1,2,1},
                {0,0,0},
                {-1,-2,-1}};
    short x_weight=0;
    short y_weight=0;
    char ans;
    for (short i=0; i<KERNEL_SIZE; i++){
        for(short j=0; j<KERNEL_SIZE; j++){
            x_weight+=window[i][j]*x_op[i][j];
            y_weight+=window[i][j]*y_op[i][j];
        }
    }
    short val=abs(x_weight)+abs(y_weight);
    //make sure the pixel value is between 0 and 255 and add thresholds
    if(val>200)
        val=255;
    else if(val<100)
        val=0;
    ans=255-(unsigned char)(val);
    return ans;

}

//void sobel( int *a, int sx, int sy, int *out)
void sobel( char *a, int sx, int sy, char *out)
{
// (In) a: The image (row major) containing elements of int.
// (In) sx: size of image along x-dimension.
// (In) sy: size of image along y-dimension.
// (Out) out: The output image with dimensions (sx-2, sy-2).

int i, j, ctr = 0;
char *p1, *p2, *p3;
int result;
p1 = a; p2 = p1+sx; p3 = p2+sx;

	for( j = 0; j < sy-2; ++j )
	{
		for( i = 0; i < sx-2; ++i )
		{
			/*out[ctr++]*/ result = ( abs(( p1[0] + 2*p1[1] + p1[2]) 
				- (p3[0] + 2*p3[1]+ p3[2])) 
				+ abs((p1[2] + 2*p2[2] + p3[2]) - (p1[0] + 2*p2[0] + p3[0])) )/6;
			++p1; ++p2; ++p3;

			//if(result > 255)
			//	result = 255;
			out[ctr++]  = result;
		}
		//p1 += 2; p2 += 2; p3 +=2;
	}
}

//#define matrix(a,b,c) a[(b)*(cols)+(c)]
//void sobel(unsigned char *data, float *output, long rows, long cols)
//{
//int r, c;
//int gx, gy;
//	for ( r = 1; r < rows-1; r++ ) {
//		for ( c = 1; c < cols-1; c++ ) {
//			gx = -matrix(data,r-1,c-1) + matrix(data,r-1,c+1) +
//				-2*matrix(data,r,c-1) + 2*matrix(data,r,c+1) +
//				-matrix(data,r+1,c-1) + matrix(data,r+1,c+1);
//			gy = -matrix(data,r-1,c-1) - 2*matrix(data,r-1,c)
//				- matrix(data,r-1,c+1) +
//				matrix(data,r+1,c-1) + 2*matrix(data,r+1,c)
//				+ matrix(data,r+1,c+1);
//			matrix(output,r,c) = sqrt((float)(gx)*(float)(gx)+(float)(gy)*(float)(gy));
//}
//}