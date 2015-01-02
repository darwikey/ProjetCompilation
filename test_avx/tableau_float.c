#include <stdio.h>

#define N 1024

int main(void) {

	float a[N], b[N], s[N];

	for(int i=0; i<N; i++) {

		a[i] = b[i] = i;
		s[i] = a[i] + b[i] + i;
		
		printf("%f = %f + %f + %d\n", s[i], a[i], b[i], i);	
	}
	
	return 0;
}
