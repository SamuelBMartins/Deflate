/*
 * test.h
 *
 *  Created on: Dec 24, 2019
 *      Author: Samuel Martins
 */

#include "../structures/lz77Entities.h"
#include "../algorithms/dynHuffman.h"

#ifndef SRC_TEST_TEST_H_
#define SRC_TEST_TEST_H_

/* LZ77 Compression */
void testHashFunction();
void testCompression();
void printLz77Data(Le_node *node, int n);
int testLz77();
int hash(unsigned char a, unsigned char b, unsigned char c);

/* Huffman fixed Compression */
void testFixHuffCompr();
void testFixHuffDecompr();

/* File I/O */
void testWrite();
int testFile(int argc, char **argv);

/* Dynamic Huffman */
void canonicHuff(int maxBits, int nCodes, HuffCode *huffCodes);
void test_CanoniHuffman();
void test_printCanonicHuffman(int nCodes, HuffCode *huffCodes);
void testDynHuffCompr();
void testDynHuffDecompr();

/* They don't work because NodeFreqHuff is declared locally */
//void buildHeap(NodeFreqHuff **nodes, int len);
//NodeFreqHuff *buildHuffmanTree(NodeFreqHuff **nodes, int len);
//void prepareCanonicHuffman(NodeFreqHuff *node, HuffCode *lengths, int level);

/* no compression */
void testNoCompression();
void testNoDecompression();

/* deflate */
void compression();
void decompression();

#endif /* SRC_TEST_TEST_H_ */
