#include <iostream>
#include <string>
#include <unistd.h>
using namespace std;

template <typename T, typename = T::value_type>
static ostream &operator<<(ostream &os, const T &x)
  requires(!same_as<T, string>)
{
  os << '{';
  string sep;
  for (const auto &v : x) {
    os << sep << v;
    sep = ", ";
  }
  return os << '}';
}

template <typename T, typename U>
static ostream &operator<<(ostream &os, const pair<T, U> &x) {
  return os << format("({},{})", x.first, x.second);
}

template <typename... Args> static void __print(const Args &...x) {
  ((cerr << ' ' << x), ...);
  cerr << endl;
}

#define debug(...)                                                             \
  do {                                                                         \
    pid_t pid = getpid();                                                      \
                                                                               \
    cerr << "BOT[" << pid << "]: ";                                            \
    cerr << '[' << #__VA_ARGS__ << "]:";                                       \
    __print(__VA_ARGS__);                                                      \
  } while (0)
