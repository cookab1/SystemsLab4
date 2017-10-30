#include "cachelab.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <math.h>

//Names :: Andy Cook :: Brenner Harris
//UserIDs :: cookab1 :: harrisbd

const int SIZE = 64;

bool verboseFlag = false;

int main(int argc, char **argv)
{
    int hits = 0;
    int misses = 0;
    int evicts = 0;
    int numTag;
    bool helpFlag = false;
    char* fileName = NULL;
    FILE* file = NULL;
    cache_t *cache = malloc(sizeof(cache_t));

    int c = 0;
    while((c = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        switch(c) {
            case 'h':
                helpFlag = true;
                break;
            case 'v':
                verboseFlag = true;
                break;
            case 's':
                cache->setIndexBits = atoi(optarg);
                cache->num_sets = pow(2, cache->setIndexBits);
                break;
            case 'E':
                cache->assoc = atoi(optarg);
                break;
            case 'b':
            	cache->blockBits = atoi(optarg);
		cache->blockSize = pow(2, cache->blockBits);
            	break;
            case 't':
            	fileName = optarg;
        }
    }

    numTag = 64 - cache->setIndexBits - blockBits;
    //malloc the 2 dimen array
    cache->tag = calloc(cache->num_sets, sizeof cache->tag);
    int i = 0;
    for(; i < cache->num_sets; i++)
    {
	cache->tag[i] = calloc(cache->assoc, sizeof cache->tag);
    }
    
    //is the 2 dimen array allocated?
    /*int j;
    int flag = 1;
    for(i = 0; i < cache->num_sets; i++) {
        for(j = 0; j < cache->assoc; j++) {
            if (cache->tag == NULL)
                flag = 0;
        }
    }
    printf("%d \n", flag);
    */

    //Print input file
    /*
    file = fopen(fileName, "r");
    if (file) {
	while ((c = getc(file)) != EOF)
    	    putchar(c);
    	fclose(file);
    {
    */

    file = fopen(fileName, "r");
    char* buf = malloc(80);
    char opp; //the type of opporation
    unsigned long address; //the address of the memory access
    int size; //the size of the memory access
    int blockInd;
    int tag;

    while((c = getc(file)) != EOF) {
        fgets(buf, 80, file);
        //if 'I' is in the first char slot, skip the line
        if(buf[0] == ' ') {
            sscanf(buf, " %c %lx,%x", &opp, &address, &size); 

            if(isError(buf) == 1) {
	        exit(1);
            }

	    blockInd = getBits(0, cache->blockBits - 1, address);
	    tag = getBits(31 - cache->numTag, 31, address);


	    cache->num_sets[
        }
        //printf(" %c %lx,%x\n", opp, address, size);
    }
    /*
    //print line
    for(i = 0; i < 80; i++) {
	putchar(buf[i]);
    }
    */
    
    //printSummary(0, 0, 0);
    return 0;
}

int isError(char *buf) {
    //error checking and print statments
    return false;
}

int getBits(int srt, int end, unsigned long src) {

}
/*
uint64_t getBits(unsigned low, unsigned high, uint64_t source)
{
    assert(high < 64 && (low <= high));

    unsigned numBits = high - low + 1;
    if (numBits == 64)
    {
        return source;
    }
    source >>= low;
    source &= ((1L << numBits) - 1);

    return source;
}
*/
