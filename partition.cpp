#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>

#include "kk_implement.h"

using namespace std;
using namespace std::chrono;

const int STD = 0;
const int PP = 1;

// Vector printing function for readability and debugging
void print_mat(vector<long long>& vc, int dimension) {
    for (int i = 0; i < dimension; i++) {
        cout << vc[i] << "\t";
    }
    cout << endl;
}

// Vector printing function for readability and debugging
void print_mat(vector<int>& vc, int dimension) {
    for (int i = 0; i < dimension; i++) {
        cout << vc[i] << "\t";
    }
    cout << endl;
}

// Generate random instance of number partition problem of given dims
void randinst(vector<long long>& inst, int dim) {
    long long maxval = (long long) (pow(10, 12));

    // Define random number generator
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937_64 generator(seed);

    // Initialize distribution
    std::uniform_int_distribution<long long> distribution(1, maxval);

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
        std::uniform_int_distribution<int> distribution(1, dim);
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
    vector<int> randInd;
    randInd.resize(dim);
    iota(randInd.begin(), randInd.end(), 0);
    shuffle(randInd.begin(), randInd.end(), generator);

    // Choose two random indices i, j from [1, n] with i != j
    int i = randInd[0];
    int j = randInd[1];

    // Standard representation
    if (repr == STD) {
        std::bernoulli_distribution distribution(0.5);
        // Set si to -si, set sj to -sj with probability 0.5
        for (int k = 0; k < dim; k++) {
            if (k == i) {
                newsol[k] = -1 * sol[i];
            } else if (k == j) {
                if (distribution(generator) < 0.5) {
                    newsol[k] = -1 * sol[j];
                } else {
                    newsol[k] = sol[j];
                }
            } else {
                newsol[k] = sol[k];
            }
        }
    }
    // Prepartioning representation
    else {
        for (int k = 0; k < dim; k++) {
            if (k == i) {
                newsol[k] = j;
            } else {
                newsol[k] = sol[k];
            }
        }
    }
}

