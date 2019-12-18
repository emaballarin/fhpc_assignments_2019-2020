##################################################
##                                              ##
## README                                       ##
## ******                                       ##
##                                              ##
## Second Assignment - FHPC Course 2019/2020    ##
##                                              ##
## Emanuele Ballarin <emanuele@ballarin.cc>     ##
## DSSC M.Sc. @ (UniTS | UniUD | SISSA | ICTP)  ##
##                                              ##
##################################################



For the sake of conciseness, this readme file will contain only relevant
information not already evident from source code comments and/or the file
named "report.pdf" present in this same directory.



*) MODULE LOADING ON ULYSSES (*

In order to both correctly compile and correctly run the provided code (and
with the specified compiler options) on a Ulysses compute node, the following
modules are required to be loaded (and in this order; duplicates included!):

module load testing
module load intel/18.4
module load gnu/6.2.0
module load intel/18.4



*) COMPILE LINES (OPTIONS) (*

The following compile lines have been used (after having loaded modules) in
order to compile provided sources, respectively for C source files and C++
source files.

icc -Ofast -fno-alias -xHost -fma -use-intel-optimized-headers -falign-loops -ipo

icpc -std=c++17 -Ofast -fno-alias -xCORE-AVX2 -xHost -fma -use-intel-optimized-headers -qopt-subscript-in-range -falign-loops -parallel -pthread -funroll-loops -ipo -vec

In the occurrence the OpenMP libraries are needed (the case of all parallel
versions of the codes), the

-fopenmp

option has to be added to the lines above.

In the occurrence the LibRealTime is needed (the case of all Exercise 1 sources)
the

-lrt

can be added to the C compile line above.

For best performance results code has to be compiled on the same machine on
which it has to be run.


*) COMMITMENT TO FULL REPRODUCIBILITY (*

The entire collection of codes and scripts used in order to produce the
results contained in this folder and in the report are available at the
dedicated GitHub repository:

https://github.com/emaballarin/fhpc_assignments_2019-2020/tree/master/A02

The codes shipped together with this folder are exact copies of
those present inside the GitHub repository having the same name.
