#pragma once

#include <vector>
#include <string>

struct PlayerStats {
  long long time;
  int gold;
  int alive_agents;
  std::string command;
};

using GameStats = std::vector<PlayerStats>;
