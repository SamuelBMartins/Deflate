/*
 * fileIO.h
 *
 *  Created on: Dec 11, 2019
 *      Author: Samuel Martins
 */

#include <stdio.h>

#ifndef SRC_UTILS_FILEIO_H_
#define SRC_UTILS_FILEIO_H_

/* they represent the input and output file*/
FILE *in;
FILE *out;

/* add dataLen bits on a buffer that will send the data on the out file */
void writeBits(unsigned int data, int len, FILE *out);

/* read len bits from file */
unsigned int readBits(int len, FILE *in);

/* insert last bits on the out file*/
void finalizeWriting(FILE *out);

void finalizeReading(FILE *in);

/* initialize block to memorize compressed data*/
void initRwBlk();

/* switch active blok from fixed huffman to dynamic */
void fromFixToDyn();

/* choose which block is the most compressed and write it on file  */
void writeBlkOnFile(char *block, int len);

#endif /* SRC_UTILS_FILEIO_H_ */
