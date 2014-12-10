
int main() {

	int a[1000];
	
	for (int i = 0; i < 10; i++)
	{
		int b[1000];
		int c[1000];
		int i;
		
		#pragma omp simd
		for (i = 0; i < 1000; i++) {
			a[i] = b[i] + 5.0;
			c[i] = (a[i] + b[i] + c[i]) / 3;
		}
	}
	
    return 0;
}