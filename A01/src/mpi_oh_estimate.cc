// A simple program to evaluate the MPI overhead associated to just MPI machinery initialization.

#include <chrono>
#include <iostream>
#include <mpi.h>
#include <thread>

#define USE MPI
#define NDEBUG  // Mainly to disable eventual asserts

using namespace std::this_thread;
using namespace std::chrono_literals;

int main(int argc, char* argv[])
{
    // Should run in exactly 2s + memory allocation time, in the ideal world! ;-)
    MPI_Init(&argc, &argv);
    sleep_for(2s);
    MPI_Finalize();
    return 0;
}
