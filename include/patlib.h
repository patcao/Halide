#include <stdio.h>
#include <time.h>
#include <sys/time.h>

long start;
long msec;
#define timing(func,name) start = clock(); func; msec = (clock() - start) * 1000 / CLOCKS_PER_SEC; printf("%s Timing: %lu ms \n",name, msec);


