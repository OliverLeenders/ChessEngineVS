#include "zobrist_hashmap.h"
zobrist_hashmap::zobrist_hashmap()
{
	Utility util;
	for (int i = 0; i < 781; i++)
	{
		this->zobrist_base_numbers[i] = util.random_64_bit_num();
	}
	this->map->reserve(10000000); 
}

void zobrist_hashmap::add(uint64_t hash, double eval)
{
	this->map->insert(std::make_pair(hash, eval));
}

bool zobrist_hashmap::contains_hash(uint64_t hash)
{
	return !(this->map->find(hash) == this->map->end());
}


zobrist_hashmap::~zobrist_hashmap()
{
	delete this->map;
	delete this->prev_map;
	delete[] this->zobrist_base_numbers;
}