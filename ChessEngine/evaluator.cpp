#include "evaluator.h"
#include <iostream>
Evaluator::Evaluator()
{
	/*for (int i = 0; i < 64; i++)
	{
		int index = 8 * (7 - (i / 8)) + (i % 8);
		std::cout << this->RookTable[index] << " ";
		if (i % 8 == 7)
		{
			std::cout << std::endl;
		}
	}*/
	for (int i = 0; i < 781; i++)
	{
		this->zobrist_base_numbers[i] = this->random_64_bit_num();
	}
	this->zobrist_hashmap->reserve(1000000);
	return;
}

Evaluator::~Evaluator()
{
	delete[] this->zobrist_base_numbers;
	this->zobrist_hashmap->clear();
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

bool Evaluator::compare(Board* b1, Board* b2) {
	int i1 = b1->last_move_origin;
	int j1 = b1->last_move_target;
	int i2 = b2->last_move_origin;
	int j2 = b2->last_move_target;
	Board* prev_1 = b1->prev_pos;
	Board* prev_2 = b2->prev_pos;
	if ((!prev_1->position[j1]->is_empty()) && (!prev_2->position[j2]->is_empty())) {
		double diff_1 = prev_1->position[j1]->value() - prev_1->position[i1]->value();
		double diff_2 = prev_2->position[j2]->value() - prev_2->position[i2]->value();
		return diff_1 > diff_2;
	}
	else if (!prev_2->position[j2]->is_empty()) {
		return 0 > 1;
	}
	else if (!prev_1->position[j1]->is_empty()) {
		return 0 < 1;
	}
	double diff_1;
	double diff_2;
	switch (prev_1->position[i1]->get_type())
	{
	case 1:
		diff_1 = (KingTable[j1] - KingTable[i1]) / 100.0;
	case 2:
		diff_1 = (KingTable[mirror_vertical(j1)] - KingTable[mirror_vertical(i1)]) / 100.0;
	case 3:
		diff_1 = (QueenTable[j1] - QueenTable[i1]) / 100.0;
	case 4:
		diff_1 = (QueenTable[mirror_vertical(j1)] - QueenTable[mirror_vertical(i1)]) / 100.0;
	case 5:
		diff_1 = (RookTable[j1] - RookTable[i1]) / 100.0;
	case 6:
		diff_1 = (RookTable[mirror_vertical(j1)] - RookTable[mirror_vertical(i1)]) / 100.0;
	case 7:
		diff_1 = (BishopTable[j1] - BishopTable[i1]) / 100.0;
	case 8:
		diff_1 = (BishopTable[mirror_vertical(j1)] - BishopTable[mirror_vertical(i1)]) / 100.0;
	case 9:
		diff_1 = (KnightTable[j1] - KnightTable[i1]) / 100.0;
	case 10:
		diff_1 = (KnightTable[mirror_vertical(j1)] - KnightTable[mirror_vertical(i1)]) / 100.0;
	case 11:
		diff_1 = (PawnTable[j1] - PawnTable[i1]) / 100.0;
	case 12:
		diff_1 = (PawnTable[mirror_vertical(j1)] - PawnTable[mirror_vertical(i1)]) / 100.0;
	default:
		break;
	}
	switch (prev_2->position[i2]->get_type())
	{
	case 1:
		diff_2 = (KingTable[j2] - KingTable[i2]) / 100.0;
	case 2:
		diff_2 = (KingTable[mirror_vertical(j2)] - KingTable[mirror_vertical(i2)]) / 100.0;
	case 3:
		diff_2 = (QueenTable[j2] - QueenTable[i2]) / 100.0;
	case 4:
		diff_2 = (QueenTable[mirror_vertical(j2)] - QueenTable[mirror_vertical(i2)]) / 100.0;
	case 5:
		diff_2 = (RookTable[j2] - RookTable[i2]) / 100.0;
	case 6:
		diff_2 = (RookTable[mirror_vertical(j2)] - RookTable[mirror_vertical(i2)]) / 100.0;
	case 7:
		diff_2 = (BishopTable[j2] - BishopTable[i2]) / 100.0;
	case 8:
		diff_2 = (BishopTable[mirror_vertical(j2)] - BishopTable[mirror_vertical(i2)]) / 100.0;
	case 9:
		diff_2 = (KnightTable[j2] - KnightTable[i2]) / 100.0;
	case 10:
		diff_2 = (KnightTable[mirror_vertical(j2)] - KnightTable[mirror_vertical(i2)]) / 100.0;
	case 11:
		diff_2 = (PawnTable[j2] - PawnTable[i2]) / 100.0;
	case 12:
		diff_2 = (PawnTable[mirror_vertical(j2)] - PawnTable[mirror_vertical(i2)]) / 100.0;
	default:
		break;
	}
	return diff_1 > diff_2;
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