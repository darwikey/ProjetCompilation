float sqrt(float);

int main() {

	float a[1000];
	float b[1000];
	int i;
	
	#pragma omp simd
	for (i=0; i < 1000; i++) {
		a[i] = sqrtf(b[i]) + 5.0;
	}

    return 0;
}