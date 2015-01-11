
  float a[1000];


int main() {

  int i;
  float b[1000];
  float start;
  float cst;

  start = 0.;
  cst = -10.;
  
  for (i=0; i < 16; i++){
    b[i] = start;
    start = start + 1.;
  }

  #pragma omp simd
  for (i=0; i < 16; i++) {
    a[i] = b[i] + 5.0;
    a[i] = a[i] - cst;
  }

  for (i=0; i < 16; i++)
    printfloat(a[i]);

  return 0;
}
