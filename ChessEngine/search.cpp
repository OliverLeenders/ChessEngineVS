#include "search.h"
#include <iostream>
/**
 * @brief Construct a new Search:: Search object
 *
 */
Search::Search()
{
}
/**
 * @brief Destroy the Search:: Search object
 *
 */
Search::~Search()
{
}

/**
 * @brief performs an alpha-beta pruning minimax search according to the negamax framework
 * and gathers a principal variation
 *
 * @param pos position to search from
 * @param alpha alpha value for AB pruning
 * @param beta beta value for AB pruning
 * @param depth_left depth left to search
 * @param PV principal variation
 * @param e evaluator object
 * @param left_most whether the current position is left most in the tree (set to true if starting search)
 * @return double evaluation
 */
double Search::alpha_beta(Board* pos, double alpha, double beta, unsigned int depth_left, std::list<Board*>* PV, Evaluator* e, bool left_most)
{
    if (depth_left == 0)
    {
        return this->quiescence(pos, alpha, beta, e);
    }
    else
    {
        // initializing PV line
        // getting legal moves
        uint64_t pos_hash = e->zobrist_hash(pos);
        bool pos_is_transposition = e->contains_z_hash(pos_hash);
        std::list<Board*>* moves = pos->get_legal_moves();
        moves->sort(e->compare);
        if (moves->size() == 0 && pos->num_checks > 0)
        {
            delete moves;
            e->add_hash(pos_hash, DBL_MAX);
            return DBL_MAX;
        }
        else if (moves->size() == 0)
        {
            delete moves;
            e->add_hash(pos_hash, 0.00);
            return 0.00;
        }
        std::list<Board*>* line = new std::list<Board*>;

        int i = 0;
        for (Board* const& move : *moves)
        {
            double score;
            uint64_t move_hash = e->zobrist_hash(move);
            bool is_transposition = e->contains_z_hash(move_hash);

            if (i == 0 && left_most)
            {
                if (is_transposition)
                {
                    // std::cout << "here" << std::endl;
                    score = e->zobrist_hashmap->find(move_hash)->second;
                }
                else
                {
                    score = -this->alpha_beta(move, -beta, -alpha, depth_left - 1, line, e, true);
                }
                for (Board* const& pv_move : *PV)
                {
                    delete pv_move;
                }
                PV->clear();

                for (Board* const& line_move : *line)
                {
                    PV->push_back(line_move->clone());
                }
                PV->push_front(move->clone());
            }
            else
            {
                if (e->contains_z_hash(e->zobrist_hash(move)))
                {
                    score = e->zobrist_hashmap->find(move_hash)->second;
                }
                else
                {
                    score = -this->alpha_beta(move, -beta, -alpha, depth_left - 1, line, e, false);
                }
                if (score >= beta)
                {
                    for (Board* const& imove : *moves)
                    {
                        delete imove;
                    }
                    for (Board* const& line_move : *line)
                    {
                        delete line_move;
                    }
                    delete line;
                    delete moves;
                    if (!pos_is_transposition) {
                        e->add_hash(pos_hash, beta);
                    }
                    return beta;
                    //  fail hard beta-cutoff
                }
                else if (score > alpha)
                {
                    for (Board* const& pv_move : *PV)
                    {
                        delete pv_move;
                    }

                    PV->clear();
                    for (Board* const& line_move : *line)
                    {
                        PV->push_back(line_move->clone());
                    }
                    PV->push_front(move->clone());
                    //std::cout << b->get_last_move() << std::endl;
                    alpha = score; // alpha acts like max in MiniMax
                }
            }
            i++;
        }
        for (Board* const& b : *moves)
        {
            delete b;
        }
        for (Board* const& b : *line)
        {
            delete b;
        }
        delete line;
        delete moves;

        if (!pos_is_transposition) {
            e->add_hash(pos_hash, alpha);
        }
    }
    return alpha;
}

double Search::alpha_beta_prev_PV(Board* pos, double alpha, double beta, unsigned int depth_left, std::list<Board*>* PV, Evaluator* e, bool left_most, std::list<Board*>* prev_pv)
{
    if (prev_pv->size() == 0)
    {
        return this->alpha_beta(pos, alpha, beta, depth_left, PV, e, left_most);
    }
    if (depth_left == 0)
    {
        return this->quiescence(pos, alpha, beta, e);
    }
    else
    {
        std::list<Board*>* line = new std::list<Board*>;
        std::list<Board*>* moves = pos->get_legal_moves();
        moves->sort(e->compare);
        uint64_t pos_hash = e->zobrist_hash(pos);
        bool pos_is_transposition = e->contains_z_hash(pos_hash);
        if (moves->size() == 0 && pos->num_checks > 0)
        {
            delete moves;
            delete line;
            if (!pos_is_transposition) {
                e->add_hash(pos_hash, DBL_MAX);
            }
            return DBL_MAX;
        }
        else if (moves->size() == 0)
        {
            delete moves;
            delete line;
            if (!pos_is_transposition) {
                e->add_hash(pos_hash, 0.00);
            }
            return 0.00;
        }

        Board* pv_pos = prev_pv->front();
        prev_pv->pop_front();

        moves->push_front(pv_pos);
        int i = 0;
        for (Board* const& move : *moves)
        {
            double score;
            uint64_t move_hash = e->zobrist_hash(move);

            if (i == 0 && left_most)
            {
                if (e->contains_z_hash(move_hash))
                {
                    score = e->zobrist_hashmap->find(move_hash)->second;
                }
                else
                {
                    score = -this->alpha_beta_prev_PV(move, -beta, -alpha, depth_left - 1, line, e, true, prev_pv);
                }
                for (Board* const& pv_move : *PV)
                {
                    delete pv_move;
                }
                PV->clear();
                for (Board* const& line_move : *line)
                {
                    PV->push_back(line_move->clone());
                }
                PV->push_front(move->clone());
            }
            else if (!move->equals(pv_pos))
            {
                if (e->contains_z_hash(move_hash))
                {
                    score = e->zobrist_hashmap->find(move_hash)->second;
                    // std::cout << "here" << std::endl;
                }
                else
                {
                    score = -this->alpha_beta(move, -beta, -alpha, depth_left - 1, line, e, false);
                }
                if (score >= beta)
                {
                    for (Board* const& move : *moves)
                    {
                        delete move;
                    }
                    for (Board* const& line_move : *line)
                    {
                        delete line_move;
                    }
                    delete line;
                    delete moves;
                    if (!pos_is_transposition) {
                        e->add_hash(pos_hash, beta);
                    }

                    return beta;
                    //  fail hard beta-cutoff
                }
                else if (score > alpha)
                {
                    for (Board* const& pv_move : *PV)
                    {
                        delete pv_move;
                    }

                    PV->clear();
                    for (Board* const& line_move : *line)
                    {
                        PV->push_back(line_move->clone());
                    }
                    PV->push_front(move->clone());
                    //std::cout << b->get_last_move() << std::endl;
                    alpha = score; // alpha acts like max in MiniMax
                }
            }
            i++;
        }
        for (Board* const& b : *moves)
        {
            delete b;
        }
        for (Board* const& b : *line)
        {
            delete b;
        }
        delete line;
        delete moves;

        if (!pos_is_transposition) {
            e->add_hash(pos_hash, alpha);
        }
}

    return alpha;
}

