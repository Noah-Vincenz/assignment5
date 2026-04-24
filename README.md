# Sudoku Solver

A C++ Sudoku solver using constraint propagation and heuristic search. Solves 9x9 (and NxN) puzzles efficiently by combining arc consistency, naked pairs elimination, and best-first search with MRV.

## Features

- **Constraint propagation**: Arc consistency eliminates impossible values after each assignment
- **Naked pairs**: Detects identical pairs in rows, columns, and boxes to prune further
- **MRV heuristic**: Minimum Remaining Values — always branches on the most constrained cell
- **Single-successor recursion**: Skips trivial branches where only one option is valid
- **Best-first search**: Priority queue ordered by heuristic value (fewest undetermined cells)
- **Breadth-first search**: Baseline FIFO search for comparison

## Build

```bash
cmake -B build
cmake --build build
```

## Run

```bash
# easy puzzle (solved by propagation alone)
./build/BasicSudoku

# hard puzzle via breadth-first search
./build/BreadthFSSudoku

# hard puzzle via best-first search
./build/BestFSSudoku
```

## Test

```bash
cd build && ctest --output-on-failure
```

Runs 21 tests covering constructor behavior, constraint propagation, conflict detection, search correctness, and performance comparison.

## Algorithm Overview

### Constraint Propagation (`setSquare`)

When a cell is assigned a value:
1. Remove that value from all peers (same row, column, box)
2. If any peer is reduced to one candidate, repeat from step 1
3. Detect naked pairs (two cells with identical 2-element candidate sets) and eliminate those values from other cells in the same unit

This loop continues until no more eliminations occur. Easy puzzles are fully solved by propagation alone.

### Search (`successors`)

When propagation alone can't solve the puzzle:
1. **MRV**: Select the undetermined cell with fewest candidates
2. For each candidate, create a copy of the board and try assigning it
3. Keep only copies where `setSquare` succeeds (no contradictions)
4. If only one successor survives and isn't a solution, recurse into it

### Performance

On the "world's hardest Sudoku" (Arto Inkala):

| Strategy | Nodes Expanded |
|---|---|
| Breadth-first search | 131 |
| Best-first search | 71 |

## Architecture

```
Searchable.h          — abstract interface (provided, not modified)
├── Sudoku.h          — puzzle state + constraint propagation + search
├── BreadthFirstSearch.h  — FIFO queue search
├── BestFirstSearch.h     — priority queue search (min heuristic)
├── SudokuTestHelper.h    — shared test utilities
├── BasicSudoku.cpp       — easy puzzle driver
├── BreadthFSSudoku.cpp   — hard puzzle BFS driver
├── BestFSSudoku.cpp      — hard puzzle best-first driver
└── tests/SudokuTests.cpp — Google Test suite
```

## License

MIT