// Repeated random algorithm
void rrand(vector<long long>& inst,
           vector<int>& sol,
           int dim,
           int iters,
           int repr) {
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
void hc(vector<long long>& inst,
        vector<int>& sol,
        int dim,
        int iters,
        int repr) {
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
void anneal(vector<long long>& inst,
            vector<int>& sol2,
            int dim,
            int iters,
            int repr) {
    // Initialize random number generator for probabilities
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    // Create S by copying original random solution S''
    vector<int> sol(sol2.begin(), sol2.end());

    // Create vector S'
    vector<int> sol1;
    sol1.resize(dim);

    // Residue values for S, S', S''
    long long res = resid(inst, sol, repr);
    long long res1;
    long long res2 = res;

    double cooling;
    double prob;

    for (int i = 1; i < iters + 1; i++) {
        cooling = pow(10, 10) * pow(0.8, floor(i / 300));
        neighbor(sol, sol1, dim, repr);
        res1 = resid(inst, sol1, repr);
        // residue(S') < residue(S)
        if (res1 < res) {
            sol = sol1;
            res = res1;
        } else {
            prob = exp(-(res1 - res) / cooling);
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
    // User input + defaults
    int dim = 100;
    double avgVal = (double) 0.0;
    int numTrials = 100;
    int numIters = 25000;
    ifstream infile;
    string file = "";
    ofstream outfile;
    int flag = 0;
    int alg = 0;

    // Array of representations
    int repr[2] = {STD, PP};

    // Initialize timer
    auto start = high_resolution_clock::now();
    auto stop = high_resolution_clock::now();
    auto duration = stop - start;

    // flag 0: partition.exe [flag] [alg] [input file] - test input
    // flag 1: partition.exe [flag] [dimension] [numTrials] [numIters]
    if (atoi(argv[1]) == 0) {
        alg = strtol(argv[2], NULL, 10);
        file = argv[3];
        flag = strtol(argv[1], NULL, 10);
    } else {
        dim = strtol(argv[2], NULL, 10);
        numTrials = strtol(argv[3], NULL, 10);
        numIters = strtol(argv[4], NULL, 10);
        flag = strtol(argv[1], NULL, 10);
    }

    // Variables to store residue, NP instance, sol vectors
    long long res;
    vector<long long> npInst;
    vector<int> sol;
    sol.resize(dim);

    // flag 0: run specified algorithm on input file
    if (flag == 0) {
        // Read numerical values from file
        infile.open(file);
        char temp[256];
        while (infile.getline(temp, 30)) {
            npInst.push_back(stoll(temp));
        }

        switch (alg) {
            // Karmarkar-Karp
            case 0:
                res = kk(npInst);
                break;
            // Repeated random
            case 1:
                randsol(sol, dim, STD);
                rrand(npInst, sol, dim, numIters, STD);
                res = resid(npInst, sol, STD);
                break;
            // Hill climbing
            case 2:
                randsol(sol, dim, STD);
                hc(npInst, sol, dim, numIters, STD);
                res = resid(npInst, sol, STD);
                break;
            // Simulated annealing
            case 3:
                randsol(sol, dim, STD);
                anneal(npInst, sol, dim, numIters, STD);
                res = resid(npInst, sol, STD);
                break;
            // Prepartitioned RR
            case 11:
                randsol(sol, dim, PP);
                rrand(npInst, sol, dim, numIters, PP);
                res = resid(npInst, sol, PP);
                break;
            // Prepartitioned HC
            case 12:
                randsol(sol, dim, PP);
                hc(npInst, sol, dim, numIters, PP);
                res = resid(npInst, sol, PP);
                break;
            // Prepartitioned SA
            case 13:
                randsol(sol, dim, PP);
                anneal(npInst, sol, dim, numIters, PP);
                res = resid(npInst, sol, PP);
                break;
            // Default to KK
            default:
                std::cout << "default";
                res = kk(npInst);
        }
        std::cout << res;
    }
    // flag 1: run all algorithms for numTrials random instances
    else if (flag == 1) {
        npInst.resize(dim);
        outfile.open("pa3avg.csv");
        outfile << "kk time, kk res, "
                   "rr std time, rr std res, rr pp time, rr pp res, "
                   "hc std time, hc std res, hc pp time, hc pp res, "
                   "anneal std time, anneal std res, anneal pp time, anneal pp "
                   "res, avg vector val\n";
        for (int i = 0; i < numTrials; i++) {
            // Generate random instance
            randinst(npInst, dim);

            for (int ind = 0; ind < dim; ind++) {
                avgVal += (double) npInst[ind];
            }

            avgVal = avgVal / (double) dim;

            // KK algorithm
            start = high_resolution_clock::now();
            kk(npInst);
            stop = high_resolution_clock::now();
            duration = duration_cast<microseconds>(stop - start);
            res = kk(npInst);
            // Output data
            outfile << duration.count() * 0.001 << ",";
            outfile << res << ",";

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
                outfile << duration.count() * 0.001 << ",";
                outfile << res << ",";

                // Hill climbing
                randsol(sol, dim, rep);
                start = high_resolution_clock::now();
                hc(npInst, sol, dim, numIters, rep);
                stop = high_resolution_clock::now();
                duration = duration_cast<microseconds>(stop - start);
                res = resid(npInst, sol, dim);
                // Output data
                outfile << duration.count() * 0.001 << ",";
                outfile << res << ",";

                // Simulated annealing
                randsol(sol, dim, rep);
                start = high_resolution_clock::now();
                anneal(npInst, sol, dim, numIters, rep);
                stop = high_resolution_clock::now();
                duration = duration_cast<microseconds>(stop - start);
                res = resid(npInst, sol, dim);
                // Output data
                outfile << duration.count() * 0.001 << ",";
                outfile << res << ",";
            }
            outfile << avgVal;
            outfile << "\n";
        }
        outfile.close();
    }
    return 0;
}
