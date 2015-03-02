/*
	mipmalloc.h

	$Id: mipmalloc.h 47 2005-04-20 19:55:49Z mjs $
*/

#ifndef MIPMALLOC_H
#define MIPMALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

void IrlFree(void *);
void *pvIrlMalloc(unsigned int, char *pchName);
void *pvIrlRealloc(void *, int, char *pchName);
char *pchIrlStrdup(const char *);
void vFreeAllMem(void);

#ifdef __cplusplus
}
#endif

#endif /* MIPMALLOC_H */
