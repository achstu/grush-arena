#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
using namespace std;

#if true
#define debug(...) {};
#else
#include "debug.hpp"
#endif


int main() {
  // vector<string> actions = {"FIRE", "GO", "MINE", "LEFT", "RIGHT", "BACK"};
  vector<string> actions = {"GO", "GO", "GO", "LEFT", "RIGHT", "BACK"};
  random_device rd;
  mt19937 g(rd());

  int board_size, turns;
  cin >> board_size >> turns;
  debug(board_size, turns);

  int players;
  cin >> players;
  debug(players);
  for (int p = 0; p < players; p++) {
    int row, col;
    cin >> row >> col;
    debug(row, col);
  }

  for (int t = 0; t < turns; t++) {
    int agents;
    cin >> agents;
    debug(agents);

    for (int i = 0; i < agents; i++) {
      int row, col;
      string type;
      int dist;
      char rot;
      bool has_gold;
      cin >> row >> col >> type >> dist >> rot >> has_gold;
      debug(row, col, type, dist, rot, has_gold);
    }

    for (int i = 0; i < agents; i++) {
      shuffle(actions.begin(), actions.end(), g);
      cout << actions.front() << "\n";
    }
    cout.flush();
  }
}
