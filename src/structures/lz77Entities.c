/*
 * Lz77Entities.c
 *
 *  Created on: Dec 11, 2019
 *      Author: Samuel Martins
 *
 *      linked list used to transmit data from lz77 to huffman
 */

#include "lz77Entities.h"

#include <stdlib.h>

static Le_node *head = NULL;

/* the pointer to the toil of the linked list makes the insertion easier*/
static Le_node *tail = NULL;

Le_node *le_getList(){
	return head;
}

/* add node at the end of the list */
Le_node *le_addNode(Le_node *data){
	if(!head){
		head = data;
		tail = data;
	}else {
		tail->next = data;
		tail = data;
	}

	return data;
}

Le_node *le_addLiteral(unsigned char c){

	//calloc allow us to check if a var is empty
	Le_node *new = calloc(1, sizeof(Le_node));
	new->isLiteral = 1;
	new->data.literal = c;

	return le_addNode(new);
}

Le_node *le_addMatch(unsigned int length, unsigned int distance){

	//calloc allow us to check if a var is empty
	Le_node *new = calloc(1, sizeof(Le_node));
	new->isLiteral = 0;
	new->data.match.distance = distance;
	new->data.match.length = length;

	return le_addNode(new);
}

void le_freeList_recurs(Le_node *node){
	if(node->next)
		le_freeList_recurs(node->next);
	free(node);
}

void le_freeList(){
	if(head)
		le_freeList_recurs(head);

	head = NULL;
	tail = NULL;
}
