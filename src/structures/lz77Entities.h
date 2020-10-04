/*
 * Lz77Entities.h
 *
 *  Created on: Dec 11, 2019
 *      Author: Samuel Martins
 */

#ifndef SRC_STRUCTURES_LZ77ENTITIES_H_
#define SRC_STRUCTURES_LZ77ENTITIES_H_

struct le_match {
	unsigned int distance;
	unsigned int length;
};

union le_data {
	unsigned char literal;
	struct le_match match;
};

/*
 * it contains both char and lz77 match (data) and a int (isLiteral) that tell us
 * if the node is a char or not
 */
typedef struct le_node{
	int isLiteral;
	union le_data data;
	struct le_node *next;
} Le_node;

Le_node *le_getList();

Le_node *le_addLiteral(unsigned char c);

Le_node *le_addMatch(unsigned int length, unsigned int distance);

/* free memory */
void le_freeList();

#endif /* SRC_STRUCTURES_LZ77ENTITIES_H_ */
