## Game Overview

Grush is a turn-based strategy game where players control multiple agents competing to collect gold and eliminate opponents. Each player starts with a base and a set of agents, with the goal of accumulating the most gold.

## Game Components

### Board
- Grid-based playing field of size N x N (specified at game start)
- Zero-indexed coordinates (row, column)
- Contains walls that block movement and line of sight/fire

### Agents
Each agent has:
- **Position**: Current location on the board
- **Rotation**: Facing direction (U=Up, R=Right, D=Down, L=Left)
- **Gold Carrying**: Whether agent currently holds gold
- **Type**: Can detect nearby objects (WALL, GOLD, ALLY, ENEMY)

### Objects
- **Gold**: Collectible resources that spawn on the board
- **Walls**: Impassable obstacles that block movement and line of sight/fire
- **Bases**: Starting positions for each players used for gold accumulation

## Turn Structure

1. **Sensory Phase**: Each agent receives information about the nearest object in its facing direction:
   - Distance to object
   - Type of object

2. **Action Phase**: Each agent simultaneously chooses one action

## Agent Actions

| Action | Effect |
|--------|--------|
| **GO** | Move forward one cell in current facing direction |
| **FIRE** | Shoot in facing direction, eliminating first agent in line of sight |
| **MINE** | Collect gold from current cell (if present) |
| **LEFT** | Rotate 90 degrees counter-clockwise |
| **RIGHT** | Rotate 90 degrees clockwise |
| **BACK** | Rotate 180 degrees |

Victory is determined by most gold collected.

## Communication Protocol

### Initialization

When your bot starts, it receives:
```text
<board_size> <max_turns>
<player_count>
<your_base_row> <your_base_col>
<enemy1_base_row> <enemy1_base_col>
...
```
The line with `<player_count>` contains number of players in the game (including you). The following `<player_count>` lines contains positions of players bases, starting from yours.
Note: row col are zero indexed.

### Game Loop

Each turn follows this pattern:

+ **Read turn information**

```text
<alive_agents_count>
<row> <col> <TYPE> <dist> <ROTATION> <has_gold>
<row> <col> <TYPE> <dist> <ROTATION> <has_gold>
...
```
Where:
- `row`, `col`: Agent position (zero-indexed)
- `TYPE`: One of: `WALL`, `GOLD`, `ALLY`, `ENEMY`
- `dist`: Distance to detected object (integer)
- `ROTATION`: Agent facing: `U` (Up), `R` (Right), `D` (Down), `L` (Left)
- `has_gold`: Boolean flag: `0` (no gold) or `1` (carrying gold)

+ **Send actions** (one line per agent, in the same order as given by arena at the beggining of time step)


**Note:** Always flush output after writing actions.
