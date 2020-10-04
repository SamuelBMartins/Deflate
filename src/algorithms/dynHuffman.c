/*
 * dynHuffman.c
 *
 *  Created on: Dec 31, 2019
 *      Author: Samuel Martins
 *
 *      This file contains functions used in both compression and
 *      decompression of dynamic Huffman
 */

#include "dynHuffman.h"

/*
 * Written from the pseudo-code on the RFC
 * See RFC for more information
 *
 * The codes that are 0 long don't have the variable "code" initialized
 */
void canonicHuff(int maxBits, int nCodes, HuffCode *huffCodes){
	int bl_count[1 << maxBits];

	/* every element at 0 */
	for (int i = 0; i < 1 << maxBits; ++i) bl_count[i] = 0;

	for (int i = 0; i < nCodes; i++){
		bl_count[huffCodes[i].len]++;
	}

	int code = 0;
	bl_count[0] = 0;
	int next_code[1 << maxBits];

	/* every element at 0 */
	for(int i = 0; i < (1 << maxBits); i++) next_code[i] = 0;

	for (int bits = 1; bits < 1 << maxBits; bits++) {
		code = (code + bl_count[bits-1]) << 1;
		next_code[bits] = code;
	}

	for (int n = 0; n < nCodes; n++) {
		int len = huffCodes[n].len;
		if (len != 0) {
			huffCodes[n].code = next_code[len];
			next_code[len]++;
		}
	}
}

/*
 * The arrays containing the Huffman codes have to be reinitialized
 * after every block compression / decompression
 */
void cleanArrayHuffCodes(){
	for(int i = 0; i < N_CODES_LIT; i++){
		litHuffCodes[i].code = 0;
		litHuffCodes[i].len = 0;
	}

	for(int i = 0; i < N_CODES_DIST; i++){
		distHuffCodes[i].code = 0;
		distHuffCodes[i].len = 0;
	}
}
