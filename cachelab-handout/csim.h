#include "cachelab.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

char * filename;
int sval, bval, eval;

void usage(int argc, char * argv[]);
void printErr();

