#include <cassert>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <queue>
#include <set>
#include <cstring>
#include <stack>

using namespace std;
typedef long long ll;

unsigned long long xor128() {
  static unsigned long long rx = 123456789, ry = 362436069, rz = 521288629, rw = 88675123;
  unsigned long long rt = (rx ^ (rx << 11));
  rx = ry;
  ry = rz;
  rz = rw;
  return (rw = (rw ^ (rw >> 19)) ^ (rt ^ (rt >> 8)));
}

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

const int BASE_SCORE_PATTERN[9][GRID_SIZE * GRID_SIZE] = {
  // N = 8
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
  },
  // N = 9
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
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
  },
  // N = 10
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
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
  },
  // N = 11
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
    X, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
  },
  // N = 12
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
    X, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
  },
  // N = 13
  {
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 1, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 0, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
  },
  // N = 14
  {
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 1, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 0, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
  },
  // N = 15
  {
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 1, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 0, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
  },
  // N = 16
  {
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 1, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 0, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
  },
};

const int CHAIN_PATTERN[9][GRID_SIZE * GRID_SIZE] = {
  // N = 8
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
  },
  // N = 9
  {
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, 0, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 1, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 2, 3, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, 3, 4, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 3, 4, 5, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 4, 5, 6, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 5, 6, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 6, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
  },
  // N = 10
  {
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, 0, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 1, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 2, 3, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, 3, 4, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 3, 4, 5, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 4, 5, 6, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 5, 6, 7, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 6, 7, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 7, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
  },
  // N = 11
  {
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, 0, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 1, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 2, 3, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, 3, 4, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 3, 4, 5, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 4, 5, 6, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 5, 6, 7, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 6, 7, 8, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 7, 8, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 8, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
  },
  // N = 12
  {
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, 0, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 1, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 2, 3, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, 3, 4, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 3, 4, 5, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 4, 5, 6, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 5, 6, 7, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 6, 7, 8, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 7, 8, 9, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 8, 9, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 9, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
  },
  // N = 13
  {
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, 0, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 1, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 2, 3, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, 3, 4, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 3, 4, 5, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 4, 5, 6, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 5, 6, 7, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 6, 7, 8, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 7, 8, 9, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 8, 9, 10, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 9, 10, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 10, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
  },
  // N = 14
  {
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, 0, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 1, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 2, 3, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, 3, 4, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 3, 4, 5, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 4, 5, 6, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 5, 6, 7, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 6, 7, 8, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 7, 8, 9, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 8, 9, 10, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 9, 10, 11, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 10, 11, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 11, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
  },
  // N = 15
  {
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, 0, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 1, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 2, 3, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, 3, 4, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 3, 4, 5, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 4, 5, 6, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 5, 6, 7, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 6, 7, 8, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 7, 8, 9, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 8, 9, 10, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 9, 10, 11, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 10, 11, 12, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 11, 12, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 12, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
  },
  // N = 16
  {
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, 0, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 1, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 0, 1, 2, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 1, 2, 3, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 2, 3, 4, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 3, 4, 5, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 4, 5, 6, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 5, 6, 7, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 6, 7, 8, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 7, 8, 9, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 8, 9, 10, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 9, 10, 11, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 10, 11, 12, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 11, 12, 13, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 12, 13, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, 13, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
  },
};

struct Chunk {
  int id;
  int cnt;

  Chunk(int id = -1, int cnt = -1) {
    this->id = id;
    this->cnt = cnt;
  }

  bool operator>(const Chunk &n) const {
    return cnt < n.cnt;
  }
};

struct Jewel {
  int color;
  int cnt;

  Jewel(int color = -1, int cnt = -1) {
    this->color = color;
    this->cnt = cnt;
  }

