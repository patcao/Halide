#include <stdio.h>
#include <stdlib.h>
#include "fib.h"

int main(){
	//buffer_t* b =(buffer_t *) malloc(sizeof(buffer_t));
	buffer_t dstBuf = {0};
	uint8_t *dst = (uint8_t *) malloc(sizeof(uint8_t) * 100);

	dstBuf.host = dst;
	dstBuf.extent[0] = 50;
	dstBuf.extent[1] = 0;
	dstBuf.extent[2] = 0;
	dstBuf.extent[3] = 0;
	dstBuf.stride[0] = 1;
	dstBuf.stride[1] = 50;
	dstBuf.min[0] = 0;
	dstBuf.min[1] = 0;
	dstBuf.elem_size = 1;


	fib(&dstBuf);
	printf("%d \n",*dstBuf.host);
	return 0;
}
