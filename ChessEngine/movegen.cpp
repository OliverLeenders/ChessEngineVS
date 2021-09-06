#include "movegen.h"

Movegen::Movegen(Board* board)
{
    this->starting_pos = board;
}

Movegen::~Movegen()
{
}

int Movegen::generate_moves(int depth)
{
    if (depth == 0)
    {
        return 1;
    }
    else
    {
        int move_count = 0;
        std::list<Board*>* possible_moves = this->starting_pos->get_legal_moves();
        for (Board* const& b : *possible_moves)
        {
            Movegen* m = new Movegen(b);
            move_count += m->generate_moves(depth - 1);
            delete m;
        }
        for (Board* const& b : *possible_moves) {
            delete b;
        }
        delete possible_moves;
        return move_count;
    }
}