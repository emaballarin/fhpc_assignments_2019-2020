/*
 * COMPILE LINE (icc): -Ofast -fno-alias -xCORE-AVX2 -xHost -fma -use-intel-optimized-headers -falign-loops -qopenmp -parallel -pthread -ipo -vec
 */

#define _XOPEN_SOURCE 700
#define _GNU_SOURCE
#define N_default 1000

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char** argv)
{

    unsigned long int N = N_default;
    unsigned long int nthreads = 1;
    double S = 0;

    if (argc > 1)
        N = atoi(*(argv + 1));

    N++;

#pragma omp parallel default(none) shared(N, nthreads, S)
    {
#pragma omp master
        {
            nthreads = omp_get_num_threads();
        }
        unsigned long int me = omp_get_thread_num();

        // No need to do this inside a `single` subregion or outside the `parallel` region.
        // Computational overhead due to duplication if minimal for realistic nthreads and
        // in this way there is no need to put `barrier`s.
        unsigned long int partlen_base = (unsigned long int)(N / nthreads);
        unsigned long int remaind = N - partlen_base * nthreads;
        unsigned long int true_partlen = partlen_base + (unsigned long int)(me < remaind);
        unsigned long int mystart = me * true_partlen + remaind * (unsigned long int)(me >= remaind);
        unsigned long int mystop = mystart + true_partlen;

        double* array;

        // MEMORY ALLOCATION
        array = (double*)malloc(true_partlen * sizeof(double));

// MEMORY INITIALIZATION
#pragma parallel
        for (unsigned long int ii = mystart; ii < mystop; ii++)
        {
            array[ii - mystart] = (double)ii;
        }

        double Slocal = 0;  // this will store the summation

#pragma parallel
        for (unsigned long int jj = 0; jj < true_partlen; jj++)
        {
            Slocal += array[jj];
        }

        free(array);

#pragma omp critical
        S += Slocal;
    }


    printf("%g SUM\n\n\n", S);

    return 0;
}
