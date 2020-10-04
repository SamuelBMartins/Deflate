/*
 * testNoCompression.c
 *
 *  Created on: Dec 27, 2019
 *      Author: Samuel Martins
 *
 *  Compress and decompress a file with only the noCompression method
 */

#include "test.h"
#include "../algorithms/noCompression.h"
#include "../utils/utils.h"
#include "../utils/fileIO.h"

#include <stdio.h>
#include <stdlib.h>

void testNoCompression(){
	int len = feof(in) ? 0 : 1;

	while(!feof(in)){
		char *buffer = malloc(sizeof(char)*BLOCK_SIZE);
		len = fread(buffer, sizeof(char), BLOCK_SIZE, in);

		if(feof(in)){
			writeBits(4, 3, out);
		}else{
			writeBits(0, 3, out);
		}
		noCompr(buffer, len);

		free(buffer);
	}
	finalizeWriting(out);
}

void testNoDecompression(){
	int lastCycle = 0;
	while(!lastCycle){
		lastCycle = readBits(3, in);
		noDecompr();
	}
	finalizeReading(in);
}
