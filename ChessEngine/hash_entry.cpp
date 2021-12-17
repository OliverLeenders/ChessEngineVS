#include "hash_entry.h"

hash_entry::hash_entry(int set_score, int set_depth_left, int set_score_type, Move* set_best_move) {
	this->score = set_score;
	this->score_type = set_score_type;
	this->depth_left = set_depth_left;
	this->best_move = set_best_move->clone();
	this->times_hit = 0;
}

hash_entry::~hash_entry() {
	delete this->best_move;
}
