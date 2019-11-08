// A simple program to evaluate the MPI overhead associated to just MPI machinery initialization.
//
// Usage: perform a timed run with e.g. `/usr/bin/time` (a.k.a. "GNU Time") or bash own `time`;
//        Any additional "real" or "elapsed" time with respect to 2s should be considered plain MPI overhead,
//        modulo some (negligible) variable allocation time.
//
// (C) 2019-* Emanuele Ballarin <emanuele@ballarin.cc>
// Distribution: MIT License.

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
