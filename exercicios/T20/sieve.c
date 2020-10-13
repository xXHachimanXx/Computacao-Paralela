	/*
 * Adapted from: http://w...content-available-to-author-only...s.org/sieve-of-eratosthenes
 *
 * Programa testado no PARCODE
 * 
 * Tempo sequencial:
 * real 0m3.012s
 * user 0m2.926s
 * sys  0m0.080s 
 * 
 * Tempo paralelo multicore:
 * real 0m1.876s
 * user 0m7.123s
 * sys  0m0.076s
 *
 * Tempo paralelo com GPU:
 * real 0m1.697s
 * user 0m0.688s
 * sys  0m1.036s

 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

int sieveOfEratosthenes(int n)
{
    // Create a boolean array "prime[0..n]" and initialize
    // all entries it as true. A value in prime[i] will
    // finally be false if i is Not a prime, else true.
    int primes = 0;
    bool *prime = (bool *)malloc((n + 1) * sizeof(bool));
    int sqrt_n = sqrt(n);

    memset(prime, true, (n + 1) * sizeof(bool));
	
	//#pragma omp parallel for schedule(dynamic)
    #pragma omp target map(tofrom: prime[0:n+1]) 
    for (int p = 2; p <= sqrt_n; p++)
    {
        // If prime[p] is not changed, then it is a prime
        if (prime[p] == true)
        {
            // Update all multiples of p
			#pragma omp teams distribute parallel for simd
            for (int i = p * 2; i <= n; i += p)
                prime[i] = false;
        }
    }

    // count prime numbers
	#pragma omp parallel for reduction(+:primes)
    for (int p = 2; p <= n; p++)
        if (prime[p])			
            primes++;

    return (primes);
}

int main()
{
    int n = 100000000;
    printf("%d\n", sieveOfEratosthenes(n));
    return 0;
}
