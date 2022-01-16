#include "evaluator.h"
#include <iostream>


Evaluator::Evaluator() {
	//init_tables();
}

Evaluator::~Evaluator() {}

int Evaluator::mirror_vertical(int i) {
	return  8 * (7 - (i / 8)) + (i % 8);
}

void Evaluator::init_tables() {
	// kings 
	int bonus_dia_distance[15] = { 5, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for (int i = 0; i < 64; ++i) {
		for (int j = 0; j < 64; ++j) {
			distances_bonus[i][j] = 14 - (std::abs(i % 8 - j % 8) + std::abs(i / 8 - j / 8));
			dist_queen_to_king[i][j] = (distances_bonus[i][j] * 5) / 2;
			dist_rook_to_king[i][j] = distances_bonus[i][j] / 2;
			dist_knight_to_king[i][j] = distances_bonus[i][j];
			// bishop distances
			dist_bishop_to_king[i][j] += bonus_dia_distance[std::abs(north_east_diagonal[i] - north_east_diagonal[j])];
			dist_bishop_to_king[i][j] += bonus_dia_distance[std::abs(north_west_diagonal[i] - north_west_diagonal[j])];

		}
	}
}

int Evaluator::evaluate(Board* b)
{
	// insufficient material
	if (b->pawn_list->size() == 0 && b->queen_list->size() == 0 && b->rook_list->size() == 0) {
		// std::cout << "insufficient material" << std::endl;
		if ((b->bishop_list->size() == 0 && b->knight_list->size() == 0) || (b->bishop_list->size() == 0 && b->knight_list->size() == 1) || (b->bishop_list->size() == 1 && b->knight_list->size() == 0)) {
			return 0;
		}
	}

	int mg_eval = 0;
	int eg_eval = 0;

	int white_king_safety = 0;
	int black_king_safety = 0;
	int wkp = b->white_king_pos;
	int bkp = b->black_king_pos;
	if (wkp <= 2 || (wkp >= 6 && wkp < 8)) {
		if (wkp % 8 != 7 && b->position[wkp + 9]->get_type() == 11) {
			white_king_safety += 15;
		}
		if (wkp % 8 != 0 && b->position[wkp + 7]->get_type() == 11) {
			white_king_safety += 15;
		}
		if (b->position[wkp + 8]->get_type() == 11) {
			white_king_safety += 20;
		}
	}
	if ((bkp >= 56 && bkp <= 58) || (bkp >= 62)) {
		if (bkp % 8 != 7 && b->position[bkp - 7]->get_type() == 12) {
			black_king_safety -= 15;
		}
		if (bkp % 8 != 0 && b->position[bkp - 9]->get_type() == 12) {
			black_king_safety -= 15;
		}
		if (b->position[bkp - 8]->get_type() == 12) {
			black_king_safety -= 20;
		}
	}

	int white_king_tropism = 0;
	int black_king_tropism = 0;
	int black_material = 0;
	int white_material = 0;
	int eg_tropism_w = 0;
	int eg_tropism_b = 0;
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
			eg_tropism_w += 2 * distances_bonus[i][bkp];
			white_material += 1;
			break;
		case 2:
			mg_eval -= mg_value[type] + KingTable[i];
			eg_eval -= eg_value[type] + KingTableEndGame[i];
			eg_tropism_b += 2 * distances_bonus[i][wkp];
			black_material += 1;
			break;
			// queen
		case 3:
			mg_eval += mg_value[type] + QueenTable[mirror_vertical(i)];
			eg_eval += eg_value[type] + QueenTableEndGame[mirror_vertical(i)];
			black_king_tropism += dist_queen_to_king[i][bkp];
			white_material += mg_value[type];
			break;
		case 4:
			mg_eval -= mg_value[type] + QueenTable[i];
			eg_eval -= eg_value[type] + QueenTableEndGame[i];
			white_king_tropism += dist_queen_to_king[i][wkp];
			black_material += mg_value[type];
			break;
			// rook
		case 5:
			mg_eval += mg_value[type] + RookTable[mirror_vertical(i)];
			eg_eval += eg_value[type] + RookTableEndGame[mirror_vertical(i)];
			black_king_tropism += dist_rook_to_king[i][bkp];
			white_material += mg_value[type];
			break;
		case 6:
			mg_eval -= mg_value[type] + RookTable[i];
			eg_eval -= eg_value[type] + RookTableEndGame[i];
			white_king_tropism += dist_rook_to_king[i][wkp];
			black_material += mg_value[type];
			break;
			// bishop
		case 7:
			mg_eval += mg_value[type] + BishopTable[mirror_vertical(i)];
			eg_eval += eg_value[type] + BishopTableEndGame[mirror_vertical(i)];
			black_king_tropism += dist_bishop_to_king[i][bkp];
			white_material += mg_value[type];
			break;
		case 8:
			mg_eval -= mg_value[type] + BishopTable[i];
			eg_eval -= eg_value[type] + BishopTableEndGame[i];
			white_king_tropism += dist_bishop_to_king[i][wkp];
			black_material += mg_value[type];
			break;
			// knight
		case 9:
			mg_eval += mg_value[type] + KnightTable[mirror_vertical(i)];
			eg_eval += eg_value[type] + KnightTableEndGame[mirror_vertical(i)];
			black_king_tropism += dist_knight_to_king[i][bkp];
			white_material += mg_value[type];
			break;
		case 10:
			mg_eval -= mg_value[type] + KnightTable[i];
			eg_eval -= eg_value[type] + KnightTableEndGame[i];
			white_king_tropism += dist_knight_to_king[i][wkp];
			black_material += mg_value[type];
			break;
			// pawn
		case 11:
			mg_eval += mg_value[type] + PawnTable[mirror_vertical(i)];
			eg_eval += eg_value[type] + PawnTableEndGame[mirror_vertical(i)];
			white_material += mg_value[type];
			break;
		case 12:
			mg_eval -= mg_value[type] + PawnTable[i];
			eg_eval -= eg_value[type] + PawnTableEndGame[i];
			black_material += mg_value[type];
			break;
		default:
			break;
		}
	}

	int game_phase = 4 * b->queen_list->size() + 2 * b->rook_list->size() + b->bishop_list->size() + b->knight_list->size();
	int mg_phase = std::min(24, game_phase);
	int eg_phase = 24 - game_phase;
	if (white_material == 0 || black_material == 0) {
		std::cout << b->pos_as_str() << std::endl;
	}
	int e = (mg_eval * mg_phase + eg_eval * eg_phase) / 24;
	e += (((black_king_tropism + white_king_safety) * white_material) - ((white_king_tropism + black_king_safety) * black_material)) / INITIAL_MATERIAL_VALUE;
	e += ((eg_tropism_w * (INITIAL_MATERIAL_VALUE - white_material)) - (eg_tropism_b * (INITIAL_MATERIAL_VALUE - black_material))) / INITIAL_MATERIAL_VALUE;
	if (b->white_to_move)
	{
		return e;
	}
	else
	{
		return -e;
	}
}
// position startpos moves d2d4 d7d5 c2c4 e7e6 b1c3 g8f6 c1g5 f8e7 e2e3 e8g8 g1f3 b7b6 f1d3 c8b7 c4d5 e6d5 f3e5 b8c6 e1g1 c6e5 d4e5 f6e4 d3e4 d5e4 d1d8 e7d8 g5f4 d8e7 a1d1 a8d8 e5e6 f7e6 f4c7 d8d3 c7e5 e7f6 e5f6 f8f6 h2h3 b7c6 f1e1 f6f7 a2a3 b6b5 c3e2 d3d1 e1d1 e6e5 e2c3 a7a5 d1d6 f7f6 d6d8 f6f8 d8d6 f8c8 c3d5 g8f7 g1h2 a5a4 h2g3 h7h6 d5b4 c6e8 b4d5 c8c2 d5c3 c2b2 c3e4 b2a2 d6b6 f7e7 e4d6 e8d7 d6e4 a2a3 e4c5 a3a2 b6b7 a2d2 c5d7 d2d7 b7b5 a4a3 b5e5 e7f6 e5a5 d7d3 a5a7 f6g6 g3f4
// position fen 6Q1/8/3p4/8/2P4K/8/1k5P/q7 w - - 0 50 

