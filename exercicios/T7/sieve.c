/*
 * Adapted from: http://w...content-available-to-author-only...s.org/sieve-of-eratosthenes
 * 
 * Tempo sequencial:
 * 
 * real	0m4.412s
 * user	0m4.318s
 * sys	0m0.080s
 * 
 * Tempo paralelo:
 * 
 * 
 * Tempo paralelo com escalonamento:
 * 
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
	
	#pragma omp parallel for schedule(dynamic)
    for (int p = 2; p <= sqrt_n; p++)
    {
        // If prime[p] is not changed, then it is a prime
        if (prime[p] == true)
        {
            // Update all multiples of p
            for (int i = p * 2; i <= n; i += p)
                prime[i] = false;
        }
    }

    // count prime numbers
	#pragma omp parallel for reduction(+:primes)
    for (int p = 2; p <= n; p++)
        if (prime[p])			
            primes++;

	free(prime);

    return (primes);
}

int main()
{
    int n = 100000000;
    printf("%d\n", sieveOfEratosthenes(n));
    return 0;
}
