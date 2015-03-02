/*
	miputil.h
	
	$Id: miputil.h 101 2009-03-03 15:00:38Z frey $
*/

#ifndef MIPUTIL_H
#define MIPUTIL_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#if MALLOC_H_NEEDED
#include <malloc.h>
#endif

#include "mipmalloc.h"

#define NR_END 1
#define FREE_ARG char*

/* Prototypes */

#ifdef __cplusplus
extern "C" {
#endif

float dot(float *vec1, float *vec2, int length);
void set_float(float *array, int number, float constant);
void set_int(int *array, int number, int constant);
void set_double(double *array, int number, double constant);
double sum_double(double *array, int number);
float sum_float(float *array, int number);
void scale_float(float *array, int number, float fac);
int sum_int(int *array, int number);
float sum_short(short *array, int number);
int *ivector(int, char* pointername);
float *vector(int, char* pointername);
double *dvector(int, char* pointername);
void free_ivector(int *);
void free_vector(float *);
void free_dvector(double *);
float *pfDupVec(float *pfVec, int iVecLen, char *pchName);
void reorder(int zdim, int ydim, int xdim, float *Matrix, float *TempMatrix);
void irl_abort(char *pcRoutine, char *pcErrMsg);
double *pdLsfToPsf(int iNumPix, int iNumLsfPix, double *pdLsf,
	double DownSizeFac);
char *pchStripExt(char *pchName, char *pchExt);
char *pchStripDirname(char *pchName);
char *pchGetDir(char *pchName);
void PrintTimes(char *pchMsg);
void vSwapWords(void *pvIn, int iNum);
short* RealToShort(float *pfVals, int numpixels,float fOffset,
	float fScaleFact);
void vZeroNeg(float *array, int number);

#ifdef __cplusplus
}
#endif

#endif /* MIPUTIL_H */
