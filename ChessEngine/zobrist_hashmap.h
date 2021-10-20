#pragma once
#include <stdint.h>
#include <unordered_map>

#include "utility.h"

class zobrist_hashmap
{
public:
	zobrist_hashmap();
	void add(uint64_t hash, double eval);
	bool contains_hash(uint64_t hash);
	~zobrist_hashmap();
	std::unordered_map<uint64_t, double>* map = new std::unordered_map<uint64_t, double>;
	std::unordered_map<uint64_t, double>* prev_map = new std::unordered_map<uint64_t, double>;

	uint64_t* zobrist_base_numbers = new uint64_t[781];
private:
};

