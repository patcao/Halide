#include <stdio.h>
#include <stdlib.h>
#include "fib.h"

int main(){
	buffer_t* b =(buffer_t *) malloc(sizeof(buffer_t));
	fib(b);
	return 0;
}
