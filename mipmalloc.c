/*
	mipmalloc.c
	
	$Id: mipmalloc.c 101 2009-03-03 15:00:38Z frey $
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef MALLOC_H_NEEDED
#include <malloc.h>
#endif
#include "errdefs.h"
#include "mipmalloc.h"
#include "printmsg.h"

#ifdef DEBUG_MALLOC

/* Linkstructure containg information about each memory allocation */
typedef struct MemListNode_s{
	void *pvPtr; /*pointer to data */
	char *pchStr; /* string passed to IrlMalloc describing memory */
	int iSize; /*size in bytes of memory pointed to by pvPtr */
	struct MemListNode_s *psNext; /*pointer to next node in list */
} MemListNode;

/* global pointer to memory list*/
static MemListNode *psgMemItemsList=NULL;

static int iAddMemItem(void *pvData, int iNbytes, char *pchPointerName)
/* Adds a memory node to the head of the linked list. */
/* We add the new node at the head of the list so additions are 0(1) */
{
	MemListNode *psNode;

	psNode=(MemListNode *)malloc(sizeof(MemListNode));
	if (psNode == NULL){
		vErrorHandler(ECLASS_FATAL, ETYPE_MALLOC, "AddMemItem:Node", 
			"Error Allocating MemListNode for %s", pchPointerName);
		return -1;
	}
	psNode->psNext=psgMemItemsList;
	psNode->pvPtr=pvData;
	psNode->iSize=iNbytes;
	psNode->pchStr=malloc(strlen(pchPointerName)+1);
	if (psNode->pchStr == NULL){
		vErrorHandler(ECLASS_FATAL, ETYPE_MALLOC, "AddMemItem:Str",
			"Error Allocating MemItemName %s", pchPointerName);
		free(psNode);
	}
	strcpy(psNode->pchStr, pchPointerName);
  psgMemItemsList = psNode;
	return 0;
}

static MemListNode *psGetMemListNode(void *pvData)
/* finds and returns a pointer to the memory list node corresponding to the
   memory pointed to by pvData. Returns NULL if not found.
*/
{
	MemListNode *psNode;

	psNode=psgMemItemsList;
	while(psNode != NULL){
		if (pvData == psNode->pvPtr)
			return psNode;
		psNode=psNode->psNext;
	}
	return NULL;
}

static MemListNode *psFreeMemNode(MemListNode *psNode)
/* frees the memory and associated items pinted to by psNode. Returns
   NULL if successful
*/
{
	if (psNode != NULL){
		if (psNode->pvPtr != NULL) free(psNode->pvPtr);
		if (psNode->pchStr != NULL) free(psNode->pchStr);
		free(psNode);
	}
	return NULL;
}

void vFreeAllMem(void)
/* Traverses the MemItemsList freeing all the memory and nodes */
{
	MemListNode *psNode = psgMemItemsList;
	MemListNode *psNext;
	int nitems=0, totsize=0;

	while (psNode != NULL){
		vPrintMsg(9,"free %s: %d\n",psNode->pchStr, psNode->iSize);
		totsize += psNode->iSize;
		psNext=psNode->psNext;
		psNode=psFreeMemNode(psNode);
		psNode=psNext;
		nitems++;
	}
	vPrintMsg(6, "freed %d items using %d kB\n",nitems,totsize/1024);
  psgMemItemsList = NULL;
}

void *pvIrlMalloc(unsigned int iNbytes, char *pchPointerName)
/* allocates memory with size iNbytes. The string pchPointerName is
   used to provide information if the allocation fails. If there
	is an error, ErrorHandler is called with a fatal error. If this is
	not trapped, a NULL is returned. Memory allocated via IrlMalloc is
	kept track of in the Memory Items List
*/
{
	void *pvData;

	pvData = malloc((size_t)iNbytes);
	if (pvData == NULL){
		vErrorHandler(ECLASS_FATAL, ETYPE_MALLOC, "IrlMalloc", 
			"Error Allocating %s", pchPointerName);
		return (NULL);
	}
	if (iAddMemItem(pvData, iNbytes, pchPointerName)) {
    free(pvData);
		return NULL;
  }
	return(pvData);
}

