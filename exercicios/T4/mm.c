/*
 * Programa testado no PARCODE
 *
 * Sequencial:
 * real	1m56.258s
 * user	1m56.081s
 * sys	0m0.080s
 * 
 * 
 * Paralelo:
 * real	0m37.209s
 * user	2m28.415s
 * sys	0m0.140s
 */
#include <stdio.h>
#include <stdlib.h>

void mm(double *a, double *b, double *c, int width)
{
  #pragma omp parallel for schedule(guided)
  for (int i = 0; i < width; i++)
  {
    for (int j = 0; j < width; j++)
    {
      double sum = 0;
      for (int k = 0; k < width; k++)
      {
        double x = a[i * width + k];
        double y = b[k * width + j];
        sum += x * y;
      }
      c[i * width + j] = sum;
    }
  }
}

int main()
{
  int width = 1000;
  double *a = (double *)malloc(width * width * sizeof(double));
  double *b = (double *)malloc(width * width * sizeof(double));
  double *c = (double *)malloc(width * width * sizeof(double));

  #pragma omp parallel for schedule(dynamic)
  for (int i = 0; i < width; i++)
  {
	#pragma omp simd
    for (int j = 0; j < width; j++)
    {
      a[i * width + j] = i;
      b[i * width + j] = j;
      c[i * width + j] = 0;
    }
  }

  mm(a, b, c, width);
  
  //  for(int i = 0; i < width; i++) {
  //  for(int j = 0; j < width; j++) {
  //    printf("\n c[%d][%d] = %f",i,j,c[i*width+j]);
  //  }
  // }
  
}
