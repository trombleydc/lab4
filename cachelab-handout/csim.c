/******************************************************
 *       Authors: Daniel Trombley (trombleydc)        *
 *                Marc Settin (settinmf)              *
 ******************************************************/

/******************************************************
 *                      Header                        *
 ******************************************************/

//includes
#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>

//struct - Cache data structure

struct Cache {
    int sval, eval, bval, numSets, blockSize;
    unsigned long ** tags;    
};

//global Variable - verbose flag

bool vflag = false;

//functions

//parses & handles command line arguments
void usage(int argc, char * argv[], char * filename, struct Cache * cache);

//initializes remaining empty cache data
void init(struct Cache * cache);

//parses & handles file data
void go(char * filename, struct Cache * cache);

//
char * handleTag(struct Cache * cache, unsigned long addr);

//
unsigned long getBits(char startBit, char endBit, unsigned long addr);

//
void printVerbose();

//prints error for invalid command line arguements 
void printErr();

/******************************************************
 *                    Source Code                     *
 ******************************************************/


/*
 * main
 */
int main(int argc, char * argv[])
{ 
    struct Cache cache;
    char filename[80] = ""; 
    usage(argc, argv, filename, &cache);
    init(&cache);
    go(filename, &cache);

    printSummary(0, 0, 0);
    return 0;
}


/*
 * usage
 *
 * usages the command line arguments 
 *
 * param: int argc - number of command line arguments
 * param: char * argv[] - command line arguments 
 * param: string & filename - set to the name of the text file to be 
 *                            used for input
 */
void usage(int argc, char * argv[], char * filename, struct Cache * cache)
{
    int c; 
    opterr = 0;

    while ((c = getopt (argc, argv, "hvs:E:b:t:")) != -1)
        switch (c)
        {
            case 'h':
                printErr();
                break;
            case 'v':
                vflag = true;
                break;
            case 's':
                (*cache).sval = atoi(optarg);
                break;
            case 'E':
                (*cache).eval = atoi(optarg);
                break;
            case 'b':
                (*cache).bval = atoi(optarg);
                break;
            case 't':
                strcpy(filename, optarg);
                break;  
            case '?':
                printErr();
                break;
            default:
                printErr(); 
        }

    if ((*cache).sval == 0 || (*cache).eval == 0 || (*cache).bval == 0 || filename[0] == 0)
        printErr(argv);   
}

/*
 * init
 *
 */
void init(struct Cache * cache)
{
    (*cache).numSets = 1 << (*cache).sval;
    (*cache).blockSize = 1 << (*cache).bval;

    int i, j;

    (*cache).tags = (unsigned long **) malloc((*cache).numSets * sizeof(unsigned long *));
    for (i = 0; i < (*cache).numSets; i++)
        (*cache).tags[i] = (unsigned long *) malloc((*cache).eval * sizeof(unsigned long));

    for (i = 0; i < (*cache).numSets; i++)
        for (j = 0; j < (*cache).eval; j++)
            (*cache).tags[i][j] = -1;
    }

/*
 * go
 */
void go(char * filename, struct Cache * cache) {
    FILE * file;
    char buf[80];
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening trace file");
        exit(0);
    }

    char inst;
    char status[13], mStat[13], len[4] = ""; 
    unsigned long addr;

    while (fgets(buf, 80, file) != NULL) {
        if (buf[0] == ' '){ 
            strcpy(mStat, "");
            sscanf(buf, " %c %lu,%s", &inst, &addr, len);
            strcpy(status, handleTag( cache, addr));
            if (inst == 'M')
                strcpy(mStat, handleTag(cache, addr));
            if (vflag) {
                printf("%c %lu,%s %s %s\n", inst, addr, len, status, mStat); 
            }
        }
    }
    fclose(file);
}

char * handleTag(struct Cache * cache, unsigned long addr) {
    int j, k;

    unsigned long set = getBits((*cache).bval - 1, (*cache).bval + (*cache).sval - 1, addr);
    unsigned long tag = getBits((*cache).bval + (*cache).sval - 1, sizeof(unsigned long) - 1, addr);

    for (j = 0; j < (*cache).eval; j++) {
        if ((*cache).tags[set][j] == tag) {
            return "hit";
        } else if ((*cache).tags[set][j] == -1) {
            for (k = j; k > 0; k--) {
                (*cache).tags[set][k] = (*cache).tags[set][k - 1];
            }
            (*cache).tags[set][0] = tag; 
        }
    }
    return "miss";
}

unsigned long getBits(char startBit, char endBit, unsigned long addr) {
    char totalBits = sizeof(unsigned long) * 8;
    return addr <<  (totalBits - endBit) >> (totalBits - endBit + startBit); 
}

/*
 * printVerbose
 */
void printVerbose() {

}

/*
 * printErr
 *
 */
void printErr()
{
    printf("usage: csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf(" Options: \n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");

    printf(" : \n");
    exit(0);
}

