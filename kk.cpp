#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>

using namespace std;
using namespace std::chrono;

const int STD = 0;
const int PP = 1;

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
void randsol(vector<int>& newsol, int dim, int repr) {
    // Initialize random number generator
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    // Standard representation
    if (repr == STD) {
        // Fill in vector with -1 and +1 with equal probability
        std::bernoulli_distribution distribution(0.5);
        for (int i = 0; i < dim; i++) {
            if (distribution(generator) < 0.5) {
                newsol[i] = -1;
            } else {
                newsol[i] = 1;
            }
        }
    }
    // Prepartioning representation
    else {
        std::uniform_int_distribution distribution(1, dim);
        for (int i = 0; i < dim; i++) {
            newsol[i] = distribution(generator);
        }
    }
}

// Returns neighboring solution for given representation
// TODO - make copy of solution and return pointer
vector<long> neighbor(vector<int>& sol,
                      vector<int>& newsol,
                      int dims,
                      int repr) {}

// Karmarkar-Karp algorithm: returns residue of given input, representation.
int kk(vector<long>& inpt) {}

// Repeated random algorithm
void rrand(vector<long>& inst,
           vector<int>& sol,
           int dims,
           int iters,
           int repr) {
    vector<int> newsol;
    newsol.resize(dims);
    for (int i = 0; i < iters; i++) {
        // Generate random solution
        randsol(newsol, dims, repr);
        if (kk(newsol) < kk(sol)) {
            sol = newsol;
        }
    }
    return;
}

// Hill climbing algorithm
void hc(vector<long>& inst, vector<int>& sol, int dims, int iters, int repr) {
    vector<int> newsol;
    newsol.resize(dims);
    for (int i = 0; i < iters; i++) {
        neighbor(sol, newsol, dims, repr);
        if (kk(newsol) < kk(sol)) {
            sol = newsol;
        }
    }
    return;
}

// Simulated annealing algorithm. Returns best solution sol2 seen so far.
void anneal(vector<long>& inst, vector<long>& sol2, int iters, int repr) {
    // Initialize random number generator for probabilities
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    // Create S by copying original random solution S''
    // TODO see if can avoid copying values into new vector
    vector<long> sol(sol2.begin(), sol2.end());

    // Residue values for S, S', S''
    int res = kk(sol);
    int res1;
    int res2 = res;

    for (int i = 0; i < iters; i++) {
        double cooling = pow(10, 10) * pow(0.8, floor(i / 300));
        vector<long> sol1 = neighbor(sol, repr);
        res1 = kk(sol1);
        // residue(S') < residue(S)
        if (res1 < res) {
            sol = sol1;
            res = res1;
        } else {
            double prob = exp(-(res1 - res) / cooling);
            if (distribution(generator) < prob) {
                sol = sol1;
                res = res1;
            }
        }
        // residue(S) < residue(S'')
        if (res < res2) {
            sol2 = sol;
            res2 = res;
        }
    }
    return;
}

int main(int argc, char* argv[]) {
    int dims = 100;
    return 0;
}
