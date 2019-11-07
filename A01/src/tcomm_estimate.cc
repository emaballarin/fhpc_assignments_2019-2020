#define lli long long int


#define NDEBUG  // Mainly to disable eventual asserts
#define USE MPI
#include <iostream>
#include <mpi.h>


using namespace std;


int main(int argc, char* argv[])
{
    int procId;
    int errCode;
    int P;  // To-be number of processes
    MPI_Status status;
    int master{0};    // To increase readability in master<->slaves communication
    int MPItag{628};  // Whatever

    double time_start, time_stop;

    errCode = MPI_Init(&argc, &argv);
    errCode = MPI_Comm_size(MPI_COMM_WORLD, &P);
    errCode = MPI_Comm_rank(MPI_COMM_WORLD, &procId);

    time_start = MPI_Wtime();

    for (int i = 0; i < 10000000; i++)
    {
        if (procId == master)
        {
            lli foo = 2000000000;
            for (int slave = 1; slave < P; slave++)
            {
                MPI_Ssend(&foo, 1, MPI_LONG_LONG, slave, MPItag, MPI_COMM_WORLD);
            }
        }
        else
        {
            lli bar;
            MPI_Recv(&bar, 1, MPI_LONG_LONG, master, MPItag, MPI_COMM_WORLD, &status);
        }
    }

    time_stop = MPI_Wtime();

    MPI_Finalize();

    cout << "Time to send ONE Long Long Int (times number of processes): " << (time_stop - time_start) / 10000000
         << endl;


    return 0;
}
