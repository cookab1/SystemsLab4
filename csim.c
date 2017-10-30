#include "cachelab.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <math.h>

//Names :: Andy Cook :: Brenner Harris
//UserIDs :: cookab1 :: harrisbd

bool verboseFlag = false;

int main(int argc, char **argv)
{
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
            	break;
            case 't':
            	fileName = optarg;
        }
    }

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

    while((c = getc(file)) != EOF) {
        fgets(buf, 80, file);
        sscanf(buf, " %c %lx,%x", &opp, &address, &size); 

        if(isError(buf) == 1) {
	    printf("%s\n","Error");
	    exit(1);
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
    
    return false;
}

int getBits(int srt, int end, unsigned long src) {
    unsigned numBits = end - srt + 1;
    if (numBits == 32)
    {
        return src;
    }
    src >>= srt;
    src &= ((1L << numBits) - 1);
    return src; 
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
