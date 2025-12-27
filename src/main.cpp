#include "map.hpp"
#include "process.hpp"
#include <chrono>
#include <format>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {

  int players = argc - 1;
  assert(1 <= players && players <= 4);
  std::vector<ChildProcess> childs;
  Grush grush = Grush::semi_random(players, 10);

  // std::cout << grush.to_string() << std::endl;

  for (int p = 0; p < players; p++) {
    std::string command = argv[p + 1];
    if (command.ends_with(".py")) {
      command = "python3 " + command;
    }
    std::cout << p << ": " << command << "\n";
    childs.emplace_back(command);
  }

  std::cout << "players added\n";

  for (int p = 0; p < players; p++) {
    ChildProcess &bot = childs[p];

    std::cout << std::format("SENDING:\n{} {}\n{}\n", N, ITER, players);
    std::cout.flush();

    bot.write(std::format("{} {}\n{}\n", N, ITER, players));
    bot.write(grush.players[p].base.to_string() + "\n");

    for (int i = 0; i < players; i++) {
      if (i == p)
        continue;
      bot.write(grush.players[i].base.to_string() + "\n");
    }
  }

  for (int it = 0; it < ITER; it++) {
    system("sleep 0.01");
    std::cout << it << " ITER" << std::endl;
    system("clear");
    std::cout << grush.to_string() << std::endl;
    grush.update_vision();

    for (int p = 0; p < players; p++) {

      // measure time
      auto start = std::chrono::high_resolution_clock::now();

      ChildProcess &bot = childs[p];

      bot.write(std::format("{}\n", grush.players[p].agents.size()));
      for (auto &agent : grush.players[p].agents) {
        std::cerr << "ARENA: to agent " << agent.describe() << "\n";
        bot.write(agent.describe() + "\n");
      }

      for (auto &agent : grush.players[p].agents) {
        agent.action = parse(bot.readline());
        // std::cout << agent.action << "\n";
      }

      auto end = std::chrono::high_resolution_clock::now();
      auto duration =
          std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

      std::cout << std::format("player {} on iter {}: {}ms\n", p, it,
                               duration.count());
    }

    grush.update({});
    // std::cout << grush.to_string() << std::endl;
  }
}
