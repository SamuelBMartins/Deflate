/*
 * testFileIO.c
 *
 *  Created on: Dec 11, 2019
 *      Author: Samuel Martins
 */

#include "test.h"
#include "../utils/fileIO.h"

/*
 * I want to write: "ciao" : 0x63 69 61 6F
 * 01100011
 * 01101001
 * 01100001
 * 01101111
 */
void testWrite(){
	writeBits(0xC, 5, out); //0b01100
	writeBits(0x1B4, 10, out); //0b0110110100
	writeBits(0x2C, 6, out); //0b101100
	writeBits(0x16, 7, out); //0b0010110
	writeBits(0xF, 4, out); //0b1111
	finalizeWriting(out);
}

int testFile(int argc, char **argv) {
	testWrite();

	return 0;
}
