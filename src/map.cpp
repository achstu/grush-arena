#include "map.hpp"
#include "action.hpp"
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <format>
#include <sstream>
#include <vector>

std::string Agent::describe() const {
  auto vision_to_string = [](VisualObject vo) {
    switch (vo) {
    case WALL:
      return "WALL";
    case GOLD:
      return "GOLD";
    case ALLY:
      return "ALLY";
    case ENEMY:
      return "ENEMY";
    }
    assert(false);
  };
  auto orientation_to_string = [](Orientation o) {
    switch (o) {
    case U:
      return "U";
    case R:
      return "R";
    case D:
      return "D";
    case L:
      return "L";
    }
    assert(false);
  };

  return std::format("{} {} {} {}", position.to_string(),
                     vision_to_string(vision.object), vision.distance,
                     orientation_to_string(orientation));
}

Agent::Agent() {
  holding_gold = false;
  zombie = false;
  owner = 0; // TODO: owning player
}

static inline Position advance(Position p, Orientation o) {
  auto [r, c] = p;
  switch (o) {
  case U:
    return {r - 1, c};
  case R:
    return {r, c + 1};
  case D:
    return {r + 1, c};
  case L:
    return {r, c - 1};
  }
  assert(false);
}

inline Position Agent::target() const { return advance(position, orientation); }

Grush Grush::empty() {
  Grush grush;
  // clear map
  for (int row = 0; row < N; row++) {
    for (int col = 0; col < N; col++) {
      grush.gold[row][col] = 0;
      grush.square[row][col] = EMPTY;
      grush.agents_ptr[row][col] = nullptr;
      grush.congestation[row][col] = 0;
    }
  }

  // add border
  for (int row = 0; row < N; row++) {
    grush.square[row][0] = BLOCK;
    grush.square[row][N - 1] = BLOCK;
  }
  for (int col = 0; col < N; col++) {
    grush.square[0][col] = BLOCK;
    grush.square[N - 1][col] = BLOCK;
  }

  // init agents
  grush.players = {};

  return grush;
}

void Grush::update() {
  // assert(turns.size() == players.size());

  // save input in agents
  // for (int p = 0; p < players.size(); p++) {
  // assert(players[p].agents.size() == turns[p].size());

  // for (int i = 0; i < turns[p].size(); i++) {
  // players[p].agents[i].action = turns[p][i];
  // }
  // }

  for (int row = 0; row < N; row++) {
    for (int col = 0; col < N; col++) {
      assert(congestation[row][col] == 0);
    }
  }
  for (auto &player : players) {
    for (auto &agent : player.agents) {
      auto [r, c] = agent.position;
      assert(agents_ptr[r][c] != nullptr);
      // assert(agents_ptr[r][c] == &agent);
    }
  }

  // mark zombie agents
  for (auto &player : players) {
    for (auto &agent : player.agents) {
      if (agent.action == FIRE && agent.vision.agent_ptr != nullptr) {
        agent.vision.agent_ptr->zombie = true;
      }
    }
  }
  // remove killed zombie-agents from game
  for (auto &player : players) {
    std::vector<Agent> &agents = player.agents;
    for (int i = agents.size() - 1; i >= 0; i--) {
      auto [r, c] = agents[i].position;
      if (agents[i].zombie) {
        agents_ptr[r][c] = nullptr;
        gold[r][c] += agents[i].holding_gold;

        // remove from vector
        std::swap(agents[i], agents.back());
        agents.pop_back();
      }
    }
  }

  // resolve rotations (LEFT, RIGHT, BACK)
  // PERF: this can be done in the first loop
  for (auto &player : players) {
    for (auto &agent : player.agents) {
      // note: if action is other than specified above
      //       then identity rotation will be applied
      agent.orientation =
          Orientation((agent.orientation + rotation(agent.action)) % 4);
    }
  }

  // mark targets squares (GO)
  // PERF: this also can be done in initial loop
  for (auto &player : players) {
    for (auto &agent : player.agents) {
      if (agent.action == GO) {
        auto [tr, tc] = advance(agent.position, agent.orientation);
        congestation[tr][tc] += 1;
      }
    }
  }
  // resolve movement, move agents
  for (auto &player : players) {
    for (auto &agent : player.agents) {
      auto [r, c] = agent.position;
      auto [tr, tc] = advance(agent.position, agent.orientation);
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
    for (auto &agent : player.agents) {
      auto [r, c] = agent.position;
      if (agent.action == MINE && gold[r][c] > 0 && !agent.holding_gold) {
        gold[r][c]--;
        agent.holding_gold = true;
      }
    }
  }

  // put gold in home base
  for (auto &player : players) {
    for (auto &agent : player.agents) {
      if (agent.position == player.base) {
        auto [r, c] = agent.position;
        gold[r][c] += agent.holding_gold;
        agent.holding_gold = false;
      }
    }
  }
}

Vision Grush::calculate_vision(int player, Position pos, Orientation o) {
  Position curr = advance(pos, o);

  for (int dist = 1; dist < N; dist++) {
    auto [r, c] = curr;

    if (square[r][c] == BLOCK) {
      return {dist, WALL, nullptr};
    }

    if (agents_ptr[r][c] != nullptr) {
      VisualObject obj = agents_ptr[r][c]->owner == player ? ALLY : ENEMY;
      return {dist, obj, agents_ptr[r][c]};
    }

    if (gold[r][c] > 0) {
      return {dist, GOLD, nullptr};
    }

    curr = advance(curr, o);
  }

  assert(false);
  return {-1, GOLD, nullptr};
}

void Grush::update_vision() {
  for (int p = 0; p < players.size(); p++) {
    for (auto &agent : players[p].agents) {
      agent.vision = calculate_vision(p, agent.position, agent.orientation);
    }
  }
}


std::string Grush::to_string() const {
  const static std::string colors[] = {
      "\033[0;41m", // red
      "\033[0;42m", // green
      "\033[0;44m", // blue
      "\033[0;45m", // magenta

  };

  const static std::string reset = "\033[0m";
  const static std::string gold_color = "\033[0;43m"; // yellow
  const static std::string wall_color = "\033[0;47m"; // white

  std::ostringstream ss;
  for (int row = 0; row < N; row++) {
    for (int col = 0; col < N; col++) {
      if (square[row][col] == BLOCK)
        ss << wall_color;
      else if (agents_ptr[row][col] != nullptr)
        ss << colors[agents_ptr[row][col]->owner];
      else if (gold[row][col] > 0)
        ss << gold_color;

      ss << "  " << reset;
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

  Grush grush = Grush::empty();

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

  bool aaa[N][N];
  for (int row = 1; row < N - 1; row++) {
    for (int col = 1; col < N - 1; col++) {
      aaa[row][col] = false;
    }
  }

  for (int p = 0; p < num_players; p++) {
    std::vector<Agent> agents;

    for (int i = 0; i < num_agents; i++) {
      Agent a;
      a.owner = p;
      a.position = {0, 0};
      while (aaa[a.position.row][a.position.col] || grush.square[a.position.row][a.position.col] == BLOCK ||
             manhattan_distance(bases[p], a.position) > 10) {
        a.position = {rand() % N, rand() % N};
      }
      aaa[a.position.row][a.position.col] = true;
      agents.push_back(a);
    }
    grush.players.emplace_back(bases[p], agents);
  }

  for (auto &player : grush.players) {
    for (auto &agent : player.agents) {
      auto [r, c] = agent.position;
      grush.agents_ptr[r][c] = &agent;
    }
  }

  return grush;
}
