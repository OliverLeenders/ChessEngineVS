#include "utility.h"

Utility::Utility() {}

void Utility::remove_first_occurance(std::list<int>* l, int i) {
	std::list<int>::iterator itr;
	bool not_removed = true;
	for (itr = l->begin(); itr != l->end() && not_removed; itr++) {
		if (*itr == i) {
			l->erase(itr);
			not_removed = true;
		}
	}
}


uint64_t Utility::random_64_bit_num()
{
	uint64_t num = 0;
	for (int i = 0; i < 64; i++)
	{
		num = num * 2 + rand() % 2;
	}
	return num;
}



Utility::~Utility() {}
