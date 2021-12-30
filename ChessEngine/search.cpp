#include "search.h"
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
int Search::evaluate(Board* pos, unsigned int depth)
{
	this->search_depth = depth;
	this->node_count = 0;
	std::list<Move*>* PV = new std::list<Move*>;
	for (unsigned int j = 0; j < depth - 1; j++) {
		std::vector<Move*>* init_v = new std::vector<Move*>();
		init_v->push_back(new Move(-1, -1, false, false, false, 0));
		init_v->push_back(new Move(-1, -1, false, false, false, 0));
		killer_moves->push_back(init_v);
	}
	int res = this->alpha_beta(pos, -INT_MAX, INT_MAX, depth, PV);
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
	std::list<Move*>* prev_PV = new std::list<Move*>;
	int res = 0;
	std::string best_move = "";

	for (unsigned int i = 1; i <= depth; i++)
	{
		this->search_depth = i;
		this->node_count = 0;
		// init killer moves
		for (unsigned int j = 0; j < i - 1; j++) {
			std::vector<Move*>* init_v = new std::vector<Move*>();
			init_v->push_back(new Move(-1, -1, false, false, false, 0));
			init_v->push_back(new Move(-1, -1, false, false, false, 0));
			killer_moves->push_back(init_v);
		}
		std::list<Move*>* PV = new std::list<Move*>;
		for (unsigned i = 0; i < depth; i++) {
			std::vector<Move> init;
			init.push_back(Move(-1, -1, false, false, false, 0));
			init.push_back(Move(-1, -1, false, false, false, 0));
		}
		res = this->alpha_beta_prev_PV(pos, -INT_MAX, INT_MAX, i, PV, true, prev_PV);
		if (std::abs(res) < EVAL_SCORE_CUTOFF) {
			std::cout << "info score cp " << res;
		}
		else {
			std::cout << "info score mate " << Utility::sgn(res) * (MATE_IN_ZERO - std::abs(res) + 1) / 2;
		}
		std::cout << " depth " << i;
		std::cout << " pv ";
		for (Move* const& move : *PV)
		{
			std::cout << move->to_string() << " ";
		}
		std::cout << "nodes " << this->node_count;
		std::cout << std::endl;

		best_move = PV->front()->to_string();

		for (Move* const& i : *prev_PV)
		{
			delete i;
		}
		prev_PV->clear();
		delete prev_PV;
		prev_PV = PV;
		for (std::vector<Move*>* const& v : *killer_moves) {
			for (Move* const& move : *v) {
				delete move;
			}
			delete v;
		}
		killer_moves->clear();
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

int Search::alpha_beta(Board* pos, int alpha, int beta, unsigned int depth_left, std::list<Move*>* PV)
{

	if (depth_left == 0)
	{
		pos->compute_pin_rays();
		pos->compute_other_checks();
		if (pos->num_checks != 0) {
			this->search_depth++;
			int score = this->alpha_beta(pos, alpha, beta, depth_left + 1, PV);
			this->search_depth--;
		}
		return this->quiescence(pos, alpha, beta);
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
			return -MATE_IN_ZERO + (this->search_depth - depth_left);
		}
		// if is stalemate
		else if (moves->size() == 0)
		{
			delete moves;
			return 0;
		}
		
		if (depth_left < killer_moves->size()) {
			std::stable_sort(moves->begin(), moves->end(), [pos, this, depth_left](Move* m_1, Move* m_2) -> bool {return Evaluator::compare(pos, m_1, m_2, this->killer_moves->at(depth_left)); });
		}
		else {
			std::stable_sort(moves->begin(), moves->end(), [pos, this, depth_left](Move* m_1, Move* m_2) -> bool {return Evaluator::compare(pos, m_1, m_2, nullptr); });
		}

		bool is_check = pos->num_checks > 0;

		if (pos->num_checks == 0 && depth_left >= 3) {
			int pos_fifty = pos->fifty_move_rule_counter;
			pos->fifty_move_rule_counter = 0;
			
			pos->switch_move();
			// null window search
			int score = -alpha_beta(pos, -beta, -beta + 1, depth_left - 1 - 2, PV);
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
					score = -this->alpha_beta(pos, -beta, -alpha, depth_left - 1, line);
				}
				else {
					if (i < 7) {
						score = -this->alpha_beta(pos, -beta, -alpha, depth_left - 2, line);
					}
					else {
						score = -this->alpha_beta(pos, -beta, -alpha, depth_left / 3, line);
					}
					if (score > alpha && score < beta) {
						for (Move* const& lm : *line) {
							delete lm;
						}
						line->clear();
						score = -this->alpha_beta(pos, -beta, -alpha, depth_left - 1, line);
					}
				}
			}
			// fail high -- beta cutoff
			if (score >= beta)
			{
				// update killer moves
				if ((!move->is_capture) && depth_left < killer_moves->size()) {
					if (!killer_moves->at(depth_left)->at(1 - swap_first)->equals(move)) {
						delete killer_moves->at(depth_left)->at(1 - swap_first);
						killer_moves->at(depth_left)->at(1 - swap_first) = move->clone();
					}
				}
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
int Search::alpha_beta_prev_PV(Board* pos, int alpha, int beta, unsigned int depth_left, std::list<Move*>* PV, bool left_most, std::list<Move*>* prev_pv)
{
	// if previous PV has been exhausted
	if (prev_pv->size() == 0)
	{
		return this->alpha_beta(pos, alpha, beta, depth_left, PV);
	}
	// if depth left has been exhausted
	if (depth_left == 0)
	{
		return this->quiescence(pos, alpha, beta);
	}
	else
	{
		this->node_count++;
		// allocate PV line of subtree
		std::list<Move*>* line = new std::list<Move*>;
		// generate possible moves
		std::vector<Move*>* moves = pos->possible_moves();
		// generate a hash for the position

		// if is checkmate
		if (moves->size() == 0 && pos->num_checks > 0)
		{
			delete moves;
			delete line;
			return -MATE_IN_ZERO + (this->search_depth - depth_left);
		}
		// if is stalemate
		else if (moves->size() == 0)
		{
			delete moves;
			delete line;
			return 0;
		}

		// get & pop previous PV move
		Move* pv_pos = prev_pv->front();
		prev_pv->pop_front();

		// add PV move to move list and swap to front
		Move* first = moves->front();
		moves->front() = pv_pos;
		moves->push_back(first);

		// perform move ordering 
		if (depth_left < killer_moves->size()) {
			std::stable_sort(moves->begin() + 1, moves->end(), [pos, this, depth_left](Move* m_1, Move* m_2) -> bool {return Evaluator::compare(pos, m_1, m_2, this->killer_moves->at(depth_left)); });
		}
		else {
			std::stable_sort(moves->begin() + 1, moves->end(), [pos, this, depth_left](Move* m_1, Move* m_2) -> bool {return Evaluator::compare(pos, m_1, m_2, nullptr); });
		}

		// index for checking the leftmost child move
		int i = 0;

		// iterate over moves
		for (Move* const& move : *moves)
		{
			// make move on board
			pos->make_move(move);
			int score;
			bool draw_score = false;
			// check for threefold repetition
			int repetition_counter = 0;

			for (int history_index = pos->stack_moves->size() - 1; history_index >= 0; history_index--) {
				if (pos->stack_moves->at(history_index)->is_capture) {
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
			if (!draw_score) {
				score = -this->alpha_beta(pos, -beta, -alpha, depth_left - 1, line);

			}
			// if is left-most child
			if (i == 0 && left_most)
			{
				// left most path along search tree should be initial PV
				// clear current PV
				for (Move* const& pv_move : *PV)
				{
					delete pv_move;
				}
				PV->clear();

				// copy PV from subtree PV
				for (Move* const& line_move : *line)
				{
					PV->push_back(line_move->clone());
				}
				PV->push_front(move->clone());
			}
			// if move is not left most, i.e. not PV pos 
			else
			{
				// beta-cutoff branch
				if (score >= beta)
				{
					// update killer moves
					if ((!move->is_capture) && depth_left < killer_moves->size()) {
						if (swap_first && (!killer_moves->at(depth_left)->at(0)->equals(move))) {
							delete killer_moves->at(depth_left)->at(0);
							killer_moves->at(depth_left)->at(0) = move->clone();
						}
						else if ((!swap_first) && (!killer_moves->at(depth_left)->at(1)->equals(move))) {
							delete killer_moves->at(depth_left)->at(1);
							killer_moves->at(depth_left)->at(1) = move->clone();
						}
					}
					// undo move on board
					pos->unmake_move();
					// free move list
					for (Move* const& move : *moves)
					{
						delete move;
					}
					// clear subtree PV
					for (Move* const& line_move : *line)
					{
						delete line_move;
					}
					delete line;
					delete moves;
					// if is new position
					return beta;
					//  fail hard beta-cutoff
				}
				// alpha branch
				else if (score > alpha)
				{

					// clear current PV
					for (Move* const& pv_move : *PV)
					{
						delete pv_move;
					}

					PV->clear();
					// copy over subtree pv
					for (Move* const& line_move : *line)
					{
						PV->push_back(line_move->clone());
					}
					// add current move as first move
					PV->push_front(move->clone());
					// if is new position
					alpha = score; // alpha acts like max in MiniMax
				}
			}
			// increment counter
			i++;
			// undo move on board
			pos->unmake_move();
			// delete subtree PV
			for (Move* const& b : *line)
			{
				delete b;
			}
			line->clear();
		}
		// delete move list
		for (Move* const& b : *moves)
		{
			delete b;
		}
		delete line;
		delete moves;

	}
	// return score
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
int Search::quiescence(Board* pos, int alpha, int beta)
{
	// increase node count
	this->node_count++;
	// compute standin pat -- preliminary eval  
	int stand_pat = Evaluator::evaluate(pos);
	// stand_pat beta cutoff5ä
	if (stand_pat >= beta)
	{
		return beta;
	}
	else if (stand_pat > alpha)
	{
		alpha = stand_pat;
	}

	std::vector<Move*>* moves = pos->get_legal_captures();
	std::stable_sort(moves->begin(), moves->end(), [pos, this](Move* m_1, Move* m_2) -> bool {return Evaluator::compare(pos, m_1, m_2, nullptr); });
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
			score = -this->quiescence(pos, -beta, -alpha);
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
