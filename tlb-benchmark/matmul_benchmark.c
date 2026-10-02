#include "matmul_benchmark.h"

#define m 500
#define N 1000
#define K 1500
double a[m*N];
double b[N*K];
double c[m*K];


double get(Matrix M, size_t i, size_t j)
{
  assert(0 <= i && i < M.rows && "error get i fuera de bounds\n");
  assert(0 <= j && j < M.cols && "error get j fuera de bounds\n");
  return M.data[i*M.cols + j];
}
void set(Matrix M, size_t i, size_t j, double x)
{
  assert(0 <= i && i < M.rows && "error get i fuera de bounds\n");
  assert(0 <= j && j < M.cols && "error get j fuera de bounds\n");
  M.data[i*M.cols + j] = x;
}
void print_matrix(Matrix M)
{
  printf("Matrix: \n\n");
  printf("[");
  for (int i=0; i < M.rows; ++i)
  {
    printf( (i==0)? "[": ",[" );
    for (int j=0; j < M.cols; ++j)
    {
      printf( ((j==0)? "%lf" : ", %lf"), get(M, i,j));
    }
    printf("]\n");
  } 
  printf("]");
}

void fill_random_array(double* a, size_t n);
void matrix_mul(Matrix A, Matrix B, Matrix C);
void f()
{
  Matrix A = {.data=a, .rows=m, .cols=N};
  Matrix B = {.data=b, .rows=N, .cols=K};
  Matrix C = {.data=c, .rows=m, .cols=K};

  fill_random_array(A.data, A.rows*A.cols);
  fill_random_array(B.data, B.rows*B.cols);

  matrix_mul(A, B, C);
  print_matrix(A);
  print_matrix(B);
  print_matrix(C);
}

void fill_random_array(double* a, size_t n)
{
  for (int i = 0; i < n; ++i) 
  {
    a[i] = (double)random() / INT_MAX;
  }
}

void matrix_mul(Matrix A, Matrix B, Matrix C)
{
  assert(A.cols == B.rows && "Numero de columans de A debe coincidir con el numero de filas de B para matmul");

  double dot_prod;
  for (int i=0; i < A.rows; ++i)
  {
    for (int j=0; j < B.cols; ++j)
    {
      dot_prod = 0;
      for (int k = 0; k < A.cols; ++k)
      {
        dot_prod += get(A, i, k) * get(B, k, j);
      }
      set(C, i, j, dot_prod);
    }
  }
}


