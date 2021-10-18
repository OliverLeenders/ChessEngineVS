#include "evaluator.h"
#include <iostream>

Evaluator::Evaluator()
{
	for (int i = 0; i < 781; i++)
	{
		this->zobrist_base_numbers[i] = this->random_64_bit_num();
	}
	this->zobrist_hashmap->reserve(10000000);
	return;
}

Evaluator::~Evaluator()
{
	delete[] this->zobrist_base_numbers;
	this->zobrist_hashmap->clear();
	this->prev_zobrist_hashmap->clear();
	delete this->prev_zobrist_hashmap;
	delete this->zobrist_hashmap;
	return;
}

uint64_t Evaluator::random_64_bit_num()
{
	uint64_t num = 0;
	for (int i = 0; i < 64; i++)
	{
		num = num * 2 + rand() % 2;
	}
	return num;
}

void Evaluator::add_hash(uint64_t hash, double eval)
{
	this->zobrist_hashmap->insert(std::make_pair(hash, eval));
}

bool Evaluator::contains_z_hash(uint64_t hash)
{
	return !(this->zobrist_hashmap->find(hash) == this->zobrist_hashmap->end());
}

uint64_t Evaluator::zobrist_hash(Board* b)
{
	uint64_t hash = 0;
	
	std::list<int>::iterator itr;
	hash = hash xor this->zobrist_base_numbers[(b->white_king_pos * 12) + b->position[b->white_king_pos]->get_type()];
	hash = hash xor this->zobrist_base_numbers[(b->black_king_pos * 12) + b->position[b->black_king_pos]->get_type()];
	for (itr = b->queen_list->begin(); itr != b->queen_list->end(); itr++) {
		hash = hash xor this->zobrist_base_numbers[(*itr * 12) + b->position[*itr]->get_type()];
	}
	for (itr = b->rook_list->begin(); itr != b->rook_list->end(); itr++) {
		hash = hash xor this->zobrist_base_numbers[(*itr * 12) + b->position[*itr]->get_type()];
	}
	for (itr = b->bishop_list->begin(); itr != b->bishop_list->end(); itr++) {
		hash = hash xor this->zobrist_base_numbers[(*itr * 12) + b->position[*itr]->get_type()];
	}
	for (itr = b->knight_list->begin(); itr != b->knight_list->end(); itr++) {
		hash = hash xor this->zobrist_base_numbers[(*itr * 12) + b->position[*itr]->get_type()];
	}
	for (itr = b->pawn_list->begin(); itr != b->pawn_list->end(); itr++) {
		hash = hash xor this->zobrist_base_numbers[(*itr * 12) + b->position[*itr]->get_type()];
	}
	int i = 64;
	for (int j = 0; j < 4; j++)
	{
		if (b->castling_rights[j])
		{
			hash = hash xor this->zobrist_base_numbers[i + j];
		}
	}
	if (b->en_passant_target_index > 0)
	{
		hash = hash xor this->zobrist_base_numbers[i + 4 + (b->en_passant_target_index % 8)];
	}
	if (!b->white_to_move)
	{
		hash = hash xor this->zobrist_base_numbers[i + 12];
	}
	return hash;
}

int Evaluator::mirror_vertical(int i) {
	return  8 * (7 - (i / 8)) + (i % 8);
}

double Evaluator::evaluate(Board* b)
{
	double e = 0.0;
	e += 3.5 + this->KingTable[b->white_king_pos] / 100.0;
	e -= 3.5 + this->KingTable[this->mirror_vertical(b->black_king_pos)] / 100.0;

	std::list<int>::iterator itr;
	for (itr = b->queen_list->begin(); itr != b->queen_list->end(); itr++) {
		if (b->position[*itr]->is_white()) {
			e += 9.0 + this->QueenTable[*itr] / 100.0;
		}
		else {
			e -= 9.0 + this->QueenTable[this->mirror_vertical(*itr)] / 100.0;
		}
	}
	for (itr = b->rook_list->begin(); itr != b->rook_list->end(); itr++) {
		if (b->position[*itr]->is_white()) {
			e += 5.0 + this->RookTable[*itr] / 100.0;
		}
		else {
			e -= 5.0 + this->RookTable[this->mirror_vertical(*itr)] / 100.0;
		}
	}

	for (itr = b->bishop_list->begin(); itr != b->bishop_list->end(); itr++) {
		if (b->position[*itr]->is_white()) {
			e += 3.2 + this->BishopTable[*itr] / 100.0;
		}
		else {
			e -= 3.2 + this->BishopTable[this->mirror_vertical(*itr)] / 100.0;
		}
	}

	for (itr = b->knight_list->begin(); itr != b->knight_list->end(); itr++) {
		if (b->position[*itr]->is_white()) {
			e += 3.0 + this->KnightTable[*itr] / 100.0;
		}
		else {
			e -= 3.0 + this->KnightTable[this->mirror_vertical(*itr)] / 100.0;
		}
	}

	for (itr = b->pawn_list->begin(); itr != b->pawn_list->end(); itr++) {
		if (b->position[*itr]->is_white()) {
			e += 1.0 + this->PawnTable[*itr] / 100.0;
		}
		else {
			e -= 1.0 + this->PawnTable[this->mirror_vertical(*itr)] / 100.0;
		}
	}

	
	if (b->white_to_move)
	{
		return e;
	}
	else
	{
		return -e;
	}
}

