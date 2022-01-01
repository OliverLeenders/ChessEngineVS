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

constexpr int EVAL_SCORE_CUTOFF = 900000;
constexpr int MATE_IN_ZERO = 1000000;

class Search
{
private:
public:
	Search();
	int node_count = 0;
	int evaluate(Board* pos, unsigned int depth);
	int search_depth = 0;

	std::vector<Move*>* prev_pv = new std::vector<Move*>();

	int alpha_beta(Board* pos, int alpha, int beta, unsigned int depth_left, std::list<Move*>* PV, int ply, bool left_most);
	int quiescence(Board* pos, int alpha, int beta, int ply);
	int evaluate_iterative_deepening(Board* pos, unsigned int depth);
	std::vector<std::vector<Move*>*>* killer_moves = new std::vector<std::vector<Move*>*>();
	bool swap_first = true;
	~Search();
};

#endif