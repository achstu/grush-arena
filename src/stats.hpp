#pragma once

#include "constants.hpp"

#include <cassert>
#include <fstream>
#include <string>
#include <vector>

struct PlayerStats {
  long long microseconds;
  int gold;
  int alive_agents;
  std::string command;
  // TODO: command is const acrros iterations
  //       and should be removed from stats
};

using GameStats = std::vector<PlayerStats>;
using History = std::vector<GameStats>;

static inline void export_history(const History &h) {
  // player data
  const static std::string color_names[] = {"red", "green", "blue", "magenta",
                                            "cyan"};

  std::ofstream f("../report/statistics.txt");
  for (int p = 0; p < (int)h[0].size(); p++) {
    f << color_names[p] << ' ';
  }
  f << '\n';
  assert(h.size() == ITER);
  f << ITER << '\n';

  for (const auto& stats : h) {
    for (const auto& p : stats) {
      f << p.gold << ' ' << p.alive_agents << ' ' << p.microseconds<< '\n';
    }
  }
  
  f.close();
}
