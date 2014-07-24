#include <stdio.h>
#include <time.h>

long start;
int msec;
#define timing(func,name) start = clock(); func; msec = (clock() - start) * 1000 / CLOCKS_PER_SEC; printf("%s Timing: %d ms \n",name, msec);


