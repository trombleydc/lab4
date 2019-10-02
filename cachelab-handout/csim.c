#include "csim.h"

int main(int argc, char * argv[])
{ 
    usage(argc, argv);
    
    printSummary(0, 0, 0);
    return 0;
}

/*
 * loader
 *
 */
 void loader()
 {
    printf("**************\nParse Succesfull\n********\n\n");
 }
/*
 * usage
 *
 * parses the command line arguments 
 *
 * param: int argc - number of command line arguments
 * param: char * argv[] - command line arguments 
 * param: string & filename - set to the name of the text file to be 
 *                            used for input
 */
void usage(int argc, char * argv[])
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
        sval = atoi(optarg);
        break;
      case 'E':
        eval = atoi(optarg);
        break;
      case 'b':
        bval = atoi(optarg);
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
    
    if (sval == 0 || eval == 0 || bval == 0)
        printErr(argv);
    else
    loader();
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
 
