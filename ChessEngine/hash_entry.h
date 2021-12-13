#pragma once

class hash_entry
{
public:
	hash_entry(int set_score, int set_depth_left);
	int score;
	int depth_left;
	unsigned times_hit;
	~hash_entry();
}; 

