#include <array>
#include <chrono>
#include <iostream>
#include <map>
#include <random>

using namespace std;
using namespace std::chrono;

// Heap indexing functions
int parent(int i) {
    return ((i - 1) / 2);
}

int left(int i) {
    return (2 * i + 1);
}

int right(int i) {
    return (2 * i + 2);
}

// Insert into heap
void insert(vector<long long>& heap, long long val) {
    heap.push_back(val);  // stick new value at the end
    int index = heap.size() - 1;

    long long temp;
    while (index != 0 && heap[parent(index)] <
                             heap[index]) {  // bubble while parents are smaller
        temp = heap[parent(index)];          // swapping steps
        heap[parent(index)] = heap[index];
        heap[index] = temp;

        index = parent(index);
    }
}

// Max the Heap, make sure it follows the property
void maxHeapify(vector<long long>& heap, int root) {
    int smallest = root;
    int left_i = left(root);
    int right_i = right(root);

    // find index of smallest element at the top
    if (left_i < heap.size() && heap[left_i] > heap[root]) {
        smallest = left_i;
    } else {
        smallest = root;
    }

    if (right_i < heap.size() && heap[right_i] > heap[smallest]) {
        smallest = right_i;
    }

    long long temp;
    if (smallest != root) {             // bubble while parents are smaller
        temp = heap[parent(smallest)];  // swapping steps
        heap[parent(smallest)] = heap[smallest];
        heap[smallest] = temp;

        // MinHeapify again
        maxHeapify(heap, smallest);
    }
}

// pop the root of the heap
long long deleteMax(vector<long long>& heap) {
    long long max = heap[0];

    heap[0] = heap.back();
    heap.pop_back();

    maxHeapify(heap, 0);
    return (max);
}

long long kk(
    vector<long long>& inst) {  // inst is the matrix, dim is size of it

    vector<long long> heap;
    for (int i = 0; i < inst.size(); i++) {
        insert(heap, inst[i]);
    }

    while (heap.size() > 1) {
        long long p1 = deleteMax(heap);
        long long p2 = deleteMax(heap);
        insert(heap, abs(p1 - p2));
    }

    return (heap[0]);
}

// Computes residue given a number partition instance and representation.
// inst is the elements, sol is the given solution, repr is type of
// representation (0 is +/- 1, 1 is prepartition)
long long resid(vector<long long>& inst, vector<int>& sol, int repr) {
    if (repr == 0) {
        long long sum = 0;

        // Iterate through all numbers and multiply by its solution
        for (int i = 0; i < inst.size(); i++) {
            sum += sol[i] * inst[i];
        }
        // cout << "\t" << sum << "\t" << abs(sum) << endl;
        return (abs(sum));
    } else {
        // Use a map to find the unique groups in the array
        map<int, long long> prepar;
        for (int i = 0; i < inst.size(); i++) {
            map<int, long long>::iterator find = prepar.find(sol[i]);
            if (find != prepar.end()) {
                prepar[sol[i]] = prepar[sol[i]] + inst[i];
            } else {
                prepar.emplace(sol[i], inst[i]);
            }
        }

        // Push unique values into heap
        vector<long long> out;
        map<int, long long>::iterator vals = prepar.begin();
        while (vals != prepar.end()) {
            out.push_back(vals->second);
            vals++;
        }

        // Run specified algorithm
        return (kk(out));
    }
}