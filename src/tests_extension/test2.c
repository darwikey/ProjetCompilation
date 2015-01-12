int main() {

  float a[1000];
  float b[1000];
  float c[1000];
  int i;
	
#pragma omp simd
  for (i=500; i < 1000; i++) {
    a[i] = b[i] + 5.0;
    c[i] = 3. * a[i] - b[i];
  }

  return 0;
}
