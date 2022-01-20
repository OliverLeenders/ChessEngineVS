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
	pos->transposition_table->switch_prev();
	Evaluator::reset_history();
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
	stop_now = false;
	int res = 0;
	for (int i = 1; i <= depth && (!stop_now); i++) {
		this->search_depth = depth;
		this->node_count = 0;
		std::list<Move*>* PV = new std::list<Move*>;
		for (int killer_i = 0; killer_i < i; killer_i++) {
			std::vector<Move*>* v = new std::vector<Move*>();
			v->push_back(new Move(-1, -1, false, false, false, 0));
			v->push_back(new Move(-1, -1, false, false, false, 0));
			this->killer_moves->push_back(v);
		}

		res = this->alpha_beta(pos, -INT_MAX, INT_MAX, i, PV, 0, true);
		if (!stop_now) {
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
		}
		for (Move* const& move : *this->prev_pv) {
			delete move;
		}
		// clearing the previous principal variation
		prev_pv->clear();
		for (Move* const& move : *PV) {
			this->prev_pv->push_back(move->clone());
			delete move;
		}
		delete PV;
		// clearing the killer moves
		for (std::vector<Move*>* const& v : *killer_moves) {
			for (Move* const& move : *v) {
				delete move;
			}
			delete v;
		}
		killer_moves->clear();
		pos->transposition_table->switch_prev();
	}
	for (Move* const& m : *this->prev_pv) {
		delete m;
	}
	prev_pv->clear();
	Evaluator::reset_history();
	std::cout << "bestmove " << best_move << "\n" << std::endl;
	this->stop_now = true;
	return res;
}

int Search::evaluate_iterative_deepening_time(Board* pos, int ms)
{
	std::string best_move = "NOMOVE";
	stop_now = false;
	this->start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	this->duration = std::chrono::milliseconds(ms);
	int depth = 1;
	bool time_left = true;
	int res;
	while (time_left) {
		this->search_depth = depth;
		this->node_count = 0;
		std::list<Move*>* PV = new std::list<Move*>;
		for (int killer_i = 0; killer_i < depth; killer_i++) {
			std::vector<Move*>* v = new std::vector<Move*>();
			v->push_back(new Move(-1, -1, false, false, false, 0));
			v->push_back(new Move(-1, -1, false, false, false, 0));
			this->killer_moves->push_back(v);
		}


		res = this->alpha_beta(pos, -INT_MAX, INT_MAX, depth, PV, 0, true);
		std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
		if ((!stop_now) && now - this->start_time < this->duration) {
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

			best_move = PV->front()->to_string();
		}
		else {
			time_left = false;
		}
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
		pos->transposition_table->switch_prev();
		depth++;
	}
	for (Move* const& m : *this->prev_pv) {
		delete m;
	}
	prev_pv->clear();
	Evaluator::reset_history();
	std::cout << "bestmove " << best_move << "\n" << std::endl;
	this->start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	this->duration = std::chrono::milliseconds(INT32_MAX);
	this->stop_now = true;
	return res;
}

