/*
	errdefs.h
	
	$Id: errdefs.h 47 2005-04-20 19:55:49Z mjs $
*/

#ifndef ERRDEFS_H
#define ERRDEFS_H

#include <setjmp.h>

/* Prototypes */

#ifdef __cplusplus
extern "C" {
#endif

void vErrorHandler(int ErrClass, int ErrType, char *Location, char *Fmt, ...);
void vWarningsOn(void);
void vWarningsOff(void);
void vInfoOff(void);
void vInfoOn(void);
void vTrapFatalErrs(jmp_buf *jmp_env);
void vSetExitOnFatalErrs(void);
char *pchErrorMsg(void);
void vSetErrLog(FILE *fp);

#ifdef __cplusplus
}
#endif

/* Error classes */

#define ECLASS_UNKNOWN 0 /* Don't use this */
#define ECLASS_FATAL 1
#define ECLASS_WARN 2
#define ECLASS_INFO 3
#define ECLASS_DEBUG 4
#define MAX_ECLASS 4

/* Error Types */

#define ETYPE_UNKNOWN 0 /* Don't use this */
#define ETYPE_MALLOC 1
#define ETYPE_IO 2
#define ETYPE_DEBUG 3
#define ETYPE_INFO 4
#define ETYPE_FOPEN 5
#define ETYPE_SYNTAX 6
#define ETYPE_ILLEGAL_VALUE 7
#define ETYPE_USAGE 8
#define MAX_ETYPE 8

#endif /*ERRDEFS_H*/
