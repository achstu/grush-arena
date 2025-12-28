#include "map.hpp"
#include "action.hpp"
#include "stats.hpp"
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <sstream>
#include <vector>

Grush::Grush() {
  // clear map
  for (int row = 0; row < N; row++) {
    for (int col = 0; col < N; col++) {
      gold[row][col] = 0;
      square[row][col] = EMPTY;
      agents_ptr[row][col] = nullptr;
      congestation[row][col] = 0;
    }
  }

  // add border
  for (int row = 0; row < N; row++) {
    square[row][0] = BLOCK;
    square[row][N - 1] = BLOCK;
  }
  for (int col = 0; col < N; col++) {
    square[0][col] = BLOCK;
    square[N - 1][col] = BLOCK;
  }

  // init agents
  // players = {};
}

void Grush::update() {
  // check for invariants
  for (int row = 0; row < N; row++) {
    for (int col = 0; col < N; col++) {
      assert(congestation[row][col] == 0);
    }
  }
  for (auto &player : players) {
    for (auto &agent : player.alive_agents()) {
      auto [r, c] = agent.position;
      // assert(agents_ptr[r][c] != nullptr);
      assert(agents_ptr[r][c] == &agent);
    }
  }

  // mark zombie agents
  for (auto &player : players) {
    for (auto &agent : player.alive_agents()) {
      if (agent.action == FIRE && agent.vision.agent_ptr != nullptr) {
        agent.vision.agent_ptr->is_zombie = true;
      }
    }
  }
  // remove killed zombie-agents from game
  for (auto &player : players) {
    for (auto &agent : player.alive_agents()) {
      if (agent.is_zombie) {
        auto [r, c] = agent.position;
        agents_ptr[r][c] = nullptr;
        gold[r][c] += agent.has_gold;
        agent.is_alive = false;
      }
    }
  }

  // resolve rotations (LEFT, RIGHT, BACK)
  // PERF: this can be done in the first loop
  for (auto &player : players) {
    for (auto &agent : player.alive_agents()) {
      // note: if action is other than specified above
      //       then identity rotation will be applied
      agent.orientation =
          Orientation((agent.orientation + rotation(agent.action)) % 4);
    }
  }

  // mark targets squares (GO)
  // PERF: this also can be done in initial loop
  for (auto &player : players) {
    for (auto &agent : player.alive_agents()) {
      if (agent.action == GO) {
        auto [tr, tc] = agent.position.advance(agent.orientation);
        congestation[tr][tc] += 1;
      }
    }
  }
  // resolve movement, move agents
  for (auto &player : players) {
    for (auto &agent : player.alive_agents()) {
      auto [r, c] = agent.position;
      auto [tr, tc] = agent.position.advance(agent.orientation);
      if (agent.action == GO && agents_ptr[tr][tc] == nullptr &&
          square[tr][tc] == EMPTY && congestation[tr][tc] == 1) {
        agent.position = {tr, tc};
        std::swap(agents_ptr[r][c], agents_ptr[tr][tc]);
      }
    }
  }
  // PERF: change to less naive O(#agents) not O(#sqaures)
  for (int row = 0; row < N; row++) {
    for (int col = 0; col < N; col++) {
      congestation[row][col] = 0;
    }
  }

  // resolve MINE action
  // PERF: also can be done in initial loop
  for (auto &player : players) {
    for (auto &agent : player.alive_agents()) {
      auto [r, c] = agent.position;
      if (agent.action == MINE && gold[r][c] > 0 && !agent.has_gold) {
        gold[r][c]--;
        agent.has_gold = true;
      }
    }
  }

  // put gold in home base
  for (auto &player : players) {
    for (auto &agent : player.alive_agents()) {
      if (agent.position == player.base) {
        auto [r, c] = agent.position;
        gold[r][c] += agent.has_gold;
        agent.has_gold = false;
      }
    }
  }
}

Vision Grush::calculate_vision(int player, Position pos, Orientation o) {
  Position curr = pos.advance(o);

  for (int dist = 1; dist < N; dist++) {
    auto [r, c] = curr;

    if (square[r][c] == BLOCK) {
      return Vision(dist, WALL, nullptr);
    }

    if (agents_ptr[r][c] != nullptr) {
      VisualObject obj = agents_ptr[r][c]->owner == player ? ALLY : ENEMY;
      return Vision(dist, obj, agents_ptr[r][c]);
    }

    if (gold[r][c] > 0) {
      return Vision(dist, GOLD, nullptr);
    }

    curr = curr.advance(o);
  }

  assert(false);
  return Vision(0, WALL, nullptr);
}

