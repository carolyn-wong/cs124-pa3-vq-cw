#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>

using namespace std;
using namespace std::chrono;

// TODO - vector vs array for storing solutions

// Vector printing function for readability and debugging
void print_mat(vector<long>& vc, int dimension) {
    for (int i = 0; i < dimension; i++) {
        cout << vc[i] << "\t";
    }
    cout << endl;
}

// Generate random solution of size 100
// TODO - change to vector pointer so don't recreate a new vector every time
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

// Karmarkar-Karp algorithm: returns residue of given input.
int kk(vector<long>& sol) {}

// Repeated random algorithm
void rrand(vector<long>& sol, int iters) {
    for (int i = 0; i < iters; i++) {
        vector<long> newsol = randsol(100);
        if (kk(newsol) < kk(sol)) {
            sol = newsol;
        }
    }
    return;
}

// Hill climbing algorithm
void hc(vector<long>& sol, int iters) {}

// Simulated annealing algorithm
void anneal(vector<long>& sol, int iters) {}

int main(int argc, char* argv[]) {
    return 0;
}
