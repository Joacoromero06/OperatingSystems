#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <assert.h>

#define N (64 * 1024 * 1024)

int arr[N];
double test(int);

int main(int argc, char* argv[]){
  int stride = 1, stride_ant, aux;
  char* fn;
  switch(argc){
  case 0: exit(1);
  case 1: fn = "cacheBenchmark.csv"; break;
  default: fn = argv[1];
  }
  FILE* f = fopen(fn, "w");
  if (!f) {perror("file error at fopen\n");}
  fprintf(f, "stride,strideInBytes,timeSeconds,iterations,iterationRatioSeconds\n");

  stride_ant = stride; // x0 x1 = 1, 1
  while (stride <= 12000){
    double time = test(stride);
    int iterations = (int) (N/stride);
    printf("Stride=%d\nTime taken 64MB (%lf)s\nN° iteration: %d\n\n", stride, time, iterations);
    fprintf(f, "%d,%ld,%lf,%d,%lf\n", stride, stride*sizeof(int), time, iterations, iterations/time);
    
    aux = stride + stride_ant;
    stride_ant = stride;
    stride = aux;
  }
  return 0;
}
float computeSec(struct timespec* x){return x->tv_sec + x->tv_nsec / 1e9;}
float computeElapsedSec(struct timespec* l, struct timespec* f){
  return computeSec(l) - computeSec(f);
}

double test(int stride){
  struct timespec start, end;
  int sum; 

  clock_gettime(CLOCK_MONOTONIC, &start);
  for(int i = 0; i < N; i += stride){
    sum += arr[i];
    arr[i] = sum * 2;
  }
  clock_gettime(CLOCK_MONOTONIC, &end);
  return computeElapsedSec(&end, &start);
}

