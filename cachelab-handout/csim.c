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
char * handleTag();

//
unsigned long getBits(char startBit, char endBit, char * addrStr);

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
    //print success for testing
    else
        printf("**************\nParse Succesfull\n********\n\n"); 
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

    //print struct for testing
    printf("**************\nsval: %d\neval: %d\nbval: %d\nnumSets: %d\nblockSize: %d\ntags:\n",
            (*cache).sval, (*cache).eval, (*cache).bval, (*cache).numSets, (*cache).blockSize);
    for (i = 0; i < (*cache).numSets; i++) { 
        for (j = 0; j < (*cache).eval; j++) {
            printf("%lu ", (*cache).tags[i][j]);
        }
        printf("\n");
    }

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
    else printf("\n\n\nFile open successful :\n");

    char inst;
    char addrStr[8] = ""; 
    unsigned long block, set, tag;
    int i, j;
    bool hit == false;

    while (fgets(buf, 80, file) != NULL) {
        if (buf[0] == ' '){


            sscanf(buf, " %c %s,", &inst, addrStr);



            if (vflag) {
                printf("%s", &buf[1]); 
            }
        }
    }
    printf("\n\n");

    fclose(file);
}

char * handleTag() {
    set = getBits((*cache).bval + 1, (*cache).bval + (*cache).sval, addrStr);
    tag = getBits((*cache).bval + (*cache).sval + 1, sizeof(unsigned long) - 1, addrStr);

    for(i = 0; i < set; i++) {
        if ((*cache).tag[set][i] == tag) {
            hit == true;
        } else if ((*cache).tag[set][i] == -1) {
            for (j = i; j > 0; j--) {
                (*cache).tag[set][j] = (*cache).tag[set][j - 1];
            }
            (*cache).tag[set][0] = tag;
        }
    }
}
}

unsigned long getBits(char startBit, char endBit, char * addrStr) {
    char totalBits = sizeof(unsigned long) * 8;
    return strtoul(addrStr, NULL, 16) <<  (totalBits - endBit) >> (totalBits - endBit + startBit); 
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

