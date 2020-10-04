/*
 * testLz77.c
 *
 *  Created on: Dec 3, 2019
 *      Author: Samuel Martins
 */

#include "test.h"
#include "../structures/lz77Entities.h"
#include "../utils/utils.h"
#include "../utils/fileIO.h"
#include "../algorithms/lz77.h"

#include <stdio.h>
#include <stdlib.h>

void testHashFunction(){
	printf("%d\n", hash('a', 'b', 'c'));
	printf("%d\n", hash('a', 'b', 'e'));
	printf("%d\n", hash('a', 'f', 'e'));
	printf("%d\n", hash('g', 'h', 'a'));
	printf("%d\n", hash('g', 'h', 'e'));
}

void testCompression(){
	char *buffer = malloc(sizeof(char)*BLOCK_SIZE);
	fread(buffer, sizeof(char), BLOCK_SIZE, in);
	lz77Compr(buffer, BLOCK_SIZE);
	free(buffer);
}

/* print firsts n lz77 nodes */
void printLz77Data(Le_node *node, int n){
	if(node->isLiteral){
		printf("%c\n", node->data.literal);
	}else {
		printf("length: %d\tdistance: %d\n", node->data.match.length, node->data.match.distance);
	}
	if(node->next && n)
		printLz77Data(node->next, n - 1);
}

int testLz77() {
	testCompression();
	testHashFunction();
	printLz77Data(le_getList(), 10);
	le_freeList(le_getList());

	return 0;
}
