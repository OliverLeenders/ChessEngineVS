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
double Search::alpha_beta(Board* pos, double alpha, double beta, unsigned int depth_left, std::list<Move*>* PV, Evaluator* e, bool left_most)
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
		std::list<Move*>* moves = pos->possible_moves();
		//moves->sort(e->compare);
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
		std::list<Move*>* line = new std::list<Move*>;

		int i = 0;
		for (Move* const& move : *moves)
		{
			pos->make_move(move);
			double score;
			uint64_t move_hash = e->zobrist_hash(pos);
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
					score = -this->alpha_beta(pos, -beta, -alpha, depth_left - 1, line, e, true);
				}
				for (Move* const& pv_move : *PV)
				{
					delete pv_move;
				}
				PV->clear();

				for (Move* const& line_move : *line)
				{
					PV->push_back(line_move->clone());
				}
				PV->push_front(move->clone());
			}
			else
			{
				if (is_transposition)
				{
					score = e->zobrist_hashmap->find(move_hash)->second;
				}
				else
				{
					score = -this->alpha_beta(pos, -beta, -alpha, depth_left - 1, line, e, false);
				}
				if (score >= beta)
				{

					pos->unmake_move();
					for (Move* const& imove : *moves)
					{
						delete imove;
					}
					for (Move* const& line_move : *line)
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
					for (Move* const& pv_move : *PV)
					{
						delete pv_move;
					}

					PV->clear();
					for (Move* const& line_move : *line)
					{
						PV->push_back(line_move->clone());
					}
					PV->push_front(move->clone());
					//std::cout << b->get_last_move() << std::endl;
					alpha = score; // alpha acts like max in MiniMax
				}
			}
			i++;
			pos->unmake_move();
		}
		for (Move* const& b : *moves)
		{
			delete b;
		}
		for (Move* const& b : *line)
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
 * Alpha beta search with a given previous PV which is held as the leftmost branch of the tree. Should give a speed boost due to more beta-cutoffs.
 *
 * \param pos position to search
 * \param alpha alpha value
 * \param beta beta value
 * \param depth_left depth left to search
 * \param PV principal variation
 * \param e evalutator object
 * \param left_most boolean indicating whether branch is left most
 * \param prev_pv previous PV
 * \return eval
 */
double Search::alpha_beta_prev_PV(Board* pos, double alpha, double beta, unsigned int depth_left, std::list<Move*>* PV, Evaluator* e, bool left_most, std::list<Move*>* prev_pv)
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
		std::list<Move*>* line = new std::list<Move*>;
		std::list<Move*>* moves = pos->possible_moves();
		//moves->sort(e->compare);
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

		Move* pv_pos = prev_pv->front();
		prev_pv->pop_front();

		moves->push_front(pv_pos);
		int i = 0;
		for (Move* const& move : *moves)
		{
			pos->make_move(move);
			double score;
			uint64_t move_hash = e->zobrist_hash(pos);

			if (i == 0 && left_most)
			{
				if (e->contains_z_hash(move_hash))
				{
					score = e->zobrist_hashmap->find(move_hash)->second;
				}
				else
				{
					score = -this->alpha_beta_prev_PV(pos, -beta, -alpha, depth_left - 1, line, e, true, prev_pv);
				}
				for (Move* const& pv_move : *PV)
				{
					delete pv_move;
				}
				PV->clear();
				for (Move* const& line_move : *line)
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
					score = -this->alpha_beta(pos, -beta, -alpha, depth_left - 1, line, e, false);
				}
				if (score >= beta)
				{

					pos->unmake_move();
					for (Move* const& move : *moves)
					{
						delete move;
					}
					for (Move* const& line_move : *line)
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
					for (Move* const& pv_move : *PV)
					{
						delete pv_move;
					}

					PV->clear();
					for (Move* const& line_move : *line)
					{
						PV->push_back(line_move->clone());
					}
					PV->push_front(move->clone());
					//std::cout << b->get_last_move() << std::endl;
					alpha = score; // alpha acts like max in MiniMax
				}
			}
			i++;
			pos->unmake_move();
		}
		for (Move* const& b : *moves)
		{
			delete b;
		}
		for (Move* const& b : *line)
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
	std::list<Move*>* PV = new std::list<Move*>;
	Evaluator* e = new Evaluator();
	double res = this->alpha_beta(pos, -DBL_MAX, DBL_MAX, depth, PV, e, true);
	std::cout << PV->size() << std::endl;
	for (Move* const& i : *PV)
	{
		std::cout << i->to_string() + " ";
		delete i;
	}
	std::cout << std::endl;
	delete PV;
	delete e;
	return res;
}

double Search::evaluate_iterative_deepening(Board* pos, unsigned int depth)
{
	std::list<Move*>* prev_PV = new std::list<Move*>;
	double res = 0.00;
	Evaluator* e = new Evaluator();
	std::string best_move = "";

	for (unsigned int i = 1; i <= depth; i++)
	{

		std::list<Move*>* PV = new std::list<Move*>;
		res = this->alpha_beta_prev_PV(pos, -DBL_MAX, DBL_MAX, i, PV, e, true, prev_PV);
		std::cout << "info score " << std::fixed << (int)(res * 100 + 0.5);
		std::cout << " pv ";
		for (Move* const& i : *PV)
		{
			std::cout << " " << i->to_string() << " ";
		}
		std::cout << "nodes " << e->zobrist_hashmap->size();
		std::cout << std::endl;
		best_move = PV->front()->to_string();
		for (Move* const& i : *prev_PV)
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
	for (Move* const& i : *prev_PV)
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

	std::list<Move*>* moves = pos->get_legal_captures();
	//moves->sort(e->compare);
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
		for (Move* const& move : *moves)
		{
			pos->make_move(move);
			double score;
			uint64_t move_hash = e->zobrist_hash(pos);
			bool is_transposition = e->contains_z_hash(move_hash);
			if (is_transposition)
			{
				score = e->zobrist_hashmap->find(move_hash)->second;
			}
			else
			{
				score = -this->quiescence(pos, -beta, -alpha, e);
			}
			if (score >= beta)
			{
				pos->unmake_move();
				for (Move* const& b : *moves)
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
			pos->unmake_move();
		}
		for (Move* const& b : *moves)
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
