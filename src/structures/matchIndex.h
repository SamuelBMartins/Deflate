/*
 * matchIndex.h
 *
 *  Created on: Dec 11, 2019
 *      Author: Samuel Martins
 */

#ifndef SRC_STRUCTURES_MATCHINDEX_H_
#define SRC_STRUCTURES_MATCHINDEX_H_

typedef struct mi_match{
	int index;
	struct mi_match *next;
} mi_Match;

/* create a node and link it t the root */
mi_Match *mi_addNode(int element, mi_Match *head);

/* free memory */
void mi_freeList(mi_Match *head);

#endif /* SRC_STRUCTURES_MATCHINDEX_H_ */
