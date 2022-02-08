#pragma once

#include "move.h"

constexpr int EXACT_SCORE = 0;
constexpr int UPPER_BOUND = 1;
constexpr int LOWER_BOUND = 2;
constexpr int VAL_UNKNOWN = 2000000;

class hash_entry
{
public:
	hash_entry(uint64_t z_key ,int set_score, int ply, int set_flag, Move* set_best_move);
	hash_entry();
	uint64_t key;
	int score;
	int ply;
	int flag;
	unsigned times_hit = 0;
	Move best_move;
	~hash_entry();
}; 

