/*
 * lz77.c
 *
 *  Created on: Nov 25, 2019
 *      Author: Samuel Martins
 */

#include "lz77.h"
#include "../structures/matchIndex.h"
#include "../structures/lz77Entities.h"
#include "../utils/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 * Hash function
 * multiplication method
 */
int hash(unsigned char a, unsigned char b, unsigned char c){
	unsigned long IDChars =  a + (b << 8) + (c << 16);
	return HASH_SIZE * fmod(IDChars * (sqrt(5)-1)/2, 1);
}

/* clean memory */
void freeMemory(mi_Match **hashTable){
	for (int i = 0; i < HASH_SIZE; ++i) {
		if(hashTable[i])
			mi_freeList(hashTable[i]);
	}
	free(hashTable);
}

/* doesn't search for matches for the next len bytes */
int skipBytes(char *block, mi_Match **hashTable, char *ptr, int len){
	for (int i = 0; i < len; ++i) {
		int index = hash(ptr[i+0], ptr[i+1], ptr[i+2]);
		hashTable[index] = mi_addNode(ptr + i - block, hashTable[index]);
	}
	return len;
}

/*
 * search for match with biggest length
 * return length best match
 */
int bestMatch(mi_Match *match, char *block, int len, char *ptr) {
	int lengthMax = 0;
	mi_Match *nodeMax;

	for (; match; match = match->next) {
		if(ptr - (block + match->index) > MAX_DISTANCE)
			break; //matches are in increasing order

		int length = 0;
		for(int i = match->index; block[i] == ptr[length] && (ptr - block) + length < len; i++){
			if(++length >= 256)
				break;
		}

		if (length > lengthMax) {
			lengthMax = length;
			nodeMax = match;
		}
	}

	if(lengthMax < 3){
		le_addLiteral(*ptr);
		return 0;
	}else{
		le_addMatch(lengthMax, ptr - (block + nodeMax->index));
		return lengthMax - 1;
	}
}

void lz77Compr(char *block, int len){
	mi_Match **hashTable = calloc(HASH_SIZE, sizeof(mi_Match *));

	/* ptr divides search_buffer to lookhaed_buffer */
	char *ptr;

	/* search for match and update hashtable */
	for(ptr = block; ptr + 2 - block < len; ptr++){
		int index = hash(ptr[0], ptr[1], ptr[2]);
		mi_Match *match = hashTable[index];
		hashTable[index] = mi_addNode(ptr - block, match);

		int nSkips = bestMatch(match, block, len, ptr);
		ptr += skipBytes(block, hashTable, ptr, nSkips);
	}

	/* add remaining bytes */
	for (; ptr - block < len; ptr++)
		le_addLiteral(*ptr);

	freeMemory(hashTable);
}

/* we can decompress a lz77 match only if it's in the same block*/
void lz77Decompr(char **ptr, int len, int dist){
	char *ptrDataToCopy = *ptr - dist;
	for(int i = 0; i < len; i++)
		*(*ptr)++ = ptrDataToCopy[i];
}
