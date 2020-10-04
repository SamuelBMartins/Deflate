/*
 * fixHuffmanCompr.c
 *
 *  Created on: Dec 4, 2019
 *      Author: Samuel Martins
 *
 *      TODO: search match beyond the block
 */

#include "../utils/fileIO.h"
#include "../structures/lz77Entities.h"
#include "../utils/utils.h"
#include "fixHuffman.h"

#include <stdio.h>

/* Convert literal to huffman code as said in the RFC */
void writeLiteral(Le_node *node){
	unsigned int value = 0;
	if(node->data.literal < 144){
		value = 0x30;
		value += node->data.literal;
		writeBits(value, 8, out);
	} else {
		value = 0x190;
		value += node->data.literal - 144;
		writeBits(value, 9, out);
	}
}

/* Convert length lz77 to huffman code as said in the RFC */
void writeLength(Le_node *node){
	unsigned int code = lengthCodeOf(node->data.match.length) + 1;

	unsigned int value = 0;
	if(code < 24){
		value += code;
		writeBits(value, 7, out);
	} else {
		value = 0xC0;
		value += code - 24;
		writeBits(value, 8, out);
	}

	/* write extra bits */
	writeBits(
			node->data.match.length - stdValuesLength[code - 1],
			extraBitsLength[code - 1],
			out
		);
}

void writeDistance(Le_node *node){
	unsigned int code = distanceCodeOf(node->data.match.distance);
	writeBits(code, 5, out);

	/* write extra bits */
	writeBits(
			node->data.match.distance - stdValuesDistance[code],
			extraBitsDistance[code],
			out
		);
}

void fixHuffCompr(Le_node *head){

	/* header bits */
	if(feof(in))
		writeBits(5, 3, out);
	else
		writeBits(1, 3, out);

	for(Le_node *nextNode = head; nextNode; nextNode = nextNode->next){
		if(nextNode->isLiteral){
			writeLiteral(nextNode);
		} else {
			writeLength(nextNode);
			writeDistance(nextNode);
		}
	}

	writeBits(0, 7, out); //end of block
}
