#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define alloc(type) ((type*) malloc(sizeof(type)))
#define alloc_arr(type, size) ((type*) malloc(sizeof(type)*size))
#define dbg(str, exp, show) (printf("%s == ", str), show(exp))

void showint(int x){ printf("%d", x); }
int main() {
  int* x = alloc(int);
  int* a = alloc_arr(int, 10);
  char* s = "hola";
  char* sh = alloc_arr(char, strlen(s) + 1);


  assert(sizeof(x) == 8);
  dbg("sizeof(a)", sizeof(a), showint);
  //assert(sizeof(a) == 40);
  return 0;
}
