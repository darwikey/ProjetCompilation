#include <stdio.h>


int f(int x){
  return x+4;
}

int main() {
  int a = 1;
  
  int b = f(a);

  printf("%d", b);

  return 0;
}
