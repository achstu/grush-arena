#include "bot.hpp"
#include "map.hpp"
#include "stats.hpp"

#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#define clear() system("clear")
// #define clear()

int main(int argc, char *argv[]) {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  int players = argc - 1;
  assert(1 <= players && players <= 4);

  std::vector<Bot> bots;
  // Grush grush = Grush::semi_random(players, 15);
  Grush grush = Grush::hand_crafted(players);

  for (int p = 0; p < players; p++) {
    std::string command = argv[p + 1];
    if (command.ends_with(".py")) {
      command = "python3 " + command;
    }
    Bot bot(command);
    bots.push_back(std::move(bot));
  }

  for (int p = 0; p < players; p++) {
    Player &player = grush.players[p];
    Bot &bot = bots[p];

    std::getline(bot.out, bot.name);

    bot.in << N << " " << ITER << "\n";
    bot.in << players << "\n";
    bot.in << player.base.to_string() << "\n";

    for (int i = 0; i < players; i++) {
      if (i == p) {
        continue;
      }
      bot.in << grush.players[i].base.to_string() << "\n";
    }
    bot.in.flush();
  }

  // print initial position
  std::cout << "INITIAL POSITION" << std::endl;
  std::cout << grush.to_string() << std::endl;
  system("sleep 2");
  clear();

  History history;

  for (int it = 0; it < ITER; it++) {

    grush.update_vision();

    GameStats stats;

    for (int p = 0; p < players; p++) {

      // measure time
      auto start = std::chrono::high_resolution_clock::now();

      Bot &bot = bots[p];
      Player &player = grush.players[p];

      bot.in << player.alive_agents_count() << "\n";
      for (const auto &agent : player.alive_agents()) {
        bot.in << agent.describe() << "\n";
      }
      bot.in.flush();

      for (auto &agent : player.alive_agents()) {
        std::string line;
        bot.out >> line;

        // std::cout << "ARENA[0] " << line << std::endl;
        agent.action = parse(line);
      }

      auto end = std::chrono::high_resolution_clock::now();
      auto duration =
          std::chrono::duration_cast<std::chrono::microseconds>(end - start);

      {
        auto [r, c] = player.base;
        size_t time = duration.count();
        stats.emplace_back(time, grush.gold[r][c],
                           (int)player.alive_agents_count(), bot.name);
      }
    }

    grush.update();

    // print game state
    clear();
    std::cout << it << " ITER" << std::endl;
    std::cout << grush.to_string(stats) << std::endl;
    // system("sleep 0.01");

    history.push_back(std::move(stats));
  }

  export_history(history);
}
