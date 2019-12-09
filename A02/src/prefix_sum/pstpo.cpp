/**
 ******************************************************
 ** PStPO: Prefix Sum through Premature Optimization **
 ******************************************************
 *
 * (C) 2019 - Emanuele Ballarin <emanuele@ballarin.cc>
 *     Distribution: Apache License
 *     https://raw.githubusercontent.com/emaballarin/fhpc_assignments_2019-2020/master/LICENSE
 *
 * PREFACE:
 * I surely was not one among the first to tackle this exercise for the
 * assignment, and literally everyone I knew (and who did it already) was talking
 * about how their implementation "Well, it works... but it doesn't scale better
 * than the serial version". For that reason, without further ado, I decided to
 * ask an oldtime friend for help. That friend, however, is said to be nothing
 * less than the root of all evils. They told me not to go down such road.
 * I did it nonetheless. As for now, it seems to work pretty well... ¯\_(ツ)_/¯
 *
 * COMPILE LINE (Intel PSXE 2019.5): icpc -std=c++17 -Ofast -fno-alias -xCORE-AVX2 -xHost -fma -use-intel-optimized-headers -qopt-subscript-in-range -falign-loops -qopenmp -parallel -pthread -funroll-loops -ipo -vec
 * COMPILE LINE (GNU g++ 9.2, base): g++ -Ofast -flto -faggressive-loop-optimizations -fassociative-math -fcode-hoisting -fcombine-stack-adjustments -fdelayed-branch -fdelete-null-pointer-checks -fexpensive-optimizations -ffast-math -ffinite-math-only -fguess-branch-probability -fhoist-adjacent-loads -fif-conversion -fif-conversion2 -findirect-inlining -fipa-pure-const -fivopts -flive-range-shrinkage -flra-remat -flto -fmerge-all-constants -fmerge-constants -fmodulo-sched -fmodulo-sched-allow-regmoves -fmove-loop-invariants -foptimize-sibling-calls -foptimize-strlen -fpartial-inlining -fpeel-loops -fpeephole -fpeephole2 -fpredictive-commoning -fprefetch-loop-arrays -freciprocal-math -free -frename-registers -freorder-blocks -freorder-blocks-and-partition -freorder-functions -frerun-cse-after-loop -freschedule-modulo-scheduled-loops -fsched2-use-superblocks -fsched-critical-path-heuristic -fsched-dep-count-heuristic -fsched-group-heuristic -fsched-interblock -fsched-last-insn-heuristic -fsched-pressure -fsched-rank-heuristic -fsched-spec -fsched-spec-insn-heuristic -fsched-spec-load -fsched-spec-load-dangerous -fschedule-fusion -fschedule-insns -fschedule-insns2 -fselective-scheduling -fsel-sched-pipelining -fsel-sched-pipelining-outer-loops -fsplit-loops -fthread-jumps -ftree-bit-ccp -ftree-builtin-call-dce -ftree-ccp -ftree-coalesce-vars -ftree-dominator-opts -ftree-loop-distribution -ftree-loop-if-convert -ftree-loop-optimize -ftree-loop-vectorize -ftree-partial-pre -ftree-phiprop -ftree-pre -ftree-slp-vectorize -ftree-switch-conversion -ftree-tail-merge -ftree-vectorize -funroll-all-loops -funroll-loops -funsafe-loop-optimizations -funsafe-math-optimizations -funswitch-loops -fuse-linker-plugin -fwhole-program -march=native -fopenmp
 * COMPILE LINE (GNU g++ 9.2, ISL):  g++ -Ofast -flto -faggressive-loop-optimizations -fassociative-math -fcode-hoisting -fcombine-stack-adjustments -fdelayed-branch -fdelete-null-pointer-checks -fexpensive-optimizations -ffast-math -ffinite-math-only -fguess-branch-probability -fhoist-adjacent-loads -fif-conversion -fif-conversion2 -findirect-inlining -fipa-pure-const -fivopts -flive-range-shrinkage -floop-nest-optimize -floop-parallelize-all -flra-remat -flto -fmerge-all-constants -fmerge-constants -fmodulo-sched -fmodulo-sched-allow-regmoves -fmove-loop-invariants -foptimize-sibling-calls -foptimize-strlen -fpartial-inlining -fpeel-loops -fpeephole -fpeephole2 -fpredictive-commoning -fprefetch-loop-arrays -freciprocal-math -free -frename-registers -freorder-blocks -freorder-blocks-and-partition -freorder-functions -frerun-cse-after-loop -freschedule-modulo-scheduled-loops -fsched2-use-superblocks -fsched-critical-path-heuristic -fsched-dep-count-heuristic -fsched-group-heuristic -fsched-interblock -fsched-last-insn-heuristic -fsched-pressure -fsched-rank-heuristic -fsched-spec -fsched-spec-insn-heuristic -fsched-spec-load -fsched-spec-load-dangerous -fschedule-fusion -fschedule-insns -fschedule-insns2 -fselective-scheduling -fsel-sched-pipelining -fsel-sched-pipelining-outer-loops -fsplit-loops -fthread-jumps -ftree-bit-ccp -ftree-builtin-call-dce -ftree-ccp -ftree-coalesce-vars -ftree-dominator-opts -ftree-loop-distribution -ftree-loop-if-convert -ftree-loop-optimize -ftree-loop-vectorize -ftree-partial-pre -ftree-phiprop -ftree-pre -ftree-slp-vectorize -ftree-switch-conversion -ftree-tail-merge -ftree-vectorize -funroll-all-loops -funroll-loops -funsafe-loop-optimizations -funsafe-math-optimizations -funswitch-loops -fuse-linker-plugin -fwhole-program -march=native -fopenmp
 */

