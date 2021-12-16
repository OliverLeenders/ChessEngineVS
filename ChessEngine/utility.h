
#ifndef Utility_H
#define Utility_H

#include <list>
class Utility
{
public:
	Utility();
	static void remove_first_occurance(std::list<int>* l, int i);
	template <typename T> static int sgn(T val) {
		return (T(0) < val) - (val < T(0));
	}
	uint64_t random_64_bit_num();
	~Utility();

private:

};

#endif