#include "zobrist_hashmap.h"
zobrist_hashmap::zobrist_hashmap(size_t set_size)
{
	Utility util;
	for (int i = 0; i < 781; i++)
	{
		this->zobrist_base_numbers[i] = util.random_64_bit_num();
	}
	this->size = set_size;
	this->count = 0;
	this->map = new hash_entry[this->size];
}

zobrist_hashmap::~zobrist_hashmap()
{
	delete[] this->map;
	delete[] this->zobrist_base_numbers;
}

int zobrist_hashmap::probe_hash(uint64_t z_key, int ply, int alpha, int beta)
{
	hash_entry* pos_hash_entry = &this->map[z_key % this->size];
	if (pos_hash_entry->key == z_key) {
		if (pos_hash_entry->ply <= ply) {
			if (pos_hash_entry->flag == EXACT_SCORE) {
				return pos_hash_entry->score;
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

/*
void RecordHash(int depth, int val, int hashf)
{
	HASHE* phashe = &hash_table[ZobristKey() % TableSize()];
	phashe->key = ZobristKey();
	phashe->best = BestMove();
	phashe->val = val;
	phashe->hashf = hashf;
	phashe->depth = depth;
}
*/

void zobrist_hashmap::clear() {
	delete[] this->map;
	this->map = new hash_entry[this->size];
}

void zobrist_hashmap::record_hash(uint64_t set_z_key, int set_ply, int set_score, int set_flag, Move* set_move) {
	hash_entry* pos_hash_entry = &map[set_z_key % this->size];
	pos_hash_entry->key = set_z_key;
	if (set_move != nullptr) {
		pos_hash_entry->best_move = set_move->clone();
	}
	else {
		pos_hash_entry->best_move = nullptr;
	}
	pos_hash_entry->score = set_score;
	pos_hash_entry->flag = set_flag;
	pos_hash_entry->ply = set_ply;
}