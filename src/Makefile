BUILD ?= debug

CXXFLAGS := -Wall -Wextra -Wpedantic -std=c++20

BOOST_PATH := $(shell nix-build --no-out-link '<nixpkgs>' -A boost.dev 2>/dev/null)
ifneq ($(BOOST_PATH),)
    BOOST_INCLUDE := -I$(BOOST_PATH)/include
else
    $(warning Boost not found via nix-build, trying system path)
    BOOST_INCLUDE := -I/usr/include
endif

ifeq ($(BUILD),release)
    CXXFLAGS += -O3 -DNDEBUG -flto
    LDFLAGS := -flto
else
    CXXFLAGS += -O0 -g -DDEBUG
endif

CXX := g++
TARGET := arena
SOURCES := $(wildcard *.cpp)
OBJECTS := $(SOURCES:.cpp=.o)

# .PHONY: all debug release clean

all: $(TARGET)

debug: 
	$(MAKE) BUILD=debug

release:
	$(MAKE) BUILD=release

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(BOOST_INCLUDE) -c $< -o $@

clean:
	rm -f *.o $(TARGET)

