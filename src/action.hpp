#pragma once

#include <stdexcept>
#include <string>
#include <vector>
#include <cassert>

enum Action {
  LEFT, RIGHT, BACK, GO, FIRE, MINE
};

#include <iostream>

static inline Action parse(const std::string& s) {
  // std::cerr << "CERR " << s << "|" << s.length() << "\n";

  if (s == "LEFT") return LEFT;
  else if (s == "RIGHT") return RIGHT;
  else if (s == "BACK") return BACK;
  else if (s == "GO") return GO;
  else if (s == "FIRE") return FIRE;
  else if (s == "MINE") return MINE;

  // throw std::runtime_error("parse error");
  assert(false);
}

static inline int rotation(Action action) {
  switch (action) {
    case LEFT: return +3;
    case RIGHT: return +1;
    case BACK: return +2;
    default: return 0;
  }
}

using Turn = std::vector<Action>;