int Search::alpha_beta(Board* pos, int alpha, int beta, unsigned int depth_left, std::list<Move*>* PV, int ply, bool left_most)
{
	std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	if (stop_now || now - this->start_time >= this->duration) {
		return 0;
	}
	// initializing hash flag
	int flag = UPPER_BOUND;
	// checking TT
	int hash_score = pos->transposition_table->probe_hash(pos->pos_hash, ply, alpha, beta);
	if (hash_score != VAL_UNKNOWN && (hash_score <= alpha || hash_score >= beta)) {
		return hash_score;
	}

	if (depth_left == 0)
	{
		pos->compute_pin_rays();
		pos->compute_other_checks();
		if (pos->num_checks != 0) {
			this->search_depth++;
			int score = this->alpha_beta(pos, alpha, beta, depth_left + 1, PV, ply, left_most);
			this->search_depth--;
		}
		int score = this->quiescence(pos, alpha, beta, ply);
		pos->transposition_table->record_hash(pos->pos_hash, ply, score, EXACT_SCORE, nullptr);
		return score;
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
			pos->transposition_table->record_hash(pos->pos_hash, ply, -MATE_IN_ZERO + ply, EXACT_SCORE, nullptr);
			return -MATE_IN_ZERO + ply;
		}
		// if is stalemate
		else if (moves->size() == 0)
		{
			delete moves;
			pos->transposition_table->record_hash(pos->pos_hash, ply, -MATE_IN_ZERO + ply, EXACT_SCORE, nullptr);
			return 0;
		}
		Move* pv_move = nullptr;
		if (prev_pv->size() > ply) {
			pv_move = this->prev_pv->at(ply);
		}
		Move* prev_best = pos->transposition_table->probe_for_prev_best_move(pos->pos_hash);

		std::vector<Move*>* killer_moves_at_depth = nullptr;
		if (ply < killer_moves->size()) {
			killer_moves_at_depth = killer_moves->at(ply);
		}

		std::stable_sort(moves->begin(), moves->end(),
			[pos, this, depth_left, ply, pv_move, prev_best, killer_moves_at_depth, left_most]
		(Move* m_1, Move* m_2) -> bool {return Evaluator::compare(pos, m_1, m_2, pv_move, prev_best, killer_moves_at_depth, left_most); });
		bool is_check = pos->num_checks > 0;
		// null move pruning
		
		int score = 0;
		bool draw_score = false;
		// check for threefold repetition
		int repetition_counter = 0;
		// check 50-move rule
		if (pos->fifty_move_rule_counter == 100) {
			draw_score = true;
			goto next;
		}

		// iterate backwards through moves stack
		for (size_t history_index = pos->stack_moves->size() - 1; history_index != SIZE_MAX; history_index--) {
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
		Move* best_move = nullptr;
		int lower_bound_score = -INT_MAX;

		if (!draw_score) {
			// null move pruning
			int game_phase = 4 * pos->queen_list->size() + 2 * pos->rook_list->size() + pos->bishop_list->size() + pos->knight_list->size();

			if (pos->num_checks == 0 && depth_left >= 3 && game_phase > 10) {
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

			std::list<Move*>* line = new std::list<Move*>();
			int i = 0;
			for (Move* const& move : *moves)
			{
				// make move on board
				pos->make_move(move);
				// if score is not 0 because of threefold repetition
				if (left_most && ply < PV->size() && i == 0) {
					score = -this->alpha_beta(pos, -beta, -alpha, depth_left - 1, line, ply + 1, true);
				}

				else if (depth_left < 3 || i < 5 || is_check || move->is_capture || move->is_promotion) {
					score = -this->alpha_beta(pos, -beta, -alpha, depth_left - 1, line, ply + 1, left_most && i == 0);
				}
				else {
					if (i < 5) {
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
				if (score > lower_bound_score) {
					delete best_move;
					best_move = move->clone();
					lower_bound_score = score;
				}

				// fail high -- beta cutoff
				if (score >= beta)
				{
					pos->unmake_move();
					if ((!move->is_capture) && ply < killer_moves->size()) {
						delete this->killer_moves->at(ply)->at(1);
						this->killer_moves->at(ply)->at(1) = this->killer_moves->at(ply)->at(0);
						this->killer_moves->at(ply)->at(0) = move->clone();
					}
					pos->transposition_table->record_hash(pos->pos_hash, ply, beta, LOWER_BOUND, best_move->clone());
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

					delete best_move;
					return beta;
					//  fail hard beta-cutoff
				}
				// score is inside alpha-beta window (PV Node)
				else if (score > alpha)
				{
					if (!move->is_capture) {
						Evaluator::history[pos->position[move->target]->get_type() - 1][move->target] += depth_left * depth_left;
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
					alpha = score; // alpha acts like max in MiniMax
					flag = EXACT_SCORE;
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

		if (draw_score) {
			pos->transposition_table->record_hash(pos->pos_hash, ply, score, flag, nullptr);
			delete best_move;
			return 0;
		}
		else {
			pos->transposition_table->record_hash(pos->pos_hash, ply, alpha, flag, best_move->clone());
			delete best_move;
			return alpha;
		}
	} // endif 
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

 // BUG ALERT
 // position startpos moves d2d4 g8f6 c2c4 b8c6 b1c3 e7e5 d4d5 c6b4 a2a3 b4a6 e2e4 f8d6 f1e2 a6c5 c1g5 a7a5 g1f3 h8f8 e1g1 h7h6 g5f6 d8f6 b2b4 a5b4 a3b4 a8a1 d1a1 c5b3 a1b2 b3d4 c4c5 d6e7 f3d4 e7c5 b4c5 e5d4 c3a4 f6f4 e2f3 d7d6 c5d6
 // position startpos moves e2e4 c7c6 d2d4 d7d5 b1c3 d5e4 c3e4 g8f6 e4f6 e7f6 c1f4 f8d6 f4d6 d8d6 d1d2 e8g8 e1c1 c8e6 g1f3 e6a2 b2b4 a7a5 c2c3 a5b4 c1b2 a2b3 d1a1 b4c3 d2c3 a8a1 b2a1 d6a3 a1b1 f8d8 f1d3 a3a2 b1c1 a2f2 c3b3 f2g2 h1e1 g2g4 e1e7 g4h5 e7b7 d8e8 b7b8 h5h6 c1b1 h6e3 b8e8 e3e8 b3c3 g7g6 b1c2 e8e6 h2h3 e6h3 c3c6 g8g7 d3g6 h7g6 c6e4 h3c8 c2d3 c8a6 d3e3 a6a3 e3f2 a3b4 f2g3 b4c4 g3g2 c4a2 g2g3 a2a4 e4f4 a4c6 g3g2 c6d5 f4g4 d5b7 g2g3 b7c6 g3g2 c6c2 g2g3 c2c7 g3f2 c7a7 f2g2 a7a2 g2h3 a2e2 h3g3 e2b5 g4h4 b5d5 h4g4 g6g5 g3g2 g7g8 g4c8 g8h7 g2g3 d5e4 c8f8 e4e6 f8d8 e6f5 d8e8 f5f4 g3f2 h7g7 e8e3 f4g4 e3e8 g4f5 f2g2 g5g4 f3e1 f5d5 g2g3 d5d4 e1g2 f6f5 e8e7 d4f6 e7f6 g7f6 g3f4 f6e6 g2e3 g4g3 f4g3 e6e5 g3f3 e5e6 f3f4 e6f6 e3f5 f6e6 f4e4 e6f6 e4f4 f6e6 f4e4 e6f6 f5d4 f6g6 e4f4 g6f6 f4e4 f6g6 e4f4 g6f6

int Search::quiescence(Board* pos, int alpha, int beta, int ply)
{
	// increase node count
	// std::cout << "quiescence" << std::endl;
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


	std::stable_sort(moves->begin(), moves->end(),
		[pos, this](Move* m_1, Move* m_2) -> bool {return Evaluator::compare(pos, m_1, m_2, nullptr, nullptr, nullptr, false); });
	//std::cout << moves->size() << std::endl;
	if (moves->size() == 0)
	{
		//std::cout << pos->pos_as_str() << std::endl;
		delete moves;
		return Evaluator::evaluate(pos);
	}
	else
	{
		for (Move* const& move : *moves)
		{
			pos->make_move(move);
			int score = -this->quiescence(pos, -beta, -alpha, ply + 1);
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