/*******************************************************************************
 * GLOBAL PRAGMAS:
 *******************************************************************************/
#pragma once

/*******************************************************************************
 * DEFINES:
 *******************************************************************************/
#define usi unsigned int            // A shorthand
#define llu long long unsigned int  // A shorthand
#define NDEBUG                      // Mainly to disable asserts when "shipping to production"

// Delete all the #define below this line...
//#define PRINTOUT
//#define DIAGNOSTIC
//#define TIMEPROF_SERIAL
//#define TIMEPROF_PARALLEL
#define TIMEPROF_TIMECALLS
//#define __INTEL_COMPILER
//#define OLDGNU

/*******************************************************************************
 * INCLUDES:
 *******************************************************************************/
#ifdef __INTEL_COMPILER
    #include <aligned_new>  // Override new() functions with alignment-aware calls (Intel only)
#endif

#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <omp.h>

/*******************************************************************************
 * FUNCTIONS (FORWARD DECLARATIONS, IF NEEDED)
 *******************************************************************************/


/*******************************************************************************
 * FUNCTIONS
 *******************************************************************************/
template<typename T>
inline void scansum_serial(const T* const input_ptr, const std::size_t input_len, T* const output_ptr)
{
#ifdef DIAGNOSTIC  // Check that the raw-pointer array is not NULL
    assert(static_cast<int>((input_ptr != nullptr) && (input_ptr != NULL)));
#endif  // ifdef DIAGNOSTIC (Check that the raw-pointer array is not NULL)

#ifdef TIMEPROF_SERIAL
    auto t_start = std::chrono::high_resolution_clock::now();
#endif  // ifdef TIMEPROF_SERIAL

    output_ptr[0] = input_ptr[0];
    for (llu i{1}; i < input_len; ++i)  // Already vectorized by the compiler at most (-Ofast -vec)
    {
        output_ptr[i] = output_ptr[i - 1] + input_ptr[i];
    }

#ifdef TIMEPROF_SERIAL  // Function execution time
    auto t_stop = std::chrono::high_resolution_clock::now();
    std::cout << "\n*********************************************************************************************\n"
              << "CALLED: scansum_serial for " << input_len << " elements.\n"
              << "Elapsed time (mu_s): "
              << std::chrono::duration_cast<std::chrono::microseconds>(t_stop - t_start).count()
              << "\n*********************************************************************************************\n"
              << std::endl;
#endif  // ifdef TIMEPROF_SERIAL (Function execution time)
};


