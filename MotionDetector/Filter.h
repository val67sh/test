
#ifndef _FILTER_H_
#define _FILTER_H_

#include <memory>
#include <intrin.h>
#include <tmmintrin.h>
#include <emmintrin.h>
#include <mmintrin.h>
#include <immintrin.h>
#include <ammintrin.h>

#define CELL_ROW_3x3	3
#define KERNEL_SIZE	3

/********************************************************************************
 Integral Image computation:

 Integral Image array (DWORD)	monochrome Image (BYTE)	
 y y y A B y y					d0  d1  d2  d3  d4  d5  d6
 y y y D X - -					d7  d8  d9  d10 d11 d12 d13
 - - - - - - -					d14 d15 d16 d17 d18 d19 d20
 sum X = B + D - A + d11
 if used _mm_add_epi32: B + d11, than sum X = _mm_add_epi32 + D - A

 bytes into words
 __m64 _mm_unpacklo_pi8 (__m64 m1, __m64 m2) / __m128i _mm_unpacklo_epi8 (__m128i m1, __m128i m2)
 __m64 _mm_unpackhi_pi8 (__m64 m1, __m64 m2) / __m128i _mm_unpackhi_epi8 (__m128i m1, __m128i m2)
 example code:
 __m64 _mm_unpacklo_pi8 (__m64 bytes_string, __m64 zerro_byte_string)

********************************************************************************/

typedef union uROW_3_
{
	unsigned char row[CELL_ROW_3x3];
	//char x_0; 
	//char x_1;
	//char x_2;
	DWORD dRow;
}uROW_3;

typedef union uCOLUM_3_
{
	char x_0; 
	char x_1;
	char x_2;
	DWORD dRow;
}uCOLUM_3;

typedef struct sDxCernel_3x3_
{
	uROW_3 Row3_0;
	uROW_3 Row3_1;
	uROW_3 Row3_2;
}sDxCernel_3x3;


typedef struct sDyCernel_3x3_
{
	uROW_3 row3_0;
	uROW_3 row3_1;
	uROW_3 row3_2;
}sDyCernel_3x3;

//const char dx_mask[KERNEL_SIZE][KERNEL_SIZE] = { {-1,0,1},{-2,0,2},{-1,0,1}};
//const char dy_mask[KERNEL_SIZE][KERNEL_SIZE] = { {1,2,1},{0,0,0},{-1,-2,-1}};

const char dx_mask[KERNEL_SIZE][KERNEL_SIZE+1] = { {-1,0,1,0},{-2,0,2,0},{-1,0,1,0}};

const char dy_mask[KERNEL_SIZE][KERNEL_SIZE+1] = { {1,2,1,0},{0,0,0,0},{-1,-2,-1,0}};


void sse_optim_test();
void test_gradinet();
void CreateGradientArray(char* pDataSrc,char* pDataDst,int nW, int nH);
void CreateLaplasianArray(unsigned char* pDataSrc,unsigned char* pDataDst,int nW, int nH);
void ArrayParser(char* pDataSrc,char* pDataDst,int nW, int nH);
char sobel_op(char window[KERNEL_SIZE][KERNEL_SIZE]);

void sobel( char *a, int sx, int sy, char *out);

