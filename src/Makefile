# remove asserts in release builds
# CXXFLAGS := -DNDEBUG
CXXFLAGS = $(shell cat compile_flags.txt)
BOOST = $(shell nix-build --no-out-link '<nixpkgs>' -A boost.dev)
BOOST_INCLUDE = -I$(BOOST)/include

CXX := g++

default: arena

arena: main.o bot.o map.o
	$(CXX) $^ -o $@

# bot.o: bot.hpp bot.cpp
# $(CXX) $(CXXFLAGS) -I$(BOOSTLIB)/include -c bot.cpp -o bot.o

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(BOOST)/include -c $< -o $@

clear:
	rm *.o
