#include <stdio.h>
#include <iostream>
#include "lodepng.h"
#include <stdint.h>
#include <stdlib.h>

using namespace std;

unsigned int height,width;

vector<unsigned char> decodeOneStep(const char* fileName){
	vector<unsigned char> image;

	unsigned error = lodepng::decode(image,width,height,fileName);

	if(error) cout << "decoder error" << error << ":" << lodepng_error_text(error) << endl;

	
	return image;	
}

void encodeOneStep(const char* filename, vector<unsigned char> image){
	lodepng::encode(filename,image,width,height);
}

uint8_t* getRGB(vector<unsigned char> image, int x, int y){
	uint8_t *arr = (uint8_t *)malloc(4*sizeof(uint8_t));

	arr[0] = (uint8_t) image[ 4*(x*height + y)];
	arr[1] = (uint8_t) image[ 4*(x*height + y) + 1 ];
	arr[2] = (uint8_t) image[ 4*(x*height + y) + 2 ];
	arr[3] = (uint8_t) image[ 4*(x*height + y) + 3 ];
	return arr;
}

int main(){

	vector<unsigned char> v = decodeOneStep("images/rgb.png");
	
	vector<unsigned char> ret;
	
	/*for(unsigned int x = 0; x < width; ++x){
		for(unsigned int y = 0; y < height; ++y){			
			uint8_t *arr = getRGB(v,x,y);
			for(int i = 0; i < 4; ++i)
				ret.push_back((unsigned char)arr[i]);
		}
	}*/
	

	encodeOneStep("ret.png",ret);
	cout << "Loaded!" << endl;
}


