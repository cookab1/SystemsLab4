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
    for(; i < cache->num_sets; i++) {
	    cache->tags[i] = calloc(cache->assoc, sizeof cache->tags);
    }

    //initialize all elements to -1
    int j;
    for(i = 0; i < cache->num_sets; i++) {
	    for(j = 0; j < cache->assoc; j++) {
	        cache->tags[i][j] = -1;
	    }
    }
 
    if(isError(cache) == 1) {
        exit(1);
    }   
        
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


	    setInd = getBits(cache->blockBits, cache->blockBits + cache->setIndexBits - 1, address);
	    tag = getBits(32 - numTag, 31, address);

	    bool found = false;
	    for(i = 0; i < cache->assoc && !found; i++) {
	        if(cache->tags[setInd][i] == tag) {
    	            found = true;
    	            makeTagRecent(i, cache->tags[setInd]);
    	            hits++;
    	            if(opp == 'M')
        	        hits++;
	        }
	        else if(i == cache->assoc - 1) {
		    misses++;
		    if(shiftAllTags(tag, cache->tags[setInd], cache->assoc))
    		        evicts++;
    		    if(opp == 'M')
        		hits++;
	        }
	    }
        } 
    }
    
    printSummary(hits, misses, evicts);
    return 0;
}


// Helper methods

int isError(cache_t *cache) {
    bool error = false;

    //check if the cache was malloced correctly
    int i;
    int j;
    for(i = 0; i < cache->num_sets; i++) {
        for(j = 0; j < cache->assoc; j++) {
            if (cache->tags == NULL) {
                error = true;
                printf("%s\n", "Cache allocation failed");        
            }
        }
    }

    //input validation
    
    //is assoc 1,2,4, or 8?
    if(cache->assoc != 1 && cache->assoc != 2 && cache->assoc != 4 && cache->assoc != 8) {
        error = true;
        printf("%s\n", "The Associativity should be 1, 2, 4, or 8");
    }

    return error;
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
    if(set[assoc - 1] != -1)
        evicts = true;

    //shift all elements down the priority line
    int i;
    for(i = assoc - 1; i > 0; i--) {
	    set[i] = set[i - 1];
    }
    set[0] = tag;
    
    return evicts;
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
