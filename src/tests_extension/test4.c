int main() {

  float a[1000];
  int i;

  for (i = 0; i < 10; i++)
    {
      float b[1000];
      float c[1000];
		
      int j;
      
      #pragma omp simd
      for (j = 0; j < 1000; j++) {
	a[j] = b[j] + 5.0;
	c[j] = (a[j] + b[j] + c[j]) * 3.;
      }
    }
	
  return 0;
}
