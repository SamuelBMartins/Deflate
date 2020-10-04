/*
 * deflate.c
 *
 *  Created on: Dec 29, 2019
 *      Author: Samuel Martins
 *
 *	 Deflate algorithm
 *      TODO: block 16k lz77Entities
 */

#include "utils/utils.h"
#include "utils/fileIO.h"
#include "algorithms/lz77.h"
#include "algorithms/noCompression.h"
#include "algorithms/fixHuffman.h"
#include "algorithms/dynHuffman.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* write in a buffer BLOCK_SIZE B from file and compress this block */
void compression(){
	while(!feof(in)){
			char *buffer = malloc(sizeof(char)*BLOCK_SIZE);
			int lenBuffer = fread(buffer, sizeof(char), BLOCK_SIZE, in);
			lz77Compr(buffer, lenBuffer);
			fixHuffCompr(le_getList());
			fromFixToDyn();
			dynHuffCompr(le_getList());
			writeBlkOnFile(buffer, lenBuffer);

			free(buffer);
			le_freeList();
		}
		finalizeWriting(out);
}

/* choose method decompression from firsts 3 bits*/
void decompression(){
	int lastCycle = 0;
	while(!lastCycle){
		lastCycle = readBits(1, in);
		int type = readBits(2, in);
		switch (type) {
			case 0:
				noDecompr();
				break;
			case 1:
				fixHuffDecompr();
				break;
			case 2:
				dynHuffDecompr();
				break;
			default:
				fprintf(stderr, "Type code error!\n");
				break;
		}
	}
	finalizeReading(in);
}

int main(int argc, char **argv) {

	/* check number of parameters*/
	if (argc != 4) {
		fprintf(stderr, "Arguments have to be 3\nex: deflate -c input.txt output.bin\n");
		return 1;
	}

	/* check if it can open the files*/
	if((in = fopen(argv[2], "rb")) && (out = fopen(argv[3], "wb"))){
		init();
		if (!strcmp(argv[1], "-c")) {
			compression();
		} else if (!strcmp(argv[1], "-d")){
			decompression();
		}else {
			fprintf(stderr, "Argument not valid\n");
			return 2;
		}

	} else {
		fprintf(stderr, "Impossible to open file\n");
		return 3;
	}

	return 0;
}
