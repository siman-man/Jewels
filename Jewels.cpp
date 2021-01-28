#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <set>
#include <string>

using namespace std;
typedef long long ll;

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

struct Move {
  int fromY;
  int fromX;
  int toY;
  int toX;

  Move(int fromY = -1, int fromX = -1, int toY = -1, int toX = -1) {
    this->fromY = fromY;
    this->fromX = fromX;
    this->toY = toY;
    this->toX = toX;
  }

  string to_str() {
    return to_string(fromY) + " " + to_string(fromX) + " " + to_string(toY) + " " + to_string(toX);
  }
};

int N;
int C;
int g_grid[GRID_SIZE * GRID_SIZE];
int g_targetGrid[GRID_SIZE * GRID_SIZE];
int g_jewelsCounter[MAX_C + 1];
ll g_removed[GRID_SIZE * GRID_SIZE];
int g_turn;
ll g_removeId;

inline int calcZ(int y, int x) {
  return x * GRID_SIZE + y;
}

class JewelsSolver {
public:
  void init() {
    cin >> N;
    cin >> C;

    fprintf(stderr, "N: %d, C: %d\n", N, C);

    g_turn = 0;
    g_removeId = 0;
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
    memset(g_jewelsCounter, 0, sizeof(g_jewelsCounter));

    for (int y = 1; y <= N; ++y) {
      for (int x = 1; x <= N; ++x) {
        int z = calcZ(y, x);
        int color = g_grid[z];
        g_jewelsCounter[color]++;
      }
    }
  }

  void run() {
    showGrid();
    for (int i = 0; i < MOVE_NUM; i++) {
      int r1 = 0;
      int c1 = i % N;
      int r2 = 1 + (i % (N - 1));
      int c2 = c1;
      Move move(r1, c1, r2, c2);
      cout << move.to_str() << endl;
      cout.flush();
      readGridData();
      updateGridData();
      int runtime;
      cin >> runtime;
      ++g_turn;
    }
  }

  int applyMove() {
    int moveScore = 0;
    int combo = 0;
    bool matched = true;

    while (matched) {
      matched = false;
      ++g_removeId;

      // check vertical line
      for (int y = 1; y <= N; ++y) {
        int match = 0;

        for (int x = 1; x < N; ++x) {
          int z1 = calcZ(y, x);
          int z2 = calcZ(y, x + 1);

          if (g_grid[z1] == g_grid[z2]) {
            ++match;
          } else {
            if (match >= 2) {
              matched = true;

              for (int x2 = x - match; x2 <= x; ++x2) {
                int z = calcZ(y, x2);
                g_removed[z] = g_removeId;
              }
            }

            match = 0;
          }
        }

        if (match >= 2) {
          matched = true;

          for (int x2 = N - match; x2 <= N; ++x2) {
            int z = calcZ(y, x2);
            g_removed[z] = g_removeId;
          }
        }
      }

      // check horizontal line
      for (int x = 1; x <= N; ++x) {
        int match = 0;

        for (int y = 1; y < N; ++y) {
          int z1 = calcZ(y, x);
          int z2 = calcZ(y + 1, x);

          if (g_grid[z1] == g_grid[z2]) {
            ++match;
          } else {
            matched = true;

            for (int y2 = y - match; y2 <= y; ++y2) {
              int z = calcZ(y2, x);
              g_removed[z] = g_removeId;
            }

            match = 0;
          }
        }

        for (int y2 = N - match; y2 <= N; ++y2) {
          int z = calcZ(y2, x);
          g_removed[z] = g_removeId;
        }
      }

      // fall jewels
      for (int y = 1; y <= N; ++y) {
        for (int x = 1; x <= N; ++x) {
        }
      }

      if (matched) ++combo;
    }

    return moveScore * combo;
  }

  void selectBestGrid() {
  }

  void mergeGrid() {
    memset(g_targetGrid, X, sizeof(g_targetGrid));
    int jewelsCount[N + 1];
    memset(jewelsCount, 0, sizeof(jewelsCount));

    // merge score grid
    mergeScoreGrid();

    // merge chain grid
    mergeChainGrid();
  }

  void clearTargetGrid() {
    memset(g_targetGrid, X, sizeof(g_targetGrid));

    for (int y = 1; y <= N; ++y) {
      for (int x = 1; x <= N; ++x) {
        int z = calcZ(y, x);
        g_targetGrid[z] = E;
      }
    }
  }

  void mergeScoreGrid() {
    int id = 0;
    bool updated = true;
  }

  void mergeChainGrid() {
    int id = 0;
    bool updated = true;
  }

  int calcLineScore(int matches) {
    assert(matches >= 3);
    return LINE_SCORE[matches];
  }

  void showColorCount() {
    fprintf(stderr, "%d: (", g_turn);
    for (int color = 1; color <= C; ++color) {
      fprintf(stderr, "%2d", g_jewelsCounter[color]);
      if (color < C) fprintf(stderr, ", ");
    }
    fprintf(stderr, ")\n");
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