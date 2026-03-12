# Grush Arena

[![C++20](https://img.shields.io/badge/C++-20-blue.svg)](https://isocpp.org/std/the-standard)

A tournament arena for bot programming competitions based on the custom game **Grush** (Gold Rush).

## Overview
Grush is a turn-based strategy game where AI-controlled agents compete to collect gold and eliminate opponents. This project provides an arena program that simulates games between competing bots.

Bots read game state from standard input and output their actions, allowing implementation in any programming language. The arena handles game logic, enforces rules, and provides visual feedback through a simple text-based output.

## Creating Your Own Bot
- Read the complete Game Rules to understand the protocol
- Implement your strategy in any language that can read from stdin and write to stdout
- Test against the example bots

## Building

+ Prerequisites
- C++20 compatible compiler
- Boost C++ Library
- Make build system

+ Clone the repository
```bash
git clone https://github.com/achstu/grush-arena.git
cd grush
```

+ Compile arena program and example bot
```bash
make -C src/
make -C example/
```

+ Run with
```bash
./src/arena ./example/bot ./example/bot.py
```
