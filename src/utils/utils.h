/*
 * utils.h
 *
 *  Created on: Dec 26, 2019
 *      Author: Samuel Martins
 */

#include <stdio.h>

#ifndef SRC_UTILS_UTILS_H_
#define SRC_UTILS_UTILS_H_

/*
 * zlib uses a buffer of 16'383
 * 0x7FFF = 2^16 - 1 = 64k
 */
#define BLOCK_SIZE 0xFFFF

/* used to calculate literal codes and extra bits */
unsigned int stdValuesLength[29];
int extraBitsLength[29];

unsigned int stdValuesDistance[30];
int extraBitsDistance[30];

/* invoke this function before everything */
void init();

/* for compression */
int lengthCodeOf(unsigned int length);
int distanceCodeOf(unsigned int distance);

/* for decompression */
int findLen(int literal);
int findDist(int code);

#endif /* SRC_UTILS_UTILS_H_ */
