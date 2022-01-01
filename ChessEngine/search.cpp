#include "search.h"
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
	delete this->prev_pv;
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
int Search::evaluate(Board* pos, unsigned int depth)
{
	this->search_depth = depth;
	this->node_count = 0;
	std::list<Move*>* PV = new std::list<Move*>;
	int res = this->alpha_beta(pos, -INT_MAX, INT_MAX, depth, PV, 0, true);
	if (std::abs(res) < EVAL_SCORE_CUTOFF) {
		std::cout << "info score cp " << res;
	}
	else {
		std::cout << "info score mate " << Utility::sgn(res) * (MATE_IN_ZERO - std::abs(res) + 1) / 2;
	}
	std::cout << " pv ";
	for (Move* const& move : *PV)
	{
		std::cout << move->to_string() << " ";
	}
	std::cout << "nodes " << this->node_count;
	std::cout << " depth " << depth;

	std::cout << std::endl;

	std::string best_move = PV->front()->to_string();
	std::cout << "bestmove " << best_move << std::endl;
	std::cout << std::endl;
	for (Move* const& move : *PV) {
		delete move;
	}
	for (std::vector<Move*>* const& v : *killer_moves) {
		for (Move* const& move : *v) {
			delete move;
		}
		delete v;
	}
	killer_moves->clear();
	delete PV;
	return res;
}


/**
 * @brief runs an evalutation with a certain depth
 *
 * @param pos position to search from
 * @param depth depth
 * @return int evaluation
 */
int Search::evaluate_iterative_deepening(Board* pos, unsigned int depth)
{
	std::string best_move = "NOMOVE";

	for (int i = 1; i <= depth; i++) {
		this->search_depth = depth;
		this->node_count = 0;
		std::list<Move*>* PV = new std::list<Move*>;
		int res = this->alpha_beta(pos, -INT_MAX, INT_MAX, i, PV, 0, true);
		if (std::abs(res) < EVAL_SCORE_CUTOFF) {
			std::cout << "info score cp " << res;
		}
		else {
			std::cout << "info score mate " << Utility::sgn(res) * (MATE_IN_ZERO - std::abs(res) + 1) / 2;
		}
		std::cout << " pv ";
		for (Move* const& move : *PV)
		{
			std::cout << move->to_string() << " ";
		}
		std::cout << "nodes " << this->node_count;
		std::cout << " depth " << i;

		std::cout << std::endl;

		best_move = PV->front()->to_string();
		for (Move* const& move : *this->prev_pv) {
			delete move;
		}
		prev_pv->clear();
		for (Move* const& move : *PV) {
			this->prev_pv->push_back(move->clone());
			delete move;
		}
		for (std::vector<Move*>* const& v : *killer_moves) {
			for (Move* const& move : *v) {
				delete move;
			}
			delete v;
		}
		killer_moves->clear();
	}
	for (Move* const& m : *this->prev_pv) {
		delete m;
	}
	prev_pv->clear();
	std::cout << "bestmove " << best_move << "\n" << std::endl;
}

