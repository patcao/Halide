
#include <Halide.h>
#include <stdio.h>
using namespace Halide;

int main(int argc, char **argv) {

    Var n;
    Func fib;
	
	fib(n)=fib(n-1)+fib(n-2);
	fib(1) = 1;
	fib(2) = 2;


    fib.trace_stores();
    Image<uint8_t> ret = fib.realize(4);

    return 0;
}
