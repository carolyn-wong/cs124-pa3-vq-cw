#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>

using namespace std;
using namespace std::chrono;

// Vector printing function for readability and debugging
void print_mat(vector<long>& vc, int dimension) {
    for (int i = 0; i < dimension; i++) {
        cout << vc[i] << "\t";
    }
    cout << endl;
}

// Generate random solution of size 100
vector<long> randsol(int dim) {
    // Define random number generator
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937_64 generator(seed);

    // Initialize distribution
    long maxval = long(pow(10, 12));
    std::uniform_int_distribution<long> distribution(1, maxval);

    // Fill solution with random values
    vector<long> sol;
    sol.resize(dim);
    for (int i = 0; i < dim; i++) {
        sol[i] = distribution(generator);
    }

    return sol;
}

// Karmarkar-Karp algorithm
int kk() {}

// Repeated random algorithm
void rrand(vector<long>& sol, int iters) {}

// Hill climbing algorithm
void hc(vector<long>& sol) {}

// Simulated annealing algorithm
void anneal(vector<long>& sol) {}

int main(int argc, char* argv[]) {
    return 0;
}