void Grush::update_vision() {
  for (int p = 0; p < (int)players.size(); p++) {
    for (auto &agent : players[p].alive_agents()) {
      agent.vision = calculate_vision(p, agent.position, agent.orientation);
    }
  }
}

std::string Grush::to_string(const GameStats &stats) const {
  const static std::string colors[] = {
      "\033[0;41m", // red
      "\033[0;42m", // green
      "\033[0;44m", // blue
      "\033[0;45m", // magenta
      "\033[0;46m", // cyan
  };
  const static std::string colors_fg[] = {
      "\033[0;31m", // red
      "\033[0;32m", // green
      "\033[0;34m", // blue
      "\033[0;35m", // magenta
      "\033[0;36m", // cyan
  };

  const static std::string color_names[] = {"red", "green", "blue", "magenta", "cyan"};

  const static std::string reset = "\033[0m";
  const static std::string bold = "\033[1m";
  const static std::string black_fg = "\033[0;31m";
  const static std::string gold_color = "\033[0;43m"; // yellow
  const static std::string wall_color = "\033[0;47m"; // white

  std::ostringstream ss;
  for (int row = 0; row < N; row++) {
    for (int col = 0; col < N; col++) {
      if (square[row][col] == BLOCK) {

        ss << wall_color << "  " << reset;

      } else if (agents_ptr[row][col] != nullptr) {
        Agent *a = agents_ptr[row][col];
        
        ss << colors[a->owner];
        ss << bold;
        ss << (a->has_gold ? "**" : "  ");
        ss << reset;

      } else if (gold[row][col] > 0) {

        ss << gold_color << "  " << reset;
      } else {
        ss << "  ";
      }
    }

    if (row == 0 && stats.size() == players.size()) {
      ss << "\t";
      ss << "Players\t\t";
      ss << "Gold" << "\t";
      ss << "Agents" << "\t";
      ss << "Time" << "\t";
      ss << "Program" << "\t";
      ss << reset;
    }

    else if (stats.size() == players.size() && row - 1 < (int)players.size()) {
      int p = row - 1;

      ss << "\t";
      ss << bold << colors_fg[p] << color_names[p] << reset << "\t\t";
      // if (p != 3) ss << "\t";
      ss << bold;
      ss << stats[p].gold << "\t";
      ss << stats[p].alive_agents << "\t";
      ss << stats[p].time << "us\t";
      ss << stats[p].command << "\t";
      ss << reset;
    }
    ss << "\n";
  }

  return ss.str();
}

int manhattan_distance(Position a, Position b) {
  return std::abs(a.row - b.row) + std::abs(a.col - b.col);
}

Grush Grush::semi_random(int num_players, int num_agents) {
  srand(time(nullptr));

  Grush grush;

  const static Position bases[] = {
      {1, 1}, {N - 2, N - 2}, {1, N - 2}, {N - 2, 1}};

  for (int row = 1; row < N - 1; row++) {
    for (int col = 1; col < N - 1; col++) {
      bool skip = false;
      for (int i = 0; i < 4; i++) {
        if (bases[i] == Position{row, col})
          skip = true;
      }
      if (skip)
        continue;

      if (rand() % 100 == 0)
        grush.gold[row][col] += 100;
      else if (rand() % 10 == 0)
        grush.square[row][col] = BLOCK;
    }
  }

  bool occupied[N][N];
  for (int row = 1; row < N - 1; row++) {
    for (int col = 1; col < N - 1; col++) {
      occupied[row][col] = false;
    }
  }
  std::function<Position(Position)> find_position;
  find_position = [&](Position base) -> Position {
    Position p = {rand() % N, rand() % N};
    auto [r, c] = p;
    if (occupied[r][c] || grush.square[r][c] == BLOCK ||
        manhattan_distance(p, base) > 20) {
      return find_position(base);
    }
    return p;
  };

  for (int p = 0; p < num_players; p++) {
    std::vector<Agent> agents;

    for (int i = 0; i < num_agents; i++) {
      Agent agent(find_position(bases[p]), U, p);
      auto [r, c] = agent.position;
      occupied[r][c] = true;
      agents.push_back(std::move(agent));
    }

    grush.players.emplace_back(bases[p], std::move(agents));
  }

  for (auto &player : grush.players) {
    for (auto &agent : player.alive_agents()) {
      auto [r, c] = agent.position;
      grush.agents_ptr[r][c] = &agent;
    }
  }

  return grush;
}
