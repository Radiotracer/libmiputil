/*
	getline.h
	
	$Id: getline.h 70 2006-09-14 15:26:53Z mjs $
*/

#ifndef GETLINE_H
#define GETLINE_H

#ifdef __cplusplus
extern "C" {
#endif

char *pchGetLine(FILE *fpIn, int bRaw, int *pLinesRead);
void vFreeLineBuf(void);

#ifdef __cplusplus
}
#endif

#endif /* GETLINE_H */
