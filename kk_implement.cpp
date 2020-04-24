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
int insert(vector<long>& heap, long val) {
    heap.push_back(val);  // stick new value at the end
    int index = heap.size() - 1;

    long temp;
    while (index != 0 && heap[parent(index)] <
                             heap[index]) {  // bubble while parents are smaller
        temp = heap[parent(index)];          // swapping steps
        heap[parent(index)] = heap[index];
        heap[index] = temp;

        index = parent(index);
    }
}

// Max the Heap, make sure it follows the property
void maxHeapify(vector<long>& heap, int root) {
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

    long temp;
    if (smallest != root) {             // bubble while parents are smaller
        temp = heap[parent(smallest)];  // swapping steps
        heap[parent(smallest)] = heap[smallest];
        heap[smallest] = temp;

        // MinHeapify again
        maxHeapify(heap, smallest);
    }
}

// pop the root of the heap
long deleteMax(vector<long>& heap) {
    long max = heap[0];

    heap[0] = heap.back();
    heap.pop_back();

    maxHeapify(heap, 0);
    return (max);
}

int kk(vector<long>& inst) {  // inst is the matrix, dim is size of it

    vector<long> heap;
    for (int i = 0; i < inst.size(); i++) {
        insert(heap, inst[i]);
    }

    // for(int i = 0; i < heap.size(); i++){
    //   cout << heap[i] << "\t" << i << endl;
    // }
    while (heap.size() > 1) {
        long p1 = deleteMax(heap);
        long p2 = deleteMax(heap);
        insert(heap, abs(p1 - p2));
    }

    return (heap[0]);
}

// Computes residue given a number partition instance and representation.
// inst is the elements, sol is the given solution, repr is type of
// representation (0 is +/- 1, 1 is prepartition)
int resid(vector<long>& inst, vector<int>& sol, int repr) {
    if (repr == 0) {
        long sum = 0;

        for (int i = 0; i < inst.size(); i++) {
            sum += inst[i];
        }
        return (sum);
    } else {
        // Use a map to find the unique groups in the array
        map<int, long> prepar;
        for (int i = 0; i < inst.size(); i++) {
            map<int, long>::iterator find = prepar.find(sol[i]);
            if (find != prepar.end()) {
                prepar[sol[i]] = prepar[sol[i]] + inst[i];
            } else {
                prepar.emplace(sol[i], inst[i]);
            }
        }

        // Push unique values into heap
        vector<long> out;
        map<int, long>::iterator vals = prepar.begin();
        while (vals != prepar.end()) {
            out.push_back(vals->second);
            vals++;
        }

        for (int i = 0; i < out.size(); i++) {
            cout << out[i] << "\t" << endl;
        }

        // Run the KK algorithm
        return (kk(out));
    }
}

// int main(){

//   vector<long> inst = {10, 6, 7, 5, 8};
//   vector<int> sol = {1, 2, 2, 4, 5};
//   cout << resid(inst, sol, 1) << endl;
//   // cout << kk(inst) << endl;

//   return 0;
// }
