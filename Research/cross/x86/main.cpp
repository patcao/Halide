#include <stdio.h>
#include <stdlib.h>
#include "canny.h"
#include "lodepng.h"
unsigned width, height;

unsigned char * decode(const char* filename){
	unsigned error;
	unsigned char* image;

	error = lodepng_decode32_file(&image, &width, &height, filename);
	if(error) printf("error %u: %s\n",error ,lodepng_error_text(error));
	return image;
}

void encode(const char* filename, const unsigned char* image){
	unsigned error = lodepng_encode32_file(filename, image, width, height);
	if(error) 
		printf("error %u: %s/n" , error , lodepng_error_text(error));
}

int main(){
	uint8_t *src = decode("rgb.png");
	uint8_t *output = (uint8_t *)malloc(sizeof(uint8_t) * width * height*3);
	
	uint8_t *src2 = (uint8_t *)malloc(sizeof(uint8_t) * width * height * 3);


	for(int c = 0;c < 3; ++c)
		for(int y = 0; y < height; ++y)
			for(int x = 0; x < width; ++x)
				src2[x + y*width + c*width*height] = src[ 4*(x + y*width) + c] ;


	buffer_t dstBuf = {0};
	buffer_t srcBuf = {0};		

	dstBuf.host = output;
	srcBuf.host = src2;
	srcBuf.extent[0] = dstBuf.extent[0] = width;
	srcBuf.extent[1] = dstBuf.extent[1] = height;
	srcBuf.extent[2] = dstBuf.extent[2] = 3;

	srcBuf.min[0] = dstBuf.min[0] = 0;
	srcBuf.min[1] = dstBuf.min[1] = 0;
	srcBuf.min[2] = dstBuf.min[2] = 0;

	srcBuf.stride[0] = dstBuf.stride[0] = 1;
	srcBuf.stride[1] = dstBuf.stride[1] = width;
	srcBuf.stride[2] = dstBuf.stride[2] = width * height;

	srcBuf.elem_size = dstBuf.elem_size = 1;

	
	canny(&srcBuf, &dstBuf);
	printf("%d \n",*dstBuf.host);


	uint8_t *out = (uint8_t *)malloc(sizeof(uint8_t) * width * height * 4);

	for(int c = 0;c < 3; ++c)
		for(int y = 0; y < height; ++y)
			for(int x = 0; x < width; ++x)
				out[ 4*(x+y*width) + c] = output[x + y*width + c*width*height] ;

	for(int a = 1; a < width * height; ++a)
		out[a*4 - 1] = src[4*a - 1];

	encode("output.png",out);
	return 0;
}
