#include <iostream>
#include <time.h>

#define lli long long int
#define NDEBUG  // Mainly to disable eventual asserts
#define USE MPI

using namespace std;

int main()
{
    lli* randarray = new lli[1000000000];

    // Fill with random numbers an array
    srand(time(NULL));
    for (lli element = 0; element < 1000000000; element++)
    {
        lli putvar = static_cast<lli>(rand() % 1000000000);
        randarray[element] = putvar;
    }


    double time_start, time_stop;

    lli foo = 0;

    time_start = clock();

    for (lli i = 0; i <= 1000000000; i++)
    {
        foo += randarray[i];
    }

    time_stop = clock();

    cout << "Time to perform ONE Long Long Int summation: " << (time_stop - time_start) / 1000000000 << endl;

    return 0;
}
