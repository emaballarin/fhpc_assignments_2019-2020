#!/bin/bash

# Load modules
module load testing
module load intel/18.4
module load gnu/6.2.0
module load intel/18.4

# Relocate
cd ../src/touch_by

# Compile TF
icc -Ofast -fno-alias -xHost -fma -use-intel-optimized-headers -falign-loops -ipo -lrt ./01_array_sum.c -o tf_serial.x
icc -Ofast -fno-alias -xHost -fma -use-intel-optimized-headers -falign-loops -ipo -lrt -fopenmp ./01_array_sum.c -o tf_parallel.x

# Compile TBA
icc -Ofast -fno-alias -xHost -fma -use-intel-optimized-headers -falign-loops -ipo -lrt ./04_touch_by_all.c -o tba_serial.x
icc -Ofast -fno-alias -xHost -fma -use-intel-optimized-headers -falign-loops -ipo -lrt -fopenmp ./04_touch_by_all.c -o tba_parallel.x

# Relocate
cd ../prefix_sum

# Compile PSUM
icpc -std=c++17 -Ofast -fno-alias -xCORE-AVX2 -xHost -fma -use-intel-optimized-headers -qopt-subscript-in-range -falign-loops -qopenmp -parallel -pthread -funroll-loops -ipo -vec ./pstpo.cpp -DCHEATALLOC -DCONSTEXPR -o psum.x

# Cleanup modules
module purge
module purge
