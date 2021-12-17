#pragma once
#include <stdint.h>
#include <unordered_map>

#include "utility.h"
#include "hash_entry.h"

#define EXACT_SCORE 0;
#define UPPER_BOUND 1;
#define LOWER_BOUND 2;

class zobrist_hashmap
{
public:
	zobrist_hashmap(size_t set_size);
	void add(uint64_t hash, hash_entry entry);
	bool contains_hash(uint64_t hash);
	~zobrist_hashmap();
	std::vector<hash_entry*>* map;
	size_t size;
	size_t count;

	uint64_t* zobrist_base_numbers = new uint64_t[781];
private:
};

