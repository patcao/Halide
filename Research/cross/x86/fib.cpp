#include <stdio.h>
#include <Halide.h>
#include <math.h>

using namespace Halide;
#include "image_io.h"

using namespace std;

int main(){
	Var x;
	
	Func fib;
	fib(x) = cast<uint8_t>(1);
	RDom r (2,15);
	fib(r) = fib(r-1) + fib(r-2);
	
	//Image<int> output = fib.realize(18);

	
	Func f;
	f(x) = cast<uint8_t>(8);
	Image<uint8_t> test = f.realize(50);
	cout << (int)test(45) << endl;
	
	std::vector<Argument> args;
	f.compile_to_file("fib",args);
	
	save(test,"output.png");
	return 0;
}
