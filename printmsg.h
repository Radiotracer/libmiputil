/*
	printmsg.h
	
	$Id: printmsg.h 47 2005-04-20 19:55:49Z mjs $
*/

#ifndef PRINTMSG_H
#define PRINTMSG_H

/* Prototypes */

#ifdef __cplusplus
extern "C" {
#endif

void vSetMsgLevel(int NewMsgLevel);
void vSetMsgFilePtr(int Level, FILE *FilePtr);
void vSetAllMsgFilePtr(FILE *FilePtr);
void vPrintMsg(int Level,  char *Fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* PRINTMSG_H */
