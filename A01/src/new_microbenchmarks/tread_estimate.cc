// Please, ONE PROCESS ONLY!
#define lli long long int
#define NDEBUG  // Mainly to disable eventual asserts
#define USE MPI

#include <fstream>
#include <iostream>
#include <mpi.h>
#include <string>


using namespace std;


lli readN(string);  // Read a long long int from a file and return it


inline lli readN(string filepath)
{
    lli retN;
    ifstream myfile;

    myfile.open(filepath);
    myfile >> retN;
    myfile.close();

    return retN;
}


int main(int argc, char* argv[])
{

    lli N;

    //    MPI_Init(&argc, &argv);
    int procId;
    int errCode;
    int P;  // To-be number of processes

    double time_start, time_stop;

    errCode = MPI_Init(&argc, &argv);
    errCode = MPI_Comm_size(MPI_COMM_WORLD, &P);
    errCode = MPI_Comm_rank(MPI_COMM_WORLD, &procId);

    time_start = MPI_Wtime();
    for (lli i = 0; i < 500000; i++)
    {
        N = readN("../data/N.txt");
    }
    time_stop = MPI_Wtime();

    cout << "Time to perform 500000 Long Long Int read from file: " << (time_stop - time_start) << endl;
    MPI_Finalize();

    return 0;
}
