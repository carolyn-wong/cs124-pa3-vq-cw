#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>

using namespace std;
using namespace std::chrono;

const int STD = 0;
const int PP = 1;

// TODO - vector vs array for storing solutions
// TODO - implement KK algorithm
// TODO - improve efficiency of generating random indices in range [1, n]

// Vector printing function for readability and debugging
void print_mat(vector<long>& vc, int dimension) {
    for (int i = 0; i < dimension; i++) {
        cout << vc[i] << "\t";
    }
    cout << endl;
}

// Generate random instance of number partition problem of given dims
void randinst(vector<long>& inst, int dim) {
    // Define random number generator
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937_64 generator(seed);

    // Initialize distribution
    long maxval = long(pow(10, 12));
    std::uniform_int_distribution<long> distribution(1, maxval);

    // Fill solution with random values
    for (int i = 0; i < dim; i++) {
        inst[i] = distribution(generator);
    }
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
void neighbor(vector<int>& sol, vector<int>& newsol, int dim, int repr) {
    // Initialize random number generator
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);

    // Generate vector of values 1 through n
    // TODO - see if can make this more efficient
    vector<int> randInd;
    randInd.resize(dim);
    iota(randInd.begin(), randInd.end(), 1);
    shuffle(randInd.begin(), randInd.end(), generator);

    // Choose two random indices i, j from [1, n] with i != j
    int i = randInd[0];
    int j = randInd[1];

    // Standard representation
    if (repr == STD) {
        std::bernoulli_distribution distribution(0.5);
        // Set si to -si, set sj to -sj with probability 0.5
        newsol[i] = -1 * newsol[i];
        if (distribution(generator) < 0.5) {
            newsol[j] = -1 * newsol[j];
        }
    }
    // Prepartioning representation
    else {
        newsol[i] = j;
    }
}

// Karmarkar-Karp algorithm: returns residue of given input, representation.
int kk(vector<long>& inst, vector<int>& sol, int repr) {}

// Repeated random algorithm
void rrand(vector<long>& inst, vector<int>& sol, int dim, int iters, int repr) {
    vector<int> newsol;
    newsol.resize(dim);
    for (int i = 0; i < iters; i++) {
        // Generate random solution
        randsol(newsol, dim, repr);
        if (kk(inst, newsol, repr) < kk(inst, sol, repr)) {
            sol = newsol;
        }
    }
}

// Hill climbing algorithm
void hc(vector<long>& inst, vector<int>& sol, int dim, int iters, int repr) {
    vector<int> newsol;
    newsol.resize(dim);
    for (int i = 0; i < iters; i++) {
        neighbor(sol, newsol, dim, repr);
        if (kk(inst, newsol, repr) < kk(inst, sol, repr)) {
            sol = newsol;
        }
    }
}

// Simulated annealing algorithm. Returns best solution sol2 seen so far.
void anneal(vector<long>& inst,
            vector<int>& sol2,
            int dim,
            int iters,
            int repr) {
    // Initialize random number generator for probabilities
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    // Create S by copying original random solution S''
    // TODO see if can avoid copying values into new vector
    vector<int> sol(sol2.begin(), sol2.end());
    // Create vector S'
    vector<int> sol1;
    sol1.resize(dim);

    // Residue values for S, S', S''
    int res = kk(inst, sol, repr);
    int res1;
    int res2 = res;

    for (int i = 0; i < iters; i++) {
        double cooling = pow(10, 10) * pow(0.8, floor(i / 300));
        sol1 = neighbor(sol, sol1, dim, repr);
        res1 = kk(inst, sol1, repr);
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
}

int main(int argc, char* argv[]) {
    int dim = 100;
    return 0;
}
