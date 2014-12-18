int main() {

	int a[1000];
	int i;
	for (int i = 0; i < 10; i++)
	{
		int b[1000];
		int c[1000];
		
		int j;
		#pragma omp simd
		for (j = 0; j < 1000; j++) {
			a[j] = b[j] + 5.0;
			c[j] = (a[j] + b[j] + c[j]) / 3;
		}
	}
	
	return 0;
}
