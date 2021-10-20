#include "board.h"
#include <iostream>

/**
 * @brief Construct a new Board:: Board object -- standard constructor
 *
 */
Board::Board()
{
	for (int i = 0; i < 64; i++)
	{
		this->position[i] = new Piece(0);
	}
	this->white_to_move = true;
	for (int i = 0; i < 4; i++)
	{
		this->castling_rights[i] = true;
	}
	for (int i = 0; i < 64; i++)
	{
		this->pins[i] = 0;
		this->checks[i] = false;
	}
	this->prev_pos = nullptr;
	transposition_table = new zobrist_hashmap;
	this->pos_hash = this->hash(this);
}
/**
 * @brief Construct a new Board:: Board object from a given position
 *
 * @param set_pos position to copy
 * @param who_to_move which side to move
 */
Board::Board(Piece* set_pos[], bool who_to_move, bool* set_castling_rights)
{
	for (int i = 0; i < 64; i++)
	{
		this->position[i] = set_pos[i];
	}
	this->white_to_move = who_to_move;
	for (int i = 0; i < 4; i++)
	{
		this->castling_rights[i] = set_castling_rights[i];
	}
	delete[] set_pos;
	delete[] set_castling_rights;
	for (int i = 0; i < 64; i++)
	{
		this->pins[i] = 0;
		this->checks[i] = false;
	}
	this->prev_pos = nullptr;
	transposition_table = new zobrist_hashmap;
	this->pos_hash = this->hash(this);
}

/**
 * @brief Construct a new Board:: Board object according to specified fen
 *
 * @param fen fen-string
 */
Board::Board(std::string fen)
{
	// TODO: fix leak
	for (int i = 0; i < 64; i++)
	{
		this->position[i] = new Piece(0);
	}
	int pos_index = 56;
	long unsigned int i = 0;
	for (; i < fen.size(); i++)
	{
		char c = fen[i];
		switch (c)
		{
		case '/':
			pos_index -= pos_index % 8;
			pos_index -= 8;
			break;
		case 'K':
			this->position[pos_index]->set_piece_type(1);
			this->white_king_pos = pos_index;
			if (pos_index % 8 != 7)
			{
				pos_index++;
			}
			break;
		case 'k':
			this->position[pos_index]->set_piece_type(2);
			this->black_king_pos = pos_index;
			if (pos_index % 8 != 7)
			{
				pos_index++;
			}
			break;
		case 'Q':
			this->position[pos_index]->set_piece_type(3);
			this->queen_list->push_back(pos_index);
			if (pos_index % 8 != 7)
			{
				pos_index++;
			}
			break;
		case 'q':
			this->position[pos_index]->set_piece_type(4);
			this->queen_list->push_back(pos_index);
			if (pos_index % 8 != 7)
			{
				pos_index++;
			}
			break;
		case 'R':
			this->position[pos_index]->set_piece_type(5);
			this->rook_list->push_back(pos_index);
			if (pos_index % 8 != 7)
			{
				pos_index++;
			}
			break;
		case 'r':
			this->position[pos_index]->set_piece_type(6);
			this->rook_list->push_back(pos_index);
			if (pos_index % 8 != 7)
			{
				pos_index++;
			}
			break;
		case 'B':
			this->position[pos_index]->set_piece_type(7);
			this->bishop_list->push_back(pos_index);
			if (pos_index % 8 != 7)
			{
				pos_index++;
			}
			break;
		case 'b':
			this->position[pos_index]->set_piece_type(8);
			this->bishop_list->push_back(pos_index);
			if (pos_index % 8 != 7)
			{
				pos_index++;
			}
			break;
		case 'N':
			this->position[pos_index]->set_piece_type(9);
			this->knight_list->push_back(pos_index);
			if (pos_index % 8 != 7)
			{
				pos_index++;
			}
			break;
		case 'n':
			this->position[pos_index]->set_piece_type(10);
			this->knight_list->push_back(pos_index);
			if (pos_index % 8 != 7)
			{
				pos_index++;
			}
			break;
		case 'P':
			this->position[pos_index]->set_piece_type(11);
			this->pawn_list->push_back(pos_index);
			if (pos_index % 8 != 7)
			{
				pos_index++;
			}
			break;
		case 'p':
			this->position[pos_index]->set_piece_type(12);
			this->pawn_list->push_back(pos_index);
			if (pos_index % 8 != 7)
			{
				pos_index++;
			}
			break;
		case '1':
			this->position[pos_index]->set_piece_type(0);
			if (pos_index % 8 != 7)
			{
				pos_index++;
			}
			break;
		case '2':
			for (int j = 0; j < 2; j++)
			{
				this->position[pos_index]->set_piece_type(0);
				if (pos_index % 8 != 7)
				{
					pos_index++;
				}
			}
			break;
		case '3':
			for (int j = 0; j < 3; j++)
			{
				this->position[pos_index]->set_piece_type(0);
				if (pos_index % 8 != 7)
				{
					pos_index++;
				}
			}
			break;
		case '4':
			for (int j = 0; j < 4; j++)
			{
				this->position[pos_index]->set_piece_type(0);
				if (pos_index % 8 != 7)
				{
					pos_index++;
				}
			}
			break;
		case '5':
			for (int j = 0; j < 5; j++)
			{
				this->position[pos_index]->set_piece_type(0);
				if (pos_index % 8 != 7)
				{
					pos_index++;
				}
			}
			break;
		case '6':
			for (int j = 0; j < 6; j++)
			{
				this->position[pos_index]->set_piece_type(0);
				if (pos_index % 8 != 7)
				{
					pos_index++;
				}
			}
			break;
		case '7':
			for (int j = 0; j < 7; j++)
			{
				this->position[pos_index]->set_piece_type(0);
				if (pos_index % 8 != 7)
				{
					pos_index++;
				}
			}
			break;
		case '8':
			for (int j = 0; j < 8; j++)
			{
				this->position[pos_index]->set_piece_type(0);
				if (pos_index % 8 != 7)
				{
					pos_index++;
				}
			}
			break;
		case ' ':
			i++;
			goto rest;
		default:
			break;
		}
	}
rest:
	if (fen[i] == 'w')
	{
		this->white_to_move = true;
	}
	else
	{
		this->white_to_move = false;
	}
	i += 2;
	for (int j = 0; j < 4; j++)
	{
		this->castling_rights[j] = false;
	}
	// some side has castling rights
	if (fen[i] != '-')
	{
		while (fen[i] != ' ')
		{
			if (fen[i] == 'K')
			{
				this->castling_rights[0] = true;
			}
			else if (fen[i] == 'Q')
			{
				this->castling_rights[1] = true;
			}
			else if (fen[i] == 'k')
			{
				this->castling_rights[2] = true;
			}
			else if (fen[i] == 'q')
			{
				this->castling_rights[3] = true;
			}
			i++;
		}
	}
	i++;
	if (fen[i] != '-')
	{
		// get en passant target square
		int col = (int)fen[i] - 97;
		int row = (fen[i + 1] - '0') - 1;
		this->en_passant_target_index = row * 8 + col;
	}
	else
	{
		this->en_passant_target_index = -1;
	}
	//std::cout << fen[i] << std::endl;
	/*
	for (int j = 0; j < 4; j++)
	{
		std::cout << this->castling_rights[j];
	}
	std::cout << std::endl;
	std::cout << "en passant target: " + std::to_string(this->en_passant_target_index) << std::endl;
	*/
	for (int i = 0; i < 64; i++)
	{
		this->pins[i] = 0;
		this->checks[i] = false;
	}
	transposition_table = new zobrist_hashmap;
	this->pos_hash = this->hash(this);
	compute_attacked_squares();
	compute_pin_rays();
	compute_other_checks();
}

/**
 * @brief set a position at an index to a specific piece
 *
 * @param type
 * @param pos
 * @return int
 */
int Board::set_piece(unsigned type, int pos)
{
	//std::cout << "type: " + std::to_string(type) + ", pos: " + std::to_string(pos) + ", moved: " + std::to_string(set_moved) << std::endl;
	this->position[pos]->set_piece_type(type);
	return 0;
}
/**
 * @brief Destroy the Board:: Board object and release memory
 *
 */
Board::~Board()
{
	for (int i = 0; i < 64; i++)
	{
		delete this->position[i];
	}
	delete[] this->attacked;
	delete[] this->castling_rights;
	delete[] this->check_direction;
	delete[] this->pins;
	delete[] this->checks;
	delete[] this->position;

	delete this->transposition_table;

	delete this->queen_list;
	delete this->rook_list;
	delete this->bishop_list;
	delete this->knight_list;
	delete this->pawn_list;

	this->stack_captures->clear();
	delete this->stack_captures;

	for (Move* m : *this->stack_moves) {
		delete m;
	}
	this->stack_moves->clear();
	delete this->stack_moves;

	for (bool* c : *this->stack_castling_rights) {
		delete[] c;
	}
	this->stack_castling_rights->clear();
	delete this->stack_castling_rights;

	this->stack_hashes->clear();
	delete this->stack_hashes;

	this->stack_en_passant_target_index;
	delete this->stack_en_passant_target_index;
}


uint64_t Board::hash(Board* b)
{
	uint64_t hash = 0;

	std::list<int>::iterator itr;
	hash = hash xor this->transposition_table->zobrist_base_numbers[(b->white_king_pos * 12) + b->position[b->white_king_pos]->get_type()];
	hash = hash xor this->transposition_table->zobrist_base_numbers[(b->black_king_pos * 12) + b->position[b->black_king_pos]->get_type()];
	for (itr = b->queen_list->begin(); itr != b->queen_list->end(); itr++) {
		hash = hash xor this->transposition_table->zobrist_base_numbers[(*itr * 12) + b->position[*itr]->get_type()];
	}
	for (itr = b->rook_list->begin(); itr != b->rook_list->end(); itr++) {
		hash = hash xor this->transposition_table->zobrist_base_numbers[(*itr * 12) + b->position[*itr]->get_type()];
	}
	for (itr = b->bishop_list->begin(); itr != b->bishop_list->end(); itr++) {
		hash = hash xor this->transposition_table->zobrist_base_numbers[(*itr * 12) + b->position[*itr]->get_type()];
	}
	for (itr = b->knight_list->begin(); itr != b->knight_list->end(); itr++) {
		hash = hash xor this->transposition_table->zobrist_base_numbers[(*itr * 12) + b->position[*itr]->get_type()];
	}
	for (itr = b->pawn_list->begin(); itr != b->pawn_list->end(); itr++) {
		hash = hash xor this->transposition_table->zobrist_base_numbers[(*itr * 12) + b->position[*itr]->get_type()];
	}
	int i = 64;
	for (int j = 0; j < 4; j++)
	{
		if (b->castling_rights[j])
		{
			hash = hash xor this->transposition_table->zobrist_base_numbers[i + j];
		}
	}
	if (b->en_passant_target_index > 0)
	{
		hash = hash xor this->transposition_table->zobrist_base_numbers[i + 4 + (b->en_passant_target_index % 8)];
	}
	if (!b->white_to_move)
	{
		hash = hash xor this->transposition_table->zobrist_base_numbers[i + 12];
	}
	return hash;
}



