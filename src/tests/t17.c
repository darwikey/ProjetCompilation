float f(float x) {
  float a;
  float b;
  a=1000.;
  b=20123.*x;
  return a+b*3.-8.;
}
int main() {
  float a;
  a = 0.001;
  printfloat(f(a));
  return 0;
}