void *pvIrlRealloc(void *pvData, int iNbytes, char *pchPointerName)
/* Changes size of a block of memory allocated with pvIrlMalloc. This
	also updates the memory items list. If the reallocation fails or the
	memory isn't found in the list, a warning is issued and an attempt
	is made to reallocate it anyway. However, it will then not be tracked
	in MemItemsList
*/
{

	if (pvData == NULL){
		pvData = (void *)pvIrlMalloc(iNbytes, pchPointerName);
	}else{
		MemListNode *psNode;

		psNode=psGetMemListNode(pvData);
		if (psNode == NULL){
			vErrorHandler(ECLASS_WARN, ETYPE_MALLOC, "IrlRealloc", 
				"Error Reallocating %s: %x not found", pchPointerName, pvData);
		}else{
			pvData = realloc(pvData, iNbytes);
			if (psNode != NULL){
				psNode->pvPtr=pvData;
				psNode->iSize=iNbytes;
			}
		}
	}
	if (pvData == NULL){
		vErrorHandler(ECLASS_FATAL, ETYPE_MALLOC, "IrlRealloc", 
			"Error Reallocating %s ", pchPointerName);
	}
	return(pvData);
}
	
void IrlFree(void *pvData)
/* Frees memory pointed to by pvData and removes the corresponding
	Node from MemItemsList
*/
{
	MemListNode *psNode, **ppsLast;

	if (pvData == NULL)
		return;
	ppsLast=&psgMemItemsList;
	psNode=psgMemItemsList;
	while(psNode != NULL)	{
		if (psNode->pvPtr == pvData)
			break;
		ppsLast=&((*ppsLast)->psNext);
		psNode=psNode->psNext;
	}
	if (psNode == NULL)	{
		vErrorHandler(ECLASS_WARN, ETYPE_MALLOC, "IrlFree", 
			"Pointer %x not in MemList", pvData);
	}
	else {
		/* Found the pointer we are looking for*/
#ifdef DEBUG
		fprintf(stderr,"freeing %s\n",psNode->pchStr);
#endif
		free(psNode->pchStr);
		free(psNode->pvPtr);
		*ppsLast=psNode->psNext;
		free(psNode);
	}
}

#else

void *pvIrlMalloc(unsigned int iNbytes, char *pchPointerName)
{
	void *pvData;

	pvData = malloc((size_t)iNbytes);
	if (pvData == NULL){
		vErrorHandler(ECLASS_FATAL, ETYPE_MALLOC, "IrlMalloc", 
			"Error Allocating %s", pchPointerName);
	}
	return(pvData);
}
void *pvIrlRealloc(void *pvInPtr, int iNbytes, char *pchPointerName)
{
	void *pvData;

	if (pvInPtr == NULL){
		pvData = (void *)malloc(iNbytes);
	}else{
		pvData = realloc(pvInPtr, iNbytes);
	}
	if (pvData == NULL){
		vErrorHandler(ECLASS_FATAL, ETYPE_MALLOC, "IrlRealloc", 
			"Error Allocating %s", pchPointerName);
	}
	return(pvData);
}
	
void IrlFree(void *pvData)
{
	if (pvData != NULL)
		free(pvData);
}

void vFreeAllMem()
{
		vErrorHandler(ECLASS_WARN, ETYPE_MALLOC, "vFreeAllMem", 
			"Can't Free all memory when using non-debug IrlMalloc\n");
}
#endif
/* DEBUG_MALLOC */

char *pchIrlStrdup(const char *pchIn)
{
	int len;
	char *pchNew;

	len = (int) strlen(pchIn)+1;
	pchNew = (char *)pvIrlMalloc((unsigned int)len, "IrlStrdup:New");
	strcpy(pchNew, pchIn);
	return pchNew;
}