//*********************************************************************************************************
			//// dx
			//nDxRow[0] = DxKernel.Row3_0.row[0] *(-1) + DxKernel.Row3_0.row[1] *(-2) + DxKernel.Row3_0.row[2] *(-1);
			//nDxRow[1] = DxKernel.Row3_2.row[0]*(1) + DxKernel.Row3_2.row[1] *(2) + DxKernel.Row3_2.row[2]*(1);
			//nDx = nDxRow[0]+nDxRow[1];	//abs(nDxRow[0]+nDxRow[1]);
			//// dy
			//nDxColume[0] = DxKernel.Row3_0.row[0]*(-1) + DxKernel.Row3_1.row[0]*(-2) + DxKernel.Row3_2.row[0]*(-1);
			//nDxColume[1] = DxKernel.Row3_0.row[2]*(1) + DxKernel.Row3_1.row[2]*(2) + DxKernel.Row3_2.row[2]*(1);
			//nDy = nDxColume[0]+nDxColume[1];
				//----------------------------------------------------
			//// dx
			//nDxRow[0] = DxKernel.Row3_0.row[0] *(-1) + DxKernel.Row3_1.row[0] *(-2) + DxKernel.Row3_2.row[0] *(-1);
			//nDxRow[1] = DxKernel.Row3_0.row[2]*(1) + DxKernel.Row3_1.row[2] *(2) + DxKernel.Row3_2.row[2]*(1);
			//nDx = nDxRow[0]+nDxRow[1];	
			//// dy
			//nDxColume[0] = DxKernel.Row3_0.row[0]*(-1) + DxKernel.Row3_0.row[1]*(-2) + DxKernel.Row3_0.row[2]*(-1);
			//nDxColume[1] = DxKernel.Row3_2.row[0]*(1) + DxKernel.Row3_2.row[1]*(2) + DxKernel.Row3_2.row[2]*(1);
			//nDy = nDxColume[0]+nDxColume[1];
				//------------------------------------ [] -------------
			//nDx = DxKernel.Row3_0.row[2] + 2*DxKernel.Row3_1.row[2] + DxKernel.Row3_2.row[2] - 
			//	(DxKernel.Row3_0.row[0] + 2*DxKernel.Row3_1.row[0] + DxKernel.Row3_2.row[0]);
			//nDy = DxKernel.Row3_0.row[0] + 2*DxKernel.Row3_0.row[1] + DxKernel.Row3_0.row[2] - 
			//	(DxKernel.Row3_2.row[0] + 2*DxKernel.Row3_2.row[1] + DxKernel.Row3_2.row[2]);
				//-----------------------------------------------------

//					!!! ATTENTION !!!
///* laplacian.c */
//#include <stdio.h>
//#include <stdlib.h>
//#include <float.h>
//#include "mypgm.h"
//
//void laplacian_filtering( )
//     /* Spatial filtering of image data */
//     /* 8-neighbor Laplacian filter */
//     /* Input: image1[y][x] ---- Outout: image2[y][x] */
//{
//  /* Definition of 8-neighbor Laplacian filter */
//  int weight[3][3] = {{ 1,  1,  1 },
//		      { 1,  -8,  1 },
//		      { 1,  1,  1 }};
//  double pixel_value;
//  double min, max;
//  int x, y, i, j;  /* Loop variable */
//  
//  /* Maximum values calculation after filtering*/
//  printf("Now, filtering of input image is performed\n\n");
//  min = DBL_MAX;
//  max = -DBL_MAX;
//  for (y = 1; y < y_size1 - 1; y++) {
//    for (x = 1; x < x_size1 - 1; x++) {
//      pixel_value = 0.0;
//      for (j = - 1; j < 2; j++) {
//	    for (i = -1; i < 2; i++) {
//	      pixel_value += weight[j + 1][i + 1] * image1[y + j][x + i];
//	    }
//      }
//      if (pixel_value < min) min = pixel_value;
//      if (pixel_value > max) max = pixel_value;
//    }
//  }
//  if ((int)(max - min) == 0) {
//    printf("Nothing exists!!!\n\n");
//    exit(1);
//  }
//
//  /* Initialization of image2[y][x] */
//  x_size2 = x_size1;
//  y_size2 = y_size1;
//  for (y = 0; y < y_size2; y++) {
//    for (x = 0; x < x_size2; x++) {
//      image2[y][x] = 0;
//    }
//  }
//  
//  /* Generation of image2 after linear transformtion */
//  for (y = 1; y < y_size1 - 1; y++) {
//    for (x = 1; x < x_size1 - 1; x++) {
//      pixel_value = 0.0;
//      for (j = - 1; j < 2; j++) {
//	    for (i = -1; i < 2; i++) {
//	      pixel_value += weight[j + 1][i + 1] * image1[y + j][x + i];
//	    }
//      }
//      pixel_value = MAX_BRIGHTNESS * (pixel_value - min) / (max - min);
//      image2[y][x] = (unsigned char)pixel_value;
//    }
//  }
//}
//
//main( )
//{
//  load_image_data( );     /* Input of image1 */ 
//  laplacian_filtering( ); /* Laplacian filtering is applied to image1 */
//  save_image_data( );     /* Output of image2 */
//  return 0;
//}
//
//  http://www.codeproject.com/Articles/37299/Implementation-of-Laplacian-of-Gaussion-Edge-Detec
//*********************************************************************************************************

#endif	//_FILTER_H_