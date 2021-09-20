#ifndef Move_H
#define Move_H

#include <string>

class Move
{
public:
	Move(int set_origin, int set_target, bool set_is_capture, bool set_is_promotion, unsigned set_promotion_type);
	Move(std::string move_str, bool side_to_move, bool set_is_capture);
	~Move();
	int origin;
	int target;
	bool is_capture;
	bool is_promotion;
	unsigned promotion_type;
	std::string to_string();
private: 
	std::string index_to_string(int i);
	std::string type_to_lowercase_letter(unsigned type);
	unsigned char_to_type(char t);
	int num_str_to_int(char num);
	int char_str_to_int(char c);
};

class move
{
};

#endif