int Search::alpha_beta(Board* pos, int alpha, int beta, unsigned int depth_left, std::list<Move*>* PV, int ply, bool left_most)
{

	if (depth_left == 0)
	{
		pos->compute_pin_rays();
		pos->compute_other_checks();
		if (pos->num_checks != 0) {
			this->search_depth++;
			int score = this->alpha_beta(pos, alpha, beta, depth_left + 1, PV, ply, left_most);
			this->search_depth--;
		}
		return this->quiescence(pos, alpha, beta, ply);
	}
	else
	{
		// increment node counter
		this->node_count++;
		// get legal moves
		std::vector<Move*>* moves = pos->possible_moves();
		// sort moves


		// if is checkmate
		if (moves->size() == 0 && pos->num_checks > 0)
		{
			delete moves;
			return -MATE_IN_ZERO + ply;
		}
		// if is stalemate
		else if (moves->size() == 0)
		{
			delete moves;
			return 0;
		}
		if (prev_pv->size() > ply) {
			std::stable_sort(moves->begin(), moves->end(), [pos, this, depth_left, ply, left_most](Move* m_1, Move* m_2) -> bool {return Evaluator::compare(pos, m_1, m_2, this->prev_pv->at(ply), left_most); });
		}
		else {
			std::stable_sort(moves->begin(), moves->end(), [pos, this, depth_left, ply](Move* m_1, Move* m_2) -> bool {return Evaluator::compare(pos, m_1, m_2, nullptr, false); });

		}


		bool is_check = pos->num_checks > 0;

		if (pos->num_checks == 0 && depth_left >= 3) {
			int pos_fifty = pos->fifty_move_rule_counter;
			pos->fifty_move_rule_counter = 0;

			pos->switch_move();
			// null window search
			int score = -alpha_beta(pos, -beta, -beta + 1, depth_left - 1 - 2, PV, ply + 1, false);
			pos->switch_move();

			pos->fifty_move_rule_counter = pos_fifty;

			if (score >= beta) {
				for (Move* const& m : *moves) {
					delete m;
				}
				delete moves;
				return beta;
			}
		}

		// create PV line
		std::list<Move*>* line = new std::list<Move*>();
		int i = 0;
		for (Move* const& move : *moves)
		{
			// make move on board
			pos->make_move(move);

			int score;
			bool draw_score = false;
			// check for threefold repetition
			int repetition_counter = 0;

			// iterate backwards through moves stack
			for (int history_index = pos->stack_moves->size() - 1; history_index >= 0; history_index--) {
				if (pos->stack_moves->at(history_index)->is_capture || pos->stack_moves->at(history_index)->is_pawn_push) {
					goto next;
				}
				if (pos->stack_hashes->at(history_index) == pos->pos_hash) {
					repetition_counter++;
				}
				if (repetition_counter == 2) {
					draw_score = true;
					score = 0;
					goto next;
				}
			}
		next:
			// if score is not 0 because of threefold repetition
			if (!draw_score) {
				if (depth_left < 3 || i < 5 || is_check || move->is_capture || move->is_promotion) {
					score = -this->alpha_beta(pos, -beta, -alpha, depth_left - 1, line, ply + 1, left_most && i == 0);
				}
				else {
					if (i < 9) {
						score = -this->alpha_beta(pos, -beta, -alpha, depth_left - 2, line, ply + 1, left_most && i == 0);
					}
					else {
						score = -this->alpha_beta(pos, -beta, -alpha, depth_left / 3, line, ply + 1, left_most && i == 0);
					}
					if (score > alpha && score < beta) {
						for (Move* const& lm : *line) {
							delete lm;
						}
						line->clear();
						score = -this->alpha_beta(pos, -beta, -alpha, depth_left - 1, line, ply + 1, left_most && i == 0);
					}
				}
			}
			// fail high -- beta cutoff
			if (score >= beta)
			{
				pos->unmake_move();
				// cleanup
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
				return beta;
				//  fail hard beta-cutoff
			}
			// score is inside alpha-beta window (PV Node)
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
			} // all other moves fail low

			pos->unmake_move();
			for (Move* const& b : *line)
			{
				delete b;
			}
			line->clear();
			i++;
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
 * @brief performs a quiescence search such that only "quiet" positions are evaluated
 *
 * @param pos starting position
 * @param alpha alpha value for AB pruning
 * @param beta beta value for AB pruning
 * @param e evaluator object
 * @return int evaluation
 */
int Search::quiescence(Board* pos, int alpha, int beta, int ply)
{
	// increase node count
	this->node_count++;
	std::vector<Move*>* moves = pos->get_legal_captures();
	// if not in check
	if (pos->num_checks == 0) {
		// compute standing pat -- preliminary eval  
		int stand_pat = Evaluator::evaluate(pos);

		// stand_pat beta cutoff
		if (stand_pat >= beta)
		{
			for (Move* const& m : *moves) {
				delete m;
			}
			delete moves;
			return beta;
		}
		else if (stand_pat > alpha)
		{
			alpha = stand_pat;
		}
	}
	else {
		// if in check => search all check evasions = all legal moves
		for (Move* const& m : *moves) {
			delete m;
		}
		delete moves;
		moves = pos->possible_moves();

		if (moves->size() == 0) {
			delete moves;
			return -MATE_IN_ZERO + ply;
		}
	}


	std::stable_sort(moves->begin(), moves->end(), [pos, this](Move* m_1, Move* m_2) -> bool {return Evaluator::compare(pos, m_1, m_2, nullptr, false); });
	//std::cout << moves->size() << std::endl;
	if (moves->size() == 0)
	{
		//std::cout << pos->pos_as_str() << std::endl;
		delete moves;

		int score = Evaluator::evaluate(pos);
		return score;
	}
	else
	{
		for (Move* const& move : *moves)
		{
			pos->make_move(move);
			int score;
			score = -this->quiescence(pos, -beta, -alpha, ply + 1);
			if (score >= beta)
			{
				pos->unmake_move();
				for (Move* const& b : *moves)
				{
					delete b;
				}
				delete moves;
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
	}
	return alpha;
}
