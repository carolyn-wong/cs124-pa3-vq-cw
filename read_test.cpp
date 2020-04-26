#include <iostream>
#include <fstream>
#include <vector>

using namespace std;


int main(int argc, char* argv[]){



  int dim = atoi(argv[1]);
  string file = argv[2];
  // Read numerical values from file
  ifstream infile;
  infile.open(file);

  vector<long long> npInst;
  npInst.resize(dim);

  char temp[256];
  cout << "start" << endl;
  for (int i = 0; i < dim; i++) {
      infile.getline(temp, 30);
      npInst[i] = stoll(temp);
      cout << npInst[i] << endl;
  }
  cout << "midend" << endl;
  for (int i = 0; i < dim; i++){
    cout << npInst[i] << endl;
  }


  return 0;
}
