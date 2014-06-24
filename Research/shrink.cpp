#include <Halide.h>
#include <stdio.h>
using Halide::Image;
#include "image_io.h"
#include <time.h>
#include <sys/time.h>
using namespace std;
using namespace Halide;
int start,diff,msec;
#define timing(func,name) start = clock(); func; diff = clock() - start; msec = diff * 1000 / CLOCKS_PER_SEC; cout << name << " Timing: " << msec << " ms" << endl;


Image<uint8_t> parallel(Func in, int W, int H,int xsplit, int ysplit);
Image<uint8_t> normal(Func in,int W, int H);
Image<uint8_t> cpp(Func in,int W, int H);
void pgradient(Func in, int W, int H);
void gradient(Func in, int W, int H);

uint64_t GetTimeStamp();

int main(){
	
	Image<uint8_t> input = load<uint8_t>("images/hres-freeway.png");

	int W = input.width(), H = input.height();

	Var x,y,c;
	Func in("input"),f("parallel");
	in(x,y,c) = (sqrt(input(clamp(x,0,W-1),clamp(y,0,H-1),c)));
	
/*	
	timing(normal(in,W/2,H/2), "Normal Half Image");
	timing(parallel(in,W/2,H/2,256,256), "Parallel Half Image");
	timing(cpp(in,W/2,H/2), "C++ Half Image");

	timing(normal(in,W,H),"Normal Full Image");
	timing(parallel(in,W,H,256,256),"Parallel Full Image");
	timing(cpp(in,W,H), "C++ Full Image");
*/


timing(parallel(in,W/2,H/2,256,256),"Optimized Half Image");
timing(parallel(in,W,H,256,256), "Optimized Full Image");
//timing(cpp(in,W,H), "C++:");
	

	timing(
	for(int i = 0; i < 10; ++i)
		parallel(in,W,H,256,256);
		, "10 Runs Parallel");

/*
	timing(
		for(int i = 0; i < 100; ++i)
			normal(in,W,H);
			,"100 Runs Normal");
*/

/*
	Image<uint8_t> output2 = normal(in,W,H);
	Image<uint8_t> output3 = cpp(in,W,H);
*/


	//save(output, "shrunk.png");
	//save(output2, "shrunk2.png");
	return 0;
}

Image<uint8_t> parallel(Func in, int W, int H,int xsplit,int ysplit){

	Var x,y,c;
	Func f("parallel");
	f(x,y,c) = cast<uint8_t>((in(2*x,2*y,c) + in(2*x+1, 2*y,c) + in(2*x,2*y+1,c) + in(2*x+1,2*y+1,c))/ 4);
	Var x_outer, y_outer, x_inner, y_inner, tile_index;
	
	f.tile(x,y,x_outer,y_outer,x_inner,y_inner,xsplit,ysplit);
	 //.fuse(x_outer,y_outer,tile_index)
	 //.parallel(tile_index);

	 Var x_inner_outer, y_inner_outer, x_vectors, y_pairs;
	 
	f
	.tile(x_inner, y_inner, x_inner_outer, y_inner_outer, x_vectors, y_pairs, 4, 2)
	.vectorize(x_vectors)
	.unroll(y_pairs);

	//f.parallel(c);

	//in.trace_stores();
	//f.trace_stores();
	
	
	Image<uint8_t> output = f.realize(W,H,3);
	return output;
}

Image<uint8_t> normal(Func in,int W, int H){
	Var x,y,c;
	Func g;
	g(x,y,c) =cast<uint8_t>((in(2*x,2*y,c) + in(2*x+1, 2*y,c) + in(2*x,2*y+1,c) + in(2*x+1,2*y+1,c))/ 4);

	Image<uint8_t> output2 = g.realize(W,H,3);
	return output2;
}

Image<uint8_t> cpp(Func in,int W, int H){
	Image<uint8_t> ret(W/2,H/2,3);

	for(int c = 0; c < 3; ++c)
		for(int x = 0; x < W/2; ++x)
			for(int y = 0; y < H/2; ++y)
				cast<uint8_t> ((in(2*x,2*y,c) + in(2*x+1, 2*y,c) + in(2*x,2*y+1,c) + in(2*x+1,2*y+1,c))/ 4);
	return ret;
}


void pgradient(Func in,int W, int H){
	Var x,y;
	Func gradient_fast("gradient_fast");
	gradient_fast(x, y) = x + y;

	Var x_outer, y_outer, x_inner, y_inner, tile_index;
	gradient_fast
		.tile(x, y, x_outer, y_outer, x_inner, y_inner, 256, 256)
		.fuse(x_outer, y_outer, tile_index)
		.parallel(tile_index);

	Var x_inner_outer, y_inner_outer, x_vectors, y_pairs;
/*	gradient_fast
		.tile(x_inner, y_inner, x_inner_outer, y_inner_outer, x_vectors, y_pairs, 4, 2)
		.vectorize(x_vectors)
		.unroll(y_pairs);
*/
	gradient_fast.realize(W,H);
}
void gradient(Func in, int W, int H){
	Var x,y;
	Func gradient("gradient");
	gradient(x,y) = x + y;
	gradient.realize(W,H);
}

uint64_t GetTimeStamp() {
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return tv.tv_sec*(uint64_t)1000000+tv.tv_usec;
}
