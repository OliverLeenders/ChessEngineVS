#include "evaluator.h"
#include <iostream>

int Evaluator::mg_table[12][64] = { 0 };
int Evaluator::eg_table[12][64] = { 0 };

Evaluator::Evaluator() {
	//init_tables();
}

Evaluator::~Evaluator() {}

int Evaluator::mirror_vertical(int i) {
	return  8 * (7 - (i / 8)) + (i % 8);
}

void Evaluator::init_tables() {
	// kings 
	for (int i = 0; i < 64; i++) {
		mg_table[0][i] = KingTable[mirror_vertical(i)];
		eg_table[0][i] = KingTableEndGame[mirror_vertical(i)];
		mg_table[1][i] = KingTable[i];
		eg_table[1][i] = KingTableEndGame[i];
		mg_table[2][i] = QueenTable[mirror_vertical(i)];
		eg_table[2][i] = QueenTableEndGame[mirror_vertical(i)];
		mg_table[3][i] = QueenTable[i];
		eg_table[3][i] = QueenTableEndGame[i];
		mg_table[4][i] = RookTable[mirror_vertical(i)];
		eg_table[4][i] = RookTableEndGame[mirror_vertical(i)];
		mg_table[5][i] = RookTable[i];
		eg_table[5][i] = RookTableEndGame[i];
		mg_table[6][i] = BishopTable[mirror_vertical(i)];
		eg_table[6][i] = BishopTableEndGame[mirror_vertical(i)];
		mg_table[7][i] = BishopTable[i];
		eg_table[7][i] = BishopTableEndGame[i];
		mg_table[8][i] = KnightTable[mirror_vertical(i)];
		eg_table[8][i] = KnightTableEndGame[mirror_vertical(i)];
		mg_table[9][i] = KnightTable[i];
		eg_table[9][i] = KnightTableEndGame[i];
		mg_table[10][i] = PawnTable[mirror_vertical(i)];
		eg_table[10][i] = PawnTableEndGame[mirror_vertical(i)];
		mg_table[11][i] = PawnTable[i];
		eg_table[11][i] = PawnTableEndGame[i];
	}
}

