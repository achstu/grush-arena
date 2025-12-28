# grush

### Contestants

- https://github.com/mastach/grush_clash
- https://github.com/opprotossball/GRush-Crusher

### Usage

Compile arena program (requires c++20)
```bash
make -C src/
make -C example/
```

Running arena program
```bash
./src/arena ./example/bot ./example/bot.py
```

### Info

Your program reads input from standard input (normally "the keyboard") and writes output to standard output (normally "the screen"). Your program is started once at the beginning of a game and keeps running until the end of the game.

You should strictly follow the dialogue described in the game rules. You may assume that all input to your program is completely correct.

### Communication

First of input contains: size of board and number of turns (separated by single space)

Next line contain single number `p` - number of players in the game, including you.

Next `p` lines contains positions of players bases in format `row col` (two integers sep by space).
First of those lines is postion of your base.

Note: `row col` are zero indexed.

Then game starts.

On the beggingin of each game turn, on the first line of inputs is single integer `a` - number of alive agents.

The next `a` lines contains info about agnets in following format:

`row col TYPE dist ROTATION has_gold` where
- `row col` pos of agent
- `TYPE` is one of strings: `WALL`, `GOLD`, `ALLY`, `ENEMY`
- `dist` (intteger) is distance to that object (of type `TYPE`)
- `ROTATION` is single character: `U`, `R`, `D` or `L` (rotation of agent)
- `has_hold` is boolean flag (`0` or `1`)


Your task is print `a` lines on stdout conating information about agent actions. For each agent print single word:
- `GO`
- `FIRE`
- `MINE`
- `LEFT`
- `RIGHT`
- `BACK`


Remember to flush the buffer.

### Technical

Note: when you write a move to standard output, make sure to flush the stream, so the referee is not waiting for the move. Below is explained how you can flush in some of the supported programming language.

In C++ use `cout.flush();` to flush the output buffer.

You can use `input()` to read from standard input, and print to write to standard output. After you write a move to standard output, your program should either supply `flush=True` to print, or call `sys.stdout.flush()` to make sure output is written immediately.
