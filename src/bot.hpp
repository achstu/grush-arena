#pragma once

#include <boost/process.hpp>
#include <string>

namespace bp = boost::process;

struct Bot {
  bp::opstream in;
  bp::ipstream out;
  bp::child c;
  std::string command;

  Bot(const std::string &);
  ~Bot();

  Bot(const Bot &) = delete;
  Bot &operator=(const Bot &) = delete;

  Bot(Bot &&) = default;
  Bot &operator=(Bot &&) = default;
};
