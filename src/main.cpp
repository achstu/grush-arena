#include "map.hpp"
#include "bot.hpp"
#include "stats.hpp"

#include <chrono>
#include <format>
#include <iostream>
#include <string>
#include <vector>


int main(int argc, char *argv[]) {

  int players = argc - 1;
  assert(1 <= players && players <= 4);
  std::vector<Bot> bots;
  Grush grush = Grush::semi_random(players, 10);

  std::cout << grush.to_string() << std::endl;

  for (int p = 0; p < players; p++) {
    std::string command = argv[p + 1];
    if (command.ends_with(".py")) {
      command = "python3 " + command;
    }
    Bot bot(command);
    bots.push_back(std::move(bot));
    // std::cout << p << ": " << command << "\n";
  }

  // std::cout << "players added\n";

  for (int p = 0; p < players; p++) {
    Bot &bot = bots[p];

    // std::cout << std::format("SENDING:\n{} {}\n{}\n", N, ITER, players);
    // std::cout.flush();

    bot.in << std::format("{} {}\n{}\n", N, ITER, players);
    bot.in << grush.players[p].base.to_string() + "\n";

    for (int i = 0; i < players; i++) {
      if (i == p)
        continue;
      bot.in << grush.players[i].base.to_string() + "\n";
    }
  }


  // print initial position
  std::cout << "INITIAL POSITION" << std::endl;
  std::cout << grush.to_string() << std::endl;
  system("sleep 0.1");
  system("clear");

  for (int it = 0; it < ITER; it++) {

    grush.update_vision();

    GameStats stats;

    for (int p = 0; p < players; p++) {

      // measure time
      auto start = std::chrono::high_resolution_clock::now();

      Bot &bot = bots[p];
      Player &player = grush.players[p];

      bot.in << player.agents.size() << "\n";
      for (auto &agent : player.agents) {
        // std::cerr << "ARENA: to agent " << p << " => "<< agent.describe() <<
        // "\n";
        bot.in << agent.describe() << "\n";
      }
      bot.in.flush();

      for (auto &agent : player.agents) {
        std::string line;
        bot.out >> line;
        // std::getline(bot.out, line);
        agent.action = parse(line);
        // std::cout << "ARENA: agent action" << agent.action << "\n";
      }

      auto end = std::chrono::high_resolution_clock::now();
      auto duration =
          std::chrono::duration_cast<std::chrono::microseconds>(end - start);

      {
        auto [r, c] = player.base;
        size_t time = duration.count();
        stats.push_back(
            PlayerStats(time, grush.gold[r][c], (int)player.agents.size(), bot.command));
      }
    }

    grush.update();

    // print game state
    std::cout << it << " ITER" << std::endl;
    std::cout << grush.to_string(stats) << std::endl;
    system("sleep 0.01");
    if (it < ITER - 1) system("clear");
  }
}
