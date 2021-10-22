#pragma once
#include <stdint.h>
#include <unordered_map>

#include "utility.h"
#include "hash_entry.h"

class zobrist_hashmap
{
public:
	zobrist_hashmap();
	void add(uint64_t hash, hash_entry entry);
	bool contains_hash(uint64_t hash);
	~zobrist_hashmap();
	std::unordered_map<uint64_t, hash_entry>* map = new std::unordered_map<uint64_t, hash_entry>;
	std::unordered_map<uint64_t, hash_entry>* prev_map = new std::unordered_map<uint64_t, hash_entry>;

	uint64_t* zobrist_base_numbers = new uint64_t[781];
private:
};

