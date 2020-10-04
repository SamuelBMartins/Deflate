/*
 * fileIO.c
 *
 *  Created on: Dec 11, 2019
 *      Author: Samuel Martins
 *
 *   Manages write/read bits on file
 *   Every block will compress in at least 2 ways, fixed huffman
 *   and dynamic huffman, each variant of the compressed data is saved
 *   in a block and then they will be compared in order to chose the most
 *   compressed
 */

#include "fileIO.h"
#include "utils.h"
#include "../algorithms/noCompression.h"

#include <stdlib.h>
#include <stdio.h>

/* 8KB read buffer */
#define R_BLK_SIZE 0x1FFF

/* every block as a RwBlk for each method compression*/
typedef struct rwBlk {
	unsigned char *blk;
	unsigned char *ptr;
	unsigned long buff;
	int buffLen;
} RwBlk;

static RwBlk noComprWBlk;
static RwBlk fixHuffWBlk;
static RwBlk dynHuffWBlk;

/* pointer to the active method compression block */
static RwBlk *actualWBlk;

static RwBlk rBlk;

void emptyingBuffer(RwBlk *blk){
	blk->buff = 0;
	blk->buffLen = 0;
}

/*
 * The bits of header, before the compressed data, can be at max
 * 3b header + 286 * 9b + 30 * 5b huffman alphabet = 2'727b =~ 341 B
 *
 * worst case lz77 is 9b len 18b dist = 27b for a match of 3B = 24b so 27 - 24 = 3b
 * it can happend BLOCK_SIZE / 3 times then 3 * BLOCK_SIZE / 3 = BLOCK_SIZE b = BLOCK_SIZE / 8 B
 *
 * Huffman is a optimal algorithm and the worst case is a compressed block
 * as big as the uncompressed
 *
 *  A compressed block can't be larger than the uncompressed block by:
 * BLOCK_SIZE + 341 + (BLOCK_SIZE / 8)
 *
 * So 2*BLOCK_SIZE + 400 is enough
 */
void initRwBlk(){
	fixHuffWBlk.blk = malloc((2 * BLOCK_SIZE + 400) * sizeof(char));
	fixHuffWBlk.ptr = fixHuffWBlk.blk;
	emptyingBuffer(&fixHuffWBlk);

	dynHuffWBlk.blk = malloc((2 * BLOCK_SIZE + 400) * sizeof(char));
	dynHuffWBlk.ptr = dynHuffWBlk.blk;
	emptyingBuffer(&dynHuffWBlk);

	noComprWBlk.blk = malloc((2 * BLOCK_SIZE + 400) * sizeof(char));
	noComprWBlk.ptr = noComprWBlk.blk;
	emptyingBuffer(&noComprWBlk);

	rBlk.blk = malloc(R_BLK_SIZE*sizeof(char));

	/* the pointer is at the end to force the refilling of the buffer*/
	rBlk.ptr = rBlk.blk + R_BLK_SIZE;
	emptyingBuffer(&rBlk);

	/* the fist algrithm will be the fixed huffman */
	actualWBlk = &fixHuffWBlk;
}

/*
 * In order to speed up the reading we created a buffer that read
 * on the file at least 8KB, this would reduce the request of file system
 * instead of make a request every byte we want to read
 */
unsigned char getCharFromRBlk(){
	if(rBlk.ptr - rBlk.blk == R_BLK_SIZE){
		fread(rBlk.blk, sizeof(char), R_BLK_SIZE, in);
		rBlk.ptr = rBlk.blk;
	}
	return *rBlk.ptr++;
}

/*
 * use shifts and or operators in order to return only the requested number
 * of bits and it save the bits not sent in a unsigned long buffer for the
 * next request
 */
unsigned int readBits(int len, FILE *in){
	while(rBlk.buffLen < len){
		unsigned char nextChar = getCharFromRBlk();
		rBlk.buffLen += 8;
		rBlk.buff <<= 8;
		rBlk.buff |= nextChar;
	}

	unsigned int requestedBits = rBlk.buff >> (rBlk.buffLen - len);
	requestedBits &= (1 << len) - 1;
	rBlk.buff &= ( 1 << (rBlk.buffLen - len)) -1;
	rBlk.buffLen -= len;
	return requestedBits;
}

/* similar to readBits above */
void writeBits(unsigned int data, int len, FILE *out){
	actualWBlk->buff <<= len;
	data &= (1 << len) - 1;
	actualWBlk->buff |= data;
	actualWBlk->buffLen += len;

	while(actualWBlk->buffLen >= 8){
		unsigned int dataToInsert = actualWBlk->buff >> (actualWBlk->buffLen - 8);
		*actualWBlk->ptr++ = dataToInsert;

		/* removes bits were sent by buffer */
		actualWBlk->buff &= (1 << (actualWBlk->buffLen - 8)) - 1;
		actualWBlk->buffLen -= 8;
	}
}

/*
 * I can insert on the file at least 1 byte, if the remaining bits are less than 8
 * this function add some 0s in the last write
 */
void finalizeWriting(FILE *out){
	if(actualWBlk->buffLen > 0){
		actualWBlk->buff <<= (8 - actualWBlk->buffLen);
		fwrite(&actualWBlk->buff, sizeof(char), 1, out);
	}

	emptyingBuffer(&fixHuffWBlk);
	emptyingBuffer(&dynHuffWBlk);
	emptyingBuffer(&noComprWBlk);
	free(noComprWBlk.blk);
	free(dynHuffWBlk.blk);
	free(fixHuffWBlk.blk);
}

void finalizeReading(FILE *in){
	emptyingBuffer(&rBlk);
	free(rBlk.blk);
}

/*
 * change the buffer actually in use the divide the data generated from
 * fixed huffman to dynamic huffman
 */
void fromFixToDyn(){
	actualWBlk = &dynHuffWBlk;
}

void mergeBuffers(RwBlk *from, RwBlk *to){
	to->buff = from->buff;
	to->buffLen = from->buffLen;
}

/*
 * These function choose which block, among fixed huffman,
 * dynamic huffman and no compression, should be written on file
 */
void writeBlkOnFile(char *block, int len){
	int lenDyn = dynHuffWBlk.ptr - dynHuffWBlk.blk;
	int lenFix = fixHuffWBlk.ptr - fixHuffWBlk.blk;
	int lenNo = len + 4;

	if(lenNo < lenDyn && lenNo < lenFix){

		/* no compression */
		actualWBlk = &noComprWBlk;
		noCompr(block, len);
		fwrite(noComprWBlk.blk, sizeof(char), noComprWBlk.ptr - noComprWBlk.blk, out);
		mergeBuffers(&noComprWBlk, &fixHuffWBlk);
		mergeBuffers(&noComprWBlk, &dynHuffWBlk);
	} else if(lenDyn < lenFix) {

		/* dynamic Huffman */
		fwrite(dynHuffWBlk.blk, sizeof(char), lenDyn, out);
		mergeBuffers(&dynHuffWBlk, &fixHuffWBlk);
		mergeBuffers(&dynHuffWBlk, &noComprWBlk);
	} else {

		/* fixed Huffman */
		fwrite(fixHuffWBlk.blk, sizeof(char), lenFix, out);
		mergeBuffers(&fixHuffWBlk, &dynHuffWBlk);
		mergeBuffers(&fixHuffWBlk, &noComprWBlk);
	}

	/* reset buffers */
	fixHuffWBlk.ptr = fixHuffWBlk.blk;
	dynHuffWBlk.ptr = dynHuffWBlk.blk;
	noComprWBlk.ptr = noComprWBlk.blk;

	actualWBlk = &fixHuffWBlk;
}
