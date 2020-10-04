/*
 * dynHuffman.h
 *
 *  Created on: Dec 29, 2019
 *      Author: Samuel Martins
 */

#include "../structures/lz77Entities.h"

#ifndef SRC_ALGORITHMS_DYNHUFFMAN_H_
#define SRC_ALGORITHMS_DYNHUFFMAN_H_

#define N_CODES_LIT 286
#define N_CODES_DIST 30

/* BLC: number of bits of the lengths of the Huffman codes */
#define BLC_LIT 9
#define BLC_DIST 5

/*
 * struct used to store the link from literal value to huffman code
 */
typedef struct huffCode {
	unsigned int literal;
	int len;
	unsigned int code;
} HuffCode;

HuffCode litHuffCodes[N_CODES_LIT];
HuffCode distHuffCodes[N_CODES_DIST];

void canonicHuff(int blc, int nCodes, HuffCode *huffCodes);

/* reset litHuffCodes and distHuffCodes */
void cleanArrayHuffCodes();

void dynHuffCompr(Le_node *head);

void dynHuffDecompr();

#endif /* SRC_ALGORITHMS_DYNHUFFMAN_H_ */
