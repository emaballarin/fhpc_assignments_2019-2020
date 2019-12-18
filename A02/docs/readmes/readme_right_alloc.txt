##################################################
##                                              ##
## README                                       ##
## for "04_touch_by_all_right_alloc.c"          ##
## ******                                       ##
##                                              ##
## Second Assignment - FHPC Course 2019/2020    ##
##                                              ##
## Emanuele Ballarin <emanuele@ballarin.cc>     ##
## DSSC M.Sc. @ (UniTS | UniUD | SISSA | ICTP)  ##
##                                              ##
##################################################

The "04_touch_by_all_right_alloc.c" code is an attempt to fulfil
the request of Exercise 0, optional point.

It directly branches from the "04_touch_by_all.c" code, with the
following differences:

- All code portions related to other tasks than performing
  the vector sum have been removed for the sake of clarity;
  
- Serial execution is no longer possible. However, the
  addition of some conditional compilation machinery may
  be used to addition the code with a serial (naive) version
  of the summation algorithm.

- The OpenMP reduction has been replaced with a hand-coded
  serial summation both in-thread for local partial sums and
  out-of-thread for final summation.
  This implementation adds an overhead linear in P to the ideal
  implementation of reduction operations, which is comparable to
  common Real-World implementations of reduction for any value
  of P for which it is sensible to use shared memory parallelism.
  
- Serial for loops in thread have been automatically-vectorized by
  the means of Intel-specific #pragma parallel .
  