int Evaluator::evaluate(Board* b)
{
	int e = 0;
	// insufficient material
	if (b->pawn_list->size() == 0 && b->queen_list->size() == 0 && b->rook_list->size() == 0) {
		// std::cout << "insufficient material" << std::endl;
		if ((b->bishop_list->size() == 0 && b->knight_list->size() == 0) || (b->bishop_list->size() == 0 && b->knight_list->size() == 1) || (b->bishop_list->size() == 1 && b->knight_list->size() == 0)) {
			return 0;
		}
	}

	int mg_eval = 0;
	int eg_eval = 0;

	for (int i = 0; i < 64; i++) {
		Piece* curr = b->position[i];
		unsigned type = curr->get_type();
		switch (type)
		{
		case 0:
			break;
			// king
		case 1:
			mg_eval += mg_value[type] + KingTable[mirror_vertical(i)];
			eg_eval += eg_value[type] + KingTableEndGame[mirror_vertical(i)];
			//std::cout << KingTable[mirror_vertical(i)] << "\n";
			break;
		case 2:
			mg_eval -= mg_value[type] + KingTable[i];
			eg_eval -= eg_value[type] + KingTableEndGame[i];
			//std::cout << KingTable[i] << "\n";
			break;
			// queen

		case 3:
			mg_eval += mg_value[type] + QueenTable[mirror_vertical(i)];
			eg_eval += eg_value[type] + QueenTableEndGame[mirror_vertical(i)];
			break;
		case 4:
			mg_eval -= mg_value[type] + QueenTable[i];
			eg_eval -= eg_value[type] + QueenTableEndGame[i];
			break;
			// rook
		case 5:
			mg_eval += mg_value[type] + RookTable[mirror_vertical(i)];
			eg_eval += eg_value[type] + RookTableEndGame[mirror_vertical(i)];
			break;
		case 6:
			mg_eval -= mg_value[type] + RookTable[i];
			eg_eval -= eg_value[type] + RookTableEndGame[i];
			break;
			// bishop
		case 7:
			mg_eval += mg_value[type] + BishopTable[mirror_vertical(i)];
			eg_eval += eg_value[type] + BishopTableEndGame[mirror_vertical(i)];
			break;
		case 8:
			mg_eval -= mg_value[type] + BishopTable[i];
			eg_eval -= eg_value[type] + BishopTableEndGame[i];
			break;
			// knight
		case 9:
			mg_eval += mg_value[type] + KnightTable[mirror_vertical(i)];
			eg_eval += eg_value[type] + KnightTableEndGame[mirror_vertical(i)];
			break;
		case 10:
			mg_eval -= mg_value[type] + KnightTable[i];
			eg_eval -= eg_value[type] + KnightTableEndGame[i];
			break;
			// pawn
		case 11:
			mg_eval += mg_value[type] + PawnTable[mirror_vertical(i)];
			eg_eval += eg_value[type] + PawnTableEndGame[mirror_vertical(i)];
			break;
		case 12:
			mg_eval -= mg_value[type] + PawnTable[i];
			eg_eval -= eg_value[type] + PawnTableEndGame[i];
			break;
		default:
			break;
		}
	}

	int game_phase = 4 * b->queen_list->size() + 2 * b->rook_list->size() + b->bishop_list->size() + b->knight_list->size();
	int mg_phase = std::min(24, game_phase);
	int eg_phase = 24 - game_phase;
	e = (mg_eval * mg_phase + eg_eval * eg_phase) / 24;

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
bool Evaluator::compare(Board* pos, Move* m_1, Move* m_2, Move* pv_move, std::vector<Move*>* killer_moves_at_depth, bool left_most) {
	return score_move(pos, m_1, pv_move, killer_moves_at_depth, left_most) > score_move(pos, m_2, pv_move, killer_moves_at_depth,left_most);

}

int Evaluator::score_quiet_move(Board* pos, Move* m) {
	int origin = m->origin;
	int target = m->target;
	if (pos->position[origin]->is_white()) {
		origin = mirror_vertical(origin);
		target = mirror_vertical(target);
	}

	//offset = 0;
	unsigned type = pos->position[origin]->get_type();
	switch (type)
	{
	case 1:
	{
		return  KingTable[target] - KingTable[origin];
	}
	case 2:
		return KingTable[target] - KingTable[origin];
	case 3:
		return QueenTable[target] - QueenTable[origin];
	case 4:
		return QueenTable[target] - QueenTable[origin];
	case 5:
		return RookTable[target] - RookTable[origin];
	case 6:
		return RookTable[target] - RookTable[origin];
	case 7:
		return BishopTable[target] - BishopTable[origin];
	case 8:
		return BishopTable[target] - BishopTable[origin];
	case 9:
		return KnightTable[target] - KnightTable[origin];
	case 10:
		return KnightTable[target] - KnightTable[origin];
	case 11:
		return PawnTable[target] - PawnTable[origin];
	case 12:
		return PawnTable[target] - PawnTable[origin];
	default:
		return 0;
	}
}

int Evaluator::score_capture(Board* pos, Move* move) {
	int origin = move->origin;
	int target = move->target;
	return pos->position[target]->value() - pos->position[origin]->value() + CAPTURE_SCORE;
}

int Evaluator::score_move(Board* pos, Move* move, Move* pv_move, std::vector<Move*>* killer_moves_at_depth, bool left_most) {
	
	if (move->is_capture) {
		// score capture
		return score_capture(pos, move) + score_quiet_move(pos, move);
	}
	else {
		// score quiet move
		if (left_most && pv_move != nullptr && move->equals(pv_move)) {
			return PV_SCORE;
		}
		else if (killer_moves_at_depth != nullptr) {
			if (killer_moves_at_depth->at(0)->equals(move)) {
				return KILLER_MOVE_ONE;
			}
			else if (killer_moves_at_depth->at(1)->equals(move)) {
				return KILLER_MOVE_TWO;
			}
		}
		return score_quiet_move(pos, move);
		//return 0;
	}
}
int Evaluator::PawnTable[64] = {
	  0,   0,   0,   0,   0,   0,  0,   0,
	 98, 134,  61,  95,  68, 126, 34, -11,
	 -6,   7,  26,  31,  65,  56, 25, -20,
	-14,  13,   6,  21,  23,  12, 17, -23,
	-27,  -2,  -5,  12,  17,   6, 10, -25,
	-26,  -4,  -4, -10,   3,   3, 33, -12,
	-35,  -1, -20, -23, -15,  24, 38, -22,
	  0,   0,   0,   0,   0,   0,  0,   0,
};

int Evaluator::PawnTableEndGame[64] = {
	  0,   0,   0,   0,   0,   0,   0,   0,
	178, 173, 158, 134, 147, 132, 165, 187,
	 94, 100,  85,  67,  56,  53,  82,  84,
	 32,  24,  13,   5,  -2,   4,  17,  17,
	 13,   9,  -3,  -7,  -7,  -8,   3,  -1,
	  4,   7,  -6,   1,   0,  -5,  -1,  -8,
	 13,   8,   8,  10,  13,   0,   2,  -7,
	  0,   0,   0,   0,   0,   0,   0,   0,
};

int Evaluator::KnightTable[64] = {
	-167, -89, -34, -49,  61, -97, -15, -107,
	 -73, -41,  72,  36,  23,  62,   7,  -17,
	 -47,  60,  37,  65,  84, 129,  73,   44,
	  -9,  17,  19,  53,  37,  69,  18,   22,
	 -13,   4,  16,  13,  28,  19,  21,   -8,
	 -23,  -9,  12,  10,  19,  17,  25,  -16,
	 -29, -53, -12,  -3,  -1,  18, -14,  -19,
	-105, -21, -58, -33, -17, -28, -19,  -23,
};

int Evaluator::KnightTableEndGame[64] = {
	-58, -38, -13, -28, -31, -27, -63, -99,
	-25,  -8, -25,  -2,  -9, -25, -24, -52,
	-24, -20,  10,   9,  -1,  -9, -19, -41,
	-17,   3,  22,  22,  22,  11,   8, -18,
	-18,  -6,  16,  25,  16,  17,   4, -18,
	-23,  -3,  -1,  15,  10,  -3, -20, -22,
	-42, -20, -10,  -5,  -2, -20, -23, -44,
	-29, -51, -23, -15, -22, -18, -50, -64,
};

int Evaluator::BishopTable[64] = {
	-29,   4, -82, -37, -25, -42,   7,  -8,
	-26,  16, -18, -13,  30,  59,  18, -47,
	-16,  37,  43,  40,  35,  50,  37,  -2,
	 -4,   5,  19,  50,  37,  37,   7,  -2,
	 -6,  13,  13,  26,  34,  12,  10,   4,
	  0,  15,  15,  15,  14,  27,  18,  10,
	  4,  15,  16,   0,   7,  21,  33,   1,
	-33,  -3, -14, -21, -13, -12, -39, -21,
};

int Evaluator::BishopTableEndGame[64] = {
	-14, -21, -11,  -8, -7,  -9, -17, -24,
	 -8,  -4,   7, -12, -3, -13,  -4, -14,
	  2,  -8,   0,  -1, -2,   6,   0,   4,
	 -3,   9,  12,   9, 14,  10,   3,   2,
	 -6,   3,  13,  19,  7,  10,  -3,  -9,
	-12,  -3,   8,  10, 13,   3,  -7, -15,
	-14, -18,  -7,  -1,  4,  -9, -15, -27,
	-23,  -9, -23,  -5, -9, -16,  -5, -17,
};

int Evaluator::RookTable[64] = {
	 32,  42,  32,  51, 63,  9,  31,  43,
	 27,  32,  58,  62, 80, 67,  26,  44,
	 -5,  19,  26,  36, 17, 45,  61,  16,
	-24, -11,   7,  26, 24, 35,  -8, -20,
	-36, -26, -12,  -1,  9, -7,   6, -23,
	-45, -25, -16, -17,  3,  0,  -5, -33,
	-44, -16, -20,  -9, -1, 11,  -6, -71,
	-19, -13,   1,  17, 16,  7, -37, -26,
};

int Evaluator::RookTableEndGame[64] = {
	13, 10, 18, 15, 12,  12,   8,   5,
	11, 13, 13, 11, -3,   3,   8,   3,
	 7,  7,  7,  5,  4,  -3,  -5,  -3,
	 4,  3, 13,  1,  2,   1,  -1,   2,
	 3,  5,  8,  4, -5,  -6,  -8, -11,
	-4,  0, -5, -1, -7, -12,  -8, -16,
	-6, -6,  0,  2, -9,  -9, -11,  -3,
	-9,  2,  3, -1, -5, -13,   4, -20,
};

int Evaluator::QueenTable[64] = {
	-28,   0,  29,  12,  59,  44,  43,  45,
	-24, -39,  -5,   1, -16,  57,  28,  54,
	-13, -17,   7,   8,  29,  56,  47,  57,
	-27, -27, -16, -16,  -1,  17,  -2,   1,
	 -9, -26,  -9, -10,  -2,  -4,   3,  -3,
	-14,   2, -11,  -2,  -5,   2,  14,   5,
	-35,  -8,  11,   2,   8,  15,  -3,   1,
	 -1, -18,  -9,  10, -15, -25, -31, -50,
};

int Evaluator::QueenTableEndGame[64] = {
	 -9,  22,  22,  27,  27,  19,  10,  20,
	-17,  20,  32,  41,  58,  25,  30,   0,
	-20,   6,   9,  49,  47,  35,  19,   9,
	  3,  22,  24,  45,  57,  40,  57,  36,
	-18,  28,  19,  47,  31,  34,  39,  23,
	-16, -27,  15,   6,   9,  17,  10,   5,
	-22, -23, -30, -16, -16, -23, -36, -32,
	-33, -28, -22, -43,  -5, -32, -20, -41,
};

int Evaluator::KingTable[64] = {
	-65,  23,  16, -15, -56, -34,   2,  13,
	 29,  -1, -20,  -7,  -8,  -4, -38, -29,
	 -9,  24,   2, -16, -20,   6,  22, -22,
	-17, -20, -12, -27, -30, -25, -14, -36,
	-49,  -1, -27, -39, -46, -44, -33, -51,
	-14, -14, -22, -46, -44, -30, -15, -27,
	  1,   7,  -8, -64, -43, -16,   9,   8,
	-15,  36,  12, -54,   8, -28,  24,  14,
};

int Evaluator::KingTableEndGame[64] = {
	-74, -35, -18, -18, -11,  15,   4, -17,
	-12,  17,  14,  17,  17,  38,  23,  11,
	 10,  17,  23,  15,  20,  45,  44,  13,
	 -8,  22,  24,  27,  26,  33,  26,   3,
	-18,  -4,  21,  24,  27,  23,   9, -11,
	-19,  -3,  11,  21,  23,  16,   7,  -9,
	-27, -11,   4,  13,  14,   4,  -5, -17,
	-53, -34, -21, -11, -28, -14, -24, -43
};

int Evaluator::game_phase_inc[13] = { 0, 0, 0, 4, 4, 2, 2, 1, 1, 1, 1, 0, 0 };
int Evaluator::mg_value[13] = { 0, 0, 0, 1025, 1025, 477, 477, 365, 365, 337, 337, 82, 82 };
int Evaluator::eg_value[13] = { 0, 0, 0, 936, 936, 512, 512, 297, 297, 281, 281, 94, 94 };