/**
 * \brief Compares two moves according to a position.
 *
 * \param pos position
 * \param m_1 move 1
 * \param m_2 move 2
 * \return true if and only if move 1 & 2 are already in the correct respective order.
 */
bool Evaluator::compare(Board* pos, Move* m_1, Move* m_2, Move* pv_move, Move* prev_best, std::vector<Move*>* killer_moves_at_depth, bool left_most) {
	return score_move(pos, m_1, pv_move, prev_best, killer_moves_at_depth, left_most) > score_move(pos, m_2, pv_move, prev_best, killer_moves_at_depth, left_most);

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
	int game_phase = 4 * pos->queen_list->size() + 2 * pos->rook_list->size() + pos->bishop_list->size() + pos->knight_list->size();
	int mg_phase = std::min(24, game_phase);
	int eg_phase = 24 - game_phase;

	int wkp = pos->white_king_pos;
	int bkp = pos->black_king_pos;
	switch (type)
	{
	case 1:
	{
		if (std::abs(target - origin) == 2) {
			return 500;
		}
		return  (KingTable[target] - KingTable[origin]) * mg_phase + (KingTableEndGame[target] - KingTableEndGame[origin]) * eg_phase;
	}
	case 2:
		if (std::abs(target - origin) == 2) {
			return 500;
		}
		return ((KingTable[target] - KingTable[origin]) * mg_phase + (KingTableEndGame[target] - KingTableEndGame[origin]) * eg_phase) / 24;
	case 3:
		return ((QueenTable[target] - QueenTable[origin]) * mg_phase + (QueenTableEndGame[target] - QueenTableEndGame[origin]) * eg_phase) / 24;
	case 4:
		return ((QueenTable[target] - QueenTable[origin]) * mg_phase + (QueenTableEndGame[target] - QueenTableEndGame[origin]) * eg_phase) / 24;
	case 5:
		return ((RookTable[target] - RookTable[origin]) * mg_phase 
			+ (RookTableEndGame[target] - RookTableEndGame[origin]) * eg_phase) / 24;
	case 6:
		return ((RookTable[target] - RookTable[origin]) * mg_phase 
			+ (RookTableEndGame[target] - RookTableEndGame[origin]) * eg_phase) / 24;
	case 7:
		return ((BishopTable[target] - BishopTable[origin]) * mg_phase + (BishopTableEndGame[target] - BishopTableEndGame[origin]) * eg_phase) / 24;
	case 8:
		return ((BishopTable[target] - BishopTable[origin]) * mg_phase + (BishopTableEndGame[target] - BishopTableEndGame[origin]) * eg_phase) / 24;
	case 9:
		return ((KnightTable[target] - KnightTable[origin]) * mg_phase + (KnightTableEndGame[target] - KnightTableEndGame[origin]) * eg_phase) / 24;
	case 10:
		return ((KnightTable[target] - KnightTable[origin]) * mg_phase + (KnightTableEndGame[target] - KnightTableEndGame[origin]) * eg_phase) / 24;
	case 11:
		return ((PawnTable[target] - PawnTable[origin]) * mg_phase + (PawnTableEndGameMO[target] - PawnTableEndGameMO[origin]) * eg_phase) / 24;
	case 12:
		return ((PawnTable[target] - PawnTable[origin]) * mg_phase + (PawnTableEndGameMO[target] - PawnTableEndGameMO[origin]) * eg_phase) / 24;
	default:
		return 0;
	}
}

