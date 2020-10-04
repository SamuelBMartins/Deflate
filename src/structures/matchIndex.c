/*
 * matchIndex.c
 *
 *  Created on: Dec 11, 2019
 *      Author: Samuel Martins
 *
 *      linked list used by the hash table of lz77 in order to save matches
 *      Every position in the hash table contains a linked list
 */

#include "matchIndex.h"

#include <stdlib.h>

/*
 * Insertion in head
 * if there isn't root, make root = null
 */
mi_Match *mi_addNode(int e, mi_Match *head){

	//calloc allow us to check if a var is empty
	mi_Match *new = calloc(1, sizeof(mi_Match));
	new->index = e;
	new->next = head;
	return new;
}

void mi_freeList(mi_Match *head){
	if(head->next)
		mi_freeList(head->next);
	free(head);
}
