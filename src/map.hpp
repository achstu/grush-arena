#pragma once

#include "action.hpp"
#include <format>
#include <vector>

const int N = 45;
const int ITER = 1000;

enum Square { BLOCK, EMPTY };

struct Position {
  int row;
  int col;

  bool operator==(const Position& other) const {
    return (row == other.row) && (col == other.col);
  }
  std::string to_string() const { return std::format("{} {}", row, col); }
  // inline int hash() const { return N * row + col; }
};

enum Orientation { U, R, D, L };

enum VisualObject { WALL, GOLD, ALLY, ENEMY };

struct Agent;

struct Vision {
  int distance;
  VisualObject object;
  Agent* agent_ptr;
};

struct Agent {
  Vision vision;
  Position position;
  Orientation orientation;
  bool holding_gold;
  bool zombie;
  Action action;
  int owner;

  Agent();
  // void rotate(Action action);
  Position target() const;
  std::string describe() const;
};

struct Player {
  Position base;
  std::vector<Agent> agents;
};

struct Grush {
  Square square[N][N];
  int gold[N][N];
  int congestation[N][N];
  Agent *agents_ptr[N][N];
  std::vector<Player> players;

  // Grush() = default;
  static Grush empty();
  static Grush semi_random(int num_players, int num_agents);
  void update();
  void update_vision();
  Vision calculate_vision(int, Position, Orientation);

  std::string to_string() const;
};
