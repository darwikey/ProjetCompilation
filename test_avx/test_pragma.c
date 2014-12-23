#include <stdio.h>

#define N 262144

int main(void) {

	int a[N], b[N];

#pragma omp simd
	for(int i=0; i<N; i++) {
		
		a[i] = i;
		b[i] = N + 1 - i;
	}

	return 0;
}
