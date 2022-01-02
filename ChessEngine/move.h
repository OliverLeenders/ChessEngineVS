#pragma once

#include <string>
#include <iostream>

class Move
{
public:
	Move(int set_origin, int set_target, bool set_is_capture, bool set_is_promotion, bool set_is_pawn_push, unsigned set_promotion_type);
	Move(std::string move_str, bool side_to_move, bool set_is_capture, bool set_is_pawn_push);
	
	int origin;
	int target;
	bool is_capture;
	bool is_promotion;
	bool is_pawn_push;
	unsigned promotion_type;
	bool is_nullmove();
	std::string to_string();
	Move* clone();
	bool equals(Move* m);
	~Move();
private:
	std::string index_to_string(int i);
	std::string type_to_lowercase_letter(unsigned type);
	unsigned char_to_type(char t);
	int num_str_to_int(char num);
	int char_str_to_int(char c);
};