
int main() {

	int a[1000];
	int b[1000];
	int s = 0;
	int i;
	
	#pragma omp simd reduction(+: s)
	for (i=0; i < 1000; i++) {
		if (s == 2) // error : forbidden
		{
			s = s + a[i];
		}
	}

    return 0;
}