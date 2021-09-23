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