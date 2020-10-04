/*
 * testCanonicHuffman.c
 *
 *  Created on: Dec 16, 2019
 *      Author: Samuel Martins
 */

#include "test.h"
#include "../algorithms/dynHuffman.h"
#include "../utils/utils.h"

#include <stdio.h>

void test_printCanonicHuffman(int nCodes, HuffCode *huffCodes){
	for (int i = 0; i < nCodes; ++i) {
		printf("%d\t%x\n", huffCodes[i].len, huffCodes[i].code);
	}
}

/*
 * this is a example from the RFC the output should be the same
 * of the RFC
 */
void test_CanoniHuffman(){
		HuffCode literalLengthTree[8];
		literalLengthTree[0].len =  3;
		literalLengthTree[1].len =  3;
		literalLengthTree[2].len =  3;
		literalLengthTree[3].len =  3;
		literalLengthTree[4].len =  3;
		literalLengthTree[5].len =  2;
		literalLengthTree[6].len =  4;
		literalLengthTree[7].len =  4;

		canonicHuff(4, 8, literalLengthTree);
		test_printCanonicHuffman(8, literalLengthTree);
}
