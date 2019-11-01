/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                                                 *
 * parallel_partial_sums_cops.cc                                                                   *
 *                                                                                                 *
 * An MPI program to compute the sum of natural numbers in the [1-N] interval by using a           *
 * parallel partial sums algorithm. Version exploiting MPI Collective Operations.                  *
 *                                                                                                 *
 * Version: 01/11/2019                                                                             *
 *                                                                                                 *
 * Copyright (C) 2019-* Emanuele Ballarin <emanuele@ballarin.cc>. All rights reserved.             *
 *                                                                                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                                                 *
 * Licensed under the Apache License, Version 2.0 (the "License");                                 *
 * you may not use this file except in compliance with the License.                                *
 * You may obtain a copy of the License at                                                         *
 *                                                                                                 *
 *     http://www.apache.org/licenses/LICENSE-2.0                                                  *
 *                                                                                                 *
 * Unless required by applicable law or agreed to in writing, software                             *
 * distributed under the License is distributed on an "AS IS" BASIS,                               *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.                        *
 * See the License for the specific language governing permissions and                             *
 * limitations under the License.                                                                  *
 *                                                                                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */


// PRAGMAS
#pragma once  // Not strictly useful in single-file sources; good practice, though.


// DEFINES
#define lli long long int


// INCLUDES
#define NDEBUG  // Mainly to disable eventual asserts
#define USE MPI
#include <cmath>
#include <fstream>
#include <iostream>
#include <mpi.h>
#include <string>


// NAMESPACE DECLARATIONS
using namespace std;


// FUNCTION SIGNATURES
lli readN(string);  // Read a long long int from a file and return it


// FUNCTION IMPLEMENTATIONS
inline lli readN(string filepath)
{
    lli retN;
    ifstream myfile;

    myfile.open(filepath);
    myfile >> retN;
    myfile.close();

    return retN;
}


// MAIN FUNCTION

int main(int argc, char* argv[])
{
    /**************************
     ** Variable declaration **
     **************************/

    int procId;
    int errCode;
    int P;          // To-be number of processes
    int master{0};  // To increase readability in master<->slaves communication


    /**********************
     ** Timing variables **
     **********************/
    double time_afterinit, time_preread, time_afterread, time_aftersend1, time_afterwork, time_aftergather;


    /************************
     ** MPI Initialization **
     ************************/

    errCode = MPI_Init(&argc, &argv);
    errCode = MPI_Comm_size(MPI_COMM_WORLD, &P);
    errCode = MPI_Comm_rank(MPI_COMM_WORLD, &procId);


    /********************************************
     ** Domain-specific variable preallocation **
     ********************************************/

    lli N;
    lli partSizeFloor;
    lli netImbalance;
    lli localLowBound;
    lli localUpBound;

    // TODO
    time_afterinit = MPI_Wtime();


    /**********************
     ** Read & Broadcast **
     **********************/

    if (procId == master)  // Master node
    {
        // TODO
        time_preread = MPI_Wtime();

        // Read
        N = readN("../data/N.txt");

        // TODO
        time_afterread = MPI_Wtime();
        cout << "[id: " << procId << "] | MASTER READ: " << time_afterread - time_preread << endl;
    }

    // Everybody
    MPI_Bcast(&N, 1, MPI_LONG_LONG, master, MPI_COMM_WORLD);

    // TODO
    time_aftersend1 = MPI_Wtime();
    cout << "[id: " << procId << "] | READ+BROADCAST: " << time_aftersend1 - time_afterinit << endl;


    /***********************************
     ** Work partitioning & Execution **
     ***********************************/

    partSizeFloor = static_cast<lli>(floor(N / P));
    netImbalance = N % P;


    // Partition work according to the minimal imbalance criterion; minimize the need to compute sums/mults
    if (procId < netImbalance)
    {
        localLowBound = procId * (partSizeFloor + 1) + 1;
        localUpBound = (localLowBound + partSizeFloor);
    }
    else
    {
        localLowBound = (procId * partSizeFloor + 1 + netImbalance);
        localUpBound = (localLowBound + partSizeFloor - 1);
    }

    // Compute the partial sum (runs on each node)
    lli localPartialSum{0};
    for (lli i = localLowBound; i <= localUpBound; ++i)
    {
        localPartialSum += i;
    }

    // TODO
    time_afterwork = MPI_Wtime();
    cout << "[id: " << procId << "] | COMPUTE: " << time_afterwork - time_aftersend1 << endl;


    /***************************************
     ** Work re-gathering via Reduce(sum) **
     ***************************************/

    lli gatheredSum;

    MPI_Reduce(&localPartialSum, &gatheredSum, 1, MPI_LONG_LONG, MPI_SUM, master, MPI_COMM_WORLD);


    // TODO
    time_aftergather = MPI_Wtime();
    cout << "[id: " << procId << "] | REDUCE: " << time_aftergather - time_afterwork << endl;

    cout << "\n"
         << "--> [id: " << procId << "] | ENTIRE EXECUTION: " << time_aftergather - time_afterinit << " <-- \n"
         << endl;


    /**********************
     ** Print the result **
     **********************/

    if (procId == master)
    {
        lli finalSum{gatheredSum};
        cout << finalSum << endl;
    }


    /***************************
     ** MPI Finalize & Return **
     ***************************/

    MPI_Finalize();
    return 0;
}
