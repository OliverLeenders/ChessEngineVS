#include "zobrist_hashmap.h"
zobrist_hashmap::zobrist_hashmap()
{
	Utility util;
	for (int i = 0; i < 781; i++)
	{
		this->zobrist_base_numbers[i] = util.random_64_bit_num();
	}
	this->map->reserve(1000000); 
}

void zobrist_hashmap::add(uint64_t hash, hash_entry entry)
{
	if (this->map->bucket_size(this->map->bucket(hash)) == 0) {
		this->map->insert(std::make_pair(hash, entry));
	}
	else {
		std::pair<uint64_t, hash_entry> other_entry = *this->map->begin(this->map->bucket(hash));
		if (other_entry.second.depth_left > entry.depth_left) {
			this->map->erase(other_entry.first);
			this->map->insert(std::make_pair(hash, entry));
		}
		else if (other_entry.second.times_hit < 1) {
			this->map->erase(other_entry.first);
			this->map->insert(std::make_pair(hash, entry));
		}
	}

}

bool zobrist_hashmap::contains_hash(uint64_t hash)
{
	bool res = !(this->map->find(hash) == this->map->end());
	(*this->map->find(hash)).second.times_hit += 1;
	return res;
}


zobrist_hashmap::~zobrist_hashmap()
{
	delete this->map;
	delete this->prev_map;
	delete[] this->zobrist_base_numbers;
}