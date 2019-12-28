/*
 * Just willing to prove a point.
 *
 * Q.: Does the ++i vs i++ performance difference
 * shows up at Intel's -Ofast when the number of iterations is unknown at
 * compile-time?
 *
 * Let's find it out!
 *
 * A.: In microbenchmark-case scenarios, it does not. ;)
 *
 * Just compile & run it yourself...
*/

#include <chrono>
#include <iostream>

int main()
{
    long long int niter{0};
    unsigned long long int accum1{0};
    unsigned long long int accum2{0};

    std::cout << "Insert the `niter` number..." << std::endl;
    std::cin >> niter;
    std::cout << std::endl;

    auto t_start = std::chrono::high_resolution_clock::now();
    for (long long int i{0}; i < niter; i++)
    {
        accum1 += static_cast<unsigned long long int>(niter);
    }
    std::cout << accum1 << std::endl;
    auto t_end = std::chrono::high_resolution_clock::now();

    std::cout << "Using i++ over " << niter << " iterations." << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(t_end - t_start).count() << std::endl;
    std::cout << std::endl;

    std::cout << "Insert the `niter` number (the same as before if you want to make a comparison)..." << std::endl;
    std::cin >> niter;
    std::cout << std::endl;

    t_start = std::chrono::high_resolution_clock::now();
    for (long long int i{-1}; i < niter - 1; ++i)
    {
        accum2 += static_cast<unsigned long long int>(niter);
    }
    std::cout << accum2 << std::endl;
    t_end = std::chrono::high_resolution_clock::now();

    std::cout << "Using ++i over " << niter << " iterations." << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(t_end - t_start).count() << std::endl;
    std::cout << std::endl;
}
