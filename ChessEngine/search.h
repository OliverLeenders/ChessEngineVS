/*
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif
*/

#ifndef Search_H
#define Search_H

#include <algorithm>
#include <cmath>
#include <iostream>

#include "board.h"
#include "piece.h"
#include "evaluator.h"
#include "utility.h"

#define EVAL_SCORE_CUTOFF 900000
#define MATE_IN_ZERO 1000000

class Search
{
private:
	int alpha_beta(Board* pos, int alpha, int beta, unsigned int depth_left, std::list<Move*>* PV, bool left_most);
	int alpha_beta_prev_PV(Board* pos, int alpha, int beta, unsigned int depth_left, std::list<Move*>* PV, bool left_most, std::list<Move*>* prev_pv);
public:
	Search();
	int node_count = 0;
	int evaluate(Board* pos, unsigned int depth);
	int quiescence(Board* pos, int alpha, int beta);
	int evaluate_iterative_deepening(Board* pos, unsigned int depth);
	std::vector<std::vector<Move*>*>* killer_moves = new std::vector<std::vector<Move*>*>();
	bool swap_first = true;
	~Search();
};

#endif