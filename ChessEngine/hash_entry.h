#pragma once

class hash_entry
{
public:
	hash_entry(double set_score, int set_depth_left);
	double score;
	int depth_left;
	unsigned times_hit;
	~hash_entry();
}; 

