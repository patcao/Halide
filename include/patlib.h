#include <stdio.h>
#include <time.h>
#include <sys/time.h>
/*
long start;
int msec;
#define timing(func,name) start = clock(); func; msec = (clock() - start) * 1000 / CLOCKS_PER_SEC; printf("%s Timing: %d ms \n",name, msec);
*/

struct timeval t1,t0;
long elapsed;
#define timing(func,name) gettimeofday(&t0,0); func ; gettimeofday(&t1,0); elapsed = (t1.tv_sec - t0.tv_sec)*1000000 + t1.tv_usec - t0.tv_usec; printf("%s Timing: %lu ms \n", name, elapsed / 1000);



