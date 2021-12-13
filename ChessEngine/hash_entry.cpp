#include "hash_entry.h"

hash_entry::hash_entry(int set_score, int set_depth_left) {
	this->score = set_score;
	this->depth_left = set_depth_left;
	this->times_hit = 0;
}

hash_entry::~hash_entry() {
}
