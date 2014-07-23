
#include <Halide.h>

using Halide::Image;
#include "image_io.h"

using namespace Halide;
using namespace std;

int main(int argc, char **argv) {

    ImageParam input(type_of<uint8_t>(), 3, "input");
//    Image<uint8_t> input = load<uint8_t>("rgb.png");

	Var x("x"),y("y"),c;
	Func ret("ret");
	ret(x,y,c) = cast<uint8_t> ( input(x,y,c) );
	
	//Image<uint8_t> output = ret.realize(input.width(), input.height(), 3);
	
	
	std::vector<Argument> args;
	args.push_back(input);
	ret.compile_to_file("bright", args);
	
	//save(input,"brighter.png");
}
