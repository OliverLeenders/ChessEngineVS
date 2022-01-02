#pragma once

#include <list>
#include <vector>
#include <string>
class Utility
{
public:
	Utility();
	static void remove_first_occurance(std::list<int>* l, int i);
	template <typename T> static int sgn(T val) {
		return (T(0) < val) - (val < T(0));
	}
	uint64_t random_64_bit_num();
	static void split_string(std::vector<std::string>* v, std::string s);
	~Utility();
private:

};
