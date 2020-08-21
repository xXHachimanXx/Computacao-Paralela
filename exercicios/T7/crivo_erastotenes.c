/*
* Adapted from: http://w...content-available-to-author-only...s.org/sieve-of-eratosthenes
*/

/*
Intel(R) Core(TM) i5-1035G1 CPU @ 1.00GHz

Time before parallelization
1.503 1.504 1.509 1.510 1.502 1.502 1.502 1.518 1.534 1.516
Average:
1.51 s


Time after parallelization
1.098 0.994 1.064 1.031 1.144 1.072 1.070 1.060 1.081 1.685
Average:
1.1299 s

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


    for (int p = 2; p <= sqrt_n; p++)
    {
        // printf("%d\n",tid);
        // If prime[p] is not changed, then it is a prime
        if (prime[p] == true)
        {
            // Update all multiples of p
            #pragma omp parallel for
            for (int i = p * 2; i <= n; i += p)
                prime[i] = false;
        }
    }

    // count prime numbers
    #pragma omp parallel for reduction(+ : primes)
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