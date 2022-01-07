#include "hash_entry.h"

hash_entry::hash_entry(uint64_t set_key, int set_score, int set_ply, int set_flag, Move* set_best_move) {
	this->key = set_key;
	this->score = set_score;
	this->flag = set_flag;
	this->ply = set_ply;
	this->best_move = set_best_move->clone();
	this->times_hit = 0;
}

hash_entry::hash_entry()
{
	this->key = 0;
	this->ply = 0;
	this->score = VAL_UNKNOWN;
	this->flag = VAL_UNKNOWN;
	this->best_move = nullptr;
}

hash_entry::~hash_entry() {
	delete this->best_move;
}
