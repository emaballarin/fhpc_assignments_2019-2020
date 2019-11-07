#define lli long long int
#define NDEBUG  // Mainly to disable eventual asserts
#define USE MPI

#include <fstream>
#include <iostream>
#include <string>
#include <time.h>


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


int main()
{
    double time_start, time_stop;

    lli N;

    time_start = clock();
    for (lli i = 0; i < 10000000; i++)
    {
        N = readN("../data/N.txt");
    }
    time_stop = clock();

    cout << "Time to perform ONE Long Long Int read from file: " << (time_stop - time_start) / 10000000 << endl;

    return 0;
}
