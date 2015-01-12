
int main() {

  float a[8];
  float s;
  float start;
  int i;
  s = 0.;
  start = 1.;

  for (i=0; i < 8; i++){
    a[i] = start;
    start = start + 3.;
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