template<typename T>
inline auto scansum_blelloch90(const T* const input_ptr, const std::size_t input_len, T* const output_ptr,
                               const unsigned int nthreads)
{
#ifdef DIAGNOSTIC  // Check that the number of threads for the algorithm is nonzero
    assert(static_cast<int>(nthreads != 0));
#endif  // ifdef DIAGNOSTIC (Check that the requested number of threads is nonzero)

#ifdef DIAGNOSTIC  // Check in an on-the-fly fashion that the number of threads for the algorithm equals that of actual OMP threads
    #pragma omp parallel
    {
    #pragma omp master
        {
            assert(static_cast<int>(omp_get_num_threads() == nthreads));
        }
    }
#endif  //ifdef DIAGNOSTIC (Check in an on-the-fly fashion that the number of threads for the algorithm equals that of actual OMP threads)

#ifdef TIMEPROF_PARALLEL
    auto t_start_p = std::chrono::high_resolution_clock::now();
#endif  // ifdef TIMEPROF_PARALLEL

    usi partition_len{static_cast<usi>(input_len / nthreads)};  // Integer division
    usi remainder{static_cast<unsigned int>(
      input_len - nthreads * partition_len)};  // Why use `%` if we have already the integer quotient?

    auto sum_offsets = new T[nthreads];     // Leave uninitialized for performance reasons
    auto prefix_offsets = new T[nthreads];  // Leave uninitialized for performance reasons

#ifndef OLDGNU
    #pragma omp parallel default(none) \
      shared(input_ptr, output_ptr, partition_len, remainder, sum_offsets, prefix_offsets, nthreads)
#endif
#ifdef OLDGNU  // g++ < 8 requires this peculiar pragma call...
    #pragma omp parallel default(none) shared(partition_len, remainder, sum_offsets, prefix_offsets)
#endif
    {
        // SERVICE VARIABLES
        auto threadlocal_threadnum = omp_get_thread_num();
        bool threadlocal_addone{(threadlocal_threadnum < remainder)};
        auto threadlocal_partlen = partition_len + threadlocal_addone;
        auto threadlocal_ptradd =
          static_cast<unsigned int>(threadlocal_threadnum) * (partition_len + threadlocal_addone)
          + remainder * (!threadlocal_addone);

        /*
        * PASS 1: Partial prefix sums
        */

        scansum_serial(input_ptr + threadlocal_ptradd, threadlocal_partlen, output_ptr + threadlocal_ptradd);
        sum_offsets[threadlocal_threadnum] = (output_ptr + threadlocal_ptradd)[threadlocal_partlen - 1];

        /*
        * PASS 2: Sync
        */

#pragma omp barrier  // Ensure that the `sum_offsets` array is correctly filled before proceeding
#pragma omp single   //Compute the prefix sum of the array
        {
            scansum_serial(sum_offsets, nthreads, prefix_offsets);
        }

        /*
         * PASS 3: Offset addition
         */

#pragma parallel
        for (llu i{0}; i < threadlocal_partlen; ++i)  // Already vectorized by the compiler at most (-Ofast -vec)
        {
            (output_ptr + threadlocal_ptradd)[i] +=
              (prefix_offsets[threadlocal_threadnum - 1]) * (threadlocal_threadnum != nthreads);
        }
    }

#ifdef TIMEPROF_PARALLEL  // Function execution time
    auto t_stop_p = std::chrono::high_resolution_clock::now();
    std::cout << "\n*********************************************************************************************\n"
              << "CALLED: scansum_blelloch90 for " << input_len << " elements and over" << nthreads << " threads\n"
              << "Elapsed time (mu_s): "
              << std::chrono::duration_cast<std::chrono::microseconds>(t_stop_p - t_start_p).count()
              << "\n*********************************************************************************************\n"
              << std::endl;
#endif  // ifdef TIMEPROF_PARALLEL (Function execution time)

    delete[] sum_offsets;     // Will not be useful anymore
    delete[] prefix_offsets;  // Will not be useful anymore
}


/*******************************************************************************
 * MAIN DRIVER
 *******************************************************************************/

int main()
{
    constexpr llu testnr = 500000000;


    auto known_array_ptr = new float[testnr];
    auto test_array = new float[testnr];

    for (auto i{0}; i < testnr; ++i)
    {
        known_array_ptr[i] = static_cast<float>(i);
    }

#ifdef TIMEPROF_TIMECALLS

    int test_threads_nr;
    #pragma omp parallel
    {
    #pragma omp master
        {
            test_threads_nr = static_cast<int>(omp_get_num_threads());
        }
    }

    auto t_start_serial = std::chrono::high_resolution_clock::now();
    scansum_serial(known_array_ptr, testnr, test_array);
    auto t_stop_serial = std::chrono::high_resolution_clock::now();
    auto t_diff_serial = std::chrono::duration_cast<std::chrono::nanoseconds>(t_stop_serial - t_start_serial).count();

    std::cout << test_array[testnr - 1] << std::endl;

    delete[] known_array_ptr;
    delete[] test_array;


    auto known_array_ptr_2 = new float[testnr];
    auto test_array_2 = new float[testnr];

    for (auto i{0}; i < testnr; ++i)
    {
        known_array_ptr_2[i] = static_cast<float>(i);
    }

    #ifdef HYPERTHREAD
    test_threads_nr *= 2;
    omp_set_num_threads(test_threads_nr);
    #endif

    auto t_start_parallel = std::chrono::high_resolution_clock::now();
    scansum_blelloch90(known_array_ptr_2, testnr, test_array_2, test_threads_nr);
    auto t_stop_parallel = std::chrono::high_resolution_clock::now();
    auto t_diff_parallel =
      std::chrono::duration_cast<std::chrono::nanoseconds>(t_stop_parallel - t_start_parallel).count();

    std::cout << test_array_2[testnr - 1] << std::endl;

    delete[] known_array_ptr_2;
    delete[] test_array_2;

    auto speedup = ((double)t_diff_serial / (double)t_diff_parallel);

    std::cout << "SERIAL TIME:" << t_diff_serial << std::endl;
    std::cout << "SPEEDUP:" << speedup << std::endl;
    std::cout << "THREADS:" << test_threads_nr << std::endl;

#endif

#ifndef TIMEPROF_TIMECALLS
    scansum_blelloch90(known_array_ptr, testnr, test_array, 4);
    //scansum_serial(known_array_ptr, testnr, test_array);
    std::cout << test_array[testnr - 1] << std::endl;

    delete[] known_array_ptr;
    delete[] test_array;
#endif
}
