/*
 * Adapted from: http://w...content-available-to-author-only...s.org/sieve-of-eratosthenes
 *
 * Programa testado no PARCODE
 * 
 * Tempo sequencial:
 * real	0m4.412s
 * user	0m4.318s
 * sys	0m0.080s
 * 
 * Tempo paralelo:
 * real	0m3.659s
 * user	0m6.781s
 * sys	0m0.080s
 *
 * Tempo paralelo com escalonamento:
 * real	0m2.487s
 * user	0m9.562s
 * sys	0m0.084s
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <omp.h>
int sieveOfEratosthenes(int n)
{
    // Create a boolean array "prime[0..n]" and initialize
    // all entries it as true. A value in prime[i] will
    // finally be false if i is Not a prime, else true.
	int nthreads = 2;
    int primes = 0;
    bool **prime = (bool **)malloc(nthreads * sizeof(bool*));
    //bool *prime = (bool *)malloc((n + 1) * sizeof(bool));
    int sqrt_n = sqrt(n);

    //memset(prime, true, (n + 1) * sizeof(bool));
	
	//#pragma omp parallel for 
	#pragma omp parallel num_threads(nthreads)
	{
		int tid = omp_get_thread_num();

		prime[tid] = (bool *) malloc((n + 1) * sizeof(bool)); // Criar um vetor por thread
		memset(prime[tid], true, (n + 1) * sizeof(bool)); // preencher vetor

		#pragma omp for schedule(dynamic)
	    for (int p = 2; p <= sqrt_n; p++)
	    {
	        // If prime[p] is not changed, then it is a prime
	        if (prime[tid][p] == true)
	        {
	            // Update all multiples of p
	            for (int i = p * 2; i <= n; i += p)
	                prime[tid][i] = false;
	        }
	    }
	}

    // count prime numbers
	#pragma omp parallel for reduction(+:primes)
    for (int p = 2; p <= n; p++)
        if (prime[0][p] && prime[1][p])			
            primes++;

    return (primes);
}

int main()
{
    int n = 100000000;
    printf("%d\n", sieveOfEratosthenes(n));
    return 0;
}
