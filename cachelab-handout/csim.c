#include "cachelab.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

char * filename;
struct cacheDefinition {
    int sval, eval, bval, numSets, blockSize;
    unsigned long ** tags;    
} cache;

void parse(int argc, char * argv[]);
void init();
void printErr();

int main(int argc, char * argv[])
{ 
    parse(argc, argv);
    
    printSummary(0, 0, 0);
    return 0;
}


/*
 * parse
 *
 * parses the command line arguments 
 *
 * param: int argc - number of command line arguments
 * param: char * argv[] - command line arguments 
 * param: string & filename - set to the name of the text file to be 
 *                            used for input
 */
void parse(int argc, char * argv[])
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
        filename = optarg;;
        break;  
      case '?':
        printErr(argv);
        break;
      default:
        abort();
    }
    
    if (cache.sval == 0 || cache.eval == 0 || cache.bval == 0)
        printErr(argv);
    else
    init();
}

/*
 * init
 *
 */
 void init()
 {
    printf("**************\nParse Succesfull\n********\n\n");
 }

/*
 * printErr
 *
 */
 void printErr()
 {
    printf("parse: csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
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
 
