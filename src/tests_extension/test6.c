int main() {

  float a[8];
  float s;
  float start;
  int i;
  s = 0.;
  start = 0.;

  for (i=0; i < 8; i++){
    a[i] = start;
    start = start + 1.;
  }

#pragma omp simd reduction(+: s)
  for (i=0; i < 8; i++) {
    s = s + a[i];
  }

  printfloat(s);
  for (i = 0; i < 8; i++){
    printfloat(a[i]);
  }
  return 0;
}
