// A simple program to evaluate the time required to compute a Long Long Int summation.
//
// (C) 2019-* Emanuele Ballarin <emanuele@ballarin.cc>
// Distribution: MIT License.

#include <iostream>
#include <mpi.h>

#define lli long long int
#define NDEBUG  // Mainly to disable eventual asserts
#define USE MPI

using namespace std;

int main(int argc, char* argv[])
{

    int procId;
    int errCode;
    int P;

    double time_start, time_stop;

    errCode = MPI_Init(&argc, &argv);
    errCode = MPI_Comm_size(MPI_COMM_WORLD, &P);
    errCode = MPI_Comm_rank(MPI_COMM_WORLD, &procId);

    lli foo = 0;

    time_start = MPI_Wtime();
    for (lli i = 0; i <= 1000000000; i++)
    {
        foo += i;
    }
    time_stop = MPI_Wtime();

    cout << "Time to perform 1000000000 Long Long Int summations: " << (time_stop - time_start) << endl;

    MPI_Finalize();

    return 0;
}
