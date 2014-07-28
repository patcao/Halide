#include <Halide.h>

using Halide::Image;
#include "image_io.h"
#include <patlib.h>


using namespace Halide;
using namespace std;

Func convolution(Func f, Func hx, Expr kernel_width,Expr kernel_height);
Func gaussianBlur(Func in);
Func sobelMag(Func in);
Func sobelAng(Func in);

int test_case = 0;
/** TEST CASES
 * Case 0: Default Scheduling
 * Case 1: Only compute root on Gauss
 * Case 2: Only vectorize with tiling
 * Case 3: Only run parallel
 * Case 4: Vectorize with computer root
 * Case 5: Vectorize with computer root and parallel
 **/ 
int main(int argc, char **argv) {
    Image<uint8_t> input = load<uint8_t>("x86/rgb.png");


//    ImageParam input(type_of<uint8_t>(), 3, "input");


	Var x("x"),y("y"),c("c");
	Func in("in");
	in(x,y,c) = input(clamp(x,0,input.width() - 1) , clamp(y,0,input.height() - 1), c);
	Image<uint8_t> output;

	for(int i = 0; i < 6; ++i){
		test_case = i;
		Func mag = sobelMag(gaussianBlur(in));
		printf("Case %d: ", i);
		timing( mag.realize(input.width(), input.height() , 3), "");
	}
printf("Succes! \n");
//	save(output,"both.png");
	
}


//Performs a gaussian blur with a 5x5 gaussian kernel
Func gaussianBlur(Func in){
	Var x("x1"),y("y1"),c("c1");

	Func k("gauss_kernel");
	k(x,y) = 0;
	k(0,0) = 1;	k(0,1) = 4;	 k(0,2) = 7;   k(0,3) = 4;   k(0,4) = 1;
	k(1,0) = 4;	k(1,1) = 16; k(1,2) = 26;  k(1,3) = 16;  k(1,4) = 4;
	k(2,0) = 7;	k(2,1) = 26; k(2,2) = 41;  k(2,3) = 26;  k(2,4) = 7;
	k(3,0) = 4;	k(3,1) = 16; k(3,2) = 26;  k(3,3) = 16;  k(3,4) = 4;
	k(4,0) = 1;	k(4,1) = 4;  k(4,2) = 7;   k(4,3) = 4;   k(4,4) = 1;
	Func scaled;
	scaled(x,y,c) = convolution(in,k,5,5)(x,y,c) / 273.0f;

	switch(test_case){
		case 0: break;
		case 1: scaled.compute_root(); break;
		case 2: break;
		case 3: break;
		case 4: scaled.compute_root(); break;
		case 5: scaled.compute_root(); break;
		default: break;
	}
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
	mag(x,y,c) = cast<uint8_t>(sqrt(hs*hs+vs*vs));

	Var x_outer, y_outer, x_inner, y_inner, tile_index;
	Var x_inner_outer, y_inner_outer, x_vectors, y_pairs;


/** TEST CASES
 * Case 0: Default Scheduling
 * Case 1: Only compute root on Gauss
 * Case 2: Only vectorize with tiling
 * Case 3: Only run parallel
 * Case 4: Vectorize with computer root
 * Case 5: Vectorize with computer root and parallel
 **/ 
	switch(test_case){
		case 0: break;
		case 1: break;
		case 2: 
			mag.tile(x,y,x_outer,y_outer,x_inner,y_inner,256,256);
			mag
			.tile(x_inner, y_inner, x_inner_outer, y_inner_outer, x_vectors, y_pairs, 4, 2)
			.vectorize(x_vectors)
			.unroll(y_pairs);
			break;
		case 3: 
			mag.tile(x,y,x_outer,y_outer,x_inner,y_inner,256,256)
			.fuse(x_outer,y_outer,tile_index)
			.parallel(tile_index);
			break;
		case 4:
			mag.tile(x,y,x_outer,y_outer,x_inner,y_inner,256,256);
			mag
			.tile(x_inner, y_inner, x_inner_outer, y_inner_outer, x_vectors, y_pairs, 4, 2)
			.vectorize(x_vectors)
			.unroll(y_pairs);
			break;
		case 5: 	
			mag.tile(x,y,x_outer,y_outer,x_inner,y_inner,256,256)
			.fuse(x_outer,y_outer,tile_index)
			.parallel(tile_index);
			mag
			.tile(x_inner, y_inner, x_inner_outer, y_inner_outer, x_vectors, y_pairs, 4, 2)
			.vectorize(x_vectors)
			.unroll(y_pairs);
			break;
		default: break;
	}
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
	Expr bool0 = atan2(hs,vs) - 0.78539;
//	Expr bool45 = ;
//	Expr bool90 = ;
//	Expr bool135 = ;

	ang(x,y,c) = 5;
	return ang;
}

Func convolution(Func f, Func hx, Expr kernel_width,Expr kernel_height){
	Var x("x2"),y("y2"),c("c2");
	Func convolved("convolved");
	RDom k (0,kernel_width,0,kernel_height);
	convolved(x,y,c) += ( hx(k.x,k.y)*f(x+k.x-(kernel_width/2),y+k.y-(kernel_height/2),c));
	return convolved;
}
