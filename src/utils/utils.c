/*
 * utils.c
 *
 *  Created on: Dec 26, 2019
 *      Author: Samuel Martins
 */

#include "utils.h"
#include "fileIO.h"

void initStdValuesLength(){

	/* i can't initialize a global array so i create a copy*/
	int extraBitsLengthCopy[29] = {
			0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
			1, 1, 2, 2, 2, 2, 3, 3, 3, 3,
			4, 4, 4, 4, 5, 5, 5, 5, 0
	};
	for(int i = 0; i < 29; i++)
		extraBitsLength[i] = extraBitsLengthCopy[i];

	stdValuesLength[0] = 3;
	for (int i = 1; i < 29; ++i)
		stdValuesLength[i] = stdValuesLength[i - 1] + (1 << extraBitsLength[i - 1]);
}

void initStdValuesDistance(){
	 int extraBitsDistanceCopy[30] = {
			0, 0, 0, 0, 1, 1, 2, 2, 3, 3,
			4, 4, 5, 5, 6, 6, 7, 7, 8, 8,
			9, 9, 10, 10, 11, 11, 12, 12, 13, 13,
	};

	 for(int i = 0; i < 30; i++)
		 extraBitsDistance[i] = extraBitsDistanceCopy[i];

	stdValuesDistance[0] = 1;
	for (int i = 1; i < 30; ++i)
		stdValuesDistance[i] = stdValuesDistance[i - 1] + (1 << extraBitsDistance[i - 1]);
}

/* This function collect every function that needs to be executed at the start*/
void init(){
	initStdValuesLength();
	initStdValuesDistance();
	initRwBlk();
}

/*
 * uses the array stdValuesLength in order to find
 * the code (see RFC)
 */
int lengthCodeOf(unsigned int length){
	for (int i = 1; i < 29; ++i)
		if(length < stdValuesLength[i])
			return i - 1;
	return 28;
}

int distanceCodeOf(unsigned int distance){
	for (int i = 1; i < 30; ++i)
		if(distance < stdValuesDistance[i])
			return i - 1;
	return 29;
}

/*
 * uses the array stdValuesLength in order to find
 * the length and distance (see RFC)
 */
int findLen(int literal){
	int i = literal - 257;
	unsigned int valueExtraBits = readBits(extraBitsLength[i], in);
	return stdValuesLength[i] + valueExtraBits;
}

int findDist(int code){
	return stdValuesDistance[code] + readBits(extraBitsDistance[code], in);
}
