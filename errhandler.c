/*
	errhandler.c
	
	$Id: errhandler.c 62 2006-01-30 16:08:31Z mjs $
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <setjmp.h>
#include <string.h>
#ifdef WIN32
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#endif

#include "errdefs.h"

/* Error Class Strings */

#define MAX_ERR_CLASS_STRING_SIZE 15
static char ErrorClassStr[MAX_ECLASS][MAX_ERR_CLASS_STRING_SIZE] =
	{"Fatal", "Warning", "Information", "Debug"};

/* Error Type Strings */

#define MAX_ERR_TYPE_STRING_SIZE 40
static char ErrorTypeStr[MAX_ETYPE][MAX_ERR_TYPE_STRING_SIZE] =
	{
		"Memory Allocation Error",
		"I/O Error",
		"",
		"",
		"Error opening file",
		"Syntax error",
		"Illegal value",
		"Usage",
	};

int bNoWarnings=0;
int bNoInfo=0;
int bExitOnFatalErrs=1;

#define ERR_BUF_LEN 256

static char pchgErrBuf[ERR_BUF_LEN+1]="";
static FILE *fpgErrLog;
static int  bgErrLogSet=0;

static jmp_buf *pgSetJmpEnv;

void vErrorHandler(int iErrClass, int iErrType, char *pchErrLocation, 
	char *pchErrFmt, ...)
{
  va_list ErrArgs;
  
  if (! bgErrLogSet){
    bgErrLogSet=1;
		fpgErrLog=stderr;
  }
  if (iErrClass > MAX_ECLASS || iErrClass <= 0){
		if (fpgErrLog != NULL)
			fprintf(fpgErrLog, "Illegal Error class: %d: Type %d: in %s\n",
				iErrClass, iErrType, pchErrLocation);
    exit(-1);
  }
  
  if (iErrType >MAX_ETYPE || iErrType < 0){
		if (fpgErrLog != NULL)
			fprintf(fpgErrLog, "Illegal Error type: %d\n",iErrType);
    exit(-1);
  }
  
  if (iErrClass == ECLASS_WARN && bNoWarnings)return;
  if (iErrClass == ECLASS_INFO && bNoInfo) return;
  
  switch (iErrType){
		case ETYPE_UNKNOWN:
			snprintf(pchgErrBuf, ERR_BUF_LEN, "Unknown: in %s:\n", pchErrLocation);
			break;
		case ETYPE_INFO:
		case ETYPE_DEBUG:
			snprintf(pchgErrBuf, ERR_BUF_LEN, 
				"%s: in %s:\n      ", ErrorClassStr[iErrClass-1], pchErrLocation);
			break;
		case ETYPE_FOPEN:
			snprintf(pchgErrBuf, ERR_BUF_LEN, "%s: in %s: %s:\n     err=%s\n     ",
				ErrorClassStr[iErrClass-1], pchErrLocation, 
				ErrorTypeStr[iErrType-1], strerror(errno));
			break;
		default:
			snprintf(pchgErrBuf, ERR_BUF_LEN, 
				"%s: in %s: %s:\n     ",
				ErrorClassStr[iErrClass-1], pchErrLocation, 
				ErrorTypeStr[iErrType-1]);
			break;
  }
  
  /* print the variable argument lists */

  va_start(ErrArgs, pchErrFmt);
  vsnprintf(pchgErrBuf + strlen(pchgErrBuf), ERR_BUF_LEN - strlen(pchgErrBuf),
		pchErrFmt, ErrArgs);
  va_end(ErrArgs);

  if (fpgErrLog != NULL){
    fputs(pchgErrBuf,fpgErrLog);
	  putc('\n',fpgErrLog);
  }

  if (iErrClass == ECLASS_FATAL){
		if (bExitOnFatalErrs){
			fprintf(stderr, "Aborted\n");
			exit(iErrType);
		}else{
			longjmp(*pgSetJmpEnv,iErrType);
    }
  }
}

char *pchErrorMsg(void)
{
	return(pchgErrBuf);
}

void vInfoOff(void)
{
	bNoInfo=1;
}

void vInfoOn(void)
{
	bNoInfo=0;
}

void vWarningsOff(void)
{
	bNoWarnings=1;
}

void vWarningsOn(void)
{
	bNoWarnings=0;
}

void vSetExitOnFatalErrs(void)
{
	bExitOnFatalErrs=1;
}

void vTrapFatalErrs(jmp_buf *pSetJmpEnv)
{
	bExitOnFatalErrs=0;
	pgSetJmpEnv=pSetJmpEnv;
}

void vSetErrLog(FILE *fp)
{
	fpgErrLog=fp;
	bgErrLogSet=1;
}
