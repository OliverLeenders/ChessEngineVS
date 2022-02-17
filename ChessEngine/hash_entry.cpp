#include "hash_entry.h"

hash_entry::hash_entry(uint64_t set_key, int set_score, int set_depth_left, int set_ply, int set_flag, Move* set_best_move) : 
	best_move(set_best_move->origin, set_best_move->target, set_best_move->is_capture, set_best_move->is_promotion, set_best_move->is_pawn_push, set_best_move->promotion_type) 
{
	this->key = set_key;
	this->score = set_score;
	this->flag = set_flag;
	this->depth_left = set_depth_left;
	this->ply = set_ply;
	this->times_hit = 0;
}

hash_entry::hash_entry() : best_move(0,0,false, false, false, 0U)
{
	this->key = 0;
	this->depth_left = 0;
	this->score = VAL_UNKNOWN;
	this->flag = VAL_UNKNOWN;
}

hash_entry::~hash_entry() {
}
