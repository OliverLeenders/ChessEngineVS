#include "move.h"

/**
 * Default constructor. Creates Move object from the arguments.
 * 
 * \param set_origin move origin square
 * \param set_target move target square
 * \param set_is_capture whether move is a capture
 * \param set_is_promotion whether move is a promotion
 * \param set_promotion_type type to promote to (0 if no promotion)
 */
Move::Move(int set_origin, int set_target, bool set_is_capture, bool set_is_promotion, unsigned set_promotion_type) {
	this->origin = set_origin;
	this->target = set_target;
	this->is_capture = set_is_capture;
	this->is_promotion = set_is_promotion;
	this->promotion_type = set_promotion_type;
}

/**
 * Generates a type (white piece) from lowercase letter.
 * 
 * \param t lowercase type letter
 * \return  type of white piece
 */
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

/**
 * Constructor from move string0.
 * 
 * \param move_str move string (e.g. "g1e3" or "")
 * \param side_to_move 
 * \param set_is_capture
 */
Move::Move(std::string move_str, bool side_to_move, bool set_is_capture) {
	if (move_str.length() == 5) {
		this->is_promotion = true;
		this->promotion_type = this->char_to_type(move_str.at(4));
		this->is_capture = set_is_capture;

		if (!side_to_move && this->is_promotion) {
			this->promotion_type++;
		}
	}
	else {
		this->is_promotion = false;
		this->promotion_type = 0;
		this->is_capture = set_is_capture;
	}
	this->origin = char_str_to_int(move_str.at(0)) + 8 * num_str_to_int(move_str.at(1));
	this->target = char_str_to_int(move_str.at(2)) + 8 * num_str_to_int(move_str.at(3));
}

/**
 * Generate integer from char ('1' = 0, ... , '8' = 7).
 * 
 * \param num char to convert
 * \return converted char
 */
int Move::num_str_to_int(char num) {
	std::string num_str = "";
	num_str.push_back(num);
	return std::stoi(num_str) - 1;
}

/**
 * Create integer from char ('a' = 0, ... , 'h' = 7).
 * 
 * \param c char to convert
 * \return converted char
 */
int Move::char_str_to_int(char c) {
	c = c - 49;
	std::string c_str = "";
	c_str.push_back(c);
	return std::stoi(c_str);
}

/**
 * Default destructor.
 * 
 * \return 
 */
Move::~Move() {};

/**
 * Generates a string representation of a move according to UCI specification ("g1e3" or "e7e8q").
 * 
 * \return string representation
 */
std::string Move::to_string() {
	std::string str = "";
	str += this->index_to_string(this->origin);
	str += this->index_to_string(this->target);
	if (this->is_promotion) {
		str += this->type_to_lowercase_letter(promotion_type);
	} 
	return str;
}

/**
 * Create string representation of index (0 = "a1", ... , 63 = "h8").
 * 
 * \param i index
 * \return string representation
 */
std::string Move::index_to_string(int i)
{
	std::string str = "";
	char c = 97 + (i % 8);
	str += c;
	str += std::to_string((i / 8 + 1));
	return str;
}

/**
 * creates a lowecase letter (as a string) from a tpe (3 = "q", ...).
 * 
 * \param type type to convert
 * \return letter of type
 */
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

Move* Move::clone() {
	return new Move(this->origin, this->target, this->is_capture, this->is_promotion, this->promotion_type);
}

bool Move::equals(Move* m) {
	return (m->origin == this->origin) && (m->target == this->target) && (m->is_capture == this->is_capture) && (m->is_promotion == this->is_promotion) && (m->promotion_type && this->promotion_type);
}

