/*#include <Halide.h>
#include "image_io.h"

using namespace Halide;
using Halide::Image;
int main(int argc, char **argv) {
    
        Var x,y,c;
        Halide::Image<uint8_t> input = load<uint8_t>("images/rgb.png");

        Func input_16;
        input_16(x,y,c) = cast<uint16_t> (input(x,y,c));



        Func shrink;
        shrink(x,y,c) = (input_16(x+1,y,c) + input_16(x+1,y+1,c) + input_16(x,y+1,c) + input_16(x,y,c) )_ / 4;

        Func output;
        output(x,y,c) = cast<uint8_t> (shrink(x,y,c));

        Image<uint8_t> final(input.width() -1,input.height() - 1,3) ;

        output.realize(final);

        save(final,"shrunk_image.png");
   
    return 0;
}
*/


// Halide tutorial lesson 2.

// This lesson demonstrates how to pass in input images.

// On linux, you can compile and run it like so:
// g++ lesson_02*.cpp -g -I ../include -L ../bin -lHalide `libpng-config --cflags --ldflags` -o lesson_02
// LD_LIBRARY_PATH=../bin ./lesson_02

// On os x:
// g++ lesson_02*.cpp -g -I ../include -L ../bin -lHalide `libpng-config --cflags --ldflags` -o lesson_02
// DYLD_LIBRARY_PATH=../bin ./lesson_02

// The only Halide header file you need is Halide.h. It includes all of Halide.
#include <Halide.h>

// Include some support code for loading pngs. It assumes there's an
// Image type, so we'll pull the one from Halide namespace;
using Halide::Image;
#include "image_io.h"
using namespace Halide;

int main(int argc, char **argv) {
    
        Var x,y,c;
        Image<uint8_t> input = load<uint8_t>("images/rgb.png");

        Func input_16;
        input_16(x,y,c) = cast<uint16_t> (input(x,y,c));



        Func shrink;
        shrink(x,y,c) = (input_16(x+1,y,c) + input_16(x+1,y+1,c) + input_16(x,y+1,c) + input_16(x,y,c) ) / 4;

        Func output;
        output(x,y,c) = cast<uint8_t> (shrink(x,y,c));

        Image<uint8_t> final = output.realize(input.width() / 2, input.height() / 2, input.channels()) ;

        save(final,"shrunk_image.png");
 
    return 0;
}
