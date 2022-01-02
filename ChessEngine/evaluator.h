#pragma once

#include "board.h"
#include "utility.h"
#include "piece.h"
#include <unordered_map>

constexpr int PV_SCORE = 300000;
constexpr int CAPTURE_SCORE = 200000;
constexpr int KILLER_MOVE_SCORE = 100000;

class Evaluator
{
public:
	Evaluator();
	static int evaluate(Board* b);
	static bool compare(Board* pos, Move* m_1, Move* m_2, Move* pv_move, bool left_most);
	static int mirror_vertical(int i);
	static int score_quiet_move(Board* pos, Move *m);
	static int score_move(Board* pos, Move* m, Move* pv_move, bool left_most);
	static int score_capture(Board* pos, Move* m);
	~Evaluator();
	static void init_tables();

private:
	static int mg_table[12][64];
	static int eg_table[12][64];
	static int PawnTable[64];
	static int PawnTableEndGame[64];
	static int KnightTable[64];
	static int KnightTableEndGame[64];
	static int BishopTable[64];
	static int BishopTableEndGame[64];
	static int RookTable[64];
	static int RookTableEndGame[64];
	static int QueenTable[64];
	static int QueenTableEndGame[64];
	static int KingTable[64];
	static int KingTableEndGame[64];
	static int game_phase_inc[13];
	static int mg_value[13]; 
	static int eg_value[13];

};
