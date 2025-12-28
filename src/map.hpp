#pragma once

#include "player.hpp"
#include "stats.hpp"

#include <vector>

const int N = 45;
const int ITER = 1000;

enum Square { BLOCK, EMPTY };

struct Grush {
  Square square[N][N];
  int gold[N][N];
  int congestation[N][N];
  Agent *agents_ptr[N][N];
  std::vector<Player> players;

  // static Grush empty();
  Grush(); // empty map, no players
  static Grush semi_random(int num_players, int num_agents);
  void update();
  void update_vision();
  Vision calculate_vision(int, Position, Orientation);

  std::string to_string(const GameStats& stats = {}) const;
};
