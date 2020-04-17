#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>

using namespace std;
using namespace std::chrono;

// TODO - vector vs array for storing solutions
// TODO - implement random soln and neighbor generator for both representations

// Vector printing function for readability and debugging
void print_mat(vector<long>& vc, int dimension) {
    for (int i = 0; i < dimension; i++) {
        cout << vc[i] << "\t";
    }
    cout << endl;
}

// Generate random instance of number partition problem of given dims
// TODO - change to vector pointer so don't recreate a new vector every time
vector<long> randinst(int dim) {
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

// Returns random solution of given dimension and representation
// TODO - feed in pointer to new rand solution instead of creating new vector
vector<long> randsol(int dim, int repr) {}

// Returns neighboring solution for given representation
// TODO - make copy of solution and return pointer
vector<long> neighbor(vector<long>& sol, int repr) {}

// Karmarkar-Karp algorithm: returns residue of given input, representation.
int kk(vector<long>& inpt) {}

// Repeated random algorithm
void rrand(vector<long>& inst, vector<long>& sol, int iters, int repr) {
    for (int i = 0; i < iters; i++) {
        vector<long> newsol = randsol(100, repr);
        if (kk(newsol) < kk(sol)) {
            sol = newsol;
        }
    }
    return;
}

// Hill climbing algorithm
void hc(vector<long>& inst, vector<long>& sol, int iters, int repr) {
    for (int i = 0; i < iters; i++) {
        vector<long> newsol = neighbor(sol, repr);
        if (kk(newsol) < kk(sol)) {
            sol = newsol;
        }
    }
    return;
}

// Simulated annealing algorithm
void anneal(vector<long>& inst, vector<long>& sol, int iters, int repr) {}

int main(int argc, char* argv[]) {
    return 0;
}
