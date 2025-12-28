#pragma once

#include <string>
#include <vector>

#include "action.hpp"

enum Orientation { U, R, D, L };

struct Position {
  int row;
  int col;

  bool operator==(const Position &other) const;
  std::string to_string() const;
  Position advance(Orientation o) const;
};

enum VisualObject { WALL, GOLD, ALLY, ENEMY };

struct Agent;

struct Vision {
  int distance;
  VisualObject object;
  Agent *agent_ptr;

  Vision(int d, VisualObject o, Agent *ptr);
};

struct Agent {
  Vision vision;
  Position position;
  Orientation orientation;
  bool holding_gold;
  bool zombie;
  Action action;
  int owner;

  Agent(Position p, Orientation o, int owning_player);
  std::string describe() const;
};

struct Player {
  Position base;
  std::vector<Agent> agents;

  Player(Position b, std::vector<Agent> &&a);
};
