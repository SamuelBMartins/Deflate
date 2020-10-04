/*
 * testFixHuff.c
 *
 *  Created on: Dec 28, 2019
 *      Author: Samuel Martins
 *
 *  Compress and decompress a file with only fixed Huffman method
 */

#include "test.h"
#include "../algorithms/fixHuffman.h"
#include "../utils/utils.h"
#include "../utils/fileIO.h"
#include "../algorithms/lz77.h"
#include "../algorithms/fixHuffman.h"

#include <stdio.h>
#include <stdlib.h>

void testFixHuffCompr(){
	while(!feof(in)){
		char *buffer = malloc(sizeof(char)*BLOCK_SIZE);
		int len = fread(buffer, sizeof(char), BLOCK_SIZE, in);
		lz77Compr(buffer, len);
		//printLz77Data(le_getList(), 100);
		free(buffer);
		if(feof(in))
			writeBits(5, 3, out);
		else
			writeBits(1, 3, out);
		fixHuffCompr(le_getList());
	}
	finalizeWriting(out);
}

void testFixHuffDecompr(){
	int lastCycle = 0;
	while(lastCycle != 5){
		lastCycle = readBits(3, in);
		fixHuffDecompr();
	}
	finalizeReading(in);
}
