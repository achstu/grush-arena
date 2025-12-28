#pragma once

#include <boost/process.hpp>
#include <string>

// namespace bp = boost::process;

struct Bot {
  boost::process::opstream in;
  boost::process::ipstream out;
  boost::process::child c;

  Bot(const std::string &);
  ~Bot();

  Bot(const Bot &) = delete;
  Bot &operator=(const Bot &) = delete;

  Bot(Bot &&) = default;
  Bot &operator=(Bot &&) = default;
};