void Board::compute_attacked_squares()
{
	for (int i = 0; i < 64; i++) {
		this->attacked[i] = false;
	}
	if (!this->white_to_move) {
		int pos = this->white_king_pos;
		this->compute_king_attacked_squares(pos);
		for (int i : *this->queen_list) {
			if (this->position[i]->is_white()) {
				this->add_diagonal_attack_rays(i);
				this->add_straight_attack_rays(i);
			}
		}
		for (int i : *this->rook_list) {
			if (this->position[i]->is_white()) {
				this->add_straight_attack_rays(i);
			}
		}
		for (int i : *this->bishop_list) {
			if (this->position[i]->is_white()) {
				this->add_diagonal_attack_rays(i);
			}
		}
		for (int i : *this->knight_list) {
			if (this->position[i]->is_white()) {
				this->compute_knight_attacked_squares(i);
			}
		}
		for (int i : *this->pawn_list) {
			if (this->position[i]->is_white()) {
				if (i % 8 < 7) {
					this->attacked[i + 9] = true;
				}
				if (i % 8 > 0) {
					this->attacked[i + 7] = true;
				}
			}
		}
	}
	else {
		int pos = this->black_king_pos;
		this->compute_king_attacked_squares(pos);
		for (int i : *this->queen_list) {
			if (this->position[i]->is_black()) {
				this->add_diagonal_attack_rays(i);
				this->add_straight_attack_rays(i);
			}
		}
		for (int i : *this->rook_list) {
			if (this->position[i]->is_black()) {
				this->add_straight_attack_rays(i);
			}
		}
		for (int i : *this->bishop_list) {
			if (this->position[i]->is_black()) {
				this->add_diagonal_attack_rays(i);
			}
		}
		for (int i : *this->knight_list) {
			if (this->position[i]->is_black()) {
				this->compute_knight_attacked_squares(i);
			}
		}
		for (int i : *this->pawn_list) {
			if (this->position[i]->is_black()) {
				if (i % 8 < 7) {
					this->attacked[i - 7] = true;
				}
				if (i % 8 > 0) {
					this->attacked[i - 9] = true;
				}
			}
		}
	}
	return;
}

void Board::compute_king_attacked_squares(int pos)
{
	if (pos % 8 < 7) {
		this->attacked[pos + 1] = true;
	}
	if (pos % 8 > 0) {
		this->attacked[pos - 1] = true;
	}
	if (pos < 56) {
		this->attacked[pos + 8] = true;
	}
	if (pos > 7) {
		this->attacked[pos - 8] = true;
	}
	if (pos % 8 < 7 && pos < 56) {
		this->attacked[pos + 9] = true;
	}
	if (pos % 8 < 7 && pos > 7) {
		this->attacked[pos - 7] = true;
	}
	if (pos % 8 > 0 && pos < 56) {
		this->attacked[pos + 7] = true;
	}
	if (pos % 8 > 0 && pos > 7) {
		this->attacked[pos - 9] = true;
	}
}

void Board::compute_knight_attacked_squares(int pos) {
	if (pos % 8 < 7 && pos < 48) {
		this->attacked[pos + 17] = true;
	}
	if (pos % 8 > 0 && pos < 48) {
		this->attacked[pos + 15] = true;
	}
	if (pos % 8 < 7 && pos > 15) {
		this->attacked[pos - 15] = true;
	}
	if (pos % 8 > 0 && pos > 15) {
		this->attacked[pos - 17] = true;
	}
	if (pos % 8 < 6 && pos < 56) {
		this->attacked[pos + 10] = true;
	}
	if (pos % 8 > 1 && pos < 56) {
		this->attacked[pos + 6] = true;
	}
	if (pos % 8 < 6 && pos > 7) {
		this->attacked[pos - 6] = true;
	}
	if (pos % 8 > 1 && pos > 7) {
		this->attacked[pos - 10] = true;
	}
	return;
}

void Board::add_straight_attack_rays(int i)
{
	int j = i;
	while (j < 56)
	{
		j += 8;
		if (!this->position[j]->is_empty())
		{
			this->attacked[j] = true;
			break;
		}
		else
		{
			this->attacked[j] = true;
		}
	}
	j = i;
	while (j >= 8)
	{
		j -= 8;
		if (!this->position[j]->is_empty())
		{
			this->attacked[j] = true;
			break;
		}
		else
		{
			this->attacked[j] = true;
		}
	}
	j = i;
	while (j % 8 < 7)
	{
		j++;
		if (!this->position[j]->is_empty())
		{
			this->attacked[j] = true;
			break;
		}
		else
		{
			this->attacked[j] = true;
		}
	}
	j = i;
	while (j % 8 > 0)
	{
		j--;
		if (!this->position[j]->is_empty())
		{
			this->attacked[j] = true;
			break;
		}
		else
		{
			this->attacked[j] = true;
		}
	}
}

void Board::add_diagonal_attack_rays(int i)
{
	int j = i;
	while (j % 8 < 7 && j < 56) // diagonally right up
	{
		j += 9;
		if (!this->position[j]->is_empty())
		{
			this->attacked[j] = true;
			break;
		}
		else
		{
			this->attacked[j] = true;
		}
	}
	j = i;
	while (j % 8 > 0 && j < 56) // diagonally left up
	{
		j += 7;
		if (!this->position[j]->is_empty())
		{
			this->attacked[j] = true;
			break;
		}
		else
		{
			this->attacked[j] = true;
		}
	}
	j = i;
	while (j % 8 < 7 && j >= 8)
	{
		j -= 7;
		if (!this->position[j]->is_empty())
		{
			this->attacked[j] = true;
			break;
		}
		else
		{
			this->attacked[j] = true;
		}
	}
	j = i;
	while (j % 8 > 0 && j >= 8)
	{
		j -= 9;
		if (!this->position[j]->is_empty())
		{
			this->attacked[j] = true;
			break;
		}
		else
		{
			this->attacked[j] = true;
		}
	}
}

/**
 * @brief generates a list of possible moves
 *
 * @return std::vector<Board *>*
 */
std::vector<Move*>* Board::possible_moves()
{
	std::vector<Move*>* moves = new std::vector<Move*>();
	this->check_direction[0] = 0;
	this->check_direction[1] = 0;

	compute_attacked_squares();
	compute_pin_rays();
	compute_other_checks();

	if (this->white_to_move)
	{
		add_king_moves(moves, this->white_king_pos);
	}
	else
	{
		add_king_moves(moves, this->black_king_pos);
	}

	if (this->num_checks > 1)
	{
		return moves;
	}
	std::list<int>::iterator itr;
	for (itr = queen_list->begin(); itr != queen_list->end(); itr++) {
		if ((this->position[*itr]->is_white() && this->white_to_move) || (this->position[*itr]->is_black() && !white_to_move)) {
			this->add_queen_moves(moves, *itr);
		}
	}
	for (itr = rook_list->begin(); itr != rook_list->end(); itr++) {
		if ((this->position[*itr]->is_white() && this->white_to_move) || (this->position[*itr]->is_black() && !white_to_move)) {
			this->add_rook_moves(moves, *itr);
		}
	}
	for (itr = bishop_list->begin(); itr != bishop_list->end(); itr++) {
		if ((this->position[*itr]->is_white() && this->white_to_move) || (this->position[*itr]->is_black() && !white_to_move)) {
			this->add_bishop_moves(moves, *itr);
		}
	}
	for (itr = knight_list->begin(); itr != knight_list->end(); itr++) {
		if ((this->position[*itr]->is_white() && this->white_to_move) || (this->position[*itr]->is_black() && !white_to_move)) {
			this->add_knight_moves(moves, *itr);
		}
	}
	for (itr = pawn_list->begin(); itr != pawn_list->end(); itr++) {
		if ((this->position[*itr]->is_white() && this->white_to_move) || (this->position[*itr]->is_black() && !white_to_move)) {
			this->add_pawn_moves(moves, *itr);
		}
	}

	return moves;
}

void Board::compute_other_checks()
{
	if (this->white_to_move)
	{
		int king_pos = this->white_king_pos;
		// knight checks
		if (king_pos % 8 < 6 && king_pos < 56 && this->position[king_pos + 10]->get_type() == 10)
		{
			this->checks[king_pos + 10] = true;
			this->num_checks++;
		}
		if (king_pos % 8 > 1 && king_pos < 56 && this->position[king_pos + 6]->get_type() == 10)
		{
			this->checks[king_pos + 6] = true;
			this->num_checks++;
		}
		if (king_pos % 8 < 6 && king_pos >= 8 && this->position[king_pos - 6]->get_type() == 10)
		{
			this->checks[king_pos - 6] = true;
			this->num_checks++;
		}
		if (king_pos % 8 > 1 && king_pos >= 8 && this->position[king_pos - 10]->get_type() == 10)
		{
			this->checks[king_pos - 10] = true;
			this->num_checks++;
		}
		if (king_pos % 8 < 7 && king_pos < 48 && this->position[king_pos + 17]->get_type() == 10)
		{
			this->checks[king_pos + 17] = true;
			this->num_checks++;
		}
		if (king_pos % 8 > 0 && king_pos < 48 && this->position[king_pos + 15]->get_type() == 10)
		{
			this->checks[king_pos + 15] = true;
			this->num_checks++;
		}
		if (king_pos % 8 < 7 && king_pos >= 16 && this->position[king_pos - 15]->get_type() == 10)
		{
			this->checks[king_pos - 15] = true;
			this->num_checks++;
		}

		if (king_pos % 8 > 0 && king_pos >= 16 && this->position[king_pos - 17]->get_type() == 10)
		{
			this->checks[king_pos - 17] = true;
			this->num_checks++;
		}

		if (king_pos % 8 > 0 && king_pos < 48 && this->position[king_pos + 7]->get_type() == 12)
		{
			this->checks[king_pos + 7] = true;
			this->num_checks++;
		}
		if (king_pos % 8 < 7 && king_pos < 48 && this->position[king_pos + 9]->get_type() == 12)
		{
			this->checks[king_pos + 9] = true;
			this->num_checks++;
		}
	}
	else
	{
		int king_pos = this->black_king_pos;
		if (king_pos % 8 < 6 && king_pos < 56 && this->position[king_pos + 10]->get_type() == 9)
		{
			this->checks[king_pos + 10] = true;
			this->num_checks++;
		}
		if (king_pos % 8 > 1 && king_pos < 56 && this->position[king_pos + 6]->get_type() == 9)
		{
			this->checks[king_pos + 6] = true;
			this->num_checks++;
		}
		if (king_pos % 8 < 6 && king_pos >= 8 && this->position[king_pos - 6]->get_type() == 9)
		{
			this->checks[king_pos - 6] = true;
			this->num_checks++;
		}
		if (king_pos % 8 > 1 && king_pos >= 8 && this->position[king_pos - 10]->get_type() == 9)
		{
			this->checks[king_pos - 10] = true;
			this->num_checks++;
		}
		if (king_pos % 8 < 7 && king_pos < 48 && this->position[king_pos + 17]->get_type() == 9)
		{
			this->checks[king_pos + 17] = true;
			this->num_checks++;
		}
		if (king_pos % 8 > 0 && king_pos < 48 && this->position[king_pos + 15]->get_type() == 9)
		{
			this->checks[king_pos + 15] = true;
			this->num_checks++;
		}
		if (king_pos % 8 < 7 && king_pos >= 16 && this->position[king_pos - 15]->get_type() == 9)
		{
			this->checks[king_pos - 15] = true;
			this->num_checks++;
		}

		if (king_pos % 8 > 0 && king_pos >= 16 && this->position[king_pos - 17]->get_type() == 9)
		{
			this->checks[king_pos - 17] = true;
			this->num_checks++;
		}

		if (king_pos % 8 < 7 && king_pos >= 8 && this->position[king_pos - 7]->get_type() == 11)
		{
			this->checks[king_pos - 7] = true;
			this->num_checks++;
		}
		if (king_pos % 8 > 0 && king_pos > 8 && this->position[king_pos - 9]->get_type() == 11)
		{
			this->checks[king_pos - 9] = true;
			this->num_checks++;
		}
	}
	// add pawn checks
}

