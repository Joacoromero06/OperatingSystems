#ifndef MATMUL_BENCHMARK_H
#define MATMUL_BENCHMARK_H

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>

typedef struct{
  double *data;
  size_t rows;
  size_t cols;
} Matrix;
double get(Matrix, size_t, size_t);
void set(Matrix, size_t, size_t, double);

void print_matrix(Matrix);
void fill_random_array(double*, size_t); 
void matrix_mul(Matrix, Matrix, Matrix);

// benchmark function
void f();

#endif
