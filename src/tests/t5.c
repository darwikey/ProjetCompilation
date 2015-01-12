void *malloc(int x);

int f(void* t){
  return t[10];
}

int main() {
  int *a;
  a = malloc(1000);
  a[10]=3;
  printint(f(a));
  return 0;
}
