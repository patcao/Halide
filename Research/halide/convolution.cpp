#include <stdio.h>
#include <iostream>
#include <Halide.h>
#include <math.h>

using namespace Halide;
using namespace std;

int convolve();
Func convolution(Func f, Func kernel, Expr kernel_width,Expr kernel_height);

int main(){

	int  array1[3] = {3,4,5};
	int  array2[3] = {2,1,0};
	int total_length = 5;
	int  ret[5];

	for(int i = 0; i < total_length; ++i){
		ret[i] = 0;
		for(int k = 0; k <= i; ++k){
			if (k < 3 && i-k < 3 &&  i-k >= 0)
				ret[i] += (k < 3 && i-k < 3 && i-k >= 0) ?  array1[k]*array2[i-k] : 0;
		}
	}

	//for(int i = 0; i < total_length; ++i)
		//std::cout << ret[i] << std::endl;
	
	Var x,y;
	Func f("input"),kernel("kernel");
	kernel(x,y) = 0;
	kernel(0,0) = 1;
	kernel(1,0) = 2;
	kernel(2,0) = 1;

	kernel(0,1) = 0;
	kernel(1,1) = 0;
	kernel(2,1) = 0;

	kernel(0,2) = -1;
	kernel(1,2) = -2;
	kernel(2,2) = -1;

	f(x,y) = 0;
	int count = 0;
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
			f(j,i) = ++count;


	Func con = convolution(f,kernel,3,3);
	con.trace_stores();
	Image<int> answer = con.realize(3,3);

	for(int i = 0; i < 3; ++i){
		for(int j = 0; j < 3; ++j)
			std::cout << answer(j,i) << " ";
		cout << endl;
	}
	
	return 0;
}


Func convolution(Func f, Func kernel, Expr kernel_width,Expr kernel_height){
	Var x,y;
	Func convolved("con");
	RDom k (0,kernel_width,0,kernel_height);
	convolved(x,y) += kernel(k.x,k.y)*f(x+k.x-(kernel_width/2),y+k.y-(kernel_height/2));
	return convolved;
}
