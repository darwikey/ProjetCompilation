int main() {

  int a[1000];
  int b[1000];
  int c[1000];
  int i;
	
#pragma omp simd
  for (i=0; i < 1000; i++) {
    a[i] = b[i] + 5.0;
    c[i] = 3 * a[i] / b[i];
  }

  return 0;
}
