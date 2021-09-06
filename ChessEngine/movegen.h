#ifndef Movegen_H
#define Movegen_H

#include "board.h"
#include "piece.h"

class Movegen
{
private:
    Board* starting_pos;
public:
    Movegen(Board* board);
    int generate_moves(int depth);
    ~Movegen();
};

#endif