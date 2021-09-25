/*
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif
*/
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