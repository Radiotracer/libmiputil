/*
	linklist.h
	
	$Id: linklist.h 47 2005-04-20 19:55:49Z mjs $
*/

#ifndef LINKLIST_H
#define LINKLIST_H

/* define the structure for link list item. Note use of self-referential pointer*/

typedef struct LINK_LIST_S {
	void *pvData;               /* pointer to the node's data */
	struct LINK_LIST_S *psNext; /* pointer to next item in list */
	                            /* NULL if the corresponding node isn't in use */
} LINK_LIST;

/* Prototypes for functions that work on list */

#ifdef __cplusplus
extern "C" {
#endif

int AddNode(LINK_LIST **ppsList, void *pvData);
void ReleaseList(LINK_LIST **ppsList, void (*pRleaseNodeFunction)(void*));
void TraverseList(LINK_LIST *psList, void (*pTraverseFunction)(void*));

#ifdef __cplusplus
}
#endif

#endif /* LINKLIST_H */
