#include <stdio.h>
#include <stdlib.h>


int main() 
{
	int n = 40000000;
	double* vector = (double*) malloc(n * sizeof(double));
	double sum = 0;
	
	for(int x = 0; x < n; ++x)
		vector[x] = x;
	#pragma omp parallel for simd reduction(+: sum) schedule(dynamic)
	for(int y = 0; y < n; ++y)
		sum += vector[y];

//	printf("%lf", sum);

	free(vector);
	vector = NULL;

	return 0;
}
