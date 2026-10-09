#include <stdio.h>
#include <stdarg.h>

int sum(int n, ...);
int acum(int acc, ...);
int main() {
  int s = sum(3, 1, 2, 3);
  int a = acum(0, 1, 2, 3);

  printf("%d\n", s);
  printf("%d\n", a);

  return 0;
}

int sum(int n, ...) {
  int s = 0;

  va_list args;
  va_start(args, n);
  for (int i = 0; i < n; ++i) s += va_arg(args, int);
  va_end(args);

  return s;
}
int acum(int acc, ...) {

  va_list args;
  va_start(args, acc);
  while (va_arg(args, int) != 0) acc += va_arg(args, int);

  return acc;
}
