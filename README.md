# ChessEngineVS

Moved from ChessEngine repo which was compiled using gcc for linux, to new repo based in MS Visual Studio.

## Features
* Mailbox (single 64 square array) board representation.
* Alpha-beta-search with quiescence search 
* Zobrist hashing for repetition detection 
* Zobrist transposition table
* Piece lists
* Pesto's evaluation function
* Enemy King tropism 
* Null-Move-Pruning
* Killer Move Ordering
* PV Move Ordering
* Late-Move-Reductions
* Quiescence Search
* Iterative Deepening with PV reuse

## Roadmap
* uci console (already partially implemented) -- supports ```go depth``` and ```go movetime```
* Zobrist hashing speedup in make move

## usage

### Binaries

Pre-compiled binaries for Windows Systems only are available in the releases section. Other systems are not officially supported, however, you can try to compile for them yourself.

### entering a position 

The engine understands the following commands:
```position startpos``` -- sets the default starting position
```position fen <fen string here>``` -- sets the position according to an fen string

### Calculating

```go movetime <number of milliseconds>``` -- search n milliseconds

```go depth <depth>``` -- search to depth n

```gp wtime <ms> btime <ms> winc <ms> binc <ms>``` syntax for standard time controls with increment

```go infinite``` search until being told to stop

```stop``` -- stop calculating
```quit``` -- exit program
