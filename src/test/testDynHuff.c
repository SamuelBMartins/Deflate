/*
 * testDynHuff.c
 *
 *  Created on: Dec 31, 2019
 *      Author: Samuel Martins
 *
 *   Compress and decompress a file with only dynamic Huffman method
 */

#include "test.h"
#include "../algorithms/dynHuffman.h"
#include "../utils/utils.h"
#include "../utils/fileIO.h"
#include "../algorithms/lz77.h"

#include <stdlib.h>

/*
 * NodeFreqHuff is declared locally, move struct in the .h file
 * in order to run this test function
 *
 * We create a small build tree to check if it is work correctly
 */
void testBuildHuffTree(){
	NodeFreqHuff ** heapLiteral = calloc(6, sizeof(NodeFreqHuff *));
	heapLiteral[0] = calloc(1, sizeof(NodeFreqHuff));
	heapLiteral[0]->element = 0;
	heapLiteral[0]->value = 5;

	heapLiteral[1] = calloc(1, sizeof(NodeFreqHuff));
	heapLiteral[1]->element = 1;
	heapLiteral[1]->value = 9;

	heapLiteral[2] = calloc(1, sizeof(NodeFreqHuff));
	heapLiteral[2]->element = 2;
	heapLiteral[2]->value = 12;

	heapLiteral[3] = calloc(1, sizeof(NodeFreqHuff));
	heapLiteral[3]->element = 3;
	heapLiteral[3]->value = 13;

	heapLiteral[4] = calloc(1, sizeof(NodeFreqHuff));
	heapLiteral[4]->element = 4;
	heapLiteral[4]->value = 16;

	heapLiteral[5] = calloc(1, sizeof(NodeFreqHuff));
	heapLiteral[5]->element = 5;
	heapLiteral[5]->value = 45;

	buildHeap(heapLiteral, 6);
	NodeFreqHuff *new = buildHuffmanTree(heapLiteral, 6);
	//Tree lit[6];
	prepareCanonicHuffman(new, litHuffCodes, 0);
	printf("end");
}


void testDynHuffCompr(){
	while(!feof(in)){
		char *buffer = malloc(sizeof(char)*BLOCK_SIZE);
		int len = fread(buffer, sizeof(char), BLOCK_SIZE, in);
		lz77Compression(buffer, len);
		//printLz77Data(le_getList(), 100);
		free(buffer);
		if(feof(in))
			writeBits(6, 3, out);
		else
			writeBits(2, 3, out);
		dynHuffCompr(le_getList());
	}
	finalizeWriting(out);
}

void testDynHuffDecompr(){
	int lastCycle = 0;
	while(lastCycle != 6){
		lastCycle = readBits(3, in);
		dynHuffDecompr();
	}
	finalizeReading(in);
}
