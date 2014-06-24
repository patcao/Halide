#include <Halide.h>

using Halide::Image;

#include "image_io.h"

#include <stdio.h>
#include <iostream>
#include <limits>
#include <memory>
#include <cfloat>
#include <vector>
#include <sys/time.h>
#include <cmath>

using namespace std;
using namespace Halide;

int start,diff,msec;
#define timing(func,name) start = clock(); func; diff = clock() - start; msec = diff * 1000 / CLOCKS_PER_SEC; cout << name << " Timing: " << msec << " ms" << endl;
void sobel();

int main(int argc, char **argv) {
	timing(sobel(),"sobel");
}


void sobel(){
	Image<uint8_t> input = load<uint8_t>("images/rgb.png");

	Var x, y, c;
	Func f("f"),ang;
	Expr vs, hs; 
	Func in;

	int W = input.width();
	int H = input.height();

	Var min,max;
	//Func clam;
	//clam(x,min,max) = select( x < min, min, select(x > max, max , x));
	//Image restricted to just the red layer
	in(x,y,c) = input(clamp(x,0,W-1), clamp(y,0,H-1), 0);


	hs = -1.0f * in(x-1,y-1,c) + /*0*/ 1.0f * in(x+1,y-1,c) +
		-2.0f * in(x-1,y  ,c) + /*0*/ 2.0f * in(x+1,y  ,c) +
		-1.0f * in(x-1,y+1,c) + /*0*/ 1.0f * in(x+1,y+1,c);

	vs = -1.0f * in(x-1,y-1,c) - 2.0f * in(x,y-1,c)  - 1.0f * in(x+1,y-1,c) 		
		+ 1.0f * in(x-1,y+1,c) + 2.0f * in(x,y+1,c) + 1.0f * in(x+1,y+1,c);
	f(x,y,c) = cast<uint8_t>(Halide:: min(sqrt(vs*vs + hs*hs), 255.0f));

/*atan2(vs,hs)*/
	//ang(x,y,c) =  select(45 == atan2(vs,hs),0.0f,45.0f)   ;
	
	Image<uint8_t> output = f.realize( W, H, input.channels() );
	//Image<float> angle = ang.realize(W, H, input.channels());
	//Image<uint8_t> finalImage = final.realize(W, H, input.channels());
	
	//cout << angle(0,0,0) << endl;
	/*
	int threshold = 150;

	for(int x = 0; x < W; ++x){
		for(int y = 0; y < H; ++y){
			
	
		}
	}
	

	save(output, "sobel.png");
	*/
}
