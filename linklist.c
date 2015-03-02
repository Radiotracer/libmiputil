/*
	linklist.c
	
	$Id: linklist.c 62 2006-01-30 16:08:31Z mjs $
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "miputil.h"
#include "linklist.h"

int AddNode(LINK_LIST **ppsList, void *pvData)
/*adds a node where the pointer points to the data at pvData.
  Returns 0 if the add was successful, nonzero otherwise
*/
{
  LINK_LIST *psNext;
  
  /*find the last node, which is the one where psNext is NULL*/
  
  psNext = *ppsList;
  while(psNext != NULL){
    ppsList = &(psNext->psNext);/*ppsList is pointer containing
				  pointer to the psNext. When we find the
				  psNext containing null, we will know where
				  we need to save the new address, i.e.,
				  int *ppsList*/
    psNext = psNext->psNext;
  }
  *ppsList=(LINK_LIST *)pvIrlMalloc(sizeof(LINK_LIST),"AddNode:List");
  if(*ppsList == NULL)return(1);
  (*ppsList)->pvData = pvData;
  (*ppsList)->psNext = NULL;
  return(0);
}

void ReleaseList(LINK_LIST **ppsList, void (*pReleaseNodeDataFunc)(void*))
{
	LINK_LIST *psNext, *psCurrent;

	psCurrent = *ppsList;
	while (psCurrent != NULL){
		psNext = psCurrent->psNext;
		/* free the data for this node */
		if (psCurrent->pvData != NULL)
			(pReleaseNodeDataFunc)(psCurrent->pvData);
		IrlFree(psCurrent);
		psCurrent=psNext;
	}
	/* set the root node to NULL indicating that the entire tree list
	freed*/
	*ppsList = NULL;
}

void TraverseList(LINK_LIST *psList, void (*pTraverseFunc)(void*))
     /* calls the function pointed to by pTraverseFunc with the pointer to the data
	data for each node as the argument*/
{
  while (psList != NULL){
    (*pTraverseFunc)(psList->pvData);
    psList = psList->psNext;
  }
}
