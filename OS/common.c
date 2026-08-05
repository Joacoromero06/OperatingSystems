#include "common.h"

void Spin(int seconds) 
{
  struct timeval start, now;
  gettimeofday(&start, NULL);
  while(1) {
    gettimeofday(&now, NULL);
    double elapsed = 
      (now.tv_sec - start.tv_sec) +
      (now.tv_usec - start.tv_usec) / 1000000.0;
    if (elapsed >= seconds)
      break;
  }
}
