/*
 * dynHuffmanCompr.c
 *
 *  Created on: Dec 29, 2019
 *      Author: Samuel Martins
 */

#include "dynHuffman.h"
#include "../structures/lz77Entities.h"
#include "../utils/utils.h"
#include "../utils/fileIO.h"

#include <stdlib.h>

/*
 * Store frequency for each element (literal or distance)
 */
typedef struct nodeFreqHuff {
	int element;
	int value;
	struct nodeFreqHuff *left;
	struct nodeFreqHuff *right;
} NodeFreqHuff;

/*
 * Write frequency elements in **literal and **distance and the number
 * of the respective non-zero's elements on *litLen and *distLen
 */
void calcFreq(Le_node *node, NodeFreqHuff **literal, NodeFreqHuff **distance, int *litLen, int *distLen){

	/* Contain how many times a element appear in the block */
	int nLiteral[N_CODES_LIT] = {0};
	int nDist[N_CODES_DIST] = {0};

	/* we need to manually add the 256 (signal of end of block) */
	nLiteral[256]++;

	for(Le_node *actual = node; actual; actual = actual->next){
		if(actual->isLiteral){
			nLiteral[actual->data.literal]++;
		}else{
			nLiteral[257 + lengthCodeOf(actual->data.match.length)]++;
			nDist[distanceCodeOf(actual->data.match.distance)]++;
		}
	}

	int j = 0;
	for(int i = 0; i < N_CODES_LIT; i++){
		if(nLiteral[i]){
			NodeFreqHuff *new = calloc(1, sizeof(NodeFreqHuff));
			new->element = i;
			new->value = nLiteral[i];
			literal[j++] = new;
		}
	}
	*litLen = j;

	j = 0;
	for(int i = 0; i < N_CODES_DIST; i++){
		if(nDist[i]){
			NodeFreqHuff *new = calloc(1, sizeof(NodeFreqHuff));
			new->element = i;
			new->value = nDist[i];
			distance[j++] = new;
		}
	}
	*distLen = j;
}

/* it creates a new node as root and add the 2 nodes in the parameters as leaves*/
NodeFreqHuff *unionNodes(NodeFreqHuff *node1, NodeFreqHuff *node2){
	NodeFreqHuff *new = calloc(1, sizeof(NodeFreqHuff));
	new->element = -1;
	new->value = node1->value + node2->value;
	new->left = node1;
	new->right = node2;

	return new;
}

/* Classic heapify procedure */
void heapify(NodeFreqHuff **nodes, int index, int len){
	int smallest;

	int left = 2 * (index + 1) - 1;
	int right = left + 1;

	if(left < len && nodes[left]->value < nodes[index]->value)
		smallest = left;
	else
		smallest = index;

	if(right < len && nodes[right]->value < nodes[smallest]->value)
		smallest = right;

	if(smallest != index){
		NodeFreqHuff *tmp = nodes[smallest];
		nodes[smallest] = nodes[index];
		nodes[index] = tmp;
		heapify(nodes, smallest, len);
	}
}

/* classic build heap procedure */
void buildHeap(NodeFreqHuff **nodes, int len){
	for(int i = (len-1) / 2; i >= 0; i--) {
		heapify(nodes, i, len);
	}
}

/* Build huffman tree from heap */
NodeFreqHuff *buildHuffTree(NodeFreqHuff **nodes, int len){
	for(int i = len; i > 1; i--){

		/* remove less frequent element from heap*/
		NodeFreqHuff *tmp = nodes[0];
		nodes[0] = nodes[i -1];
		nodes[i - 1] = NULL;
		heapify(nodes, 0, i - 1);

		/* remove second less frequent element from heap*/
		NodeFreqHuff *tmp2 = nodes[0];
		nodes[0] = nodes[i - 2];
		nodes[i - 2] = NULL;
		heapify(nodes, 0, i - 2);

		/* add a tree to the heap containing the two previously removed elements */
		int j = i - 2;
		NodeFreqHuff *new = unionNodes(tmp, tmp2);
		while (j && new->value < nodes[(j - 1) / 2]->value){
			nodes[j] = nodes[(j - 1) / 2];
			j = (j - 1) / 2;
		}
		nodes[j] = new;
	}
	return *nodes;
}

