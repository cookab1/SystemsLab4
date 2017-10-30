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

    numTag = 32 - cache->setIndexBits - cache->blockBits;
    //malloc the 2 dimen array
    cache->tags = calloc(cache->num_sets, sizeof cache->tags);
    int i = 0;
    for(; i < cache->num_sets; i++)
    {
	cache->tags[i] = calloc(cache->assoc, sizeof cache->tags);
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
    int setInd;
    int tag;

    while((c = getc(file)) != EOF) {
        fgets(buf, 80, file);
        //printf("First char of buf: %c", buf[0]);
        //if 'I' is in the first char slot, skip the line
        if(buf[0] != 'I') {
            sscanf(buf, " %c %lx,%x", &opp, &address, &size); 

            if(isError(buf) == 1) {
	        exit(1);
            }

	    setInd = getBits(cache->blockBits - 1, cache->blockBits + cache->setIndexBits - 1, address);
	    tag = getBits(31 - numTag, 31, address);

	    bool found = false;
	    for(i = 0; i < cache->assoc && !found; i++) {
	        if(cache->tags[setInd][i] == tag) {
    	            found = true;
    	            makeTagRecent(i, cache->tags[setInd]);
    	            hits++;
	        }
	        else if(i == cache->assoc - 1) {
		    misses++;
		    if(shiftAllTags(tag, cache->tags[setInd], cache->assoc))
    		        evicts++;
	        }
	    }
        }
        //printf(" %c %lx,%x\n", opp, address, size);
    }
    /*
    //print line
    for(i = 0; i < 80; i++) {
	putchar(buf[i]);
    }
    */

    printSummary(hits, misses, evicts);
    return 0;
}

void makeTagRecent(int currIndex, unsigned long *set){
    unsigned long tempTag = set[currIndex];
    //we might need to make this i <= currIndex; 
    for (int i = currIndex; i > 0; i--){
        //shift all the tags down
        set[i] = set[i - 1];
    }
    set[0] = tempTag;
    
}

int isError(char *buf) {
    //error checking and print statments
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

int shiftAllTags(int tag, unsigned long *set, int assoc){
    bool evicts = false;
    if(set[assoc - 1] != 0)
        evicts = true;

    //shift all elements down the priority line
    int i;
    for(i = assoc - 1; i > 0; i--) {
	set[i] = set[i - 1];
    }
    set[0] = tag;
    
    return evicts;
}
