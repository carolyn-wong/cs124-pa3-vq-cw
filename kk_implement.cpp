#include <iostream>
#include <array>
#include <queue>
#include <random>
#include <chrono>

using namespace std;
using namespace std::chrono;

int kk(vector<long>& inst, int dim){     // inst is the matrix, dim is size of it

  priority_queue<long> q;
  for(int i = 0; i < dim; i++){
    q.push(inst[i]);
  }

  // while(!q.empty()) {
  //   std::cout << q.top() << " ";
  //   q.pop();
  // }
  // std::cout << '\n';
  while(q.size() > 1){
    long p1 = q.top(); q.pop();
    long p2 = q.top(); q.pop();

    q.push(abs(p1 - p2));
  }

  return q.top();
}

int main(){

  vector<long> inst = {10, 6, 7, 5, 8};
  int dim = inst.size();

  cout << kk(inst, dim) << endl;

  return 0;
}
