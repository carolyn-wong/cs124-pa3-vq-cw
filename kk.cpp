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

// Computes residue given a number partition instance and representation.
int resid(vector<long>& inst, vector<int>& sol, int repr) {}

// Karmarkar-Karp algorithm
int kk(vector<long>& inst, int dim) {
    // Sort instance, perform differencing on largest 2 elts
    // TODO - implement heap?
}

// Repeated random algorithm
void rrand(vector<long>& inst, vector<int>& sol, int dim, int iters, int repr) {
    vector<int> newsol;
    newsol.resize(dim);
    for (int i = 0; i < iters; i++) {
        // Generate random solution
        randsol(newsol, dim, repr);
        if (resid(inst, newsol, repr) < resid(inst, sol, repr)) {
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
        if (resid(inst, newsol, repr) < resid(inst, sol, repr)) {
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
    int res = resid(inst, sol, repr);
    int res1;
    int res2 = res;

    for (int i = 0; i < iters; i++) {
        double cooling = pow(10, 10) * pow(0.8, floor(i / 300));
        sol1 = neighbor(sol, sol1, dim, repr);
        res1 = resid(inst, sol1, repr);
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

// TODO - figure out how to compare res data for different algs
// TOOD - consider pointers to alg functions to minimize repeated code?
int main(int argc, char* argv[]) {
    // User input + defaults
    int dim = 100;
    int numTrials = 100;
    int numIters = 25000;
    ifstream infile;
    string file = "";
    ofstream outfile;
    int option = 0;

    // Array of representations
    int repr[2] = {STD, PP};

    // Variables to store residue, NP instance, sol vectors
    long res;
    vector<long> npInst;
    vector<int> sol;
    npInst.resize(dim);
    sol.resize(dim);

    // Initialize timer
    auto start = high_resolution_clock::now();
    auto stop = high_resolution_clock::now();
    auto duration = stop - start;

    // option 0: kk.exe [input file] - test input
    // option 1: kk.exe [dimension] [numTrials] [numIters]
    if (argc == 2) {
        file = argv[1];
    } else {
        dim = strtol(argv[1], NULL, 10);
        numTrials = strtol(argv[2], NULL, 10);
        numIters = strtol(argv[3], NULL, 10);
        option = 1;
    }

    // option 0: run kk algorithm on input file
    if (option == 0) {
        // Read numerical values from file
        infile.open(file);
        int temp = 0;
        for (int i = 0; i < dim; i++) {
            infile >> temp;
            npInst[i] = temp;
        }
        kk(npInst, dim);
        // TOOD - print result for kk algorithm
    }
    // option 1: run all algorithms for numTrials random instances
    else {
        outfile.open("pa3.csv");
        outfile << "kk time, kk res, "
                   "rr std time, rr std res, rr pp time, rr pp res, "
                   "hc std time, hc std res, hc pp time, hc pp res, "
                   "anneal std time, anneal std res, anneal pp time, anneal pp "
                   "res,\n";
        for (int i = 0; i < numTrials; i++) {
            // Generate random instance
            randinst(npInst, dim);

            // KK algorithm
            start = high_resolution_clock::now();
            kk(npInst, dim);
            stop = high_resolution_clock::now();
            duration = duration_cast<microseconds>(stop - start);
            res = resid(npInst, sol, dim);
            // Output data
            std::cout << duration.count() * 0.000001 << endl;
            std::cout << res << endl;
            // outfile << duration.count() * 0.000001;
            // outfile << res;

            // Execute algorithms for each representation
            for (int rep : repr) {
                // Repeated random
                randsol(sol, dim, rep);
                start = high_resolution_clock::now();
                rrand(npInst, sol, dim, numIters, rep);
                stop = high_resolution_clock::now();
                duration = duration_cast<microseconds>(stop - start);
                res = resid(npInst, sol, dim);
                // Output data
                std::cout << duration.count() * 0.000001 << endl;
                std::cout << res << endl;
                // outfile << duration.count() * 0.000001;
                // outfile << res;

                // Hill climbing
                randsol(sol, dim, rep);
                start = high_resolution_clock::now();
                hc(npInst, sol, dim, numIters, rep);
                stop = high_resolution_clock::now();
                duration = duration_cast<microseconds>(stop - start);
                res = resid(npInst, sol, dim);
                // Output data
                std::cout << duration.count() * 0.000001 << endl;
                std::cout << res << endl;
                // outfile << duration.count() * 0.000001;
                // outfile << res;

                // Simulated annealing
                randsol(sol, dim, rep);
                start = high_resolution_clock::now();
                anneal(npInst, sol, dim, numIters, rep);
                stop = high_resolution_clock::now();
                duration = duration_cast<microseconds>(stop - start);
                res = resid(npInst, sol, dim);
                // Output data
                std::cout << duration.count() * 0.000001 << endl;
                std::cout << res << endl;
                // outfile << duration.count() * 0.000001;
                // outfile << res;
            }
        }
        outfile.close();
    }
    return 0;
}
