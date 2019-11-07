/*
 * Copyright (C) 2015 - 2016 Master in High Performance Computing
 * Edit and reuse in accordance to Fair Use policies.
 *
 * Adapted from the net by Giuseppe Brandino.
 * Subsequently modified by Alberto Sartori.
 * Subsequently modified by Emanuele Ballarin <emanuele@ballarin.cc>
 *
 * - Added time and promoted to long long all important variables.
 * - Added more thorough timing acquisition.
 */

#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define NDEBUG  // Mainly to disable eventual asserts
#define USE MPI
#define SEED 35791246

int main(int argc, char* argv[])
{
    // coordinates
    double x, y;

    // number of points inside the circle
    long long int M, local_M;
    double pi;

    // times
    double start_time, end_time, aftersample_time;
    int myid, numprocs, proc;
    MPI_Status status;
    // master process
    int master = 0;
    int tag = 123;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    if (argc <= 1)
    {
        fprintf(stderr, " Usage: mpi -np n %s number_of_iterations \n", argv[0]);
        MPI_Finalize();
        exit(-1);
    }

    long long int N = atoll(argv[1]) / numprocs;

    // initialize random numbers
    srand48(SEED * (myid + 1));  // seed the number generator

    // take time of processors after initial I/O operation
    start_time = MPI_Wtime();

    local_M = 0;
    long long int i;
    for (i = 0; i < N; i++)
    {
        // take a point P(x,y) inside the unit square
        x = drand48();
        y = drand48();

        // check if the point P(x,y) is inside the circle
        if ((x * x + y * y) < 1)
            local_M++;
    }

    aftersample_time = MPI_Wtime();
    printf(" [id: %i] | 0) SAMPLE+CHECK: %10.8f \n", myid, aftersample_time - start_time);

    if (myid == 0)
    {  //if I am the master process gather results from others
        // This is a simple SERIAL operation that does not scale well with number of inputs (iterations)
        M = local_M;
        for (proc = 1; proc < numprocs; proc++)
        {
            MPI_Recv(&local_M, 1, MPI_LONG_LONG, proc, tag, MPI_COMM_WORLD, &status);

            M += local_M;
        }
        pi = 4.0 * M / (N * numprocs);
        end_time = MPI_Wtime();

        // Suppress output
        //printf("\n <><><> # of trials: %lli; estimate of Pi: %1.9f <><><> \n", N * numprocs, pi);
        //printf("\n # walltime on processor master: %10.8f \n", end_time - start_time);
        printf(" [id: %i] | 1) SEND+SUM: %10.8f \n", myid, end_time - aftersample_time);
        printf("\n [id: %i] | 2) ENTIRE EXECUTION: %10.8f \n\n", myid, end_time - start_time);
    }
    else
    {
        MPI_Ssend(&local_M, 1, MPI_LONG_LONG, master, tag, MPI_COMM_WORLD);
        end_time = MPI_Wtime();
        //printf("\n # walltime on processor %i : %10.8f \n", myid, end_time - start_time);
        printf(" [id: %i] | 1) SEND+SUM: %10.8f \n", myid, end_time - aftersample_time);
        printf("\n [id: %i] | 2) ENTIRE EXECUTION: %10.8f \n\n", myid, end_time - start_time);
    }

    MPI_Finalize();  // let MPI finish up /
}
