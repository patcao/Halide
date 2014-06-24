#include <Halide.h>

using Halide::Image;
#include "image_io.h"

using namespace Halide;
using namespace std;

Func convolution(Func f, Func hx, Expr kernel_width,Expr kernel_height);
Func gaussianBlur(Func in);
Func sobelMag(Func in);
Func sobelAng(Func in);

int main(int argc, char **argv) {
    Image<uint8_t> input = load<uint8_t>("../images/rgb.png");

	Var x("x"),y("y"),c("c");
	Func in("in");
	in(x,y,c) = input(clamp(x,0,input.width() - 1) , clamp(y,0,input.height() - 1), c);
	Func mag = sobelMag(gaussianBlur(in));
	Func ret("ret");
	ret(x,y,c) = cast<uint8_t> (mag(x,y,c) );
	
	
	Image<uint8_t> output = ret.realize(input.width(),input.height(),input.channels());
	
	
	save(output,"both.png");
	//save(input,"original.png");
}


//Performs a gaussian blur with a 5x5 gaussian kernel
Func gaussianBlur(Func in){
	Var x("x1"),y("y1"),c("c1");
/*
	k(x,y) = 0;
	k(0,0) = 2;	k(0,1) = 4;	 k(0,2) = 5;   k(0,3) = 4;  k(0,4) = 2;
	k(1,0) = 4;	k(1,1) = 9;	 k(1,2) = 12;  k(1,3) = 9;  k(1,4) = 4;
	k(2,0) = 5;	k(2,1) = 12; k(2,2) = 15;  k(2,3) = 12; k(2,4) = 5;
	k(3,0) = 4;	k(3,1) = 9;	 k(3,2) = 12;  k(3,3) = 9;  k(3,4) = 4;
	k(4,0) = 2;	k(4,1) = 4;  k(4,2) = 5;   k(4,3) = 4;  k(4,4) = 2;
	Func scaled;
	scaled(x,y,c) = convolution(in,k,5,5)(x,y,c) / 159;
*/

	Func k("gauss_kernel");
	k(x,y) = 0;
	k(0,0) = 1;	k(0,1) = 4;	 k(0,2) = 7;   k(0,3) = 4;   k(0,4) = 1;
	k(1,0) = 4;	k(1,1) = 16; k(1,2) = 26;  k(1,3) = 16;  k(1,4) = 4;
	k(2,0) = 7;	k(2,1) = 26; k(2,2) = 41;  k(2,3) = 26;  k(2,4) = 7;
	k(3,0) = 4;	k(3,1) = 16; k(3,2) = 26;  k(3,3) = 16;  k(3,4) = 4;
	k(4,0) = 1;	k(4,1) = 4;  k(4,2) = 7;   k(4,3) = 4;   k(4,4) = 1;
	Func scaled;
	scaled(x,y,c) = convolution(in,k,5,5)(x,y,c) / 273.0f;
	return scaled;
}

Func sobelMag(Func in){
	Var x, y, c;
	Expr vs, hs; 

	hs = -1.0f * in(x-1,y-1,c) + /*0*/ 1.0f * in(x+1,y-1,c) +
		-2.0f * in(x-1,y  ,c) + /*0*/ 2.0f * in(x+1,y  ,c) +
		-1.0f * in(x-1,y+1,c) + /*0*/ 1.0f * in(x+1,y+1,c);

	vs = -1.0f * in(x-1,y-1,c) - 2.0f * in(x,y-1,c)  - 1.0f * in(x+1,y-1,c) 		
		+ 1.0f * in(x-1,y+1,c) + 2.0f * in(x,y+1,c) + 1.0f * in(x+1,y+1,c);
	Func mag;
	mag(x,y,c) = sqrt(hs*hs+vs*vs);
	return mag;
}


Func sobelAng(Func in){
	Var x, y, c;
	Expr vs, hs; 

	hs = -1.0f * in(x-1,y-1,c) + /*0*/ 1.0f * in(x+1,y-1,c) +
		-2.0f * in(x-1,y  ,c) + /*0*/ 2.0f * in(x+1,y  ,c) +
		-1.0f * in(x-1,y+1,c) + /*0*/ 1.0f * in(x+1,y+1,c);

	vs = -1.0f * in(x-1,y-1,c) - 2.0f * in(x,y-1,c)  - 1.0f * in(x+1,y-1,c) 		  + 1.0f * in(x-1,y+1,c) + 2.0f * in(x,y+1,c) + 1.0f * in(x+1,y+1,c);
	Func ang;
	Expr bool0 = atan2(hs,vs) - 0.78539
	Expr bool45 = ;
	Expr bool90 = ;
	Expr bool135 = ;

	ang(x,y,c) = ;
	return ang;
}

Func convolution(Func f, Func hx, Expr kernel_width,Expr kernel_height){
	Var x("x2"),y("y2"),c("c2");
	Func convolved("convolved");
	RDom k (0,kernel_width,0,kernel_height);
	convolved(x,y,c) += ( hx(k.x,k.y)*f(x+k.x-(kernel_width/2),y+k.y-(kernel_height/2),c));
	return convolved;
}