/**
 * @brief runs an evalutation with a certain depth
 *
 * @param pos position to search from
 * @param depth depth
 * @return double evaluation
 */
double Search::evaluate(Board* pos, unsigned int depth)
{
    std::list<Board*>* PV = new std::list<Board*>;
    Evaluator* e = new Evaluator();
    double res = this->alpha_beta(pos, -DBL_MAX, DBL_MAX, depth, PV, e, true);
    std::cout << PV->size() << std::endl;
    for (Board* const& i : *PV)
    {
        std::cout << i->get_last_move() + " ";
        delete i;
    }
    std::cout << std::endl;
    delete PV;
    delete e;
    return res;
}

double Search::evaluate_iterative_deepening(Board* pos, unsigned int depth)
{
    std::list<Board*>* prev_PV = new std::list<Board*>;
    double res = 0.00;
    Evaluator* e = new Evaluator();
    std::string best_move = "";

    for (unsigned int i = 1; i <= depth; i++)
    {

        std::list<Board*>* PV = new std::list<Board*>;
        res = this->alpha_beta_prev_PV(pos, -DBL_MAX, DBL_MAX, i, PV, e, true, prev_PV);
        std::cout << "info score " << std::fixed << (int) (res * 100 + 0.5);
        std::cout << " pv ";
        for (Board* const& i : *PV)
        {
            std::cout << " " << i->get_last_move() << " ";
        }
        std::cout << std::endl;
        best_move = PV->front()->get_last_move();
        for (Board* const& i : *prev_PV)
        {
            delete i;
        }
        prev_PV->clear();
        delete prev_PV;
        prev_PV = PV;
        e->zobrist_hashmap->clear();
    }
    std::cout << "bestmove " << best_move << std::endl << std::endl;
    delete e;
    for (Board* const& i : *prev_PV)
    {
        delete i;
    }
    delete prev_PV;
    return res;
}
/**
 * @brief performs a quiescence search such that only "quiet" positions are evaluated
 *
 * @param pos starting position
 * @param alpha alpha value for AB pruning
 * @param beta beta value for AB pruning
 * @param e evaluator object
 * @return double evaluation
 */
double Search::quiescence(Board* pos, double alpha, double beta, Evaluator* e)
{
    double stand_pat = e->evaluate(pos);
    uint64_t pos_hash = e->zobrist_hash(pos);
    bool pos_is_transposition = e->contains_z_hash(pos_hash);

    if (stand_pat >= beta)
    {
        if (!pos_is_transposition) {
            e->add_hash(pos_hash, beta);
        }
        return beta;
    }
    else if (stand_pat > alpha)
    {
        alpha = stand_pat;
    }

    std::list<Board*>* moves = pos->get_legal_captures();
    moves->sort(e->compare);
    //std::cout << moves->size() << std::endl;
    if (moves->size() == 0)
    {
        //std::cout << pos->pos_as_str() << std::endl;
        delete moves;
        
        double score = e->evaluate(pos);
        if (!pos_is_transposition) {
            e->add_hash(pos_hash, score);
        }
        return score;
    }
    else
    {
        for (Board* const& move : *moves)
        {
            double score;
            uint64_t move_hash = e->zobrist_hash(move);
            bool is_transposition = e->contains_z_hash(move_hash);
            if (is_transposition)
            {
                score = e->zobrist_hashmap->find(e->zobrist_hash(move))->second;
            }
            else
            {
                score = -this->quiescence(move, -beta, -alpha, e);
            }
            if (score >= beta)
            {
                for (Board* const& b : *moves)
                {
                    delete b;
                }
                delete moves;
                if (!is_transposition) {
                    e->add_hash(move_hash, beta);
                }

                return beta; // fail hard beta-cutoff
            }
            if (score > alpha)
            {
                alpha = score; // alpha acts like max in MiniMax
            }
        }
        for (Board* const& b : *moves)
        {
            delete b;
        }
        delete moves;

        if (!pos_is_transposition) {
            e->add_hash(pos_hash, alpha);
        }
    }
    return alpha;
}
