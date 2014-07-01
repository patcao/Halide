#include <stdio.h>
#include <Halide.h>
#include <math.h>

using namespace Halide;
using namespace std;

int main(){
	Var x;
	
	Func fib;
	fib(x) = 1;
	RDom r (2,15);
	fib(r) = fib(r-1) + fib(r-2);
	
	//Image<int> output = fib.realize(18);

	
	Func f;
	f(x) = tan(-1.0f);
	Image<float> test = f.realize(50);
	cout << test(45) << endl;
	
	std::vector<Argument> args;
	f.compile_to_file("fib",args);

	return 0;
}
