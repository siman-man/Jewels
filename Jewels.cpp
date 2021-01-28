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
const int MAX_C = 10;
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
int g_grid[GRID_SIZE * GRID_SIZE];

inline int calcZ(int y, int x) {
  return y * GRID_SIZE + x;
}

class JewelsSolver {
public:
  void init() {
    cin >> N;
    cin >> C;

    fprintf(stderr, "N: %d, C: %d\n", N, C);
    memset(g_grid, X, sizeof(g_grid));
    readGridData();
  }

  void readGridData() {
    for (int y = 1; y <= N; ++y) {
      for (int x = 1; x <= N; ++x) {
        int z = calcZ(y, x);
        cin >> g_grid[z];
        if (g_grid[z] > C) {
          fprintf(stderr, "[%d, %d]: %d\n", y, x, g_grid[z]);
        }
        assert(1 <= g_grid[z] && g_grid[z] <= C);
      }
    }
  }

  void updateGridData() {
    for (int y = 1; y <= N; ++y) {
      for (int x = 1; x <= N; ++x) {
        int z = calcZ(y, x);
      }
    }
  }

  void run() {
    for (int i = 0; i < MOVE_NUM; i++) {
      int r1 = 0;
      int c1 = i % N;
      int r2 = 1 + (i % (N - 1));
      int c2 = c1;
      cout << r1 << " " << c1 << " " << r2 << " " << c2 << endl;
      cout.flush();
      readGridData();
      int runtime;
      cin >> runtime;
    }
  }

  void setupTargetGrid() {
  }

  int calcLineScore(int matches) {
    assert(matches >= 3);
    return LINE_SCORE[matches];
  }

  void showGrid() {
    for (int y = N + 1; y >= 0; --y) {
      for (int x = 0; x <= N + 1; ++x) {
        int z = calcZ(y, x);

        if (g_grid[z] == X) {
          fprintf(stderr, "X");
        } else {
          assert(0 <= g_grid[z]);
          if (g_grid[z] > C) {
            fprintf(stderr, "error: %d, [%d, %d]\n", g_grid[z], y, x);
          }
          assert(g_grid[z] <= C);
          fprintf(stderr, "%d", g_grid[z]);
        }
      }
      fprintf(stderr, "\n");
    }

    fprintf(stderr, "\n");
  }
};

int main() {
  JewelsSolver solver;
  solver.init();
  solver.run();
}