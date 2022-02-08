#include "zobrist_hashmap.h"

uint64_t zobrist_hashmap::zobrist_base_numbers[781] = { 0 };

void zobrist_hashmap::init_bases() {
	Utility util;	
	for (int i = 0; i < 781; i++)
	{
		zobrist_base_numbers[i] = util.random_64_bit_num();
	}

}

zobrist_hashmap::zobrist_hashmap(size_t set_size)
{

	this->size = set_size;
	this->count = 0;
	this->map = new hash_entry[this->size];
	this->prev_map = new hash_entry[this->size];
}



zobrist_hashmap::~zobrist_hashmap()
{
	delete[] this->map;
	delete[] this->prev_map;
}

int zobrist_hashmap::probe_hash(uint64_t z_key, int ply, int alpha, int beta)
{
	hash_entry* pos_hash_entry = &this->map[z_key % this->size];
	if (pos_hash_entry->key == z_key) {
		if (pos_hash_entry->ply <= ply) {
			if (pos_hash_entry->flag == EXACT_SCORE) {
				// TODO calc mate distance
				int score = pos_hash_entry->score;
				if (std::abs(score) >= 900000) {
					//std::cout << "score: " << score << " = m " << (1000000 - std::abs(score) + 1) / 2 << " at ply: " << ply << " => " << score - Utility::sgn(score) * ply << std::endl;
					return score - Utility::sgn(score) * ply;
				}
				else {
					return score;
				}
			}
			if (pos_hash_entry->flag == UPPER_BOUND && pos_hash_entry->score <= alpha) {
				return alpha;
			}
			if (pos_hash_entry->flag == LOWER_BOUND && pos_hash_entry->score >= beta) {
				return beta;
			}
		}
	}
	return VAL_UNKNOWN;
}

Move* zobrist_hashmap::probe_for_prev_best_move(uint64_t z_key)
{
	hash_entry* pos_hash_entry = &this->map[z_key % this->size];
	if (pos_hash_entry->key == z_key) {
		return &pos_hash_entry->best_move;
	}
	else {
		return nullptr;
	}
}


void zobrist_hashmap::switch_prev() {
	delete[] this->prev_map;
	this->prev_map = this->map;
	this->map = new hash_entry[this->size];
}

void zobrist_hashmap::clear() {
	delete[] this->prev_map;
	this->prev_map = new hash_entry[this->size];
	delete[] this->map;
	this->map = new hash_entry[this->size];
}

void zobrist_hashmap::record_hash(uint64_t set_z_key, int set_ply, int set_score, int set_flag, Move* set_move) {
	hash_entry* pos_hash_entry = &map[set_z_key % this->size];
	pos_hash_entry->key = set_z_key;
	if (set_move != nullptr) {
		pos_hash_entry->best_move.origin = set_move->origin;
		pos_hash_entry->best_move.target = set_move->target;
		pos_hash_entry->best_move.is_capture = set_move->is_capture;
		pos_hash_entry->best_move.is_pawn_push = set_move->is_pawn_push;
		pos_hash_entry->best_move.is_promotion = set_move->is_promotion;
		pos_hash_entry->best_move.promotion_type = set_move->promotion_type; 
		
	}
	else {
		Move m(0, 0, false, false, false, 0U);
		pos_hash_entry->best_move = m;
	}
	pos_hash_entry->score = set_score;
	pos_hash_entry->flag = set_flag;
	pos_hash_entry->ply = set_ply;
}



