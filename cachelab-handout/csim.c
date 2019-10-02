#include "cachelab.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

void usage(int argc, char * argv[], char * filename);

int main(int argc, char * argv[])
{
    
    char filename[10] = "File.txt";
    
    usage(argc, argv, filename);
    
    printSummary(0, 0, 0);
    return 0;
}

/*
 * loader
 *
 */
 void loader(int sValue, int eValue, int bValue, int tValue;)
 {
    printf("I HATE C!");
 }
/*
 * usage
 *
 * parses the command line arguments 
 *
 * param: int argc - number of command line arguments
 * param: char * argv[] - command line arguments
 * param: int & ngrams - set to the command line arguments (default 2)
 * param: string & filename - set to the name of the text file to be 
 *                            used for input
 */
void usage(int argc, char * argv[], char * filename)
{
    int c, sValue, eValue, bValue, tValue;

    opterr = 0;

    while ((c = getopt (argc, argv, "hvs:E:b:t:")) != -1)
    switch (c)
    {
      case 'h':
        printError(argv[0]);
        break;
      case 'v':
        break;
      case 's':
        sValue = optarg;
        break;
      case 'E':
        eValue = optarg;
        break;
      case 'b':
        bValue = optarg;
        break;
      case 't':
        tValue = optarg;
        break;  
      case '?':
        printError(argv[0]);
        return 1;
      default:
        abort();
    }
    loader(sValue, eValue, bValue, tValue);
}

/*
 * printError
 *
 */
 void printError(char * argv[])
 {
    printf("usage: " + argv[0] + " [-hv] -s <num> -E <num> -b <num> -t <file>\n");
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
 