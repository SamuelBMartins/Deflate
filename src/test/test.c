/*
 * test.c
 *
 *  Created on: Dec 24, 2019
 *      Author: Samuel Martins
 */

#include "test.h"
#include "../utils/utils.h"
#include "../utils/fileIO.h"
#include "../algorithms/lz77.h"
#include "../algorithms/noCompression.h"
#include "../algorithms/fixHuffman.h"

#include <stdio.h>
#include <stdlib.h>

/* copy of the function in deflate.c file */
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

/* copy of the function in deflate.c file */
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

void def_immagine(){
	out = fopen("Files/immagine.tiff.compr", "wb");
	in = fopen("Files/immagine.tiff", "rb");
	compression();
	fclose(in);
	fclose(out);

	out = fopen("Files/immagineDef.tiff", "wb");
	in = fopen("Files/immagine.tiff.compr", "rb");
	decompression();
	fclose(out);
	fclose(in);
}

void def_alice(){
	out = fopen("Files/alice.txt.compr", "wb");
	in = fopen("Files/alice.txt", "rb");
	compression();
	fclose(in);
	fclose(out);

	out = fopen("Files/aliceDef.txt", "wb");
	in = fopen("Files/alice.txt.compr", "rb");
	decompression();
	fclose(out);
	fclose(in);
}

void def_ff_ff_ff(){
	out = fopen("Files/ff_ff_ff.compr", "wb");
	in = fopen("Files/ff_ff_ff", "rb");
	compression();
	fclose(in);
	fclose(out);

	out = fopen("Files/ff_ff_ffDef", "wb");
	in = fopen("Files/ff_ff_ff.compr", "rb");
	decompression();
	fclose(out);
	fclose(in);
}

void def_32k_ff(){
	out = fopen("Files/32k_ff.compr", "wb");
	in = fopen("Files/32k_ff", "rb");
	compression();
	fclose(in);
	fclose(out);

	out = fopen("Files/32k_ffDef", "wb");
	in = fopen("Files/32k_ff.compr", "rb");
	decompression();
	fclose(out);
	fclose(in);
}

void def_32k_random(){
	out = fopen("Files/32k_random.compr", "wb");
	in = fopen("Files/32k_random", "rb");
	compression();
	fclose(in);
	fclose(out);

	out = fopen("Files/32k_randomDef", "wb");
	in = fopen("Files/32k_random.compr", "rb");
	decompression();
	fclose(out);
	fclose(in);
}

void def_empty(){
	out = fopen("Files/empty.compr", "wb");
	in = fopen("Files/empty", "rb");
	compression();
	fclose(in);
	fclose(out);

	out = fopen("Files/emptyDef", "wb");
	in = fopen("Files/empty.compr", "rb");
	decompression();
	fclose(out);
	fclose(in);
}

/* compress and decompress every file */
int main(int argc, char **argv) {
	init();

	def_alice();
	initRwBlk();
	def_immagine();
	initRwBlk();
	def_32k_ff();
	initRwBlk();
	def_32k_random();
	initRwBlk();
	def_empty();
	initRwBlk();
	def_ff_ff_ff();
}
