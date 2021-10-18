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


Utility::~Utility() {}
