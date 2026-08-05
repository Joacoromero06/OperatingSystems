#ifndef common_h
#define common_h

#include <sys/time.h>
#include <assert.h>
#include <stdlib.h>
double getTime() {
  struct timeval t;
  int rc = gettimeofday(&t, NULL);
  assert(rc == 0);
  return (double) t.tv_sec + (double) t.tv_usec / 1e6;
}

void Spin(int seconds) {
  double t = getTime();
  while ( getTime() - t < (double) seconds )
  ;
}
#endif