/**
 * \brief computes the pin rays and sliding piece checks for the board. Pin rays are stored in the board's pins array and have an indicator of pinning direction.
 *
 */
void Board::compute_pin_rays()
{
	for (int i = 0; i < 64; i++) {
		this->pins[i] = 0;
		this->checks[i] = 0;
	}
	num_checks = 0;
	if (this->white_to_move)
	{
		int king_pos = this->white_king_pos;

		// horizontal left -- 1
		bool comrad_on_ray = false;
		bool exists_pin = false;
		int pin_index = -1;
		bool exists_check = false;
		for (int i = king_pos - 1; i >= 0 && i % 8 != 7; i--)
		{
			if (comrad_on_ray && this->position[i]->is_white())
			{
				break;
			}
			else if (comrad_on_ray && this->position[i]->is_black())
			{
				if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 6)
				{
					exists_pin = true;
					pin_index = i;
					break;
				}
				break;
			}
			else if ((!comrad_on_ray) && this->position[i]->is_black())
			{
				if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 6)
				{
					exists_check = true;
					num_checks++;
					pin_index = i;
					unsigned int set_dir = 1;
					if (check_direction[0] != 0)
					{
						check_direction[1] = set_dir;
					}
					else
					{
						check_direction[0] = set_dir;
					}
					break;
				}
				break;
			}
			else if (this->position[i]->is_white())
			{
				comrad_on_ray = true;
			}
		}
		if (exists_pin)
		{
			for (int i = pin_index; i % 8 < 7 && this->position[i]->get_type() != 1; i++)
			{
				this->pins[i] = 1;
			}
		}
		else if (exists_check)
		{
			for (int i = pin_index; i % 8 < 7 && this->position[i]->get_type() != 1; i++)
			{
				this->checks[i] = true;
			}
		}
		// horizontal right -- 2
		comrad_on_ray = false;
		exists_pin = false;
		exists_check = false;
		pin_index = -1;
		for (int i = king_pos + 1; i % 8 != 0; i++)
		{
			if (comrad_on_ray && this->position[i]->is_white())
			{
				break;
			}
			else if (comrad_on_ray && this->position[i]->is_black())
			{
				if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 6)
				{
					exists_pin = true;
					pin_index = i;
					break;
				}
				break;
			}
			else if ((!comrad_on_ray) && this->position[i]->is_black())
			{
				if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 6)
				{
					exists_check = true;
					num_checks++;
					pin_index = i;
					unsigned int set_dir = 2;
					if (check_direction[0] != 0)
					{
						check_direction[1] = set_dir;
					}
					else
					{
						check_direction[0] = set_dir;
					}
					break;
				}
				break;
			}
			else if (this->position[i]->is_white())
			{
				comrad_on_ray = true;
			}
		}
		if (exists_pin)
		{
			for (int i = pin_index; i > 0 && this->position[i]->get_type() != 1; i--)
			{
				this->pins[i] = 2;
			}
		}
		else if (exists_check)
		{
			for (int i = pin_index; i > 0 && this->position[i]->get_type() != 1; i--)
			{
				this->checks[i] = true;
			}
		}
		// vertical down -- 3
		comrad_on_ray = false;
		exists_pin = false;
		exists_check = false;
		pin_index = -1;
		for (int i = king_pos - 8; i >= 0; i -= 8)
		{
			if (comrad_on_ray && this->position[i]->is_white())
			{
				break;
			}
			else if (comrad_on_ray && this->position[i]->is_black())
			{
				if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 6)
				{
					exists_pin = true;
					pin_index = i;
					break;
				}
				break;
			}
			else if ((!comrad_on_ray) && this->position[i]->is_black())
			{
				if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 6)
				{
					exists_check = true;
					num_checks++;
					pin_index = i;
					unsigned int set_dir = 3;
					if (check_direction[0] != 0)
					{
						check_direction[1] = set_dir;
					}
					else
					{
						check_direction[0] = set_dir;
					}
					break;
				}
				break;
			}
			else if (this->position[i]->is_white())
			{
				comrad_on_ray = true;
			}
		}
		if (exists_pin)
		{
			for (int i = pin_index; i < 64 && this->position[i]->get_type() != 1; i += 8)
			{
				this->pins[i] = 3;
			}
		}
		else if (exists_check)
		{
			for (int i = pin_index; i < 64 && this->position[i]->get_type() != 1; i += 8)
			{
				this->checks[i] = true;
			}
		}
		// vertical up -- 4
		comrad_on_ray = false;
		exists_pin = false;
		exists_check = false;
		pin_index = -1;
		for (int i = king_pos + 8; i < 64; i += 8)
		{
			if (comrad_on_ray && this->position[i]->is_white())
			{
				break;
			}
			else if (comrad_on_ray && this->position[i]->is_black())
			{
				if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 6)
				{
					exists_pin = true;
					pin_index = i;
					break;
				}
				break;
			}
			else if ((!comrad_on_ray) && this->position[i]->is_black())
			{
				if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 6)
				{
					exists_check = true;
					num_checks++;
					pin_index = i;
					unsigned int set_dir = 4;
					if (check_direction[0] != 0)
					{
						check_direction[1] = set_dir;
					}
					else
					{
						check_direction[0] = set_dir;
					}
					break;
				}
				break;
			}
			else if (this->position[i]->is_white())
			{
				comrad_on_ray = true;
			}
		}
		if (exists_pin)
		{
			for (int i = pin_index; i >= 0 && this->position[i]->get_type() != 1; i -= 8)
			{
				this->pins[i] = 4;
			}
		}
		else if (exists_check)
		{
			for (int i = pin_index; i >= 0 && this->position[i]->get_type() != 1; i -= 8)
			{
				this->checks[i] = true;
			}
		}
		// diagonal SW -- 5
		comrad_on_ray = false;
		exists_pin = false;
		exists_check = false;
		pin_index = -1;
		for (int i = king_pos - 9; i % 8 != 7 && i > 0; i -= 9)
		{
			if (comrad_on_ray && this->position[i]->is_white())
			{
				break;
			}
			else if (comrad_on_ray && this->position[i]->is_black())
			{
				if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 8)
				{
					exists_pin = true;
					pin_index = i;
					break;
				}
				break;
			}
			else if ((!comrad_on_ray) && this->position[i]->is_black())
			{
				if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 8)
				{
					exists_check = true;
					num_checks++;
					pin_index = i;
					unsigned int set_dir = 5;
					if (check_direction[0] != 0)
					{
						check_direction[1] = set_dir;
					}
					else
					{
						check_direction[0] = set_dir;
					}
					break;
				}
				break;
			}
			else if (this->position[i]->is_white())
			{
				comrad_on_ray = true;
			}
		}
		if (exists_pin)
		{
			for (int i = pin_index; i % 8 < 7 && i < 64 && this->position[i]->get_type() != 1; i += 9)
			{
				this->pins[i] = 5;
			}
		}
		else if (exists_check)
		{
			for (int i = pin_index; i % 8 < 7 && i < 64 && this->position[i]->get_type() != 1; i += 9)
			{
				this->checks[i] = true;
			}
		}
		// diagonal NW -- 6
		comrad_on_ray = false;
		exists_pin = false;
		exists_check = false;
		pin_index = -1;
		for (int i = king_pos + 7; i % 8 != 7 && i < 64; i += 7)
		{
			if (comrad_on_ray && this->position[i]->is_white())
			{
				break;
			}
			else if (comrad_on_ray && this->position[i]->is_black())
			{
				if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 8)
				{
					exists_pin = true;
					pin_index = i;
					break;
				}
				break;
			}
			else if ((!comrad_on_ray) && this->position[i]->is_black())
			{
				if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 8)
				{
					exists_check = true;
					num_checks++;
					pin_index = i;
					unsigned int set_dir = 6;
					if (check_direction[0] != 0)
					{
						check_direction[1] = set_dir;
					}
					else
					{
						check_direction[0] = set_dir;
					}
					break;
				}
				break;
			}
			else if (this->position[i]->is_white())
			{
				comrad_on_ray = true;
			}
		}
		if (exists_pin)
		{
			for (int i = pin_index; i % 8 < 7 && i > 0 && this->position[i]->get_type() != 1; i -= 7)
			{
				this->pins[i] = 6;
			}
		}
		else if (exists_check)
		{
			for (int i = pin_index; i % 8 < 7 && i > 0 && this->position[i]->get_type() != 1; i -= 7)
			{
				this->checks[i] = true;
			}
		}
		// diagonal NE -- 7
		comrad_on_ray = false;
		exists_pin = false;
		exists_check = false;
		pin_index = -1;
		for (int i = king_pos + 9; i % 8 != 0 && i < 64; i += 9)
		{
			if (comrad_on_ray && this->position[i]->is_white())
			{
				break;
			}
			else if (comrad_on_ray && this->position[i]->is_black())
			{
				if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 8)
				{
					exists_pin = true;
					pin_index = i;
					break;
				}
				break;
			}
			else if ((!comrad_on_ray) && this->position[i]->is_black())
			{
				if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 8)
				{
					exists_check = true;
					num_checks++;
					pin_index = i;
					unsigned int set_dir = 7;
					if (check_direction[0] != 0)
					{
						check_direction[1] = set_dir;
					}
					else
					{
						check_direction[0] = set_dir;
					}
					break;
				}
				break;
			}
			else if (this->position[i]->is_white())
			{
				comrad_on_ray = true;
			}
		}
		if (exists_pin)
		{
			for (int i = pin_index; i % 8 > 0 && i > 0 && this->position[i]->get_type() != 1; i -= 9)
			{
				this->pins[i] = 7;
			}
		}
		else if (exists_check)
		{
			for (int i = pin_index; i % 8 > 0 && i > 0 && this->position[i]->get_type() != 1; i -= 9)
			{
				this->checks[i] = true;
			}
		}
		// diagonal SE -- 8
		comrad_on_ray = false;
		exists_pin = false;
		exists_check = false;
		pin_index = -1;
		for (int i = king_pos - 7; i % 8 != 0 && i > 0; i -= 7)
		{
			if (comrad_on_ray && this->position[i]->is_white())
			{
				break;
			}
			else if (comrad_on_ray && this->position[i]->is_black())
			{
				if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 8)
				{
					exists_pin = true;
					pin_index = i;
					break;
				}
				break;
			}
			else if ((!comrad_on_ray) && this->position[i]->is_black())
			{
				if (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 8)
				{
					exists_check = true;
					num_checks++;
					pin_index = i;
					unsigned int set_dir = 8;
					if (check_direction[0] != 0)
					{
						check_direction[1] = set_dir;
					}
					else
					{
						check_direction[0] = set_dir;
					}
					break;
				}
				break;
			}
			else if (this->position[i]->is_white())
			{
				comrad_on_ray = true;
			}
		}
		if (exists_pin)
		{
			for (int i = pin_index; i % 8 > 0 && i < 64 && this->position[i]->get_type() != 1; i += 7)
			{
				this->pins[i] = 8;
			}
		}
		else if (exists_check)
		{
			for (int i = pin_index; i % 8 > 0 && i < 64 && this->position[i]->get_type() != 1; i += 7)
			{
				this->checks[i] = true;
			}
		}
	}
	else
	{
		int king_pos = this->black_king_pos;

		bool comrad_on_ray = false;
		bool exists_pin = false;
		int pin_index = -1;
		bool exists_check = false;

		for (int i = king_pos - 1; i > 0 && i % 8 != 7; i--)
		{
			if (comrad_on_ray && this->position[i]->is_black())
			{
				break;
			}
			else if (comrad_on_ray && this->position[i]->is_white())
			{
				if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 5)
				{
					exists_pin = true;
					pin_index = i;
					break;
				}
				break;
			}
			else if ((!comrad_on_ray) && this->position[i]->is_white())
			{
				if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 5)
				{
					exists_check = true;
					unsigned int set_dir = 1;
					if (check_direction[0] != 0)
					{
						check_direction[1] = set_dir;
					}
					else
					{
						check_direction[0] = set_dir;
					}
					num_checks++;
					pin_index = i;
					break;
				}
				break;
			}
			else if (this->position[i]->is_black())
			{
				comrad_on_ray = true;
			}
		}
		if (exists_pin)
		{
			for (int i = pin_index; i % 8 < 7 && this->position[i]->get_type() != 2; i++)
			{
				this->pins[i] = 1;
			}
		}
		else if (exists_check)
		{
			for (int i = pin_index; i % 8 < 7 && this->position[i]->get_type() != 2; i++)
			{
				this->checks[i] = true;
			}
		}
		// horizontal right -- 2
		comrad_on_ray = false;
		exists_pin = false;
		exists_check = false;
		pin_index = -1;
		for (int i = king_pos + 1; i % 8 != 0; i++)
		{
			if (comrad_on_ray && this->position[i]->is_black())
			{
				break;
			}
			else if (comrad_on_ray && this->position[i]->is_white())
			{
				if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 5)
				{
					exists_pin = true;
					pin_index = i;
					break;
				}
				break;
			}
			else if ((!comrad_on_ray) && this->position[i]->is_white())
			{
				if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 5)
				{
					exists_check = true;
					unsigned int set_dir = 2;
					if (check_direction[0] != 0)
					{
						check_direction[1] = set_dir;
					}
					else
					{
						check_direction[0] = set_dir;
					}
					num_checks++;
					pin_index = i;
					break;
				}
				break;
			}
			else if (this->position[i]->is_black())
			{
				comrad_on_ray = true;
			}
		}
		if (exists_pin)
		{
			for (int i = pin_index; i > 0 && this->position[i]->get_type() != 2; i--)
			{
				this->pins[i] = 2;
			}
		}
		else if (exists_check)
		{
			for (int i = pin_index; i > 0 && this->position[i]->get_type() != 2; i--)
			{
				this->checks[i] = true;
			}
		}
		// vertical down -- 3
		comrad_on_ray = false;
		exists_pin = false;
		exists_check = false;
		pin_index = -1;
		for (int i = king_pos - 8; i >= 0; i -= 8)
		{
			if (comrad_on_ray && this->position[i]->is_black())
			{
				break;
			}
			else if (comrad_on_ray && this->position[i]->is_white())
			{
				if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 5)
				{
					exists_pin = true;
					pin_index = i;
					break;
				}
				break;
			}
			else if ((!comrad_on_ray) && this->position[i]->is_white())
			{
				if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 5)
				{
					exists_check = true;
					unsigned int set_dir = 3;
					if (check_direction[0] != 0)
					{
						check_direction[1] = set_dir;
					}
					else
					{
						check_direction[0] = set_dir;
					}
					num_checks++;
					pin_index = i;
					break;
				}
				break;
			}
			else if (this->position[i]->is_black())
			{
				comrad_on_ray = true;
			}
		}
		if (exists_pin)
		{
			for (int i = pin_index; i < 64 && this->position[i]->get_type() != 2; i += 8)
			{
				this->pins[i] = 3;
			}
		}
		else if (exists_check)
		{
			for (int i = pin_index; i < 64 && this->position[i]->get_type() != 2; i += 8)
			{
				this->checks[i] = true;
			}
		}
		// vertical up -- 4
		comrad_on_ray = false;
		exists_pin = false;
		exists_check = false;
		pin_index = -1;
		for (int i = king_pos + 8; i < 64; i += 8)
		{
			if (comrad_on_ray && this->position[i]->is_black())
			{
				break;
			}
			else if (comrad_on_ray && this->position[i]->is_white())
			{
				if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 5)
				{
					exists_pin = true;
					pin_index = i;
					break;
				}
				break;
			}
			else if ((!comrad_on_ray) && this->position[i]->is_white())
			{
				if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 5)
				{
					exists_check = true;
					unsigned int set_dir = 4;
					if (check_direction[0] != 0)
					{
						check_direction[1] = set_dir;
					}
					else
					{
						check_direction[0] = set_dir;
					}
					num_checks++;
					pin_index = i;
					break;
				}
				break;
			}
			else if (this->position[i]->is_black())
			{
				comrad_on_ray = true;
			}
		}
		if (exists_pin)
		{
			for (int i = pin_index; i >= 0 && this->position[i]->get_type() != 2; i -= 8)
			{
				this->pins[i] = 4;
			}
		}
		else if (exists_check)
		{
			for (int i = pin_index; i >= 0 && this->position[i]->get_type() != 2; i -= 8)
			{
				this->checks[i] = true;
			}
		}
		// diagonal SW -- 5
		comrad_on_ray = false;
		exists_pin = false;
		exists_check = false;
		pin_index = -1;
		for (int i = king_pos - 9; i % 8 != 7 && i > 0; i -= 9)
		{
			if (comrad_on_ray && this->position[i]->is_black())
			{
				break;
			}
			else if (comrad_on_ray && this->position[i]->is_white())
			{
				if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 7)
				{
					exists_pin = true;
					pin_index = i;
					break;
				}
				break;
			}
			else if ((!comrad_on_ray) && this->position[i]->is_white())
			{
				if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 7)
				{
					exists_check = true;
					unsigned int set_dir = 5;
					if (check_direction[0] != 0)
					{
						check_direction[1] = set_dir;
					}
					else
					{
						check_direction[0] = set_dir;
					}
					num_checks++;
					pin_index = i;
					break;
				}
				break;
			}
			else if (this->position[i]->is_black())
			{
				comrad_on_ray = true;
			}
		}
		if (exists_pin)
		{
			for (int i = pin_index; i % 8 < 7 && i < 64 && this->position[i]->get_type() != 2; i += 9)
			{
				this->pins[i] = 5;
			}
		}
		else if (exists_check)
		{
			for (int i = pin_index; i % 8 < 7 && i < 64 && this->position[i]->get_type() != 2; i += 9)
			{
				this->checks[i] = true;
			}
		}
		// diagonal NW -- 6
		comrad_on_ray = false;
		exists_pin = false;
		exists_check = false;
		pin_index = -1;
		for (int i = king_pos + 7; i % 8 != 7 && i < 64; i += 7)
		{
			if (comrad_on_ray && this->position[i]->is_black())
			{
				break;
			}
			else if (comrad_on_ray && this->position[i]->is_white())
			{
				if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 7)
				{
					exists_pin = true;
					pin_index = i;
					break;
				}
				break;
			}
			else if ((!comrad_on_ray) && this->position[i]->is_white())
			{
				if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 7)
				{
					exists_check = true;
					unsigned int set_dir = 6;
					if (check_direction[0] != 0)
					{
						check_direction[1] = set_dir;
					}
					else
					{
						check_direction[0] = set_dir;
					}
					num_checks++;
					pin_index = i;
					break;
				}
				break;
			}
			else if (this->position[i]->is_black())
			{
				comrad_on_ray = true;
			}
		}
		if (exists_pin)
		{
			for (int i = pin_index; i % 8 < 7 && i > 0 && this->position[i]->get_type() != 2; i -= 7)
			{
				this->pins[i] = 6;
			}
		}
		else if (exists_check)
		{
			for (int i = pin_index; i % 8 < 7 && i > 0 && this->position[i]->get_type() != 2; i -= 7)
			{
				this->checks[i] = true;
			}
		}
		// diagonal NE -- 7
		comrad_on_ray = false;
		exists_pin = false;
		exists_check = false;
		pin_index = -1;
		for (int i = king_pos + 9; i % 8 != 0 && i < 64; i += 9)
		{
			if (comrad_on_ray && this->position[i]->is_black())
			{
				break;
			}
			else if (comrad_on_ray && this->position[i]->is_white())
			{
				if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 7)
				{
					exists_pin = true;
					pin_index = i;
					break;
				}
				break;
			}
			else if ((!comrad_on_ray) && this->position[i]->is_white())
			{
				if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 7)
				{
					exists_check = true;
					unsigned int set_dir = 7;
					if (check_direction[0] != 0)
					{
						check_direction[1] = set_dir;
					}
					else
					{
						check_direction[0] = set_dir;
					}
					num_checks++;
					pin_index = i;
					break;
				}
				break;
			}
			else if (this->position[i]->is_black())
			{
				comrad_on_ray = true;
			}
		}
		if (exists_pin)
		{
			for (int i = pin_index; i % 8 > 0 && i > 0 && this->position[i]->get_type() != 2; i -= 9)
			{
				this->pins[i] = 7;
			}
		}
		else if (exists_check)
		{
			for (int i = pin_index; i % 8 > 0 && i > 0 && this->position[i]->get_type() != 2; i -= 9)
			{
				this->checks[i] = true;
			}
		}
		// diagonal SE -- 8
		comrad_on_ray = false;
		exists_pin = false;
		exists_check = false;
		pin_index = -1;
		for (int i = king_pos - 7; i % 8 != 0 && i > 0; i -= 7)
		{
			if (comrad_on_ray && this->position[i]->is_black())
			{
				break;
			}
			else if (comrad_on_ray && this->position[i]->is_white())
			{
				if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 7)
				{
					exists_pin = true;
					pin_index = i;
					break;
				}
				break;
			}
			else if ((!comrad_on_ray) && this->position[i]->is_white())
			{
				if (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 7)
				{
					exists_check = true;
					unsigned int set_dir = 8;
					if (check_direction[0] != 0)
					{
						check_direction[1] = set_dir;
					}
					else
					{
						check_direction[0] = set_dir;
					}
					num_checks++;
					pin_index = i;
					break;
				}
				break;
			}
			else if (this->position[i]->is_black())
			{
				comrad_on_ray = true;
			}
		}
		if (exists_pin)
		{
			for (int i = pin_index; i % 8 > 0 && i < 64 && this->position[i]->get_type() != 2; i += 7)
			{
				this->pins[i] = 8;
			}
		}
		else if (exists_check)
		{
			for (int i = pin_index; i % 8 > 0 && i < 64 && this->position[i]->get_type() != 2; i += 7)
			{
				this->checks[i] = true;
			}
		}
	}
}


