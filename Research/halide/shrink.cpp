#include <Halide.h>
#include <stdio.h>
using Halide::Image;
#include "image_io.h"
//#include <time.h>
//#include <sys/time.h>
#include <patlib.h>
#include <math.h>

using namespace std;
using namespace Halide;
/*
int start,diff,msec;
#define timing(func,name) start = clock(); func; diff = clock() - start; msec = diff * 1000 / CLOCKS_PER_SEC; cout << name << " Timing: " << msec << " ms" << endl;
*/

Image<uint8_t> parallel(Func in, int W, int H,int xsplit, int ysplit);
Image<uint8_t> normal(Func in,int W, int H);
void cpp(uint8_t in[][768][3],int W, int H);
void pgradient(int W, int H);
void gradient (int W, int H);


//2706 1668
//768 1280
int main(){
	
	Image<uint8_t> input = load<uint8_t>("../images/rgb.png");

	int W = input.width(), H = input.height();
	int C = input.channels();
	Var x,y,c;
	//Func in("input"),f("parallel");
	//in(x,y,c) = (sqrt(input(clamp(x,0,W-1),clamp(y,0,H-1),c)));
	
	/*
	uint8_t array[1280][768][3];

	for(int row = 0; row < H; ++row)
		for(int col = 0; col < W; ++col)
			for(int dim = 0; dim < C; ++dim)
				array[row][col][dim] =input(col,row,dim);
	*/
//	cout << W << "   " << H;
	//timing(cpp(array,W,H),"c++");

/*
	timing(normal(in,W/2,H/2), "Normal Half Image");
	timing(parallel(in,W/2,H/2,256,256), "Parallel Half Image");
	timing(cpp(in,W/2,H/2), "C++ Half Image");
*/

	timing(normal(in,W,H),"Normal Full Image");
/*
	timing(parallel(in,W,H,256,256),"Parallel Full Image");
	timing(cpp(in,W,H), "C++ Full Image");
*/


/*
timing(parallel(in,W/2,H/2,256,256),"Optimized Half Image");
timing(parallel(in,W,H,256,256), "Optimized Full Image");
//timing(cpp(in,W,H), "C++:");
	

	timing(
	for(int i = 0; i < 10; ++i)
		parallel(in,W,H,256,256);
		, "10 Runs Parallel");

*/

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

void cpp(uint8_t in[][768][3],int W, int H){
		for(int x = 0; x < W; ++x)
			for(int y = 0; y < H; ++y)
				for(int d = 0; d < 3; ++d)
					if(2*x + 1 < W && 2*y + 1 > H)
						sqrt((in[2*y][2*x][d] + in[2*y][2*x + 1][d] + in[2*y + 1][2*x][d] + in[2*y+1][2*x+1][d]) / 4.0f);
}


void pgradient(int W, int H){
	Var x,y;
	Func gradient_fast("gradient_fast");
	gradient_fast(x, y) = x + y;

	Var x_outer, y_outer, x_inner, y_inner, tile_index;
	gradient_fast
		.tile(x, y, x_outer, y_outer, x_inner, y_inner, 256, 256);
		//.fuse(x_outer, y_outer, tile_index)
		//.parallel(tile_index);

	Var x_inner_outer, y_inner_outer, x_vectors, y_pairs;
	gradient_fast
		.tile(x_inner, y_inner, x_inner_outer, y_inner_outer, x_vectors, y_pairs, 4, 2)
		.vectorize(x_vectors)
		.unroll(y_pairs);

	gradient_fast.realize(W,H);
}
void gradient(int W, int H){
	Var x,y;
	Func gradient("gradient");
	gradient(x,y) = x + y;
	gradient.realize(W,H);
}
/*
uint64_t GetTimeStamp() {
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return tv.tv_sec*(uint64_t)1000000+tv.tv_usec;
}
*/
