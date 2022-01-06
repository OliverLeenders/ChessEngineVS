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
		for (int killer_i = 0; killer_i < i; killer_i++) {
			std::vector<Move*>* v = new std::vector<Move*>();
			v->push_back(new Move(-1, -1, false, false, false, 0));
			v->push_back(new Move(-1, -1, false, false, false, 0));
			this->killer_moves->push_back(v);
		}

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
	
	pos->transposition_table->clear();
	std::cout << "bestmove " << best_move << "\n" << std::endl;
}

int Search::evaluate_iterative_deepening_time(Board* pos, int ms)
{
	std::string best_move = "NOMOVE";
	this->start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	this->duration = std::chrono::milliseconds(ms);
	int depth = 1;
	bool time_left = true;
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


		int res = this->alpha_beta(pos, -INT_MAX, INT_MAX, depth, PV, 0, true);
		std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
		if (now - this->start_time < this->duration) {
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
	pos->transposition_table->clear();
	std::cout << "bestmove " << best_move << "\n" << std::endl;
	this->start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	this->duration = std::chrono::milliseconds(INT32_MAX);
}

int Search::alpha_beta(Board* pos, int alpha, int beta, unsigned int depth_left, std::list<Move*>* PV, int ply, bool left_most)
{
	std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	if (now - this->start_time >= this->duration) {
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

		int score = 0;
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
		Move* best_move = nullptr;
		int lower_bound = -VAL_UNKNOWN;

		if (!draw_score) {
			// create PV line
			std::list<Move*>* line = new std::list<Move*>();
			int i = 0;
			for (Move* const& move : *moves)
			{
				// make move on board
				pos->make_move(move);

				// if score is not 0 because of threefold repetition
				if (left_most && ply < PV->size() && i == 0) {
					score = -this->alpha_beta(pos, -beta, -alpha, depth_left - 1, line, ply + 1, left_most && i == 0);
				}
				else if (depth_left < 3 || i < 5 || is_check || move->is_capture || move->is_promotion) {
					score = -this->alpha_beta(pos, -beta, -alpha, depth_left - 1, line, ply + 1, left_most && i == 0);
				}
				else {
					if (i < 7) {
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
				if (score > LOWER_BOUND) {
					delete best_move;
					best_move = move->clone();
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
					pos->transposition_table->record_hash(pos->pos_hash, ply, beta, LOWER_BOUND, nullptr);
					delete best_move;
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

		pos->transposition_table->record_hash(pos->pos_hash, ply, score, flag, best_move);
		delete best_move;
		return alpha;
	}
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


	std::stable_sort(moves->begin(), moves->end(),
		[pos, this](Move* m_1, Move* m_2) -> bool {return Evaluator::compare(pos, m_1, m_2, nullptr, nullptr, nullptr, false); });
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