std::vector<Move*>* Board::get_legal_captures()
{
	std::vector<Move*>* moves = this->possible_moves();
	auto iterator = std::remove_if(moves->begin(), moves->end(), [](const Move* m)
		{
			bool is_not_capture = !m->is_capture;
			if (is_not_capture)
			{
				delete m;
			}
			return is_not_capture;
		});
	moves->erase(iterator, moves->end());
	return moves;
}



/**
 * @brief generates pawn moves for a piece at a specific position
 *
 * @param moves list of moves to append to
 * @param i index of piece to move in position array
 */
void Board::add_pawn_moves(std::vector<Move*>* moves, int i)
{
	if (this->position[i]->is_white()) // white pawn moves
	{
		if (i < 48 && (!this->position[i + 8]->is_black()) && (!this->position[i + 8]->is_white()) && (this->pins[i] == 0 || this->pins[i] == 3 || this->pins[i] == 4) && ((this->num_checks == 1 && this->checks[i + 8]) || this->num_checks == 0))
		{
			move_with_offset(moves, i, i + 8);
		}
		if (i < 16 && (this->position[i + 16]->is_empty()) && (this->position[i + 8]->is_empty()) && (this->pins[i] == 0 || this->pins[i] == 3 || this->pins[i] == 4) && ((this->num_checks == 1 && this->checks[i + 16]) || this->num_checks == 0))
		{
			move_with_offset(moves, i, i + 16);
		}
		// captures
		//std::cout << this->pins[i] << std::endl;
		if (i % 8 > 0 && i < 48 && this->position[i + 7]->is_black() && (this->pins[i] == 0 || this->pins[i] == 8 || this->pins[i] == 6) && ((this->num_checks == 1 && this->checks[i + 7]) || this->num_checks == 0))
		{
			move_with_offset(moves, i, i + 7);
		}

		if (i % 8 < 7 && i < 47 && this->position[i + 9]->is_black() && ((this->pins[i] == 0 || this->pins[i] == 5 || this->pins[i] == 7)) && ((this->num_checks == 1 && this->checks[i + 9]) || this->num_checks == 0))
		{
			move_with_offset(moves, i, i + 9);
		}

		// promotions
		// basic promotion
		if (i >= 48 && i < 56 && (this->position[i + 8]->is_empty()) && (this->pins[i] == 0 || this->pins[i] == 3 || this->pins[i] == 4) && ((this->num_checks == 1 && this->checks[i + 8]) || this->num_checks == 0))
		{
			promote_with_offset(moves, i, i + 8, 3);
			promote_with_offset(moves, i, i + 8, 5);
			promote_with_offset(moves, i, i + 8, 7);
			promote_with_offset(moves, i, i + 8, 9);
		}
		// capture promotions
		if (i > 48 && i < 56 && i % 8 != 0 && (this->position[i + 7]->is_black()) && (this->pins[i] == 0 || this->pins[i] == 6 || this->pins[i] == 8) && ((this->num_checks == 1 && this->checks[i + 7]) || this->num_checks == 0))
		{
			promote_with_offset(moves, i, i + 7, 3);
			promote_with_offset(moves, i, i + 7, 5);
			promote_with_offset(moves, i, i + 7, 7);
			promote_with_offset(moves, i, i + 7, 9);
		}
		if (i >= 48 && i < 55 && i % 8 != 7 && (this->position[i + 9]->is_black()) && (this->pins[i] == 0 || this->pins[i] == 5 || this->pins[i] == 7) && ((this->num_checks == 1 && this->checks[i + 9]) || this->num_checks == 0))
		{
			promote_with_offset(moves, i, i + 9, 3);
			promote_with_offset(moves, i, i + 9, 5);
			promote_with_offset(moves, i, i + 9, 7);
			promote_with_offset(moves, i, i + 9, 9);
		}

		// en-passant to the left
		if (i > 32 && i < 40 && en_passant_target_index == i + 7 && (this->pins[i] == 0 || this->pins[i] == 6 || this->pins[i] == 8) && ((this->num_checks == 1 && (this->checks[i + 7] || this->checks[this->en_passant_target_index - 8])) || this->num_checks == 0))
		{
			move_en_passant(moves, i, i + 7);
		}
		// en-passant to the right
		if (i >= 32 && i < 39 && en_passant_target_index == i + 9 && (this->pins[i] == 0 || this->pins[i] == 5 || this->pins[i] == 7) && ((this->num_checks == 1 && (this->checks[i + 9] || this->checks[this->en_passant_target_index - 8])) || this->num_checks == 0))
		{
			move_en_passant(moves, i, i + 9);
		}
	}
	else // black pawn moves
	{
		if (i > 15 && (this->position[i - 8]->is_empty()) && (this->pins[i] == 0 || this->pins[i] == 3 || this->pins[i] == 4) && ((this->num_checks == 1 && this->checks[i - 8]) || this->num_checks == 0))
		{
			move_with_offset(moves, i, i - 8);
		}
		if (i >= 48 && (this->position[i - 16]->is_empty()) && (this->position[i - 8]->is_empty()) && (this->pins[i] == 0 || this->pins[i] == 3 || this->pins[i] == 4) && ((this->num_checks == 1 && this->checks[i - 16]) || this->num_checks == 0))
		{
			move_with_offset(moves, i, i - 16);
		}

		// captures
		if (i % 8 > 0 && i > 16 && this->position[i - 9]->is_white() && (this->pins[i] == 0 || this->pins[i] == 5 || this->pins[i] == 7) && ((this->num_checks == 1 && this->checks[i - 9]) || this->num_checks == 0))
		{
			move_with_offset(moves, i, i - 9);
		}

		if (i % 8 < 7 && i >= 16 && this->position[i - 7]->is_white() && (this->pins[i] == 0 || this->pins[i] == 6 || this->pins[i] == 8) && ((this->num_checks == 1 && this->checks[i - 7]) || this->num_checks == 0))
		{
			move_with_offset(moves, i, i - 7);
		}
		// promotions
		// basic promotion
		if (i < 16 && i >= 8 && this->position[i - 8]->is_empty() && (this->pins[i] == 0 || this->pins[i] == 3 || this->pins[i] == 4) && ((this->num_checks == 1 && this->checks[i - 8]) || this->num_checks == 0))
		{
			//std::cout << i - 8 << std::endl;
			promote_with_offset(moves, i, i - 8, 4);
			promote_with_offset(moves, i, i - 8, 6);
			promote_with_offset(moves, i, i - 8, 8);
			promote_with_offset(moves, i, i - 8, 10);
		}
		// capture promotions
		if (i < 15 && i >= 8 && this->position[i - 7]->is_white() && (this->pins[i] == 0 || this->pins[i] == 8 || this->pins[i] == 6) && ((this->num_checks == 1 && this->checks[i - 7]) || this->num_checks == 0))
		{
			promote_with_offset(moves, i, i - 7, 4);
			promote_with_offset(moves, i, i - 7, 6);
			promote_with_offset(moves, i, i - 7, 8);
			promote_with_offset(moves, i, i - 7, 10);
		}
		if (i < 16 && i > 8 && this->position[i - 9]->is_white() && (this->pins[i] == 0 || this->pins[i] == 5 || this->pins[i] == 7) && ((this->num_checks == 1 && this->checks[i - 9]) || this->num_checks == 0))
		{
			promote_with_offset(moves, i, i - 9, 4);
			promote_with_offset(moves, i, i - 9, 6);
			promote_with_offset(moves, i, i - 9, 8);
			promote_with_offset(moves, i, i - 9, 10);
		}

		// en-passant to the left
		if (i > 24 && i < 32 && en_passant_target_index == i - 9 && (this->pins[i] == 0 || this->pins[i] == 5 || this->pins[i] == 7) && ((this->num_checks == 1 && (this->checks[i - 9] || this->checks[this->en_passant_target_index + 8])) || this->num_checks == 0))
		{
			// std::cout << "here" << std::endl;
			move_en_passant(moves, i, i - 9);
		}
		// en-passant to the right
		if (i >= 24 && i < 31 && en_passant_target_index == i - 7 && (this->pins[i] == 0 || this->pins[i] == 6 || this->pins[i] == 8) && ((this->num_checks == 1 && (this->checks[i - 7] || this->checks[this->en_passant_target_index + 8])) || this->num_checks == 0))
		{
			move_en_passant(moves, i, i - 7);
		}
	}
}

