int main() {

  float a[1000];
  float b[1000];
  int i;
	
#pragma omp simd
  for (i=0; i < 16; i++) {
    a[i] = b[i] + 5.0;
  }

  return 0;
}
