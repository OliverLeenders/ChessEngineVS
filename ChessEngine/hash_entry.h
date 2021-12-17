#pragma once

#include "move.h"

#define EXACT_SCORE 0;
#define UPPER_BOUND 1;
#define LOWER_BOUND 2;


class hash_entry
{
public:
	hash_entry(int set_score, int set_depth_left, int set_score_type, Move* set_best_move);
	int score;
	int depth_left;
	int score_type;
	unsigned times_hit;
	Move* best_move;
	~hash_entry();
}; 

