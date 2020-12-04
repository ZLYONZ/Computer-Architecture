#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

int hitCount = 0;
int missCount = 0;
int ReadCount = 0;
int WriteCount = 0;
int time = 0;
unsigned int setBit = 0;
unsigned int blockBit = 0;
unsigned int tagBit = 0;

typedef struct
{
    int lineValid;
    unsigned long long lineTag;
    int lruCount;
} line_st;

// Set structure has a pointer to all the line within the set
typedef struct
{
    line_st *ptrLine;
} set_st;

// Cache structure has a pointer to all the sets with the cache
typedef struct
{
    set_st *ptrSet;
} cache_st;

void printResult(bool prefetch)
{
    if (prefetch == false)
    {
        printf("Memory reads: %d\n", ReadCount);
        printf("Memory writes: %d\n", WriteCount);
        printf("Cache hits: %d\n", hitCount);
        printf("Cache misses: %d\n", missCount);
    }
}

unsigned int myLog(unsigned int x)
{
    unsigned int num = 0;
    while (x >>= 1)
        num++;
    return num;
}

void splitAddr(unsigned long long address, unsigned long long *tag, unsigned int *setIndex, unsigned int *blockOffset)
{
    unsigned long long tempVal = 0;
    *tag = address >> (setBit + blockBit);
    tempVal = address << tagBit;
    *setIndex = (unsigned int)(tempVal >> (tagBit + blockBit));
    tempVal = address << (tagBit + setBit);
    *blockOffset = (unsigned int)(tempVal >> (tagBit + setBit));
}

int getHitLine(set_st *ptrSet, int numLines, unsigned long long tag)
{
    for (int line = 0; line < numLines; line++)
    {
        // Make sure the line is valid data and the tag is matching
        if ((ptrSet->ptrLine[line].lineValid == 1) &&
            (ptrSet->ptrLine[line].lineTag == tag))
        {
            return line;
        }
    }
    return -1;
}

int getEmptyLine(set_st *ptrSet, int numLines)
{
    for (int line = 0; line < numLines; line++)
    {
        // Find an empy line
        if (ptrSet->ptrLine[line].lineValid == 0)
        {
            return line;
        }
    }
    return -1;
}

int getEvictionLine(set_st *ptrSet, int numLines)
{
    int minLruCount = INT_MAX;
    int lruLine = -1;
    for (int line = 0; line < numLines; line++)
    {
        // Find block to evict
        if (ptrSet->ptrLine[line].lruCount < minLruCount)
        {
            minLruCount = ptrSet->ptrLine[line].lruCount;
            lruLine = line;
        }
    }
    return lruLine;
}

void getOper(set_st *ptrSet, int numLines, unsigned long long tag)
{

    // check for empty line
    int line = getEmptyLine(ptrSet, numLines);
    if (line > -1)
    {
        // Found a empy line read memory
        ptrSet->ptrLine[line].lineValid = 1;
        ptrSet->ptrLine[line].lineTag = tag;
        ptrSet->ptrLine[line].lruCount = time;
        time++;
    }
    else
    {
        // find block to evict with prefetched block.
        line = getEvictionLine(ptrSet, numLines);
        if (line > -1)
        {
            // Found a block to evict
            ptrSet->ptrLine[line].lineValid = 1;
            ptrSet->ptrLine[line].lineTag = tag;
            ptrSet->ptrLine[line].lruCount = time;
            time++;
        }
        else
        {
            printf("error\n");
        }
    }
}

int main(int argc, char **argv)
{
    if (argc < 6)
    {
        printf("error:\n");
    }
    unsigned int cacheSize = atoi(argv[1]);
    unsigned int blockSize = atoi(argv[4]);
    unsigned int numSets = cacheSize / blockSize;
    unsigned int numLines = 1;
    if (memcmp(argv[2], "assoc:", 6) == 0)
    {
        char *str = strchr(argv[2], ':');
        if (str)
        {
            numLines = atoi(str + 1);
        }
    }
    numSets = numSets / numLines;
    setBit = myLog(numSets);
    blockBit = myLog(blockSize);
    tagBit = 64 - (setBit + blockBit);

    cache_st cache;
    // Memory alloction for all sets in the cache
    cache.ptrSet = (set_st *)malloc(numSets * sizeof(set_st));
    for (unsigned int i = 0; i < numSets; i++)
    {
        memset(&cache.ptrSet[i], 0, sizeof(set_st));
    }

    // Memory alloction for all lines in all sets
    for (unsigned int i = 0; i < numSets; i++)
    {
        cache.ptrSet[i].ptrLine = (line_st *)malloc(numLines * sizeof(line_st));
        for (unsigned int j = 0; j < numLines; j++)
        {
            memset(&cache.ptrSet[i].ptrLine[j], 0, sizeof(set_st));
        }
    }
    char operation = '\0';
    unsigned long long address;
    unsigned long long pc;
    char buffer[512];
    bool prefetch = false;
    unsigned long long addrTag = 0;
    unsigned int setIndex = 0;
    unsigned int blockOffset = 0;
    FILE *fp = fopen(argv[5], "r");
    if (fp)
    {
        while (fgets(buffer, 255, fp) != NULL)
        {
            if (memcmp(buffer, "#eof", 4) != 0)
            {
                sscanf(buffer, "%llx: %c %llx", &pc, &operation, &address);
                splitAddr(address, &addrTag, &setIndex, &blockOffset);
                set_st set = cache.ptrSet[setIndex];
                // check for hit
                int line = getHitLine(&set, numLines, addrTag);
                if (line > -1)
                {
                    // Hit
                    hitCount++;
                    if (operation == 'W')
                    {
                        // memory write needs to happen even if the data is in the cashe
                        WriteCount++;
                    }
                    set.ptrLine[line].lruCount = time;
                    time++;
                }
                else
                {
                    // Miss
                    missCount++;
                    // Both read and writing require to reading the memory
                    ReadCount++;
                    if (operation == 'W')
                    {
                        WriteCount++;
                    }
                    getOper(&set, numLines, addrTag);

                    if (prefetch)
                    {
                        // No miss for prefetch as cpu did not request
                        splitAddr(address + blockSize, &addrTag, &setIndex, &blockOffset);
                        set = cache.ptrSet[setIndex];
                        int line = getHitLine(&set, numLines, addrTag);
                        if (line > -1)
                        {
                            // Hit
                        }
                        else
                        {
                            ReadCount++;
                            getOper(&set, numLines, addrTag);
                        }
                    }
                }
            }
            else
            {
                break;
            }
        }
        fclose(fp);
    }
    printResult(prefetch);
    // Deallocate memory for all the lines in all the sets
    for (unsigned int i = 0; i < numSets; i++)
    {
        if (cache.ptrSet[i].ptrLine)
        {
            free(cache.ptrSet[i].ptrLine);
        }
    }
    // Deallocation memory for the sets.
    if (cache.ptrSet)
    {
        free(cache.ptrSet);
    }
    return 0;
}