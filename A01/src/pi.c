/*
 * Copyright (C) 2016 Master in High Performance Computing
 * Edit and reuse in accordance to Fair Use policies.
 *
 * Adapted from the net by Alberto Sartori.
 * Subsequently modified by Emanuele Ballarin <emanuele@ballarin.cc>
 *
 * - Added timing acquisition.
 */

// pi.c: Montecarlo integration to determine pi

// We have a circle inscribed inside a square of unit lenght. The
// ratio between the area of the square (1) and the circle (pi/4) is
// pi/4. Therefore, if we randomly choose N points inside the square,
// on average, only M=N*pi/4 points will belong to the circle. From
// the last relation we can estimate pi.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NDEBUG  // Mainly to disable eventual asserts
// if you don't have drand48 uncomment the following two lines
// #define drand48 1.0/RANDMAXrand
// #define srand48 srand
#define seed 68111  // seed for number generator

int main(int argc, char** argv)
{

    if (argc < 2)
    {
        printf(" Usage: %s number \n", argv[0]);
        return 1;
    }
    long long int N = atoll(argv[1]);
    long long int M = 0;
    double pi = 0;
    // point coordinates
    double x, y;
    clock_t start_time, end_time;
    double total_time;

    srand48(seed);  // seed the number generator

    start_time = clock();

    long long int i;
    for (i = 0; i < N; i++)
    {
        // take a point P(x,y) inside the unit square
        x = drand48();
        y = drand48();

        // check if the point P(x,y) is inside the circle
        if ((x * x + y * y) < 1)
            M++;
    }
    pi = 4.0 * M / N;  // calculate area

    end_time = clock();

    // Suppress output
    //printf("\n <><><> # of trials: %lli; estimate of Pi: %1.9f <><><> \n", N, pi);
    total_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    //printf("\n # walltime : %10.8f \n", total_time);
    printf("\n [id: 0] | 0) ENTIRE EXECUTION: %10.8f \n", total_time);
    return 0;
}
