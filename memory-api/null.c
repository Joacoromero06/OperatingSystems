#include <stdlib.h>
int main()
{
  int x = 10;
  int* p = &x;
  p = NULL;
  int y = *p;
  return 0;
}
