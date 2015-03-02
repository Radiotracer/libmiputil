/*
	miputil.c
	
	$Id: miputil.c 101 2009-03-03 15:00:38Z frey $
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef MALLOC_H_NEEDED
#include <malloc.h>
#endif
#include "miputil.h"
#include "errdefs.h"
#include "printmsg.h"

float dot(float *vec1, float *vec2, int length)
{
  int     i;
  float dotProduct = 0.0;
  
  for (i=0; i<length; i++)
    dotProduct += vec1[i]*vec2[i];
  
  return(dotProduct);
}

void set_float(float *array, int number, float constant)
{
  int   i;
  for (i=0; i<number; i++)
    array[i] = constant;
}

float sum_float(float *array, int number)
{
  int   i;
  double sum = 0.0;
  for (i=0; i<number; i++)
    sum += array[i];
  return((float)sum);
}

float sum_short(short *array, int number)
{
  int   i;
  double sum = 0.0;
  for (i=0; i<number; i++)
    sum += (float)array[i];
  return((float)sum);
}

void scale_float(float *array, int number, float fac)
{
	int   i;
	for (i=0; i<number; i++)
		array[i] *= fac;
}

float *pfDupVec(float *pfVec, int iVecLen, char *pchName)
{
	float *pfOutVec;

	pfOutVec=vector(iVecLen,pchName);
	memcpy(pfOutVec, pfVec, iVecLen*sizeof(float));
	return pfOutVec;
}
	
void set_int(int *array, int number, int constant)
{  
  int   i;
  for (i=0; i<number; i++)
    array[i] = constant;
}

int sum_int(int *array, int number)
{
  int	i;
  int	sum = 0;
  for (i=0; i<number; i++)
    sum += array[i];
  return(sum);
}

void vZeroNeg(float *array, int number)
{
	int i;
	for(i=0; i<number; i++)
		if (array[i] < 0)
			array[i] = 0;
}

void set_double(double *array, int number, double constant)
{
  int   i;
  for (i=0; i<number; i++)
    array[i] = constant;
}

double sum_double(double *array, int number)
{
  int   i;
  double sum = 0.0;
  for (i=0; i<number; i++)
    sum += array[i];
  return(sum);
}

double *dvector(int iLen, char *pchPointername)
     /* allocates a double precision vector w/length iLen */
{
  return((double *)pvIrlMalloc(sizeof(double)*iLen, pchPointername));
}


void free_dvector(double *pdVec)
{
  if (pdVec != NULL) IrlFree(pdVec);
}

float *vector(int iLen, char *pchPointername)
     /* allocates a single precision vector w/length iLen */
{
  return((float *)pvIrlMalloc(sizeof(float)*iLen, pchPointername));
}

void free_vector(float *pfVec)
{
  if (pfVec != NULL) IrlFree(pfVec);
}

int *ivector(int iLen, char *pchPointername)
     /* allocates an integer vector w/length iLen */
{
  return((int *)pvIrlMalloc(sizeof(int)*iLen, pchPointername));
}

void free_ivector(int *piVec)
{
  if (piVec != NULL) IrlFree(piVec);
}

	
void reorder(int zdim, int ydim, int xdim, float *Matrix, float *TempMatrix){
/* reorders image by swapping y and z dimensions */
  int i,j,k;
  for(j=0;j<ydim;j++)
    for(k=0;k<zdim;k++)
      for(i=0;i<xdim;i++)
        TempMatrix[j*zdim*xdim +k*xdim + i] = Matrix[k*ydim*xdim + j*xdim + i];
  return;
}

void irl_abort(char *pchRoutine, char *pchErrMsg)
{
	vErrorHandler(ECLASS_FATAL, ETYPE_UNKNOWN, pchRoutine, pchErrMsg);
}


char *pchStripExt(char *pchName, char *pchExt)
/* looks for pchExt at the end of pchName and removes it if it is found.
   This is done in place*/
{
	size_t iExtLen, iNameLen;
	int i;
	char *pchE, *pchN;

	iExtLen = strlen(pchExt);
	iNameLen = strlen(pchName);

	pchE = pchExt+iExtLen-1;
	pchN = pchName+iNameLen-1;

	i=0;
	while(pchE >= pchExt && pchN >= pchName && *pchN-- == *pchE--)
		i++;
	if (i == iExtLen)
		*(pchN+1) = '\0';
	return(pchName);
}

