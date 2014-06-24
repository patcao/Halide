// This is to test if Halide::Image can 
// use classes other than fundamental types as pixel representations
#include <stdio.h>
#include <iostream>
#include <Halide.h>
#include <math.h>
using Halide::Image;
#include "image_io.h"

using namespace Halide;
using namespace std;

class num{
	public :
	int val;
	num operator+ (const num& param);
	num operator- (const num& param);
	num operator/ (const num& param);
	num operator* (const num& param);

	static void print(num n){cout<< n.val << endl;}
};

num num::operator+ (const num& param){
	num temp;
	temp.val = val + param.val;
	return temp;
}

num num::operator- (const num& param){
	num temp;
	temp.val = val - param.val;
	return temp;
}

num num::operator/ (const num& param){
	num temp;
	temp.val = val / param.val;
	return temp;
}
num num::operator* (const num& param){
	num temp;
	temp.val = val * param.val;
	return temp;
}

int main(){
	Image<uint8_t> input = load("sobel/images/rgb.png");

}