void dynHuff_wLen(Le_node *node){
	unsigned int code = lengthCodeOf(node->data.match.length);

	unsigned int huffCode = litHuffCodes[257 + code].code;
	unsigned int len = litHuffCodes[257 + code].len;
	writeBits(huffCode, len, out);

	/* write extra bits */
	writeBits(
			node->data.match.length - stdValuesLength[code],
			extraBitsLength[code],
			out
		);
}

void dynHuff_wDist(Le_node *node){
	unsigned int code = distanceCodeOf(node->data.match.distance);

	unsigned int huffCode = distHuffCodes[code].code;
	unsigned int len = distHuffCodes[code].len;
	writeBits(huffCode, len, out);

	/* write extra bits */
	writeBits(
			node->data.match.distance - stdValuesDistance[code],
			extraBitsDistance[code],
			out
		);
}

/* This function discover the length of every huffman code*/
void prepareCanonicHuff(NodeFreqHuff *node, HuffCode *huffCodes, int level){
	if(!node) return;

	if(node->element != -1){
		huffCodes[node->element].len = level;
	}

	prepareCanonicHuff(node->left, huffCodes, level + 1);
	prepareCanonicHuff(node->right, huffCodes, level + 1);
}

void freeMem(NodeFreqHuff *nodes){
	if(!nodes) return;
	freeMem(nodes->left);
	freeMem(nodes->right);
	free(nodes);
}

/*
 * Write after the header bits 286 * 9 bits lengths of literal huffman codes
 * and 30 * 5 bits lengths distance huffman codes
 */
void writeDictionary(){
	for(int i = 0; i < N_CODES_LIT; i++){
		if(litHuffCodes[i].len)
			writeBits(litHuffCodes[i].len, 9, out);
		else
			writeBits(0, 9, out);
	}

	for(int i = 0; i < N_CODES_DIST; i++){
			if(distHuffCodes[i].len)
				writeBits(distHuffCodes[i].len, 5, out);
			else
				writeBits(0, 5, out);
		}
}

/*
 * If a element of the huffman tree has frequency 0 the len value will be
 * not initialized after the canoni huffman then we initialize at 0 every
 * element before the canonic huffman
 */
void initArrayHuffCodes(HuffCode *huffCodes, int len){
	for (int i = 0; i < len; ++i) {
		huffCodes[i].len = 0;
	}
}

void dynHuffCompr(Le_node *head){

	/* header bits */
	if(feof(in))
		writeBits(6, 3, out);
	else
		writeBits(2, 3, out);

	/* huffman */
	NodeFreqHuff **heapLiteral = calloc(N_CODES_LIT, sizeof(NodeFreqHuff *));
	NodeFreqHuff **heapDistance = calloc(N_CODES_DIST, sizeof(NodeFreqHuff *));
	int heapLiteralLen;
	int heapDistanceLen;
	calcFreq(le_getList(), heapLiteral, heapDistance, &heapLiteralLen, &heapDistanceLen);
	buildHeap(heapLiteral, heapLiteralLen);
	buildHeap(heapDistance, heapDistanceLen);
	NodeFreqHuff *treeLiteral = buildHuffTree(heapLiteral, heapLiteralLen);
	NodeFreqHuff *treeDistance = buildHuffTree(heapDistance, heapDistanceLen);

	/* canonic huffman */
	initArrayHuffCodes(litHuffCodes, N_CODES_LIT);
	initArrayHuffCodes(distHuffCodes, N_CODES_DIST);
	prepareCanonicHuff(treeLiteral, litHuffCodes, 1);
	prepareCanonicHuff(treeDistance, distHuffCodes, 1);
	canonicHuff(9, N_CODES_LIT, litHuffCodes);
	canonicHuff(5, N_CODES_DIST, distHuffCodes);

	writeDictionary();

	free(heapLiteral);
	free(heapDistance);
	freeMem(treeLiteral);
	freeMem(treeDistance);

	for(Le_node *nextNode = head; nextNode; nextNode = nextNode->next){
		if(nextNode->isLiteral){
			unsigned int code = litHuffCodes[nextNode->data.literal].code;
			unsigned int len = litHuffCodes[nextNode->data.literal].len;
			writeBits(code, len, out);
		} else {
			dynHuff_wLen(nextNode);
			dynHuff_wDist(nextNode);
		}
	}

	/* we add end of block signal */
	writeBits(litHuffCodes[256].code, litHuffCodes[256].len, out);

	cleanArrayHuffCodes();
}
