#include "search.h"
#include <iostream>
/**
 * @brief Construct a new Search:: Search object
 *
 */
 /**
  * @brief Destroy the Search:: Search object
  *
  */
Search::~Search()
{
}

Search::Search()
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
double Search::evaluate(Board* pos, unsigned int depth)
{
	std::list<Move*>* PV = new std::list<Move*>;
	double res = this->alpha_beta(pos, -DBL_MAX, DBL_MAX, depth, PV, true);
	std::cout << PV->size() << std::endl;
	for (Move* const& i : *PV)
	{
		std::cout << i->to_string() + " ";
		delete i;
	}
	std::cout << std::endl;
	delete PV;
	return res;
}

double Search::alpha_beta(Board* pos, double alpha, double beta, unsigned int depth_left, std::list<Move*>* PV, bool left_most)
{
	if (depth_left == 0)
	{
		return this->quiescence(pos, alpha, beta);
	}
	else
	{
		// initializing PV line
		// getting legal moves
		uint64_t pos_hash = pos->hash(pos);
		bool pos_is_transposition = pos->transposition_table->contains_hash(pos_hash);
		std::vector<Move*>* moves = pos->possible_moves();
		std::sort(moves->begin(), moves->end(), [pos](Move* m_1, Move* m_2) -> bool {return Evaluator::compare(pos, m_1, m_2); });
		if (moves->size() == 0 && pos->num_checks > 0)
		{
			delete moves;
			pos->transposition_table->add(pos_hash, hash_entry(DBL_MAX, depth_left + 1));
			return DBL_MAX;
		}
		else if (moves->size() == 0)
		{
			delete moves;
			pos->transposition_table->add(pos_hash, hash_entry(0.00, depth_left + 1));
			return 0.00;
		}
		std::list<Move*>* line = new std::list<Move*>;

		int i = 0;
		for (Move* const& move : *moves)
		{
			pos->make_move(move);
			double score;
			uint64_t move_hash = pos->hash(pos);
			bool is_transposition = pos->transposition_table->contains_hash(move_hash);

			if (i == 0 && left_most)
			{
				if (is_transposition)
				{
					// std::cout << "here" << std::endl;
					score = pos->transposition_table->map->find(move_hash)->second.score;
				}
				else
				{
					score = -this->alpha_beta(pos, -beta, -alpha, depth_left - 1, line, true);
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
					score = pos->transposition_table->map->find(move_hash)->second.score;
				}
				else
				{
					score = -this->alpha_beta(pos, -beta, -alpha, depth_left - 1, line, false);
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
						pos->transposition_table->add(pos_hash, hash_entry(score, depth_left));
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
					if (!pos_is_transposition) {
						pos->transposition_table->add(pos_hash, hash_entry(score, depth_left));
					}
					alpha = score; // alpha acts like max in MiniMax
				}
			}
			i++;
			pos->unmake_move();
			for (Move* const& b : *line)
			{
				delete b;
			}
			line->clear();
		}
		for (Move* const& b : *moves)
		{
			delete b;
		}
		delete moves;
		delete line;


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
double Search::alpha_beta_prev_PV(Board* pos, double alpha, double beta, unsigned int depth_left, std::list<Move*>* PV, bool left_most, std::list<Move*>* prev_pv)
{
	if (prev_pv->size() == 0)
	{
		return this->alpha_beta(pos, alpha, beta, depth_left, PV, left_most);
	}
	if (depth_left == 0)
	{
		return this->quiescence(pos, alpha, beta);
	}
	else
	{
		std::list<Move*>* line = new std::list<Move*>;
		std::vector<Move*>* moves = pos->possible_moves();
		uint64_t pos_hash = pos->hash(pos);
		bool pos_is_transposition = pos->transposition_table->contains_hash(pos_hash);
		if (moves->size() == 0 && pos->num_checks > 0)
		{
			delete moves;
			delete line;
			if (!pos_is_transposition) {
				pos->transposition_table->add(pos_hash, hash_entry(DBL_MAX, depth_left));
			}
			return DBL_MAX;
		}
		else if (moves->size() == 0)
		{
			delete moves;
			delete line;
			if (!pos_is_transposition) {
				pos->transposition_table->add(pos_hash, hash_entry(0.00, depth_left));
			}
			return 0.00;
		}

		Move* pv_pos = prev_pv->front();
		prev_pv->pop_front();


		Move* first = moves->front();
		moves->front() = pv_pos;
		moves->push_back(first);

		std::sort(moves->begin() + 1, moves->end(), [pos](Move* m_1, Move* m_2) -> bool {return Evaluator::compare(pos, m_1, m_2); });

		int i = 0;
		for (Move* const& move : *moves)
		{
			pos->make_move(move);
			double score = 0.0;
			uint64_t move_hash = pos->hash(pos);

			if (i == 0 && left_most)
			{
				if (pos->transposition_table->contains_hash(move_hash))
				{
					score = pos->transposition_table->map->find(move_hash)->second.score;
				}
				else
				{
					score = -this->alpha_beta_prev_PV(pos, -beta, -alpha, depth_left - 1, line, true, prev_pv);
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
				if (pos->transposition_table->contains_hash(move_hash))
				{
					score = pos->transposition_table->map->find(move_hash)->second.score;
					// std::cout << "here" << std::endl;
				}
				else
				{
					score = -this->alpha_beta(pos, -beta, -alpha, depth_left - 1, line, false);
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
						pos->transposition_table->add(pos_hash, hash_entry(score, depth_left));
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
					if (!pos_is_transposition) {
						pos->transposition_table->add(pos_hash, hash_entry(score, depth_left));
					}
					alpha = score; // alpha acts like max in MiniMax
				}
			}

			i++;
			pos->unmake_move();
			for (Move* const& b : *line)
			{
				delete b;
			}
			line->clear();
		}
		for (Move* const& b : *moves)
		{
			delete b;
		}
		delete line;
		delete moves;

	}

	return alpha;
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
double Search::quiescence(Board* pos, double alpha, double beta)
{
	double stand_pat = Evaluator::evaluate(pos);
	uint64_t pos_hash = pos->hash(pos);
	bool pos_is_transposition = pos->transposition_table->contains_hash(pos_hash);

	if (stand_pat >= beta)
	{
		if (!pos_is_transposition) {
			pos->transposition_table->add(pos_hash, hash_entry(stand_pat, 0));
		}
		return beta;
	}
	else if (stand_pat > alpha)
	{
		alpha = stand_pat;
	}

	std::vector<Move*>* moves = pos->get_legal_captures();
	std::sort(moves->begin(), moves->end(), [pos](Move* m_1, Move* m_2) -> bool {return Evaluator::compare(pos, m_1, m_2); });
	//std::cout << moves->size() << std::endl;
	if (moves->size() == 0)
	{
		//std::cout << pos->pos_as_str() << std::endl;
		delete moves;

		double score = Evaluator::evaluate(pos);
		if (!pos_is_transposition) {
			pos->transposition_table->add(pos_hash, hash_entry(score, 0));
		}
		return score;
	}
	else
	{
		for (Move* const& move : *moves)
		{
			pos->make_move(move);
			double score;
			uint64_t move_hash = pos->hash(pos);
			bool is_transposition = pos->transposition_table->contains_hash(move_hash);
			if (is_transposition)
			{
				score = pos->transposition_table->map->find(move_hash)->second.score;
			}
			else
			{
				score = -this->quiescence(pos, -beta, -alpha);
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
					pos->transposition_table->add(move_hash, hash_entry(score, 0));
				}
				return beta; // fail hard beta-cutoff
			}
			if (score > alpha)
			{
				if (!pos_is_transposition) {
					pos->transposition_table->add(pos_hash, hash_entry(score, 0));
				}
				alpha = score; // alpha acts like max in MiniMax
			}
			pos->unmake_move();
		}
		for (Move* const& b : *moves)
		{
			delete b;
		}
		delete moves;

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
double Search::evaluate_iterative_deepening(Board* pos, unsigned int depth)
{
	std::list<Move*>* prev_PV = new std::list<Move*>;
	double res = 0.00;
	std::string best_move = "";

	for (unsigned int i = 1; i <= depth; i++)
	{
		std::list<Move*>* PV = new std::list<Move*>;

		res = this->alpha_beta_prev_PV(pos, -DBL_MAX, DBL_MAX, i, PV, true, prev_PV);

		std::cout << "info score " << std::fixed << (int)(res * 100 + 0.5);
		std::cout << " pv ";
		for (Move* const& move : *PV)
		{
			std::cout << " " << move->to_string() << " ";
		}
		std::cout << "nodes " << pos->transposition_table->map->size();
		std::cout << std::endl;

		best_move = PV->front()->to_string();

		for (Move* const& i : *prev_PV)
		{
			delete i;
		}
		prev_PV->clear();
		delete prev_PV;
		prev_PV = PV;

		pos->transposition_table->map->clear();
	}
	std::cout << "bestmove " << best_move << std::endl << std::endl;
	for (Move* const& i : *prev_PV)
	{
		delete i;
	}
	prev_PV->clear();
	delete prev_PV;
	return res;
}


std::list<Move*>* Search::gather_PV(Board* pos) {
	std::list<Move*>* PV = new std::list<Move*>;
	gather_PV_rec(pos, PV);
	return PV;
}

void Search::gather_PV_rec(Board* pos, std::list<Move*>* PV)
{
	std::vector<Move*>* moves = pos->possible_moves();
	Move* best_move = nullptr;
	double best_score = -DBL_MAX;
	for (Move* const& m : *moves) {
		pos->make_move(m);
		uint64_t hash = pos->hash(pos);
		if (pos->transposition_table->contains_hash(hash)) {
			double move_score = (*pos->transposition_table->map->find(hash)).second.score;
			if (move_score > best_score) {
				best_move = m;
			}
		}
		pos->unmake_move();
	}
	for (Move* const& m : *moves) {
		if (best_move != nullptr) {
			if (!m->equals(best_move)) {
				delete m;
			}
		}
	}
	if (best_move == nullptr) {

		moves->clear();
		delete moves;
		return;
	}

	PV->push_back(best_move);
	pos->make_move(best_move->clone());
	gather_PV_rec(pos, PV);
	pos->unmake_move();

	moves->clear();
	delete moves;

}
