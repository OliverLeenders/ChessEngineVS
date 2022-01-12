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
