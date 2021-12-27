# ChessEngineVS

Moved from ChessEngine repo which was compiled using gcc for linux, to new repo based in MS Visual Studio.

## Features
* Mailbox (single 64 square array) board representation. 
* Alpha-beta-search with quiescence search using iterative deepening. 
* Zobrist hashing for repetition detection. 
* Piece lists. 
* Pesto's evaluation function. 

## Roadmap
* uci console (already partially implemented)
* Zobrist hashing for transposition table
* Zobrist hashing speedup in make move
