#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>

using namespace std;

int main(int argc, char* argv[]) {
    int dim = 100;
    ofstream outfile;
    long long maxval = (long long) (pow(10, 12));

    // Define random number generator, initialize distribution
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937_64 generator(seed);
    std::uniform_int_distribution<long long> distribution(1, maxval);

    // npgen.exe [dim]
    if (argc >= 2) {
        // # of numbers in input file
        dim = strtol(argv[1], NULL, 10);
    }

    outfile.open("np.txt");

    for (int i = 0; i < dim; i++) {
        // Generate random numbers in given range
        outfile << distribution(generator);
        // Insert newline after numbers for all except last number
        if (i < dim - 1) {
            outfile << "\n";
        }
    }
    outfile.close();
    return 0;
}