  bool operator>(const Jewel &n) const {
    return cnt < n.cnt;
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
int g_originGrid[GRID_SIZE * GRID_SIZE];
int g_copyGrid[GRID_SIZE * GRID_SIZE];
int g_mappingGrid[GRID_SIZE * GRID_SIZE];
int g_targetGrid[GRID_SIZE * GRID_SIZE];
int g_jewelsCounter[MAX_C + 1];
int g_remainJewelsCounter[MAX_C + 1];
int g_jewelsMapping[GRID_SIZE * GRID_SIZE];
int g_mappingCount[GRID_SIZE * GRID_SIZE];
int g_chunkCounter[GRID_SIZE * GRID_SIZE];
int g_lineHeight[GRID_SIZE];
ll g_removed[GRID_SIZE * GRID_SIZE];
int g_turn;
int g_buildTargetGridCnt;
int g_mappingId;
ll g_removeId;
queue <Move> g_moveQueue;

class JewelsSolver {
public:
  void init() {
    cin >> N;
    cin >> C;

    fprintf(stderr, "N: %d, C: %d\n", N, C);

    g_turn = 0;
    g_removeId = 0;
    g_buildTargetGridCnt = 0;
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

    updateGridData();
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
    Move move;
    int remain = 9999;
    int extLine = 2;

    for (int i = 0; i < MOVE_NUM; i++) {
      memcpy(g_originGrid, g_grid, sizeof(g_grid));
      // fprintf(stderr, "turn %d: \n", g_turn);

      if (g_moveQueue.empty()) {
        if (N >= 14) {
          extLine = 6;
        } else if (N >= 12) {
          extLine = 5;
        } else if (N >= 10) {
          extLine = 4;
        } else if (N >= 9) {
          extLine = 3;
        } else {
          extLine = 2;
        }

        while (g_moveQueue.empty() && extLine >= 0) {
          buildMappingGrid(extLine);

          if (buildTargetGrid()) {
            buildMoves();
          }

          --extLine;
        }

        if (g_moveQueue.size() > 0) {
          fprintf(stderr, "[turn %d] Que size: %d, extLine: %d\n", g_turn, (int) g_moveQueue.size(), extLine + 1);
          ++g_buildTargetGridCnt;
        }
      }

      if (g_moveQueue.empty()) {
        if (remain < 1000) {
          move = Move(1, 1, 1, 2);
        } else {
          move = selectBestMove();
        }
      } else {
        do {
          move = g_moveQueue.front();
          g_moveQueue.pop();
        } while (g_moveQueue.size() > 0 && g_originGrid[move.fromZ] == g_originGrid[move.toZ]);
      }
      // Move move(r1, c1, r2, c2);
      // assert(g_originGrid[move.fromZ] != g_originGrid[move.toZ]);
      cout << move.to_str() << endl;
      cout.flush();
      readGridData();
      int runtime;
      cin >> runtime;
      remain = 10000 - runtime;
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

  int applyMove(Move &move, bool skipFireCheck = false) {
    int moveScore = 0;
    int combo = 0;
    bool matched = true;
    swap(g_grid[move.fromZ], g_grid[move.toZ]);

    if (!skipFireCheck && !isFire(move.fromZ) && !isFire(move.toZ)) {
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

  void buildMappingGrid(int extLine) {
    clearMappingGrid();
    mergeScoreGrid(extLine);
    mergeChainGrid();
  }

  bool buildTargetGrid() {
    memcpy(g_copyGrid, g_grid, sizeof(g_grid));

    clearTargetGrid();
    mappingJewelsToTargetGrid();
    memcpy(g_grid, g_targetGrid, sizeof(g_targetGrid));

    Move move(1, 1, 1, 1);
    int score = applyMove(move, true);
    // showTargetGrid();
    int limit = 100;

    while (score != 0 && limit > 0) {
      shuffleMapping();
      mappingToTargetGrid();
      memcpy(g_grid, g_targetGrid, sizeof(g_targetGrid));
      score = applyMove(move, true);
      --limit;
    }

    memcpy(g_grid, g_copyGrid, sizeof(g_copyGrid));
    if (limit <= 0) return false;

    assert(score == 0);
    // showTargetGrid();

    return true;
  }

  void buildMoves() {
    fprintf(stderr, "buildMoves =>\n");
    memcpy(g_copyGrid, g_grid, sizeof(g_grid));
    vector <Move> moves;
    queue<int> needExchangePositions;

    for (int y = 1; y <= N; ++y) {
      for (int x = 1; x <= N; ++x) {
        int z = calcZ(y, x);
        if (g_targetGrid[z] == E) continue;
        if (g_targetGrid[z] == g_grid[z]) continue;

        needExchangePositions.push(z);
      }
    }

    int limit = 1000;
    memcpy(g_grid, g_originGrid, sizeof(g_originGrid));

    while (!needExchangePositions.empty() && limit > 0) {
      int z = needExchangePositions.front();
      int x = z / GRID_SIZE;
      int y = z % GRID_SIZE;
      needExchangePositions.pop();

      Move move = findExchangeMove(g_targetGrid[z], y, x);
      if (move.fromY == -1) {
        if (needExchangePositions.empty()) {
          memcpy(g_grid, g_copyGrid, sizeof(g_copyGrid));
          return;
        } else {
          needExchangePositions.push(z);
        }
      } else {
        moves.push_back(move);
      }

      --limit;
    }

    if (limit <= 0) return;

    for (int i = 0; i < moves.size(); ++i) {
      Move move = moves[i];
      g_moveQueue.push(move);
    }

    if (g_moveQueue.size() > 0) {
      if (N <= 12) {
        g_moveQueue.push(Move(1, 1, 1, 8));
      } else if (N == 13) {
        g_moveQueue.push(Move(1, 2, 1, 9));
      } else if (N == 14) {
        g_moveQueue.push(Move(1, 3, 1, 10));
      } else if (N == 15) {
        g_moveQueue.push(Move(1, 4, 1, 11));
      } else if (N == 16) {
        g_moveQueue.push(Move(1, 5, 1, 12));
      }
    }

    memcpy(g_grid, g_copyGrid, sizeof(g_copyGrid));
  }

  Move findExchangeMove(int targetColor, int fromY, int fromX) {
    int fromZ = calcZ(fromY, fromX);
    int fromColor = g_grid[fromZ];

    for (int x = 1; x <= N; ++x) {
      for (int y = 1; y <= N; ++y) {
        int z = calcZ(y, x);
        if (g_targetGrid[z] == g_grid[z]) continue;
        if (g_grid[z] != targetColor) continue;
        if (g_targetGrid[z] != fromColor) continue;

        swap(g_grid[fromZ], g_grid[z]);

        if (!isFire(fromZ) && !isFire(z)) {
          return Move(fromY, fromX, y, x);
        }

        swap(g_grid[fromZ], g_grid[z]);
      }
    }

    for (int x = 1; x <= N; ++x) {
      for (int y = N; y >= 1; --y) {
        int z = calcZ(y, x);
        if (g_targetGrid[z] == g_grid[z]) continue;
        if (g_grid[z] != targetColor) continue;

        swap(g_grid[fromZ], g_grid[z]);

        if (!isFire(fromZ) && !isFire(z)) {
          return Move(fromY, fromX, y, x);
        }

        swap(g_grid[fromZ], g_grid[z]);
      }
    }

    return Move();
  }

  bool mappingJewelsToTargetGrid() {
    fprintf(stderr, "%d: mappingJewelsToTargetGrid =>\n", g_buildTargetGridCnt);
    memset(g_jewelsMapping, -1, sizeof(g_jewelsMapping));
    memcpy(g_remainJewelsCounter, g_jewelsCounter, sizeof(g_jewelsCounter));
    int maxMappingId = -1;

    memset(g_chunkCounter, 0, sizeof(g_chunkCounter));
    for (int x = 1; x <= N; ++x) {
      for (int y = 1; y <= N; ++y) {
        int z = calcZ(y, x);
        int mid = g_mappingGrid[z];
        if (mid == E) continue;
        maxMappingId = max(maxMappingId, mid);

        ++g_chunkCounter[mid];
      }
    }

    priority_queue <Chunk, vector<Chunk>, greater<Chunk>> chunkPQue;
    for (int mid = 0; mid <= maxMappingId; ++mid) {
      chunkPQue.push(Chunk(mid, g_chunkCounter[mid]));
    }

    priority_queue <Jewel, vector<Jewel>, greater<Jewel>> jewelPQue;
    for (int color = 1; color <= C; ++color) {
      jewelPQue.push(Jewel(color, g_jewelsCounter[color]));
    }

    while (!chunkPQue.empty()) {
      Chunk chunk = chunkPQue.top();
      chunkPQue.pop();
      // fprintf(stderr, "mid: %d, cnt: %d\n", chunk.id, chunk.cnt);

      if (jewelPQue.empty()) {
        return false;
      } else {
        stack <Jewel> stack;

        while (!jewelPQue.empty()) {
          Jewel jewel = jewelPQue.top();
          jewelPQue.pop();
          // fprintf(stderr, "color: %d, cnt: %d\n", jewel.color, jewel.cnt);
          if (jewel.cnt < chunk.cnt) continue;

          g_jewelsMapping[chunk.id] = jewel.color;
          g_mappingCount[chunk.id] = chunk.cnt;

          mappingToTargetGrid();
          memcpy(g_grid, g_targetGrid, sizeof(g_targetGrid));
          Move move(1, 1, 1, 1);
          int score = applyMove(move, true);

          if (score == 0) {
            g_remainJewelsCounter[jewel.color] -= chunk.cnt;
            jewel.cnt -= chunk.cnt;
            jewelPQue.push(jewel);
            break;
          } else {
            stack.push(jewel);
          }
        }

        while (!stack.empty()) {
          Jewel jewel = stack.top();
          stack.pop();
          jewelPQue.push(jewel);
        }
      }
    }

    mappingToTargetGrid();

    return true;
  }

  bool shuffleMapping() {
    int limit = 100;

    while (limit > 0) {
      --limit;

      int i = xor128() % g_mappingId;
      int j = xor128() % g_mappingId;
      int color1 = g_jewelsMapping[i];
      int color2 = xor128() % C + 1;
      if (color1 == color2) continue;

      int cnt1 = g_remainJewelsCounter[i] + (g_mappingCount[i] - g_mappingCount[j]);
      int cnt2 = g_remainJewelsCounter[j] + (g_mappingCount[j] - g_mappingCount[i]);
      if (cnt1 < 0 || cnt2 < 0) continue;

      g_remainJewelsCounter[i] = cnt1;
      g_remainJewelsCounter[j] = cnt2;
      swap(g_jewelsMapping[i], g_jewelsMapping[j]);
      return true;
    }

    return false;
  }

  void mappingToTargetGrid() {
    // fprintf(stderr, "mappingToTargetGrid =>\n");

    for (int x = 1; x <= N; ++x) {
      for (int y = 1; y <= N; ++y) {
        int z = calcZ(y, x);
        int mid = g_mappingGrid[z];
        if (mid == E) continue;
        if (g_jewelsMapping[mid] == -1) continue;

        g_targetGrid[z] = g_jewelsMapping[mid];
      }
    }
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

  void clearMappingGrid() {
    memset(g_mappingGrid, X, sizeof(g_mappingGrid));
    memset(g_lineHeight, 0, sizeof(g_lineHeight));
    g_mappingId = 0;

    for (int y = 1; y <= N; ++y) {
      for (int x = 1; x <= N; ++x) {
        int z = calcZ(y, x);
        g_mappingGrid[z] = E;
      }
    }
  }

  void mergeScoreGrid(int extLine) {
    int gid = N - 8;

    for (int x = 1; x <= N; ++x) {
      for (int y = 1; y <= N; ++y) {
        int z = calcZ(y, x);
        int id = BASE_SCORE_PATTERN[gid][z];
        if (id == E) continue;

        g_mappingId = max(g_mappingId, id + 1);
        int height = g_lineHeight[x] + 1;
        int tz = calcZ(height, x);
        g_mappingGrid[tz] = id;
        ++g_lineHeight[x];
      }
    }

    for (int i = 0; i < extLine; ++i) {
      int id = g_mappingId + 1;

      for (int x = 1; x <= N; ++x) {
        int height = g_lineHeight[x] + 1;
        assert(height <= N);
        int tz = calcZ(height, x);
        g_mappingGrid[tz] = id;
        ++g_lineHeight[x];
      }

      ++g_mappingId;
    }
  }

  void mergeChainGrid() {
    int gid = N - 8;
    int baseId = g_mappingId;

    for (int x = 1; x <= N; ++x) {
      for (int y = 1; y <= N; ++y) {
        int z = calcZ(y, x);
        int id = CHAIN_PATTERN[gid][z];
        if (id == E) continue;

        int height = g_lineHeight[x] + 1;
        int tz = calcZ(height, x);
        g_mappingId = max(g_mappingId, baseId + id);
        g_mappingGrid[tz] = baseId + id;
        ++g_lineHeight[x];
      }
    }
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

  void showMappingGrid() {
    for (int y = N + 1; y >= 0; --y) {
      for (int x = 0; x <= N + 1; ++x) {
        int z = calcZ(y, x);

        if (g_mappingGrid[z] == X) {
          fprintf(stderr, "X");
        } else if (g_mappingGrid[z] == E) {
          fprintf(stderr, " ");
        } else {
          fprintf(stderr, "%c", 'A' + g_mappingGrid[z]);
        }
      }
      fprintf(stderr, "\n");
    }

    fprintf(stderr, "\n");
  }

  void showTargetGrid() {
    for (int y = N + 1; y >= 0; --y) {
      for (int x = 0; x <= N + 1; ++x) {
        int z = calcZ(y, x);

        if (g_targetGrid[z] == X) {
          fprintf(stderr, "X");
        } else if (g_targetGrid[z] == E) {
          fprintf(stderr, " ");
        } else {
          fprintf(stderr, "%d", g_targetGrid[z]);
        }
      }
      fprintf(stderr, "\n");
    }

    fprintf(stderr, "\n");
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