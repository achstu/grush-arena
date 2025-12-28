#pragma once

#include <iterator>
#include <ranges>
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
  bool has_gold;
  bool is_zombie;
  bool is_alive;
  Action action;
  int owner;

  Agent(Position p, Orientation o, int owning_player);
  std::string describe() const;
};

struct Player {
private:
  std::vector<Agent> agents;
  static inline bool is_alive(const Agent &a) { return a.is_alive; }

public:
  Position base;

  Player(Position b, std::vector<Agent> &&a);

  auto alive_agents() { return std::ranges::filter_view(agents, is_alive); }
  auto alive_agents() const {
    return std::ranges::filter_view(agents, is_alive);
  }

  int alive_agents_count() const {
    return std::ranges::distance(alive_agents());
  }
};
