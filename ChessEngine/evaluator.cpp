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
	int i = 0;
	for (; i < 64; i++)
	{
		hash = hash xor this->zobrist_base_numbers[(i * 12) + b->position[i]->get_type()];
	}
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
	for (int i = 0; i < 64; i++)
	{
		if (b->position[i]->is_white())
		{
			int index = 8 * (7 - (i / 8)) + (i % 8);
			if (b->position[i]->get_type() == 1)
			{
				e += 3.5 + this->KingTable[index] / 100.0;
			}
			else if (b->position[i]->get_type() == 3)
			{
				e += 9.0 + this->QueenTable[index] / 100.0;
			}
			else if (b->position[i]->get_type() == 5)
			{
				e += 5.0 + this->RookTable[index] / 100.0;
			}
			else if (b->position[i]->get_type() == 7)
			{
				e += 3.2 + this->BishopTable[index] / 100.0;
			}
			else if (b->position[i]->get_type() == 9)
			{
				e += 3.0 + this->KnightTable[index] / 100.0;
			}
			else if (b->position[i]->get_type() == 11)
			{
				e += 1.0 + this->PawnTable[index] / 100.0;
			}
		}
		else if (b->position[i]->is_black())
		{
			//std::cout << index << std::endl;
			if (b->position[i]->get_type() == 2)
			{
				e -= 3.5 + this->KingTable[i] / 100.0;
			}
			else if (b->position[i]->get_type() == 4)
			{
				e -= 9.0 + this->QueenTable[i] / 100.0;
			}
			else if (b->position[i]->get_type() == 6)
			{
				e -= 5.0 + this->RookTable[i] / 100.0;
			}
			else if (b->position[i]->get_type() == 8)
			{
				e -= 3.2 + this->BishopTable[i] / 100.0;
			}
			else if (b->position[i]->get_type() == 10)
			{
				e -= 3.0 + this->KnightTable[i] / 100.0;
			}
			else if (b->position[i]->get_type() == 12)
			{
				e -= 1.0 + this->PawnTable[i] / 100.0;
			}
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

bool Evaluator::compare(Board* pos, Move* m_1, Move* m_2) {
	if (m_1->is_capture && m_2->is_capture) {
		return false;
	}
	else if (m_1->is_capture) {
		return true;
	}
	else if (m_2->is_capture) {
		return false;
	}
	return false;
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