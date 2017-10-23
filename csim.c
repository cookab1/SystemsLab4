#include "cachelab.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <math.h>

int main(int argc, char **argv)
{
    bool helpFlag = 0;
    bool verboseFlag = 0;
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
    int j;
    int flag = 1;
    for(i = 0; i < cache->num_sets; i++) {
        for(j = 0; j < cache->assoc; j++) {
            if (cache->tag == NULL)
                flag = 0;
        }
    }
    printf("%d \n", flag);

    file = fopen(fileName, "r");
    if (file) {
	while ((c = getc(file)) != EOF)
    	    putchar(c);
    	fclose(file);
    }
    //char* buf = fgets(line, 80, file); //Don't know what line is

    //printSummary(0, 0, 0);
    return 0;
}

//int getBits
