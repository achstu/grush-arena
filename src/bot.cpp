#include "bot.hpp"

#include <boost/process.hpp>
#include <string>

namespace bp = boost::process;

Bot::Bot(const std::string &command)
    : c(command, bp::std_in<in, bp::std_out> out), command(command) {
  // c = bp::child(command, bp::std_in < in, bp::std_out > out);
}

Bot::~Bot() {
  in.close();
  // c.wait();
  c.terminate();
}
