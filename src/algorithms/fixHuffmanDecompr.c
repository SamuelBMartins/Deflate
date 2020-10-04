//
// Created by Corrado Pansoni
//

#include "fixHuffman.h"
#include "../utils/fileIO.h"
#include "../utils/utils.h"
#include "lz77.h"

#include <stdlib.h>

//Conversion of read values referring to RFC table
int fixHuffCode2Lit(){
    unsigned int rfc0=48;
    unsigned int rfc143=191;

    unsigned int rfc144=400;
    unsigned int rfc255=511;

    unsigned int rfc256=0;
    unsigned int rfc279=23;

    unsigned int rfc280=192;
    unsigned int rfc287=199;

    //Reading with readBits function -starts with 7 bits
    unsigned int value=readBits(7, in);
    if(value>=rfc256 && value<=rfc279){
        return value+256;
    }

    //if there are no matches with 7 bits, shift by 1 position and try with 8 (2 possibilities)
    value<<=1;
    //reads next bit
    value=value|readBits(1,in);
    if (value>=rfc0 && value<=rfc143){
        return value-48;
    }

    //second "group" of 8 bits
    if (value>=rfc280 && value<=rfc287){
        return value+88;
    }

    //if there is no match with 8 bits, try with 9 bits
    value<<=1;
    value=value|readBits(1,in);
    if (value>=rfc144 && value<=rfc255){
        return value-256;
    }

    return -1;

}

//Decompression with fixed Huffman
void fixHuffDecompr(){
    char *block, *point;
    block=(char *)malloc(sizeof(char)*BLOCK_SIZE);
    point=block;
    int lit=0;
    while((lit=fixHuffCode2Lit())!=256){
        if(lit>256){
            //findLend and findDist functions used to recover the information for the match
           int len=findLen(lit);
           int dist=findDist(readBits(5,in));
           lz77Decompr(&point,len,dist);
        }else if(lit<256){
            *point++=lit;
        }
    }
    //Block structure
    fwrite(block, sizeof(char), point-block, out);
    free(block);
}

