
int main() {

	int a[1000];
	int s = 0;
	int i;
	
	#pragma omp simd reduction(+: i) //error : forbidden
	for (i=0; i < 1000; i++) {
		
		s = s + a[i];

	}

    return 0;
}