/**
 * \brief Compares two moves according to a position.
 *
 * \param pos position
 * \param m_1 move 1
 * \param m_2 move 2
 * \return true if and only if move 1 & 2 are already in the correct respective order.
 */
bool Evaluator::compare(Board* pos, Move* m_1, Move* m_2) {
	if (m_1->is_capture && m_2->is_capture) {
		double score_1;
		double score_2;

		score_1 = pos->position[m_1->target]->value() - pos->position[m_1->origin]->value();
		score_2 = pos->position[m_2->target]->value() - pos->position[m_2->origin]->value();

		return score_1 > score_2;
	}
	else if (m_1->is_capture) {
		return true;
	}
	else if (m_2->is_capture) {
		return false;
	}
	else {
		return score_move(pos, m_1) > score_move(pos, m_2);
	}
}

double Evaluator::score_move(Board* pos, Move* m) {
	int origin = m->origin;
	int target = m->target;

	if (pos->position[origin]->is_black()) {
		origin = mirror_vertical(origin);
		target = mirror_vertical(target);
	}
	switch (pos->position[origin]->get_type() / 2)
	{
	case 1:
		return KingTable[target] - KingTable[origin];
	case 2:
		return QueenTable[target] - QueenTable[origin];
	case 3:
		return RookTable[target] - RookTable[origin];
	case 4:
		return BishopTable[target] - BishopTable[origin];
	case 5:
		return KnightTable[target] - KnightTable[origin];
	case 6:
		return PawnTable[target] - PawnTable[origin];
	default:
		return 0.0;
	}
}

short Evaluator::KingTableEndGame[64] = {
		-50,-40,-30,-20,-20,-30,-40,-50,
		-30,-20,-10,  0,  0,-10,-20,-30,
		-30,-10, 20, 30, 30, 20,-10,-30,
		-30,-10, 30, 40, 40, 30,-10,-30,
		-30,-10, 30, 40, 40, 30,-10,-30,
		-30,-10, 20, 30, 30, 20,-10,-30,
		-30,-30,  0,  0,  0,  0,-30,-30,
		-50,-30,-30,-30,-30,-30,-30,-50 };
short Evaluator::KingTable[64] = {
		-30, -40, -40, -50, -50, -40, -40, -30,
		-30, -40, -40, -50, -50, -40, -40, -30,
		-30, -40, -40, -50, -50, -40, -40, -30,
		-30, -40, -40, -50, -50, -40, -40, -30,
		-20, -30, -30, -40, -40, -30, -30, -20,
		-10, -20, -20, -20, -20, -20, -20, -10,
		20,  20,   0,   0,   0,   0,  20,  20,
		20,  30,  10,   0,   0,  10,  30,  20 };
short Evaluator::QueenTable[64] = {
		-20,-10,-10, -5, -5,-10,-10,-20,
		-10,  0,  0,  0,  0,  0,  0,-10,
		-10,  0,  5,  5,  5,  5,  0,-10,
		-5,  0,  5,  5,  5,  5,  0, -5,
		0,  0,  5,  5,  5,  5,  0, -5,
		-10,  5,  5,  5,  5,  5,  0,-10,
		-10,  0,  5,  0,  0,  0,  0,-10,
		-20,-10,-10, -5, -5,-10,-10,-20 };
short Evaluator::RookTable[64] = {
		0,  0,  0,  0,  0,  0,  0,  0,
		5, 10, 10, 10, 10, 10, 10,  5,
		-5,  0,  5,  0,  0,  5,  0, -5,
		-5,  0,  0,  0,  0,  0,  0, -5,
		-5,  0,  0,  0,  0,  0,  0, -5,
		-5,  5,  5,  0,  0,  5,  5, -5,
		-5,  0,  0,  0,  0,  0,  0, -5,
		0,  0,  0,  5,  5,  0,  0,  0 };

short Evaluator::BishopTable[64] = {
	   -20,-10,-10,-10,-10,-10,-10,-20,
	   -10,  0,  0,  0,  0,  0,  0,-10,
	   -10,  0,  5, 10, 10,  5,  0,-10,
	   -10,  5,  5, 10, 10,  5,  5,-10,
	   -10,  0, 10, 10, 10, 10,  0,-10,
	   -10, 10, 10, 10, 10, 10, 10,-10,
	   -10,  8,  0,  0,  0,  0,  8,-10,
	   -20,-10,-40,-10,-10,-40,-10,-20 };

short Evaluator::KnightTable[64] = {
	   -50,-40,-30,-30,-30,-30,-40,-50,
	   -40,-20,  0,  0,  0,  0,-20,-40,
	   -30,  0, 10, 15, 15, 10,  0,-30,
	   -30,  5, 15, 20, 20, 15,  5,-30,
	   -30,  0, 15, 20, 20, 15,  0,-30,
	   -30,  5, 10, 15, 15, 10,  5,-30,
	   -40,-20,  0,  5,  5,  0,-20,-40,
	   -50,-40,-30,-30,-30,-30,-40,-50 };

short Evaluator::PawnTable[64] = {
	   0,  0,  0,  0,  0,  0,  0,  0,
	   40, 50, 50, 50, 50, 50, 50, 40,
	   8, 10, 20, 30, 30, 20, 10,  8,
	   5,  5, 10, 25, 25, 10,  5,  5,
	   0,  0,  0, 20, 20,  0,  0,  0,
	   5, -5,-10,  0,  0,-10, -5,  5,
	   5, 10, 10,-20,-20, 10, 10,  5,
	   0,  0,  0,  0,  0,  0,  0,  0
};