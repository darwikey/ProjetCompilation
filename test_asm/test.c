#include <stdio.h>
#include <stdlib.h>

void f(float i)
{
	printf("%f\n", i);
}

int main(void) {
    //printf("Hello, world!\n");
    
	int plop = 11;
	
	plop *= 22;
	
	f(42.2);
	
	return 0;
}