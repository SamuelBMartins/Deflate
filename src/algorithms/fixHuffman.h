/*
 * fixHuffman.h
 *
 *  Created on: Dec 4, 2019
 *      Author: Samuel Martins & Corrado Pansoni
 */

#include "../structures/lz77Entities.h"

#ifndef SRC_ALGORITHMS_FIXHUFFMAN_H_
#define SRC_ALGORITHMS_FIXHUFFMAN_H_

/* compress a block with fixed Huffman */
void fixHuffCompr(Le_node *head);

/* decompress a block with fixed Huffman */
void fixHuffDecompr();

#endif /* SRC_ALGORITHMS_FIXHUFFMAN_H_ */
