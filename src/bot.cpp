#include "bot.hpp"

Bot::Bot(const std::string &command)
    : c(command, boost::process::std_in<in, boost::process::std_out> out) {
  // c = bp::child(command, bp::std_in < in, bp::std_out > out);
  // c = bp::child(command, bp::std_in < in, bp::std_out > out);
}

Bot::~Bot() {
  in.close();
  // c.wait();
  c.terminate();
}