char *pchStripDirname(char *pchName)
/* searches from end of string and returns the first character after the
	first '/' */
{
	char *pch;

	for(pch=pchName+strlen(pchName); pch >= pchName && *pch != '/'; --pch)
		;
	return pch+1;
}

char *pchGetDir(char *pchName)
/* returns a pointer to the directory portion to pchName, NULL if there
 * is no directory part.*/
{
	char *pch, *pchDir;
	size_t len;
	/* search backwards from end for  the first / */
	for(pch=pchName+strlen(pchName); pch >= pchName && *pch != '/'; --pch)
		;
	len = (size_t)pch - (size_t)pchName + 1;
	pchDir = pvIrlMalloc((unsigned int)len+1, "GetDir");
	memcpy(pchDir, pchName, len);
	pchDir[len] = '\0';
	return(pchDir);
}

#ifndef WIN32
#include <sys/times.h>
#include <sys/types.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>
void PrintTimes(char *pchMsg)
/*prints string in pchMsg followed by user, system and clock time
  since last call and user, system and clock time since the first call
*/
{
	static float fSecsPerTick;
	static time_t iLastUtime, iLastStime, iLastCtime;
	static time_t iStartUtime, iStartStime, iStartCtime;
	struct tms sTimes;
	time_t iCrntCtime;
	static int first=1;

	if (first){
		first=0;
		fSecsPerTick = 1.0/((float)sysconf(_SC_CLK_TCK));
		times(&sTimes);
		iStartUtime = sTimes.tms_utime;
		iStartStime = sTimes.tms_stime;
		time(&iStartCtime);
		iCrntCtime=iStartCtime;
	}else{
		times(&sTimes);
		time(&iCrntCtime);
		vPrintMsg(3,
			"%s: dif time (u=%.2f s=%.2f c=%d) tot time(u=%.2f s=%.2f c=%d)\n",
			pchMsg,
			-(iLastUtime - sTimes.tms_utime)*fSecsPerTick,
			-(iLastStime - sTimes.tms_stime)*fSecsPerTick,
			-(iLastCtime - iCrntCtime),
			-(iStartUtime - sTimes.tms_utime)*fSecsPerTick,
			-(iStartStime - sTimes.tms_stime)*fSecsPerTick,
			-(iStartCtime - iCrntCtime));
	}
	iLastUtime = sTimes.tms_utime;
	iLastStime = sTimes.tms_stime;
	iLastCtime = iCrntCtime;
}
#else /* not win32 */
/* with windows, PrintTimes does nothing */
void PrintTimes(char *pchMsg)
{
}
#endif 
#define SWAP(a,b,tmp) (tmp)=(a); (a)=(b); (b)=(tmp)
void vSwapWords(void *pvIn, int iNum)
/* swaps the bytes of a 4 byte item to convert between little and bigendian
	byte orders */
{
	char c;
	char *Buf;
	int i;

	Buf = (char *)pvIn;

	for(i=0; i < iNum; ++i, Buf += 4){
		SWAP(Buf[0],Buf[3],c);
		SWAP(Buf[1],Buf[2],c);
	}
}
#undef SWAP

short* RealToShort(float *pfVals, int numpixels, 
		float fOffset, float fScaleFac)
{
	short *psVals;
	int i;

	fprintf(stderr,"RealToShort %d pixels: scale=%f, offset=%f\n",
			numpixels, fScaleFac, fOffset);
	psVals = pvIrlMalloc(numpixels*sizeof(short),"RealToShort:Vals");
	for(i=0; i<numpixels; ++i)
			psVals[i] = (short)((pfVals[i] - fOffset)*fScaleFac);
	fprintf(stderr,"     float sum=%.4g, short sum=%.0f\n",
			sum_float(pfVals, numpixels), sum_short(psVals, numpixels));
	return psVals;
}
