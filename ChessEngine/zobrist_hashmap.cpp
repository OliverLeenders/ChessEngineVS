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
	this->map = new std::vector<hash_entry*>(size);
}

void zobrist_hashmap::add(uint64_t hash, hash_entry entry)
{
}

bool zobrist_hashmap::contains_hash(uint64_t hash)
{
	return true;
}


zobrist_hashmap::~zobrist_hashmap()
{
	for (hash_entry* const& x : *this->map) {
		delete x;
	}
	this->map->clear();
	delete this->map;
	delete[] this->zobrist_base_numbers;
}