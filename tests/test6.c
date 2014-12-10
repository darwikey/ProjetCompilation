int main() {

  float a[1000];
  float s = 0;
  int i;
	
#pragma omp simd reduction(+: s)
  for (i=0; i < 1000; i++) {
    s = s + a[i];
  }

  return 0;
}