int Evaluator::score_capture(Board* pos, Move* move) {
	int origin = move->origin;
	int target = move->target;
	return pos->position[target]->value() - pos->position[origin]->value() + CAPTURE_SCORE;
}

int Evaluator::score_move(Board* pos, Move* move, Move* pv_move, Move* prev_best, std::vector<Move*>* killer_moves_at_depth, bool left_most) {
	if (left_most && pv_move != nullptr && move->equals(pv_move)) {
		return PV_SCORE;
	}
	else if (prev_best != nullptr && prev_best->equals(move)) {
		//std::cout << prev_best->to_string() << std::endl;
		return PV_SCORE;
	}
	if (move->is_capture) {
		// score capture
		return score_capture(pos, move) + score_quiet_move(pos, move);
	}
	else {
		// score quiet move
		if (killer_moves_at_depth != nullptr) {
			if (killer_moves_at_depth->at(0)->equals(move)) {
				return KILLER_MOVE_ONE;
			}
			else if (killer_moves_at_depth->at(1)->equals(move)) {
				return KILLER_MOVE_TWO;
			}
		}
		int history_val = Evaluator::history[pos->position[move->origin]->get_type() - 1][move->target];
		if (history_val > 0) {
			return history_val + HISTORY_MOVE_OFFSET;
		}
		return score_quiet_move(pos, move);
	}
}

void Evaluator::reset_history() {
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 64; j++) {
			history[i][j] = 0;
		}
	}
}

// distance tables for tropism
int Evaluator::distances_bonus[64][64] = { 0 };
int Evaluator::dist_queen_to_king[64][64] = { 0 };
int Evaluator::dist_rook_to_king[64][64] = { 0 };
int Evaluator::dist_knight_to_king[64][64] = { 0 };
int Evaluator::dist_bishop_to_king[64][64] = { 0 };

int Evaluator::history[12][64] = { 0 };

int Evaluator::north_west_diagonal[64] = {
   0, 1, 2, 3, 4, 5, 6, 7,
   1, 2, 3, 4, 5, 6, 7, 8,
   2, 3, 4, 5, 6, 7, 8, 9,
   3, 4, 5, 6, 7, 8, 9,10,
   4, 5, 6, 7, 8, 9,10,11,
   5, 6, 7, 8, 9,10,11,12,
   6, 7, 8, 9,10,11,12,13,
   7, 8, 9,10,11,12,13,14
};

int Evaluator::north_east_diagonal[64] = {
   7, 6, 5, 4, 3, 2, 1, 0,
   8, 7, 6, 5, 4, 3, 2, 1,
   9, 8, 7, 6, 5, 4, 3, 2,
  10, 9, 8, 7, 6, 5, 4, 3,
  11,10, 9, 8, 7, 6, 5, 4,
  12,11,10, 9, 8, 7, 6, 5,
  13,12,11,10, 9, 8, 7, 6,
  14,13,12,11,10, 9, 8, 7
};

// piece square tables ...
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


int Evaluator::PawnTableEndGameMO[64] = {
	  0,   0,   0,   0,   0,   0,   0,   0,
	200, 240, 240, 240, 240, 240, 240, 200,
	100, 120, 120, 120, 120, 120, 120, 100,
	 70,  80,  80,  80,  80,  80,  80,  70,
	 35,  40,  40,  40,  40,  40,  40,  35,
	 25,  27,  27,  27,  27,  27,  27,  25,
	 13,  15,  15,  15,  15,  15,  15,  13,
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
