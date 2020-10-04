/*
 * dynHuffmanDecompr.c
 *
 *  Created on: 14 Dec 2019
 *      Author: Samuel Martins
 */

#include "dynHuffman.h"
#include "../utils/fileIO.h"
#include "../utils/utils.h"
#include "lz77.h"

#include <stdlib.h>

#define LITERAL 1
#define DISTANCE 0

/* used to build the decompression tree */
typedef struct nodeHuffTree {
	HuffCode *e;
	struct nodeHuffTree *left;
	struct nodeHuffTree *right;
} NodeHuffTree;

static NodeHuffTree *rootLitTree;
static NodeHuffTree *rootDistTree;

/*
 * It creates a tree parsing every element and creating new nodes if needed
 * if the next parsed bit is 1 it creates a new node on the right of the parent
 * otherwise on the left
 */
void buildTree(NodeHuffTree **root, int nCodes, HuffCode *huffCodes){
	*root = calloc(1, sizeof(NodeHuffTree));

	for(int i = 0; i < nCodes; i++){
		NodeHuffTree *actual = *root;
		if(!huffCodes[i].len)
			continue;

		for(int j = huffCodes[i].len - 1; j >= 0; j--){

			/* read only one next bit from the Huffman code*/
			if((huffCodes[i].code >> j) & 0x0001){
				if(!actual->right)
					actual->right = calloc(1, sizeof(NodeHuffTree));
				actual = actual->right;
			} else {
				if(!actual->left)
					actual->left = calloc(1, sizeof(NodeHuffTree));
				actual = actual->left;
			}
		}
		actual->e = &huffCodes[i];
	}
}

/*
 * Decompress both literals and distance code
 *
 * type = 1: literal
 * type = 0: distance
 */
int findElement(int type){
	NodeHuffTree *actual = type ? rootLitTree : rootDistTree;
	while(!actual->e ){
		int c;
		if(( c = readBits(1, in) ))
			actual = actual->right;
		else
			actual = actual->left;
	}

	return type ? actual->e->literal : findDist(actual->e->literal);
}

/* done without further Huffman compression despite RFC */
void readHuffCode(){
	for (int i = 0; i < N_CODES_LIT; ++i) {
		litHuffCodes[i].literal = i;
		litHuffCodes[i].len = readBits(9, in);
	}

	for (int i = 0; i < N_CODES_DIST; ++i) {
		distHuffCodes[i].literal = i;
		distHuffCodes[i].len = readBits(5, in);
	}
}

void freeTree(NodeHuffTree *node){
	if(!node) return;
	freeTree(node->left);
	freeTree(node->right);
	free(node);
}

/*
 * TODO: dimension decompressed block could be insufficient
 * 	if file was compressed with another compressor
 */
void dynHuffDecompr(){
	char *buffer = malloc(sizeof(char)*BLOCK_SIZE);
	char *ptr = buffer;

	readHuffCode();
	canonicHuff(BLC_LIT, N_CODES_LIT, litHuffCodes);
	canonicHuff(BLC_DIST, N_CODES_DIST, distHuffCodes);
	buildTree(&rootLitTree, N_CODES_LIT, litHuffCodes);
	buildTree(&rootDistTree, N_CODES_DIST, distHuffCodes);

	unsigned int c;
	while(( c = findElement(LITERAL)) != 256){
		if(c < 256) {
			*ptr++ = (char) c;
		} else {
			unsigned int len = findLen(c);
			unsigned int dist = findElement(DISTANCE);
			lz77Decompr(&ptr, len, dist);
		}
	}

	fwrite(buffer, sizeof(char), ptr - buffer, out);

	free(buffer);
	freeTree(rootDistTree);
	freeTree(rootLitTree);
	cleanArrayHuffCodes();
}
