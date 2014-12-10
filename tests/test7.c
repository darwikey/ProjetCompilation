int main() {

  int a[1000];
  int b[1000];
  int s = 0;
  int i;
	
#pragma omp simd reduction(+: s)
  for (i=0; i < 1000; i++) {
    a[i] = 2 * b[i];
    s = s + a[i];
  }

  return 0;
}
