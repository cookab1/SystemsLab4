#include "cachelab.h"
#include <stdlib.h>

int main(int argc, char **argv)
{
    bool helpFlag = false;
    bool verboseFlag = false;
    struct cache_t *cache = malloc(sizeof *cache);

    int c = 0;
    while((c = getopt(argc, argv, "abc:")) != -1) {
        switch(c) {
            case 'h':
                helpFlag = true;
                break;
            case 'v':
                verboseFlag = true;
                break;
            case 's':
                cache->setIndexBits = optarg;
                break;
            case 'E':
                cache->assoc = optarg;
                break;
            //case b and t
        }
    }

    char* buf = fgets(line, 80, 
    
    printSummary(0, 0, 0);
    return 0;
}

int getBits
