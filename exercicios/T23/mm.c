#include <stdio.h>
#include <stdlib.h>

/*
 * Tempo Sequencial:
 * real    1m9.236s
 * user    1m8.004s
 * sys     0m0.132s
 *
 * Tempo multicore:
 * real    0m17.290s
 * user    1m8.421s
 * sys     0m0.148s
 *
 * Tempo, warps_launched e warp_execution_efficiency GPU OpenMP:
 *
 * real    0m5.384s
 * user    0m3.656s
 * sys     0m1.756s
 * 
 * ==21740== NVPROF is profiling process 21740, command: ./mmGPU
 * ==21740== Some kernel(s) will be replayed on device 0 in order to collect all events/metrics.
 * ==21740== Replaying kernel "mm$_omp_fn$0" (done)           
 * ==21740== Profiling application: ./mmGPU
 * ==21740== Profiling result:
 * ==21740== Event result:
 * Invocations                                Event Name         Min         Max         Avg       Total
 * Device "GeForce GT 1030 (0)"
 *     Kernel: mm$_omp_fn$0
 *               1                            warps_launched          72          72          72          72
 *
 *               ==21740== Metric result:
 *               Invocations                               Metric Name                        Metric Description         Min         Max         Avg
 *               Device "GeForce GT 1030 (0)"
 *                   Kernel: mm$_omp_fn$0
 *                             1                 warp_execution_efficiency                 Warp Execution Efficiency      86.81%      86.81%      86.81%
 *
 *
 *
 *
 */



void mm(double* a, double* b, double* c, int width) 
{
//  #pragma omp parallel for schedule(dynamic)
	#pragma omp target map(to:a[0:width*width], b[0:width*width]) map(from:c[0:width*width])
	#pragma omp teams distribute parallel for simd
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
	int width = 2000;
	double *a = (double*) malloc (width * width * sizeof(double));
    double *b = (double*) malloc (width * width * sizeof(double));
    double *c = (double*) malloc (width * width * sizeof(double));
  
	#pragma omp parallel for schedule(dynamic)
    for(int i = 0; i < width; i++) {	  
      for(int j = 0; j < width; j++) {
        a[i*width+j] = i;
        b[i*width+j] = j;
        c[i*width+j] = 0;
      }
    }

    mm(a,b,c,width);

    /*	 
    for(int i = 0; i < width; i++) {
      for(int j = 0; j < width; j++) {
        printf("\n c[%d][%d] = %f",i,j,c[i*width+j]);
      }
    }*/
	

}
