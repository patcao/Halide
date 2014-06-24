#include <Halide.h>

using namespace Halide;
using Halide::Image;
#include "image_io.h"

#include <iostream>
#include <limits>
#include <memory>
#include <cfloat>
#include <vector>
#include <sys/time.h>

int main(int argc, char **argv) {


	Image<uint8_t> input1;
	input1 = load<uint8_t>("images/rgb.png");
	int H = input1.height() , W = input1.width();
	Var x("x"), y("y"), yo("yo"), yi("yi"), xo("xo"), xi("xi");

	Func 
		grad_x_h("grad_x_h"),
		grad_x_v("grad_x_v"),
		grad_y_v("grad_y_v"),
		grad_y_h("grad_y_h"),
		magnitude("magnitude"),
		input("input");

	input(x,y) = input1(clamp(x,0,input1.width() - 1),clamp(y,0,input1.height() - 1),0);

	grad_x_h(x,y) = (input(x,   y)
			- input(x+2, y));

	grad_x_v(x,y) = (grad_x_h(x, y  )
			+ grad_x_h(x, y+1) * 2.0f
			+ grad_x_h(x, y+2));

	grad_y_h(x,y) = (input(x,   y)
			+ input(x+1, y) * 2.0f
			+ input(x+2, y));

	grad_y_v(x,y) = (grad_y_h(x,y  )
			- grad_y_h(x,y+2));

	magnitude(x,y) = (grad_x_v(x,y) * grad_x_v(x,y)
			+ grad_y_v(x,y) * grad_y_v(x,y));




	//grad_x_h.parallel(y);
	//grad_x_v.parallel(y);
	//grad_y_h.parallel(y);
	//grad_y_v.parallel(y);
	//magnitude.root().parallel(y);
	
	Image<float> output = magnitude.realize(input1.width(),input1.height());

	save(output, "image.png");


}
