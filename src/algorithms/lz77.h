/*
 * lz77.h
 *
 *  Created on: Dec 3, 2019
 *      Author: Samuel Martins
 */

#include "../structures/matchIndex.h"

#ifndef SRC_LZ77COMPR_H_
#define SRC_LZ77COMPR_H_

#define HASH_SIZE (1 << 10) //TODO testing others dimensions
#define MAX_DISTANCE 1 << 15

void lz77Compr(char *block, int len);

void lz77Decompr(char **ptr, int len, int dist);

#endif /* SRC_LZ77COMPR_H_ */
