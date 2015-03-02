/*
	printmsg.c
	
	$Id: printmsg.c 82 2007-01-10 01:23:21Z mjs $
*/

#include <stdio.h>
#include <stdarg.h>
#include "errdefs.h"
#include "printmsg.h"

#define MIN_LEVEL 1
#define MAX_LEVEL 10

static int iPrintMsgLevel= 3;
static FILE *fpMsgFilePtrArray[MAX_LEVEL];
static int bInitialized=0;

static void InitMsgFileArray(void)
{
	int i;
	for(i=0; i<MAX_LEVEL; ++i){
		fpMsgFilePtrArray[i]=stderr;
	}
	bInitialized=1;
}

void vSetMsgLevel(int iNewMsgLevel)
{
	if (!bInitialized)InitMsgFileArray();
	if (iNewMsgLevel < MIN_LEVEL-1){
		vErrorHandler(ECLASS_WARN, ETYPE_ILLEGAL_VALUE, "vSetMsgLevel",
			"New Message Level (%d) is too low, setting to %d instead",
			iNewMsgLevel, MIN_LEVEL-1);
			iNewMsgLevel=MIN_LEVEL-1;
	}else if (iNewMsgLevel > MAX_LEVEL){
		vErrorHandler(ECLASS_WARN, ETYPE_ILLEGAL_VALUE, "vSetMsgLevel",
			"New Message Level (%d) is too high, setting to %d instead",
			iNewMsgLevel, MAX_LEVEL);
			iNewMsgLevel=MAX_LEVEL;

	}
	iPrintMsgLevel = iNewMsgLevel;
	if (iPrintMsgLevel < MIN_LEVEL){
		vErrorHandler(ECLASS_INFO, ETYPE_INFO,"vSetMsgLevel",
			"Supressing all messages");
	}else{
		vErrorHandler(ECLASS_INFO, ETYPE_INFO,"vSetMsgLevel",
			"Supressing messages with levels > %d",iPrintMsgLevel);
	}
}

void vSetMsgFilePtr(int iLevel, FILE *fpMsgFilePtr)
{
	if (!bInitialized)InitMsgFileArray();
	if (iLevel < MIN_LEVEL){
		vErrorHandler(ECLASS_WARN, ETYPE_ILLEGAL_VALUE, "vSetMsgFilePtr",
			"Message Level (%d) is too low, ignoring",
			iLevel);
	}else if (iLevel > MAX_LEVEL){
		vErrorHandler(ECLASS_WARN, ETYPE_ILLEGAL_VALUE, "vSetMsgFilePtr",
			"Message Level (%d) is too high, ignoring",
			iLevel);
	}else{
		fpMsgFilePtrArray[iLevel-1] = fpMsgFilePtr;
	}

}

void vSetAllMsgFilePtr(FILE *fpMsgFilePtr)
{
	int iLevel;

	if (!bInitialized)InitMsgFileArray();
	for(iLevel=MIN_LEVEL-1; iLevel < MAX_LEVEL; ++iLevel){
		fpMsgFilePtrArray[iLevel]=fpMsgFilePtr;
	}
}
	

void vPrintMsg(int iMsgLevel, char *pchMsgFmt, ...)
{
	va_list vaMsgArgs;
	
	if (!bInitialized)InitMsgFileArray();
	if (iMsgLevel < MIN_LEVEL){
		vErrorHandler(ECLASS_WARN, ETYPE_ILLEGAL_VALUE, "vPrintMsg",
			"Message Level (%d) is too low, using %d instead",
			iMsgLevel, MIN_LEVEL);
		iMsgLevel=MIN_LEVEL;
	}else if (iMsgLevel > MAX_LEVEL){
		vErrorHandler(ECLASS_WARN, ETYPE_ILLEGAL_VALUE, "vPrintMsg",
			"Message Level (%d) is too high, using %d instead",
			iMsgLevel, MAX_LEVEL);
		iMsgLevel=MAX_LEVEL;
	}
	if (fpMsgFilePtrArray[iMsgLevel-1] != NULL && iMsgLevel <= iPrintMsgLevel){
		va_start(vaMsgArgs,pchMsgFmt);
		vfprintf(fpMsgFilePtrArray[iMsgLevel-1], pchMsgFmt, vaMsgArgs);
 		fflush(fpMsgFilePtrArray[iMsgLevel-1]);
		va_end(vaMsgArgs);
	}
}
