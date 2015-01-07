int main() {

  float a[1000];
  float b[1000];
  int i;
  
  for (i=0; i < 16; i++)
    b[i] = 1.;

#pragma omp simd
  for (i=0; i < 16; i++) {
    a[i] = b[i] + 5.0;
  }

    for (i=0; i < 16; i++)
      printfloat(a[i]);

  return 0;
}
