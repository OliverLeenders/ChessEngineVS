#include "move.h"

Move::Move(int set_origin, int set_target, bool set_is_promotion, unsigned set_promotion_type) {
	this->origin = set_origin;
	this->target = set_target;
	this->is_promotion = set_is_promotion;
	this->promotion_type = set_promotion_type;
}

unsigned Move::char_to_type(char t) {
	switch (t)
	{
	case 'q':
		return 3;
	case 'r':
		return 5;
	case 'b':
		return 7;
	case 'n':
		return 9;
	default:
		return 0;
	}
}

Move::Move(std::string move_str, bool side_to_move) {
	if (move_str.length() == 5) {
		this->is_promotion = true;
		this->promotion_type = this->char_to_type(move_str.at(4));
		if (!side_to_move) {
			this->promotion_type++;
		}
	}
	else {
		this->is_promotion = false;
		this->promotion_type = 0;
	}
	this->origin = char_str_to_int(move_str.at(0)) + 8 * num_str_to_int(move_str.at(1));
	this->target = char_str_to_int(move_str.at(2)) + 8 * num_str_to_int(move_str.at(3));
}

int Move::num_str_to_int(char num) {
	switch (num)
	{
	case '1':
		return 0;
	case '2':
		return 1;
	case '3':
		return 2;
	case '4':
		return 3;
	case '5':
		return 4;
	case '6':
		return 5;
	case '7':
		return 6;
	case '8':
		return 7;
	default:
		return 0;
	}
}

int Move::char_str_to_int(char c) {
	switch (c)
	{
	case 'a':
		return 0;
	case 'b':
		return 1;
	case 'c':
		return 2;
	case 'd':
		return 3;
	case 'e':
		return 4;
	case 'f':
		return 5;
	case 'g':
		return 6;
	case 'h':
		return 7;
	default:
		return 0;
	}
}

Move::~Move() {};

std::string Move::to_string() {
	std::string str = "";
	str += this->index_to_string(this->origin);
	str += this->index_to_string(this->target);
	if (this->is_promotion) {
		str += this->type_to_lowercase_letter(promotion_type);
	} 
	return str;
}

std::string Move::index_to_string(int i)
{
	std::string str = "";
	char c = 97 + (i % 8);
	str += c;
	str += std::to_string((i / 8 + 1));
	return str;
}

std::string Move::type_to_lowercase_letter(unsigned type) {
	switch (type)
	{
	case 3:
		return "q";
	case 4:
		return "q";
	case 5:
		return "r";
	case 6:
		return "r";
	case 7:
		return "b";
	case 8:
		return "b";
	case 9:
		return "n";
	case 10:
		return "n";
	default:
		return "";
	}
}

