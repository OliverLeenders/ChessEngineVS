#include "movegen.h"

/**
 * \brief Default constructor. Sets the starting position.
 * 
 * \param board starting position
 */
Movegen::Movegen(Board* board)
{
    this->starting_pos = board;
}

/**
 * \brief Default destructor.
 * 
 */
Movegen::~Movegen()
{
}

/**
 * \brief Count number of possible positions up to given depth.
 * 
 * \param depth depth to search
 * \return number of possible positions
 */
int Movegen::generate_moves(int depth)
{
    if (depth == 0)
    {
        return 1;
    }
    else
    {
        int move_count = 0;
        std::list<Move*>* possible_moves = this->starting_pos->possible_moves();
        for (Move* const& b : *possible_moves)
        {
            this->starting_pos->make_move(b);
            Movegen* m = new Movegen(this->starting_pos);
            move_count += m->generate_moves(depth - 1);
            delete m;
            this->starting_pos->unmake_move();
        }
        for (Move * const& b : *possible_moves) {
            delete b;
        }
        delete possible_moves;
        return move_count;
    }
}