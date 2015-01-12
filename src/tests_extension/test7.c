int main() {

  float a[32];
  float b[32];
  float start;
  float s;
  int i;

  start = -11.;
  s = -50.;

  for (i=0; i < 32; i++){
    b[i] = start;
    start = start + 3.;
  }
	
  #pragma omp simd reduction(+: s)
  for (i=0; i < 32; i++) {
    a[i] = 2. * b[i];
    s = s + a[i];
  }

  printfloat(s);
  for (i = 0; i < 32; i++){
    printfloat(a[i]);
  }

  return 0;
}
