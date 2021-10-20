
#ifndef Utility_H
#define Utility_H

#include <list>
class Utility
{
public:
	Utility();
	static void remove_first_occurance(std::list<int>* l, int i);
	uint64_t random_64_bit_num();
	~Utility();

private:

};

#endif