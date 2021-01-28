#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <set>
#include <string>

using namespace std;

const int MAX_N = 16;
const int MOVE_NUM = 1000;
const int GRID_SIZE = MAX_N + 2;
const int X = -1;
const int E = -2;

const int LINE_SCORE[MAX_N + 1] = {
  0, 0, 0, 1, 4, 9, 16, 25, 36, 49, 64, 81, 100, 121, 144, 169, 196
};

const int BASE_SCORE_PATTERN[1][GRID_SIZE * GRID_SIZE] = {
  {
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, 0, 1, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 0, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
  }
};

const int CHAIN_PATTERN[1][GRID_SIZE * GRID_SIZE] = {
  {
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, 0, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 1, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 2, 3, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, 3, 4, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 3, 4, 5, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 4, 5, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 5, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
  }
};

int N;
int C;
int grid[GRID_SIZE * GRID_SIZE];

class JewelsSolver {
public:
  void init() {
  }

  void run() {
    for (int i = 0; i < MOVE_NUM; i++) {
      int r1 = 0;
      int c1 = i % N;
      int r2 = 1 + (i % (N - 1));
      int c2 = c1;
      cout << r1 << " " << c1 << " " << r2 << " " << c2 << endl;
      cout.flush();
      for (int j = 0; j < N * N; j++) {
        cin >> grid[j];
      }
      int runtime;
      cin >> runtime;
    }
  }

  int calcLineScore(int matches) {
    assert(matches >= 3);
    return LINE_SCORE[matches];
  }
};

int main() {
  cin >> N;
  cin >> C;
  for (int i = 0; i < N * N; i++) {
    cin >> grid[i];
  }

  JewelsSolver solver;
  solver.run();
}