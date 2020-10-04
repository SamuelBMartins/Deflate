/*
 * noCompression.c
 *
 *  Created on: Dec 27, 2019
 *      Author: Samuel Martins & Corrado Pansoni
 */

#include "../utils/fileIO.h"

#include <stdlib.h>
#include <stdio.h>

void noCompr(char *block, unsigned int length){

	/* header bits */
	if(feof(in)){
		writeBits(4, 3, out);
	}else{
		writeBits(0, 3, out);
	}

	unsigned int invLength = ~length;
	writeBits(length & 0xFFFF, 16, out);
	writeBits(invLength & 0xFFFF, 16, out);

	for(int i = 0; i < length; i++){
		writeBits(block[i], 8, out);
	}
}

//Decompression Function
void noDecompr(){
    unsigned short length = readBits(16, in);
    unsigned short invLength = readBits(16,in);
    unsigned char *buffer = malloc(length* sizeof(char));
    unsigned char *ptr = buffer;
    //In case of error, where LED and NLEN don't match
    if((length & 0xFFFF)!=(~invLength & 0xFFFF)){
    	fprintf(stderr, "Error in no compression! Length and its complement don't match\n");
        return;
    }
    //Reads the LENbyte and memorizes it
    for (int i = 0; i <length ; i++) {
        *ptr++ = readBits(8, in);
    }

    //Single write of the whole block
    fwrite(buffer, sizeof(char),ptr-buffer,out);
    free(buffer);
}
