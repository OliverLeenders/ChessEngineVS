#include "utility.h"

Utility::Utility() {}

void Utility::remove_first_occurance(std::list<int>* l, int i) {
	std::list<int>::iterator itr;
	bool not_removed = true;
	for (itr = l->begin(); itr != l->end() && not_removed; ) {
		if (*itr == i) {
			itr  = l->erase(itr);
			not_removed = true;
		}
		else {
			itr++;
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

/**
*Utility function to split a string into a vector.
*
* \param v vector of strings
* \param s string to split
*/
void Utility::split_string(std::vector<std::string>*v, std::string s) {
	std::string tmp = "";
	for (int i = 0; i < s.length(); i++) {
		if (s[i] != ' ') {
			tmp.push_back(s[i]);
		}
		else {
			if (tmp != "") {
				v->push_back(tmp);
			}
			tmp = "";
		}
	}
	v->push_back(tmp);
}

Utility::~Utility() {}

