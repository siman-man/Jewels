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
const int WALL = X;
const int E = -2;
const int DZ[4] = {-GRID_SIZE, 1, GRID_SIZE, -1};

inline int calcZ(int y, int x) {
  return x * GRID_SIZE + y;
}

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
  int fromZ;
  int toY;
  int toX;
  int toZ;

  Move(int fromY = -1, int fromX = -1, int toY = -1, int toX = -1) {
    this->fromY = fromY;
    this->fromX = fromX;
    this->fromZ = calcZ(fromY, fromX);
    this->toY = toY;
    this->toX = toX;
    this->toZ = calcZ(toY, toX);
  }

  string to_str() {
    return to_string(fromY - 1) + " " + to_string(fromX - 1) + " " + to_string(toY - 1) + " " + to_string(toX - 1);
  }
};

int N;
int C;
int g_grid[GRID_SIZE * GRID_SIZE];
int g_copyGrid[GRID_SIZE * GRID_SIZE];
int g_targetGrid[GRID_SIZE * GRID_SIZE];
int g_jewelsCounter[MAX_C + 1];
ll g_removed[GRID_SIZE * GRID_SIZE];
int g_turn;
ll g_removeId;

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
      // fprintf(stderr, "turn %d: \n", g_turn);
      Move move = selectBestMove();
      // Move move(r1, c1, r2, c2);
      cout << move.to_str() << endl;
      cout.flush();
      readGridData();
      updateGridData();
      int runtime;
      cin >> runtime;
      ++g_turn;

      // fprintf(stderr, "[%d]: runtime: %d\n", g_turn, runtime);
    }
  }

  Move selectBestMove() {
    int bestScore = -1;
    Move bestMove;

    for (int x = 1; x <= N; ++x) {
      for (int y = 1; y <= N; ++y) {
        int z1 = calcZ(y, x);

        for (int x2 = x; x2 <= N; ++x2) {
          for (int y2 = y; y2 <= N; ++y2) {
            int z2 = calcZ(y2, x2);
            if (z2 <= z1) continue;
            if (g_grid[z1] == g_grid[z2]) continue;

            memcpy(g_copyGrid, g_grid, sizeof(g_grid));

            Move move(y, x, y2, x2);
            int score = applyMove(move);

            if (bestScore < score) {
              bestScore = score;
              bestMove = move;
            }

            memcpy(g_grid, g_copyGrid, sizeof(g_copyGrid));
          }
        }
      }
    }

    return bestMove;
  }

  bool isFire(int z) {
    for (int direct = 0; direct < 4; ++direct) {
      int nz = z + DZ[direct];
      if (g_grid[nz] == WALL) continue;
      int nnz = nz + DZ[direct];
      int bnz = z + DZ[(direct + 2) % 4];

      if (g_grid[z] == g_grid[nz]) {
        if (g_grid[z] == g_grid[nnz]) return true;
        if (g_grid[z] == g_grid[bnz]) return true;
      }
    }

    return false;
  }

  int applyMove(Move &move) {
    int moveScore = 0;
    int combo = 0;
    bool matched = true;
    swap(g_grid[move.fromZ], g_grid[move.toZ]);

    if (!isFire(move.fromZ) && !isFire(move.toZ)) {
      return 0;
    }

    while (matched) {
      matched = false;
      ++g_removeId;

      // check vertical line
      for (int y = 1; y <= N; ++y) {
        int match = 0;

        for (int x = 1; x < N; ++x) {
          int z1 = calcZ(y, x);
          int z2 = calcZ(y, x + 1);

          if (g_grid[z1] != E && g_grid[z1] == g_grid[z2]) {
            ++match;
          } else {
            if (match >= 2) {
              moveScore += calcLineScore(match + 1);
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
          moveScore += calcLineScore(match + 1);
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

          if (g_grid[z1] != E && g_grid[z1] == g_grid[z2]) {
            ++match;
          } else {
            if (match >= 2) {
              moveScore += calcLineScore(match + 1);
              matched = true;

              for (int y2 = y - match; y2 <= y; ++y2) {
                int z = calcZ(y2, x);
                g_removed[z] = g_removeId;
              }
            }

            match = 0;
          }
        }

        if (match >= 2) {
          moveScore += calcLineScore(match + 1);
          matched = true;

          for (int y2 = N - match; y2 <= N; ++y2) {
            int z = calcZ(y2, x);
            g_removed[z] = g_removeId;
          }
        }
      }

      // fall jewels
      for (int x = 1; x <= N; ++x) {
        int removedCnt = 0;

        for (int y = 1; y <= N; ++y) {
          int z = calcZ(y, x);

          if (g_removed[z] == g_removeId) {
            ++removedCnt;
          }

          int temp = g_grid[z];
          int nz = calcZ(y - removedCnt, x);
          g_grid[z] = E;
          g_grid[nz] = temp;
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