#include <stdio.h>
#include <stdlib.h>

/**
  * Tempo sequêncial:
  * real    0m0.671s
  * user    0m0.443s
  * sys     0m0.217s
  *
  *
  * 
  */

__global__ void scan_cuda(double* a, double *s, int width) {
  // kernel scan
} 

__global__ void add_cuda(double *a, double *s, int width) {
  int t = threadIdx.x;                                                                                                           
  int b = blockIdx.x*blockDim.x; 
  
  __shared__ double o[1024];

  if(b+t < width)
	  o[t] = a[b+t];

  __syncthreads();

  for(i = blockDim.x/2; i > 0; i /= 2) {
	  if(t < i && b+t+i < width)
		  o[t] += o[t+i];

	  __syncthreads();
  }

  if(t == 0)
	  s[blockIdx.x] = o[0];
}

int main()
{
  int width = 40000000;
  int size = width * sizeof(double);

  int block_size = 1024;
  int num_blocks = (width-1)/block_size+1;
  int s_size = (num_blocks * sizeof(double));  
 
  double *a = (double*) malloc (size);
  double *s = (double*) malloc (s_size);

  for(int i = 0; i < width; i++)
    a[i] = i;

  double *d_a, *d_s;

  // alocar vetores "a" e "s" no device
  cudaMalloc((void **) &d_a, size);                                                                                              
  cudaMalloc((void **) &d_s, s_size); 

  // copiar vetor "a" para o device
  cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice); 

  // definição do número de blocos e threads (dimGrid e dimBlock)
  dim3 dimGrid(num_blocks,1,1);                                                                                                  
  dim3 dimBlock(block_size,1,1); 

  // chamada do kernel scan
  scan_cuda<<<dimGrid,dimBlock>>>(d_a, d_s, width);

  // copiar vetor "s" para o host
  cudaMemcpy(s, d_s, s_size, cudaMemcpyDeviceToHost);

  // scan no host (já implementado)
  s[0] = 0;
  for (int i = 1; i < num_blocks; i++)
    s[i] += s[i-1];
 
  // copiar vetor "s" para o device
  cudaMemcpy(s, d_s, s_size, cudaMemcpyDeviceToHost); 

  // chamada do kernel da soma 
  add_cuda<<<dimGrid,dimBlock>>>(d_a, d_s, width);

  // copiar o vetor "a" para o host
  cudaMemcpy(a, d_a, size, cudaMemcpyDeviceToHost);

  printf("\na[%d] = %f\n",width-1,a[width-1]);
  
  cudaFree(d_a);
  cudaFree(d_s);
}
