float sqrtf(float c);

int main() {

  float a[1000];
  float b[1000];
  int i;

  for (i=0; i < 1000; i++)
    b[i] = 100.;

	
#pragma omp simd
  for (i=0; i < 1000; i++) {
    a[i] = sqrtf(b[i]) + 5.0;
  }

  for (i=0; i < 1000; i++){
    printfloat(a[i]);
  }

  return 0;
}
