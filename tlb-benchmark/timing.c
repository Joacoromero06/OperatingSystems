#include <sys/time.h>
#include <assert.h>
#include <stdio.h>
#include "matmul_benchmark.h"

void f();

void time_elapsed(struct timeval s, struct timeval e)
{
  printf("Time elapsed: sec %ld. usec %ld\n", e.tv_sec - s.tv_sec, e.tv_usec - s.tv_usec);
}
int main()
{
  struct timeval start, end;

  int r = gettimeofday(&start, NULL);
  assert(r == 0);

  f();

  r = gettimeofday(&end, NULL);
  assert(r == 0);
  
  time_elapsed(start, end);
  return 0;
}







