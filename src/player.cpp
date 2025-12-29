#include "player.hpp"

#include <format>
#include <string>

bool Position::operator==(const Position &other) const {
  return (row == other.row) && (col == other.col);
}

std::string Position::to_string() const {
  return std::format("{} {}", row, col);
}

Position Position::advance(Orientation o) const {
  switch (o) {
  case U: return {row - 1, col};
  case R: return {row, col + 1};
  case D: return {row + 1, col};
  case L: return {row, col - 1};
  }
  assert(false);
  return {row, col};
}

Vision::Vision(int d, VisualObject o, Agent *ptr)
    : distance(d), object(o), agent_ptr(ptr) {}

Agent::Agent(Position p, Orientation o, int owning_player)
    : vision(0, WALL, nullptr), position(p), orientation(o), has_gold(false),
      is_zombie(false), is_alive(true), action(MINE), owner(owning_player) {}

std::string Agent::describe() const {
  auto vision_to_string = [](VisualObject vo) {
    switch (vo) {
    case WALL: return "WALL";
    case GOLD: return "GOLD";
    case ALLY: return "ALLY";
    case ENEMY: return "ENEMY";
    }
    assert(false);
  };
  auto orientation_to_string = [](Orientation o) {
    switch (o) {
    case U: return "U";
    case R: return "R";
    case D: return "D";
    case L: return "L";
    }
    assert(false);
  };

  return std::format("{} {} {} {} {}", position.to_string(),
                     vision_to_string(vision.object), vision.distance,
                     orientation_to_string(orientation), int(has_gold));
}

Player::Player(std::vector<Agent> &&a, Position b)
    : agents(std::move(a)), base(b) {}

// static inline bool is_alive(const Agent &a) { return a.is_alive; }

// auto Player::alive_agents() -> decltype(auto) {
  // return std::ranges::filter_view(agents, is_alive);
// }

// auto Player::alive_agents() const -> decltype(auto) {
  // return std::ranges::filter_view(agents, is_alive);
// }