bool Board::en_passant_illegal()
{
	if (this->white_to_move)
	{
		if (this->white_king_pos / 8 != 4)
		{
			return false;
		}
		else if (this->white_king_pos % 8 < this->en_passant_target_index % 8)
		{
			// king to the left ef ep target square
			int num_pieces_passed = 0;
			int i = this->white_king_pos + 1;
			for (; num_pieces_passed < 3 && i % 8 != 0; i++)
			{
				if (!this->position[i]->is_empty())
				{
					num_pieces_passed++;
				}
				if (num_pieces_passed == 3)
				{
					break;
				}
			}
			if (i % 8 > this->en_passant_target_index % 8 && i % 8 != 0 && (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 6))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			int num_pieces_passed = 0;
			int i = this->white_king_pos - 1;
			for (; num_pieces_passed < 3 && i % 8 != 7; i--)
			{
				if (!this->position[i]->is_empty())
				{
					num_pieces_passed++;
				}

				if (num_pieces_passed == 3)
				{
					break;
				}
			}
			if (i % 8 < this->en_passant_target_index % 8 && i % 8 != 7 && (this->position[i]->get_type() == 4 || this->position[i]->get_type() == 6))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		if (this->black_king_pos / 8 != 3)
		{
			return false;
		}
		else if (this->black_king_pos % 8 < this->en_passant_target_index % 8)
		{
			// king to the left ef ep target square
			int num_pieces_passed = 0;
			int i = this->black_king_pos + 1;
			for (; num_pieces_passed < 3 && i % 8 != 0; i++)
			{
				if (!this->position[i]->is_empty())
				{
					num_pieces_passed++;
				}

				if (num_pieces_passed == 3)
				{
					break;
				}
			}
			if (i % 8 > this->en_passant_target_index % 8 && i % 8 != 0 && (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 5))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			int num_pieces_passed = 0;
			int i = this->black_king_pos - 1;
			for (; num_pieces_passed < 3 && i % 8 != 7; i--)
			{
				if (!this->position[i]->is_empty())
				{
					num_pieces_passed++;
				}

				if (num_pieces_passed == 3)
				{
					break;
				}
			}
			if (i % 8 < this->en_passant_target_index % 8 && i % 8 != 7 && (this->position[i]->get_type() == 3 || this->position[i]->get_type() == 5))
			{
				return true;
			}
			else
			{
				//std::cout << i << std::endl;
				//std::cout << num_pieces_passed << std::endl;
				return false;
			}
		}
	}
	return false;
}

void Board::move_en_passant(std::vector<Move*>* moves, int i, int j)
{
	if (this->en_passant_illegal())
	{
		// std::cout << "en-passant is illegal here" << std::endl;
		return;
	}

	Move* move = new Move(i, j, true, false, 0);
	moves->push_back(move);
}

/**
 * @brief adds possible knight moves to a moves list
 *
 * @param moves moves list
 * @param i position of knight
 */
void Board::add_knight_moves(std::vector<Move*>* moves, int i)
{
	if (this->pins[i] != 0)
	{
		return;
	}
	if (i % 8 < 6 && i + 10 < 64 && !this->position[i + 10]->is_team_member(this->position[i]) && ((this->num_checks == 0) || (this->num_checks == 1 && (this->checks[i + 10]))))
	{
		move_with_offset(moves, i, i + 10);
	}
	if (i % 8 > 1 && i + 6 < 64 && !this->position[i + 6]->is_team_member(this->position[i]) && ((this->num_checks == 0) || (this->num_checks == 1 && (this->checks[i + 6]))))
	{
		move_with_offset(moves, i, i + 6);
	}
	if (i % 8 != 7 && i + 17 < 64 && !this->position[i + 17]->is_team_member(this->position[i]) && ((this->num_checks == 0) || (this->num_checks == 1 && (this->checks[i + 17]))))
	{
		move_with_offset(moves, i, i + 17);
	}
	if (i % 8 != 0 && i + 15 < 64 && !this->position[i + 15]->is_team_member(this->position[i]) && ((this->num_checks == 0) || (this->num_checks == 1 && (this->checks[i + 15]))))
	{
		move_with_offset(moves, i, i + 15);
	}
	// move down
	if (i % 8 > 1 && i - 10 >= 0 && !this->position[i - 10]->is_team_member(this->position[i]) && ((this->num_checks == 0) || (this->num_checks == 1 && (this->checks[i - 10]))))
	{
		move_with_offset(moves, i, i - 10);
	}
	if (i % 8 < 6 && i - 6 >= 0 && !(this->position[i - 6]->is_team_member(this->position[i])) && ((this->num_checks == 0) || (this->num_checks == 1 && (this->checks[i - 6]))))
	{
		move_with_offset(moves, i, i - 6);
	}
	if (i % 8 != 7 && i - 15 >= 0 && !this->position[i - 15]->is_team_member(this->position[i]) && ((this->num_checks == 0) || (this->num_checks == 1 && (this->checks[i - 15]))))
	{
		move_with_offset(moves, i, i - 15);
	}
	if (i % 8 != 0 && i - 17 >= 0 && !this->position[i - 17]->is_team_member(this->position[i]) && ((this->num_checks == 0) || (this->num_checks == 1 && (this->checks[i - 17]))))
	{
		move_with_offset(moves, i, i - 17);
	}
}
/**
 * @brief adds possible bishop moves to a moves list
 *
 * @param moves moves list
 * @param i position of bishop
 */
void Board::add_bishop_moves(std::vector<Move*>* moves, int i)
{
	if (this->pins[i] == 0 || this->pins[i] == 5 || this->pins[i] == 7)
	{
		// diagonally right up <=> NE
		for (int j = i + 9; j < 64 && i % 8 != 7 && j % 8 != 0 && i < 56; j += 9)
		{
			if (this->position[j]->is_empty())
			{
				if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
				{
					move_with_offset(moves, i, j);
				}
			}
			else if (!this->position[j]->is_team_member(this->position[i]))
			{
				if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
				{
					move_with_offset(moves, i, j);
				}
				break;
			}
			else if (this->position[j]->is_team_member(this->position[i]))
			{
				break;
			}
		}
	}
	if (this->pins[i] == 0 || this->pins[i] == 6 || this->pins[i] == 8)
	{
		// diagonally left up
		for (int j = i + 7; j < 64 && i % 8 != 0 && j % 8 != 7 && i < 56; j += 7)
		{
			if (this->position[j]->is_empty())
			{
				if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
				{
					move_with_offset(moves, i, j);
				}
			}
			else if (!this->position[j]->is_team_member(this->position[i]))
			{
				if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
				{
					move_with_offset(moves, i, j);
				}
				break;
			}
			else if (this->position[j]->is_team_member(this->position[i]))
			{
				break;
			}
		}
		// diagonally right down
		for (int j = i - 7; j >= 0 && i % 8 != 7 && j % 8 != 0 && i >= 8; j -= 7)
		{
			if (this->position[j]->is_empty())
			{
				if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
				{
					move_with_offset(moves, i, j);
				}
			}
			else if (!this->position[j]->is_team_member(this->position[i]))
			{
				if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
				{
					move_with_offset(moves, i, j);
				}
				break;
			}
			else if (this->position[j]->is_team_member(this->position[i]))
			{
				break;
			}
		}
	}
	// diagonally left down
	if (this->pins[i] == 0 || this->pins[i] == 5 || this->pins[i] == 7)
	{
		for (int j = i - 9; j >= 0 && i % 8 != 0 && j % 8 != 7 && i > 8; j -= 9)
		{
			if (this->position[j]->is_empty())
			{
				if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
				{
					move_with_offset(moves, i, j);
				}
			}
			else if (!this->position[j]->is_team_member(this->position[i]))
			{
				if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
				{
					move_with_offset(moves, i, j);
				}
				break;
			}
			else if (this->position[j]->is_team_member(this->position[i]))
			{
				break;
			}
		}
	}
}

/**
 * @brief adds possible rook moves to a moves list
 *
 * @param moves moves list
 * @param i position of rook
 */
void Board::add_rook_moves(std::vector<Move*>* moves, int i)
{
	if (this->pins[i] < 3)
	{
		for (int j = i + 1; j % 8 != 0 && i % 8 != 7 && j <= 63; j++)
		{
			if (this->position[j]->is_empty())
			{
				if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
				{
					move_with_offset(moves, i, j);
				}
			}
			else if (!this->position[j]->is_team_member(this->position[i]))
			{
				if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
				{
					move_with_offset(moves, i, j);
				}
				break;
			}
			else if (this->position[j]->is_team_member(this->position[i]))
			{
				break;
			}
		}
		for (int j = i - 1; j % 8 != 7 && i % 8 != 0 && j >= 0; j--)
		{
			if (this->position[j]->is_empty())
			{
				if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
				{
					move_with_offset(moves, i, j);
				}
			}
			else if (!this->position[j]->is_team_member(this->position[i]))
			{
				if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
				{
					move_with_offset(moves, i, j);
				}
				break;
			}
			else if (this->position[j]->is_team_member(this->position[i]))
			{
				break;
			}
		}
	}
	if (this->pins[i] == 0 || (this->pins[i] > 2 && this->pins[i] < 5))
	{
		for (int j = i + 8; j < 64; j += 8)
		{
			if (this->position[j]->is_empty())
			{
				if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
				{
					move_with_offset(moves, i, j);
				}
			}
			else if (!this->position[j]->is_team_member(this->position[i]))
			{
				if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
				{
					move_with_offset(moves, i, j);
				}
				break;
			}
			else if (this->position[j]->is_team_member(this->position[i]))
			{
				break;
			}
		}
		for (int j = i - 8; j >= 0; j -= 8)
		{
			if (this->position[j]->is_empty())
			{
				if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
				{
					move_with_offset(moves, i, j);
				}
			}
			else if (!this->position[j]->is_team_member(this->position[i]))
			{
				if ((this->num_checks == 1 && this->checks[j]) || this->num_checks == 0)
				{
					move_with_offset(moves, i, j);
				}
				break;
			}
			else if (this->position[j]->is_team_member(this->position[i]))
			{
				break;
			}
		}
	}
}
/**
 * @brief adds possible queen moves to a moves list
 *
 * @param moves moves list
 * @param i position of queen
 */
void Board::add_queen_moves(std::vector<Move*>* moves, int i)
{
	add_rook_moves(moves, i);
	add_bishop_moves(moves, i);
}
/**
 * @brief adds possible king moves to a moves list
 *
 * @param moves moves list
 * @param i position of king
 */
void Board::add_king_moves(std::vector<Move*>* moves, int i)
{
	if ((i % 8 != 0) && (!(this->position[i - 1]->is_team_member(this->position[i]))))
	{
		if (this->num_checks == 0 || (this->check_direction[0] != 1 && this->check_direction[0] != 2 && this->check_direction[1] != 1 && this->check_direction[1] != 2) || (this->check_direction[0] == 1 && (!this->position[i - 1]->is_team_member(this->position[i]))))
		{
			move_with_offset(moves, i, i - 1);
		}
	}
	// move right
	if ((i % 8 != 7) && (!(this->position[i + 1]->is_team_member(this->position[i]))))
	{
		if (this->num_checks == 0 || (this->check_direction[0] != 1 && this->check_direction[0] != 2 && this->check_direction[1] != 1 && this->check_direction[1] != 2) || (this->check_direction[0] == 2 && (!this->position[i + 1]->is_team_member(this->position[i]))))
		{
			move_with_offset(moves, i, i + 1);
		}
	}
	// move down
	if ((i > 7) && (!(this->position[i - 8]->is_team_member(this->position[i]))))
	{
		if (this->num_checks == 0 || (this->check_direction[0] != 3 && this->check_direction[0] != 4 && this->check_direction[1] != 3 && this->check_direction[1] != 4) || (this->check_direction[0] == 3 && (!this->position[i - 8]->is_team_member(this->position[i]))))
		{
			move_with_offset(moves, i, i - 8);
		}
	}
	// move up
	if ((i < 56) && (!(this->position[i + 8]->is_team_member(this->position[i]))))
	{
		if (this->num_checks == 0 || (this->check_direction[0] != 3 && this->check_direction[0] != 4 && this->check_direction[1] != 3 && this->check_direction[1] != 4) || (this->check_direction[0] == 4 && (!this->position[i + 8]->is_team_member(this->position[i]))))
		{
			move_with_offset(moves, i, i + 8);
		}
	}
	if ((i % 8 != 0) && (i > 7) && (!(this->position[i - 9]->is_team_member(this->position[i]))))
	{
		if (this->num_checks == 0 || (this->check_direction[0] != 5 && this->check_direction[0] != 7 && this->check_direction[1] != 5 && this->check_direction[1] != 7) || (this->check_direction[0] == 5 && (!this->position[i - 9]->is_team_member(this->position[i]))))
		{
			move_with_offset(moves, i, i - 9);
		}
	}
	if ((i % 8 != 0) && (i < 56) && (!(this->position[i + 7]->is_team_member(this->position[i]))))
	{
		if (this->num_checks == 0 || (this->check_direction[0] != 6 && this->check_direction[0] != 8 && this->check_direction[1] != 6 && this->check_direction[1] != 8) || (this->check_direction[0] == 6 && (!this->position[i + 7]->is_team_member(this->position[i]))))
		{
			move_with_offset(moves, i, i + 7);
		}
	}
	if ((i % 8 != 7) && (i > 7) && (!(this->position[i - 7]->is_team_member(this->position[i]))))
	{
		// std::cout << "here " + std::to_string(this->check_direction[0]) << std::endl;
		if (this->num_checks == 0 || (this->check_direction[0] != 6 && this->check_direction[0] != 8 && this->check_direction[1] != 6 && this->check_direction[1] != 8) || (this->check_direction[0] == 8 && (!this->position[i - 7]->is_team_member(this->position[i]))))
		{
			//std::cout << "here " + std::to_string(this->check_direction[0]) << std::endl;
			move_with_offset(moves, i, i - 7);
		}
	}
	//std::cout << std::to_string(this->check_direction[0]) << std::endl;
	if ((i % 8 != 7) && (i < 56) && (!(this->position[i + 9]->is_team_member(this->position[i]))))
	{
		if (this->num_checks == 0 || (this->check_direction[0] != 5 && this->check_direction[0] != 7 && this->check_direction[1] != 5 && this->check_direction[1] != 7) || (this->check_direction[0] == 7 && (!this->position[i + 9]->is_team_member(this->position[i]))))
		{
			move_with_offset(moves, i, i + 9);
		}
	}
	if (white_to_move && i == 4)
	{
		if (this->castling_rights[0])
		{
			if ((!this->attacked[4]) && (!this->attacked[5]) && (!this->attacked[6]) && this->position[5]->is_empty() && this->position[6]->is_empty())
			{
				Move* move = new Move(4, 6, false, false, 0);
				moves->push_back(move);
			}
		}
		if (this->castling_rights[1])
		{
			if ((!this->attacked[4]) && (!this->attacked[3]) && (!this->attacked[2]) && this->position[3]->is_empty() && this->position[2]->is_empty() && this->position[1]->is_empty())
			{
				Move* move = new Move(4, 2, false, false, 0);
				moves->push_back(move);
			}
		}
	}
	else if ((!white_to_move) && i == 60)
	{
		if (this->castling_rights[2])
		{
			if ((!this->attacked[60]) && (!this->attacked[61]) && (!this->attacked[62]) && this->position[61]->is_empty() && this->position[62]->is_empty())
			{
				Move* move = new Move(60, 62, false, false, 0);
				moves->push_back(move);
			}
		}
		if (this->castling_rights[3])
		{
			if ((!this->attacked[60]) && (!this->attacked[59]) && (!this->attacked[58]) && this->position[59]->is_empty() && this->position[58]->is_empty() && this->position[57]->is_empty())
			{
				Move* move = new Move(60, 58, false, false, 0);
				moves->push_back(move);
			}
		}
	}
}

/**
 * @brief moves a piece from a position to another using an integer offset
 *
 * @param moves list of moves to append to
 * @param i current piece location (moves from)
 * @param j intended piece location (moves to)
 */
void Board::move_with_offset(std::vector<Move*>* moves, int i, int j)
{
	if (this->position[i]->get_type() == 1 && this->attacked[j]) {
		return;
	}
	else if (this->position[i]->get_type() == 2 && this->attacked[j]) {
		return;
	}
	if (this->position[j]->is_empty()) {
		Move* move = new Move(i, j, false, false, 0);
		moves->push_back(move);
	}
	else {
		Move* move = new Move(i, j, true, false, 0);
		moves->push_back(move);
	}
}

void Board::promote_with_offset(std::vector<Move*>* moves, int i, int j, unsigned promote_to)
{
	if (this->position[j]->is_empty()) {
		Move* move = new Move(i, j, false, true, promote_to);
		moves->push_back(move);
	}
	else {
		Move* move = new Move(i, j, true, true, promote_to);
		moves->push_back(move);
	}
}

/**
 * @brief changes the side to move
 *
 */
void Board::switch_move()
{
	this->white_to_move = !(this->white_to_move);
}

/**
 * @brief stores the move with which the position was reached as a string
 *
 * @param set_move
 */
void Board::set_last_move(std::string set_move)
{
	this->last_move = set_move;
}
/**
 * @brief generates a deep copy of the board
 *
 * @return Board*
 */
Board* Board::clone()
{
	Piece** clone_position = new Piece * [64];
	for (int i = 0; i < 64; i++)
	{
		clone_position[i] = this->position[i]->clone();
	}

	bool* copy_castling_rights = new bool[4];
	for (int i = 0; i < 4; i++)
	{
		copy_castling_rights[i] = this->castling_rights[i];
	}

	Board* b = new Board(clone_position, this->white_to_move, copy_castling_rights);

	std::string buffer = this->get_last_move();
	b->set_last_move(buffer);
	b->white_king_pos = this->white_king_pos;
	b->black_king_pos = this->black_king_pos;
	b->en_passant_target_index = -1;
	return b;
}

std::string Board::pos_as_str()
{
	std::string str;
	if (white_to_move)
	{
		str += "white to move:\n";
	}
	else
	{
		str += "black to move\n";
	}
	for (int i = 0; i < 8; i++)
	{
		str += "   +---";
		for (int j = 1; j < 8; j++)
		{
			str += "+---";
		}
		str += "+\n " + std::to_string(8 - i) + " | ";

		for (int j = 0; j < 8; j++)
		{
			int pos_in_arr = (7 - i) * 8 + j;

			str += this->position[pos_in_arr]->piece_to_char();
			str += " | ";
		}
		str += '\n';
	}
	str += "   ";
	for (int j = 0; j < 8; j++)
	{
		str += "+---";
	}
	str += "+\n";
	str += "     a   b   c   d   e   f   g   h\n";
	return str;
}

/**
 * @brief generates algebraic coordinate for a position index
 *
 * @param i positon index
 * @return std::string
 */
std::string Board::get_coord_str_from_index(int i)
{
	std::string str = "";
	char c = 97 + (i % 8);
	str += c;
	str += std::to_string((i / 8 + 1));
	return str;
}

std::string Board::get_last_move()
{
	return this->last_move;
}

std::string Board::create_move_str(int from, int to)
{
	std::string move = "";
	move += this->get_coord_str_from_index(from);
	move += this->get_coord_str_from_index(to);
	return move;
}

std::string Board::get_attacked_squares()
{
	compute_attacked_squares();
	compute_pin_rays();
	compute_other_checks();
	std::string str = "";
	str += get_attacked();
	str += get_pins();
	str += get_checks();
	return str;
}

std::string Board::get_checks()
{
	this->compute_pin_rays();
	this->compute_other_checks();
	std::string str = "checks:\n";
	for (int i = 0; i < 8; i++)
	{
		str += "   +---";
		for (int j = 1; j < 8; j++)
		{
			str += "+---";
		}
		str += "+\n " + std::to_string(8 - i) + " | ";
		for (int j = 0; j < 8; j++)
		{
			int pos_in_arr = (7 - i) * 8 + j;
			if (this->checks[pos_in_arr])
			{
				str += 'X';
			}
			else
			{
				str += ' ';
			}
			str += " | ";
		}
		str += '\n';
	}
	str += "   ";
	for (int j = 0; j < 8; j++)
	{
		str += "+---";
	}
	str += "+\n";
	str += "     a   b   c   d   e   f   g   h\n\n";
	return str;
}

std::string Board::get_pins()
{
	this->compute_pin_rays();
	std::string str = "pins:\n";
	for (int i = 0; i < 8; i++)
	{
		str += "   +---";
		for (int j = 1; j < 8; j++)
		{
			str += "+---";
		}
		str += "+\n " + std::to_string(8 - i) + " | ";
		for (int j = 0; j < 8; j++)
		{
			int pos_in_arr = (7 - i) * 8 + j;
			if (pins[pos_in_arr] > 0)
			{
				str += std::to_string(pins[pos_in_arr]);
			}
			else
			{
				str += ' ';
			}
			str += " | ";
		}
		str += '\n';
	}
	str += "   ";
	for (int j = 0; j < 8; j++)
	{
		str += "+---";
	}
	str += "+\n";
	str += "     a   b   c   d   e   f   g   h\n\n";
	return str;
}

std::string Board::get_attacked()
{
	this->compute_attacked_squares();
	std::string str = "attacked by white:\n";
	for (int i = 0; i < 8; i++)
	{
		str += "   +---";
		for (int j = 1; j < 8; j++)
		{
			str += "+---";
		}
		str += "+\n " + std::to_string(8 - i) + " | ";
		for (int j = 0; j < 8; j++)
		{
			int pos_in_arr = (7 - i) * 8 + j;
			if (this->attacked[pos_in_arr])
			{
				str += "X";
			}
			else
			{
				str += " ";
			}
			str += " | ";
		}
		str += '\n';
	}
	str += "   ";
	for (int j = 0; j < 8; j++)
	{
		str += "+---";
	}
	str += "+\n";
	str += "     a   b   c   d   e   f   g   h\n\n";
	return str;
}

std::string Board::get_castling_rights() {
	std::string str = "";
	if (this->castling_rights[0]) {
		str += "K";
	}
	if (this->castling_rights[1]) {
		str += "Q";
	}
	if (this->castling_rights[2]) {
		str += "k";
	}
	if (this->castling_rights[3]) {
		str += "q";
	}
	return str;
}



bool Board::equals(Board* b)
{
	if (this->white_to_move != b->white_to_move) {
		return false;
	}

	if (this->en_passant_target_index > 0 && b->en_passant_target_index > 0 && this->en_passant_target_index != b->en_passant_target_index) {
		return false;
	}
	for (int i = 0; i < 4; i++)
	{
		if (this->castling_rights[i] != b->castling_rights[i]) {
			return false;
		}
	}

	for (int i = 0; i < 64; i++) {
		if (this->position[i]->get_type() != b->position[i]->get_type()) {
			return false;
		}
	}

	return true;
}
/**
 * \brief Makes move on the board.
 *
 * \param m Move m
 */
void Board::make_move(Move* m) {
	this->switch_move();
	// unwrapping move m
	int origin = m->origin;
	int target = m->target;
	bool is_capture = m->is_capture;
	bool is_promotion = m->is_promotion;
	unsigned promotion_type = m->promotion_type;

	// managing stack

	this->stack_moves->push_back(m);

	// copying the castling rights
	bool* copy_castling_rights = new bool[4];
	for (int i = 0; i < 4; i++) {
		copy_castling_rights[i] = this->castling_rights[i];
	}
	this->stack_castling_rights->push_back(copy_castling_rights);

	this->stack_en_passant_target_index->push_back(this->en_passant_target_index);
	this->en_passant_target_index = -1;

	this->stack_captures->push_back(this->position[target]->get_type());

	// update king position fields
	if (this->position[origin]->get_type() == 1) {
		this->white_king_pos = target;
		this->castling_rights[0] = false;
		this->castling_rights[1] = false;
	}
	else if (this->position[origin]->get_type() == 2) {
		this->black_king_pos = target;
		this->castling_rights[2] = false;
		this->castling_rights[3] = false;
	}

	// perform castling

	// if is white castling move
	if (this->position[origin]->get_type() == 1 && std::abs(target - origin) == 2) {
		std::list<int>::iterator itr;
		if (target == 6) {
			this->position[6]->set_piece_type(1);
			this->position[4]->set_piece_type(0);
			this->position[5]->set_piece_type(5);
			this->position[7]->set_piece_type(0);
			for (itr = this->rook_list->begin(); itr != this->rook_list->end(); itr++) {
				if (*itr == 7) {
					*itr = 5;
					break;
				}
			}
		}
		else if (target == 2) {
			this->position[2]->set_piece_type(1);
			this->position[4]->set_piece_type(0);
			this->position[3]->set_piece_type(5);
			this->position[0]->set_piece_type(0);
			for (itr = this->rook_list->begin(); itr != this->rook_list->end(); itr++) {
				if (*itr == 0) {
					*itr = 3;
					break;
				}
			}
		}
		this->castling_rights[0] = false;
		this->castling_rights[1] = false;

		return;
	}
	// if is black castling move
	else if (this->position[origin]->get_type() == 2 && std::abs(target - origin) == 2) {
		std::list<int>::iterator itr;
		if (target == 62) {
			this->position[62]->set_piece_type(2);
			this->position[60]->set_piece_type(0);
			this->position[61]->set_piece_type(6);
			this->position[63]->set_piece_type(0);
			for (itr = this->rook_list->begin(); itr != this->rook_list->end(); itr++) {
				if (*itr == 63) {
					*itr = 61;
					break;
				}
			}
		}
		else if (target == 58) {
			this->position[58]->set_piece_type(2);
			this->position[60]->set_piece_type(0);
			this->position[59]->set_piece_type(6);
			this->position[56]->set_piece_type(0);
			for (itr = this->rook_list->begin(); itr != this->rook_list->end(); itr++) {
				if (*itr == 56) {
					*itr = 59;
					break;
				}
			}
		}
		this->castling_rights[2] = false;
		this->castling_rights[3] = false;

		return;
	}
	// performing en passant
	if (this->position[origin]->get_type() == 11 && target == this->stack_en_passant_target_index->back()) {
		this->position[target - 8]->set_piece_type(0);
		Utility::remove_first_occurance(this->pawn_list, target - 8);
	}
	else if (this->position[origin]->get_type() == 12 && target == this->stack_en_passant_target_index->back()) {
		this->position[target + 8]->set_piece_type(0);
		Utility::remove_first_occurance(this->pawn_list, target + 8);
	}
	// make move (special case rook and king moves)
	if ((origin == 0 && this->position[origin]->get_type() == 5) || (target == 0 && this->position[target]->get_type() == 5)) {
		this->castling_rights[1] = false;
	}
	else if ((origin == 7 && this->position[origin]->get_type() == 5) || (target == 7 && this->position[target]->get_type() == 5)) {
		this->castling_rights[0] = false;
	}
	else if ((origin == 56 && this->position[origin]->get_type() == 6) || (target == 56 && this->position[target]->get_type() == 6)) {
		this->castling_rights[3] = false;
	}
	else if ((origin == 63 && this->position[origin]->get_type() == 6) || (target == 63 && this->position[target]->get_type() == 6)) {
		this->castling_rights[2] = false;
	}
	// manage en passant target square
	if ((this->position[origin]->get_type() == 11 || this->position[origin]->get_type() == 12) && std::abs(target - origin) == 16) {
		if (this->position[origin]->is_white()) {
			this->en_passant_target_index = origin + 8;
		}
		else {
			this->en_passant_target_index = origin - 8;
		}
	}

	// make move
	// update piece lists
	std::list<int>::iterator itr;
	if (is_capture) {
		if (this->position[target]->get_type() == 3 || this->position[target]->get_type() == 4) {
			Utility::remove_first_occurance(this->queen_list, target);
		}
		else if (this->position[target]->get_type() == 5 || this->position[target]->get_type() == 6) {
			Utility::remove_first_occurance(this->rook_list, target);
		}
		else if (this->position[target]->get_type() == 7 || this->position[target]->get_type() == 8) {
			Utility::remove_first_occurance(this->bishop_list, target);
		}
		else if (this->position[target]->get_type() == 9 || this->position[target]->get_type() == 10) {
			Utility::remove_first_occurance(this->knight_list, target);
		}
		else if (this->position[target]->get_type() == 11 || this->position[target]->get_type() == 12) {
			Utility::remove_first_occurance(this->pawn_list, target);
		}
	}

	if (this->position[origin]->get_type() == 3 || this->position[origin]->get_type() == 4) {
		for (itr = this->queen_list->begin(); itr != this->queen_list->end(); itr++) {
			if (*itr == origin) {
				*itr = target;
				break;
			}
		}
	}
	else if (this->position[origin]->get_type() == 5 || this->position[origin]->get_type() == 6) {
		for (itr = this->rook_list->begin(); itr != this->rook_list->end(); itr++) {
			if (*itr == origin) {
				*itr = target;
				break;
			}
		}
	}
	else if (this->position[origin]->get_type() == 7 || this->position[origin]->get_type() == 8) {
		for (itr = this->bishop_list->begin(); itr != this->bishop_list->end(); itr++) {
			if (*itr == origin) {
				*itr = target;
				break;
			}
		}
	}
	else if (this->position[origin]->get_type() == 9 || this->position[origin]->get_type() == 10) {
		for (itr = this->knight_list->begin(); itr != this->knight_list->end(); itr++) {
			if (*itr == origin) {
				*itr = target;
				break;
			}
		}
	}
	else if (this->position[origin]->get_type() == 11 || this->position[origin]->get_type() == 12) {
		for (itr = this->pawn_list->begin(); itr != this->pawn_list->end(); itr++) {
			if (*itr == origin) {
				*itr = target;
				break;
			}
		}
	}

	this->position[target]->set_piece_type(this->position[origin]->get_type());
	this->position[origin]->set_piece_type(0);

	if (is_promotion) {
		this->position[target]->set_piece_type(promotion_type);
		Utility::remove_first_occurance(this->pawn_list, target);
		if (promotion_type == 3 || promotion_type == 4) {
			this->queen_list->push_back(target);
		}
		else if (promotion_type == 5 || promotion_type == 6) {
			this->rook_list->push_back(target);
		}
		else if (promotion_type == 7 || promotion_type == 8) {
			this->bishop_list->push_back(target);
		}
		else if (promotion_type == 9 || promotion_type == 10) {
			this->knight_list->push_back(target);
		}
	}
}

void Board::unmake_move() {
	this->switch_move();
	// stack

	// move unwrapping
	Move* m = this->stack_moves->back();
	int target = m->target;
	int origin = m->origin;
	bool is_promotion = m->is_promotion;
	bool is_capture = m->is_capture;
	unsigned promotion_type = m->promotion_type;
	//delete m;

	this->stack_moves->pop_back();

	bool* castling_rights_old = this->stack_castling_rights->back();
	this->stack_castling_rights->pop_back();
	for (int i = 0; i < 4; i++) {
		this->castling_rights[i] = castling_rights_old[i];
	}
	delete castling_rights_old;

	unsigned capture_type = this->stack_captures->back();
	this->stack_captures->pop_back();

	this->en_passant_target_index = this->stack_en_passant_target_index->back();
	this->stack_en_passant_target_index->pop_back();

	// unmake kingpos
	if (this->position[target]->get_type() == 1) {
		this->white_king_pos = origin;
	}
	else if (this->position[target]->get_type() == 2) {
		this->black_king_pos = origin;
	}
	// unmake castling
	std::list<int>::iterator itr;
	if (this->position[target]->get_type() == 1 && std::abs(target - origin) == 2) {
		if (target == 6) {
			this->position[6]->set_piece_type(0);
			this->position[5]->set_piece_type(0);
			this->position[7]->set_piece_type(5);
			this->position[4]->set_piece_type(1);
			for (itr = this->rook_list->begin(); itr != this->rook_list->end(); itr++) {
				if (*itr == 5) {
					*itr = 7;
					break;
				}
			}
		}
		else if (target == 2) {
			this->position[2]->set_piece_type(0);
			this->position[3]->set_piece_type(0);
			this->position[0]->set_piece_type(5);
			this->position[4]->set_piece_type(1);
			for (itr = this->rook_list->begin(); itr != this->rook_list->end(); itr++) {
				if (*itr == 3) {
					*itr = 0;
					break;
				}
			}
		}
		return;
	}
	else if (this->position[target]->get_type() == 2 && std::abs(target - origin) == 2) {
		if (target == 62) {
			this->position[62]->set_piece_type(0);
			this->position[61]->set_piece_type(0);
			this->position[63]->set_piece_type(6);
			this->position[60]->set_piece_type(2);
			for (itr = this->rook_list->begin(); itr != this->rook_list->end(); itr++) {
				if (*itr == 61) {
					*itr = 63;
					break;
				}
			}
		}
		else if (target == 58) {
			this->position[58]->set_piece_type(0);
			this->position[59]->set_piece_type(0);
			this->position[56]->set_piece_type(6);
			this->position[60]->set_piece_type(2);
			for (itr = this->rook_list->begin(); itr != this->rook_list->end(); itr++) {
				if (*itr == 59) {
					*itr = 56;
					break;
				}
			}
		}
		return;
	}
	// unmake move
	if (this->position[target]->get_type() == 3 || this->position[target]->get_type() == 4) {
		for (itr = this->queen_list->begin(); itr != this->queen_list->end(); itr++) {
			if (*itr == target) {
				*itr = origin;
				break;
			}
		}
	}
	else if (this->position[target]->get_type() == 5 || this->position[target]->get_type() == 6) {
		for (itr = this->rook_list->begin(); itr != this->rook_list->end(); itr++) {
			if (*itr == target) {
				*itr = origin;
				break;
			}
		}
	}
	else if (this->position[target]->get_type() == 7 || this->position[target]->get_type() == 8) {
		for (itr = this->bishop_list->begin(); itr != this->bishop_list->end(); itr++) {
			if (*itr == target) {
				*itr = origin;
				break;
			}
		}
	}
	else if (this->position[target]->get_type() == 9 || this->position[target]->get_type() == 10) {
		for (itr = this->knight_list->begin(); itr != this->knight_list->end(); itr++) {
			if (*itr == target) {
				*itr = origin;
				break;
			}
		}
	}
	else if (this->position[target]->get_type() == 11 || this->position[target]->get_type() == 12) {
		for (itr = this->pawn_list->begin(); itr != this->pawn_list->end(); itr++) {
			if (*itr == target) {
				*itr = origin;
				break;
			}
		}
	}
	this->position[origin]->set_piece_type(this->position[target]->get_type());
	this->position[target]->set_piece_type(capture_type);
	// unmake capture 
	if (is_capture) {
		if (capture_type == 3 || capture_type == 4) {
			this->queen_list->push_back(target);
		}
		else if (capture_type == 5 || capture_type == 6) {
			this->rook_list->push_back(target);
		}
		else if (capture_type == 7 || capture_type == 8) {
			this->bishop_list->push_back(target);
		}
		else if (capture_type == 9 || capture_type == 10) {
			this->knight_list->push_back(target);
		}
		else if (capture_type == 11 || capture_type == 12) {
			this->pawn_list->push_back(target);
		}
	}
	// unmake en passant
	if ((this->position[origin]->get_type() == 11 || this->position[origin]->get_type() == 12) && target == this->en_passant_target_index) {
		if (this->position[origin]->is_white()) {
			this->position[target - 8]->set_piece_type(12);
			this->pawn_list->push_back(target - 8);
		}
		else {
			this->position[target + 8]->set_piece_type(11);
			this->pawn_list->push_back(target + 8);
		}
	}

	// unmake promotion
	if (is_promotion) {
		if (this->position[origin]->is_white()) {
			this->position[origin]->set_piece_type(11);
		}
		else {
			this->position[origin]->set_piece_type(12);
		}
		if (promotion_type == 3 || promotion_type == 4) {
			Utility::remove_first_occurance(this->queen_list, origin);
		}
		else if (promotion_type == 5 || promotion_type == 6) {
			Utility::remove_first_occurance(this->rook_list, origin);
		}
		else if (promotion_type == 7 || promotion_type == 8) {
			Utility::remove_first_occurance(this->bishop_list, origin);
		}
		if (promotion_type == 9 || promotion_type == 10) {
			Utility::remove_first_occurance(this->knight_list, origin);
		}
		this->pawn_list->push_back(origin);
	}
}