#include "evaluator.h"
#include <iostream>

Evaluator::Evaluator() {}

Evaluator::~Evaluator() {}

int Evaluator::mirror_vertical(int i) {
	return  8 * (7 - (i / 8)) + (i % 8);
}

int Evaluator::evaluate(Board* b)
{
	int e = 0;
	e += 350 + KingTable[b->white_king_pos];
	e -= 350 + KingTable[mirror_vertical(b->black_king_pos)];

	std::list<int>::iterator itr;
	for (itr = b->queen_list->begin(); itr != b->queen_list->end(); itr++) {
		if (b->position[*itr]->is_white()) {
			e += 900 + QueenTable[*itr];
		}
		else {
			e -= 900 + QueenTable[mirror_vertical(*itr)];
		}
	}
	for (itr = b->rook_list->begin(); itr != b->rook_list->end(); itr++) {
		if (b->position[*itr]->is_white()) {
			e += 500 + RookTable[*itr];
		}
		else {
			e -= 500 + RookTable[mirror_vertical(*itr)];
		}
	}

	for (itr = b->bishop_list->begin(); itr != b->bishop_list->end(); itr++) {
		if (b->position[*itr]->is_white()) {
			e += 320 + BishopTable[*itr];
		}
		else {
			e -= 320 + BishopTable[mirror_vertical(*itr)];
		}
	}

	for (itr = b->knight_list->begin(); itr != b->knight_list->end(); itr++) {
		if (b->position[*itr]->is_white()) {
			e += 300 + KnightTable[*itr];
		}
		else {
			e -= 300 + KnightTable[mirror_vertical(*itr)];
		}
	}

	for (itr = b->pawn_list->begin(); itr != b->pawn_list->end(); itr++) {
		if (b->position[*itr]->is_white()) {
			e += 100 + PawnTable[*itr];
		}
		else {
			e -= 100 + PawnTable[mirror_vertical(*itr)];
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
bool Evaluator::compare(Board* pos, Move* m_1, Move* m_2, std::vector<Move*>* killer_moves_at_depth) {
	if (m_1->is_capture && m_2->is_capture) {
		return score_move(pos, m_1, killer_moves_at_depth) > score_move(pos, m_2, killer_moves_at_depth);
	}
	else if (m_1->is_capture) {
		return true;
	}
	else if (m_2->is_capture) {
		return false;
	}
	else {
		return score_quiet_move(pos, m_1, killer_moves_at_depth) > score_quiet_move(pos, m_2, killer_moves_at_depth);
	}
}

int Evaluator::score_quiet_move(Board* pos, Move* m, std::vector<Move*>* killer_moves_at_depth) {
	int origin = m->origin;
	int target = m->target;

	if (!pos->position[origin]->is_black()) {
		origin = mirror_vertical(origin);
		target = mirror_vertical(target);
	}
	int offset = 0;
	if (killer_moves_at_depth != nullptr) {
		if (m->equals(killer_moves_at_depth->at(0)) || m->equals(killer_moves_at_depth->at(1))) {
			offset += 1000;
		}
	}
	//std::cout << "origin " << origin << ", target" << target <<", piece type " << pos->position[origin]->get_type() << ", ";
	switch (pos->position[origin]->get_type())
	{
	case 1:
		return KingTable[target] - KingTable[origin] + offset;
	case 2:
		return KingTable[target] - KingTable[origin] + offset;
	case 3:
		return QueenTable[target] - QueenTable[origin] + offset;
	case 4:
		return QueenTable[target] - QueenTable[origin] + offset;
	case 5:
		return RookTable[target] - RookTable[origin] + offset;
	case 6:
		return RookTable[target] - RookTable[origin] + offset;
	case 7:
		return BishopTable[target] - BishopTable[origin] + offset;
	case 8:
		return BishopTable[target] - BishopTable[origin] + offset;
	case 9:
		return KnightTable[target] - KnightTable[origin] + offset;
	case 10:
		return KnightTable[target] - KnightTable[origin] + offset;
	case 11:
		return PawnTable[target] - PawnTable[origin] + offset;
	case 12:
		return PawnTable[target] - PawnTable[origin] + offset;
	default:
		//std::cout << "fuck ";
		return 0;
	}
}

int Evaluator::score_capture(Board* pos, Move* move) {
	int origin = move->origin;
	int target = move->target;
	return pos->position[target]->value() - pos->position[origin]->value();
}

int Evaluator::score_move(Board* pos, Move* move, std::vector<Move*>* killer_moves_at_depth) {
	int origin = move->origin;
	int target = move->target;
	if (!pos->position[target]->is_empty()) {
		// score capture
		return score_capture(pos, move) + score_quiet_move(pos, move, killer_moves_at_depth);
	}
	else {
		// score quite move
		return score_quiet_move(pos, move, killer_moves_at_depth);
	}
}

int Evaluator::KingTableEndGame[64] = {
		-50,-40,-30,-20,-20,-30,-40,-50,
		-30,-20,-10,  0,  0,-10,-20,-30,
		-30,-10, 20, 30, 30, 20,-10,-30,
		-30,-10, 30, 40, 40, 30,-10,-30,
		-30,-10, 30, 40, 40, 30,-10,-30,
		-30,-10, 20, 30, 30, 20,-10,-30,
		-30,-30,  0,  0,  0,  0,-30,-30,
		-50,-30,-30,-30,-30,-30,-30,-50 };
int Evaluator::KingTable[64] = {
		-30, -40, -40, -50, -50, -40, -40, -30,
		-30, -40, -40, -50, -50, -40, -40, -30,
		-30, -40, -40, -50, -50, -40, -40, -30,
		-30, -40, -40, -50, -50, -40, -40, -30,
		-20, -30, -30, -40, -40, -30, -30, -20,
		-10, -20, -20, -20, -20, -20, -20, -10,
		20,  20,   0,   0,   0,   0,  20,  20,
		20,  30,  10,   0,   0,  10,  30,  20 };
int Evaluator::QueenTable[64] = {
		-20,-10,-10, -5, -5,-10,-10,-20,
		-10,  0,  0,  0,  0,  0,  0,-10,
		-10,  0,  5,  5,  5,  5,  0,-10,
		-5,  0,  5,  5,  5,  5,  0, -5,
		0,  0,  5,  5,  5,  5,  0, -5,
		-10,  5,  5,  5,  5,  5,  0,-10,
		-10,  0,  5,  0,  0,  0,  0,-10,
		-20,-10,-10, -5, -5,-10,-10,-20 };
int Evaluator::RookTable[64] = {
		0,  0,  0,  0,  0,  0,  0,  0,
		5, 10, 10, 10, 10, 10, 10,  5,
		-5,  0,  5,  0,  0,  5,  0, -5,
		-5,  0,  0,  0,  0,  0,  0, -5,
		-5,  0,  0,  0,  0,  0,  0, -5,
		-5,  5,  5,  0,  0,  5,  5, -5,
		-5,  0,  0,  0,  0,  0,  0, -5,
		0,  0,  0,  5,  5,  0,  0,  0 };

int Evaluator::BishopTable[64] = {
	   -20,-10,-10,-10,-10,-10,-10,-20,
	   -10,  0,  0,  0,  0,  0,  0,-10,
	   -10,  0,  5, 10, 10,  5,  0,-10,
	   -10,  5,  5, 10, 10,  5,  5,-10,
	   -10,  0, 10, 10, 10, 10,  0,-10,
	   -10, 10, 10, 10, 10, 10, 10,-10,
	   -10,  8,  0,  0,  0,  0,  8,-10,
	   -20,-10,-40,-10,-10,-40,-10,-20 };

int Evaluator::KnightTable[64] = {
	   -50,-40,-30,-30,-30,-30,-40,-50,
	   -40,-20,  0,  0,  0,  0,-20,-40,
	   -30,  0, 10, 15, 15, 10,  0,-30,
	   -30,  5, 15, 20, 20, 15,  5,-30,
	   -30,  0, 15, 20, 20, 15,  0,-30,
	   -30,  5, 10, 15, 15, 10,  5,-30,
	   -40,-20,  0,  5,  5,  0,-20,-40,
	   -50,-40,-30,-30,-30,-30,-40,-50 };

int Evaluator::PawnTable[64] = {
	   0,  0,  0,  0,  0,  0,  0,  0,
	   40, 50, 50, 50, 50, 50, 50, 40,
	   8, 10, 20, 30, 30, 20, 10,  8,
	   5,  5, 10, 25, 25, 10,  5,  5,
	   0,  0,  0, 20, 20,  0,  0,  0,
	   5, -5,-10,  0,  0,-10, -5,  5,
	   5, 10, 10,-20,-20, 10, 10,  5,
	   0,  0,  0,  0,  0,  0,  0,  0
};