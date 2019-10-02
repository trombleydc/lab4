#include "cachelab.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

struct cacheDefinition {
    int sval, eval, bval, numSets, blockSize;
    unsigned long ** tags;    
} cache;

void usage(int argc, char * argv[], char * filename);
void init();
void go(char * filename);
void printErr();

int main(int argc, char * argv[])
{ 
    char filename[80] = "";
    usage(argc, argv, filename);
    init();
    go(filename);

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
void usage(int argc, char * argv[], char * filename)
{
    int c; 
    opterr = 0;

    while ((c = getopt (argc, argv, "hvs:E:b:t:")) != -1)
        switch (c)
        {
            case 'h':
                printErr(argv);
                break;
            case 'v':
                break;
            case 's':
                cache.sval = atoi(optarg);
                break;
            case 'E':
                cache.eval = atoi(optarg);
                break;
            case 'b':
                cache.bval = atoi(optarg);
                break;
            case 't':
                strcpy(filename, optarg);
                break;  
            case '?':
                printErr(argv);
                break;
            default:
                abort();
        }

    if (cache.sval == 0 || cache.eval == 0 || cache.bval == 0)
        printErr(argv);  
    //print success for testing
    else
        printf("**************\nParse Succesfull\n********\n\n");
}

/*
 * init
 *
 */
void init()
{
    cache.numSets = 1 << cache.sval;
    cache.blockSize = 1 << cache.bval;

    int i, j;

    cache.tags = (unsigned long **) malloc(cache.numSets * sizeof(unsigned long *));
    for (i = 0; i < cache.numSets; i++)
        cache.tags[i] = (unsigned long *) malloc(cache.eval * sizeof(unsigned long));

    for (i = 0; i < cache.numSets; i++)
        for (j = 0; j < cache.eval; j++)
            cache.tags[i][j] = -1;

    //print struct for testing
    printf("**************\nsval: %d\neval: %d\nbval: %d\nnumSets: %d\nblockSize: %d\ntags:\n",
            cache.sval, cache.eval, cache.bval, cache.numSets, cache.blockSize);
    for (i = 0; i < cache.numSets; i++) { 
        for (j = 0; j < cache.eval; j++) {
            printf("%lu ", cache.tags[i][j]);
        }
        printf("\n");
    }

}

void go(char * filename) {
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
    unsigned long addr; 

    while (fgets(buf, 80, file) != NULL) {
        if (buf[0] == ' '){
           puts(buf); 
            sscanf(buf, " %c %s,", &inst, addrStr);
            addr = strtoul(addrStr, NULL, 16);
           printf("%lu\n", addr); 


        }
    }
    printf("\n\n");

    fclose(file);
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

