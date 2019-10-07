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
    int hits, misses, evictions; 
};

//global Variable - verbose flag

bool vflag = false;

//functions

//parses & handles command line arguments
void usage(int argc, char * argv[], char * filename, struct Cache * cache);

//initializes remaining empty cache data
void init(struct Cache * cache);

//reads, parses, & handles file data
void go(char * filename, struct Cache * cache);

//processes address, inserts tag, and handles hit/miss/eviction
char * handleTag(struct Cache * cache, unsigned long addr);

//inserts tag in LRU order
void insertTag(int j, struct Cache * cache, unsigned long set, unsigned long tag);

//prints error for invalid command line arguements 
void printErr();

/******************************************************
 *                    Source Code                     *
 ******************************************************/


/*
 * main
 *
 * entry point for program - declares cache, parses command line arguemnts, initializes cache,
 * processes file, and calls printSummary
 *
 * param: int argc - number of command line arguments
 * param: char * argv[] - array of command line arguments
 */
int main(int argc, char * argv[])
{ 
    struct Cache cache;
    char filename[80] = ""; 
    usage(argc, argv, filename, &cache);
    init(&cache);
    go(filename, &cache);

    printSummary(cache.hits, cache.misses, cache.evictions);
    return 0;
}


/*
 * usage
 *
 * parses the command line arguments 
 *
 * param: int argc - number of command line arguments
 * param: char * argv[] - command line arguments 
 * param: char * filename - the name of the input trace file
 * para: struct Cache * cache - simulated cache data strucutre
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
 * initializes the simulated cache data structure
 *
 * param: struct Cache * cache - simulated cache data structure
 */
void init(struct Cache * cache)
{

    (*cache).hits = 0;
    (*cache).misses = 0;
    (*cache).evictions=0;

    (*cache).numSets = 1 << (*cache).sval; //numSets = 2^s
    (*cache).blockSize = 1 << (*cache).bval; //blockSize = 2^b

    int i, j;

    //allocate tags 2d array
    (*cache).tags = (unsigned long **) malloc((*cache).numSets * sizeof(unsigned long *));
    for (i = 0; i < (*cache).numSets; i++)
        (*cache).tags[i] = (unsigned long *) malloc((*cache).eval * sizeof(unsigned long));

    //initialize tags to -1
    for (i = 0; i < (*cache).numSets; i++)
        for (j = 0; j < (*cache).eval; j++)
            (*cache).tags[i][j] = -1;
}

/*
 * go
 *
 * reads, parses, and handles file data
 *
 * param: char * filename - trace file to be processed
 * param: struct Cache * cache - simulated cache data structure
 */
void go(char * filename, struct Cache * cache) {
    //open file
    FILE * file;
    char buf[80];
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening trace file");
        exit(0);
    }


    //initialize variables
    char inst;
    char status[13], mStat[13], addrStr[8] = ""; 
    unsigned long addr;

    //read lines in file
    while (fgets(buf, 80, file) != NULL) {
        if (buf[0] == ' '){ 
            strcpy(mStat, ""); //empty mstat
            sscanf(buf, " %c %s", &inst, addrStr); //parse line
            addr = strtoul(addrStr, NULL, 16); //convert address string to unsigned long
            
            strcpy(status, handleTag( cache, addr)); //copy return value into status
            if (inst == 'M')    //if M, process a 2nd time
                strcpy(mStat, handleTag(cache, addr)); //copy return value into mstat
            
            if (vflag) {    //if verbose flag was set, print verbose
                printf("%c %s %s %s\n", inst, addrStr, status, mStat); 
            }
        }
    }
    fclose(file); //close file
}


/*
 * handleTag
 *
 * proccesses an address contatined in a line of the trace file
 * insert tag into proper set in least recently used order
 *
 * param: struct Cache * cache - simulated cache data structure
 * param: unsigned long addr - address value to be processed
 */
char * handleTag(struct Cache * cache, unsigned long addr) {
    
    int j; 
    char totalBits = sizeof(unsigned long) * 8 ;
    //get set bits
    unsigned long set = addr >> (*cache).bval << (totalBits - (*cache).sval) >> (totalBits - (*cache).sval);
    //get tag bits
    unsigned long tag = addr >> ((*cache).sval + (*cache).bval);
    
    //access tag array
    for (j = 0; j < (*cache).eval; j++) { 
        //if tag exists
        if ((*cache).tags[set][j] == tag) {  
            insertTag(j, cache, set, tag); 
            (*cache).hits += 1; 
            return "hit";
        }
        //if tag doesnt exist, and array is not at capacity
        if ((*cache).tags[set][j] == -1) { 
            insertTag(j, cache, set, tag);
            (*cache).misses += 1; 
            return "miss";      
        }
    }
    //if tag doesnt exist, and array is at capacity
    insertTag(j, cache, set, tag);
    (*cache).misses += 1;
    (*cache).evictions += 1; 
    return "miss eviction"; 
}

/*
 * insertTag
 *
 * inserts new tag at position 0 and shifts existing tags to current position + 1
 * sets an inactive tag, or evicts the least recently used tag
 *
 * param: int j - current tag index within a set
 * param: struct Cache * cache - simulated cache data structure
 * unsinged long set - set index for tags array
 * unsinged long tag - tag to be inserted
 */
void insertTag(int j, struct Cache * cache, unsigned long set, unsigned long tag) {
    int k;
    for (k = j; k  >= 0; k--) {
        (*cache).tags[set][k] = (*cache).tags[set][k - 1];
    }
    (*cache).tags[set][0] = tag; 
}

/*
 * printErr
 *
 * prints an error upon failure to parse command line arguments
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
